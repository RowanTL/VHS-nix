/*
 * Copyright 2020-22 St. Louis University
 * Contributed by: Hannah Schreiber
 *
 * This file is part of VHS.
 *
 * VHS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * VHS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with VHS. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file InvertedAugmentedShape.txx
 * Contains the implementation of the methods in the
 * \ref InvertedAugmentedShape class.
 **/

#include "InvertedAugmentedShape.h"

using namespace shape_simplification::shape;

template <class Shape, class Voxel>
InvertedAugmentedShape<Shape, Voxel>::InvertedAugmentedShape(Shape &shape)
    : _shape(shape),
      _collarModifiedSinceLastCompUpdate(false),
      _dimensions{0, 0, 0},
      _numberOfCells{0, 0, 0, 0}
{
    int x, y, z;
    for (auto it = shape.template begin<NEIGHBORHOOD>();
         it != shape.template end<NEIGHBORHOOD>();
         ++it) {
        Voxel v = *it;
        for (ExtendedNeighborsInIterator<Shape, Voxel, OUTSIDE> it(v, shape);
             !it.isEnd();
             it.next())
        {
            it.getCurrentValue(x, y, z);
            Voxel cur(x, y, z);
            if (!_components.contains(cur)){
                _addVoxel(cur);
            }
        }
        if (v.getLevel() == NEIGHBORHOOD){
            _addVoxel(v);
        }
    }
}

template <class Shape, class Voxel>
constexpr bool InvertedAugmentedShape<Shape, Voxel>::isAddable() {
    return false;
}

template <class Shape, class Voxel>
constexpr bool InvertedAugmentedShape<Shape, Voxel>::isRemovable() {
    return false;
}

template <class Shape, class Voxel>
template <Level level>
bool InvertedAugmentedShape<Shape, Voxel>::contains(int x, int y, int z) const
{
    if constexpr (level == CORE){
        if (!_shape.template contains<OUTSIDE>(x, y, z)) return false;
        int itX, itY, itZ;
        ExtendedNeighborsIterator<Voxel> it(Voxel(x, y, z));
        it.getCurrentValue(itX, itY, itZ);
        while (!it.isEnd() &&
               _shape.template contains<OUTSIDE>(itX, itY, itZ)){
            it.next();
            if (!it.isEnd()) it.getCurrentValue(itX, itY, itZ);
        }
        if (it.isEnd()) return false;
        return true;
    } else if constexpr (level == MAIN_SHAPE){
        return _shape.template contains<EX_NEIGHBORHOOD>(x, y, z)
                || contains<CORE>(x, y, z);
    } else if constexpr (level == NEIGHBORHOOD){
        return _shape.template contains<EX_MAIN_SHAPE>(x, y, z)
                || contains<MAIN_SHAPE>(x, y, z);
    } else if constexpr (level == EX_MAIN_SHAPE){
        return _shape.template contains<EX_NEIGHBORHOOD>(x, y, z);
    } else if constexpr (level == EX_NEIGHBORHOOD){
        return _shape.template contains<EX_MAIN_SHAPE>(x, y, z);
    } else {
        return _shape.template contains<CORE>(x, y, z)
                || !contains<NEIGHBORHOOD>(x, y, z);
    }
}

template <class Shape, class Voxel>
const std::array<unsigned int, 3> &
InvertedAugmentedShape<Shape, Voxel>::getShapeDimensions() const
{
    return _dimensions;
}

template <class Shape, class Voxel>
int InvertedAugmentedShape<Shape, Voxel>::getEulerCharacteristic() {
    return _numberOfCells[0] - _numberOfCells[1]
            + _numberOfCells[2] - _numberOfCells[3];
}

template <class Shape, class Voxel>
int InvertedAugmentedShape<Shape, Voxel>::getNumberOfComponents() const {
    return _components.size();
}

template <class Shape, class Voxel>
unsigned int InvertedAugmentedShape<Shape, Voxel>::getNumberOfLoops() {
    return getNumberOfComponents() - getEulerCharacteristic() +
            getNumberOfVoids();
}

template <class Shape, class Voxel>
unsigned int InvertedAugmentedShape<Shape, Voxel>::getNumberOfVoids() {
    if (_collarModifiedSinceLastCompUpdate)
        _updateCollarComponents();
    return _numberOfCollarComponents - 1;
}

template <class Shape, class Voxel>
Voxel InvertedAugmentedShape<Shape, Voxel>::getComponentRepresentative(Voxel v)
{
    return _components.find(v);
}

template <class Shape, class Voxel>
const typename InvertedAugmentedShape<Shape, Voxel>::collar_container&
InvertedAugmentedShape<Shape, Voxel>::getCollar() const {
    return _collar;
}

template <class Shape, class Voxel>
bool InvertedAugmentedShape<Shape, Voxel>::isInCollar(Voxel v) const {
    return _collar.find(v) != _collar.end();
}

template <class Shape, class Voxel>
const Shape& InvertedAugmentedShape<Shape, Voxel>::getUnderlyingShape(){
    return _shape;
}

template <class Shape, class Voxel>
void InvertedAugmentedShape<Shape, Voxel>::_addVoxel(Voxel v){
    v.setLevel(MAIN_SHAPE);
    _updateComponentsAfterAddingVoxel(v);
    _updateCollarAfterAddingVoxel(v);
    _updateDimensions(v);
    _updateNumberOfCellsAfterAddingVoxel(v);
}

template <class Shape, class Voxel>
void InvertedAugmentedShape<Shape, Voxel>::_updateComponentsAfterAddingVoxel(
        Voxel v)
{
    int x, y, z;
    _components.initialize(v);
    for (NeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()){
        it.getCurrentValue(x, y, z);
        if (contains<MAIN_SHAPE>(x, y, z)){
            _components.merge(v, Voxel(x, y, z));
        }
    }
}

template <class Shape, class Voxel>
void InvertedAugmentedShape<Shape, Voxel>::_updateCollarAfterAddingVoxel(
        Voxel v)
{
    int x, y, z;
    _collar.erase(v);
    for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
        it.getCurrentValue(x, y, z);

        if (!(contains<MAIN_SHAPE>(x, y, z))){
            Voxel c(x, y, z);
            if (contains<EX_NEIGHBORHOOD>(x, y, z))
                c.setLevel(NEIGHBORHOOD);
            else
                c.setLevel(OUTSIDE);
            _collar.insert(c);
        }
    }
    _collarModifiedSinceLastCompUpdate = true;
}

template <class Shape, class Voxel>
void InvertedAugmentedShape<Shape, Voxel>::_updateCollarComponents() {
    CC collarComponents;
    std::unordered_map<Voxel,Voxel> componentRep;

    _updateCollarComponentsFindCollarComponent(collarComponents);
    _updateCollarComponentsFindComponentRepresentative(collarComponents,
                                                       componentRep);
    _updateCollarComponentsMergeSameVoidComponents(collarComponents,
                                                   componentRep);

    _numberOfCollarComponents = collarComponents.size();
    _collarModifiedSinceLastCompUpdate = false;
}

template <class Shape, class Voxel>
void InvertedAugmentedShape<Shape, Voxel>::_updateCollarComponentsFindCollarComponent(
        CC &collarComponents)
{
    int x, y, z;
    for (Voxel v : _collar) {
        collarComponents.initialize(v);
        for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
            it.getCurrentValue(x, y, z);
            collarComponents.merge(v, Voxel(x, y, z));
        }
    }
}

template <class Shape, class Voxel>
void InvertedAugmentedShape<Shape, Voxel>::
_updateCollarComponentsFindComponentRepresentative(
        CC &collarComponents,
        std::unordered_map<Voxel,Voxel>& componentRep) {
    for (Voxel v : _collar) {
        Voxel component = collarComponents.find(v);
        if (componentRep.find(component) == componentRep.end())
            componentRep[component] = v;
        else
            componentRep[component] = std::max(v, componentRep[component]);
    }
}

template <class Shape, class Voxel>
void InvertedAugmentedShape<Shape, Voxel>::
_updateCollarComponentsMergeSameVoidComponents(
        CC &collarComponents,
        std::unordered_map<Voxel,Voxel>& componentRep)
{
    int x, y, z;
    for (auto iter : componentRep) {
        Voxel v = iter.second;
        Point p = {v[0], v[1], v[2]};

        bool done = false;
        for (unsigned i = 0; (i < 3) && !done; i++) {
            while (!done && p[i] < getShapeDimensions()[i]) {
                p[i]++;
                Voxel cur(p);
                if (!isInCollar(cur) && !contains<MAIN_SHAPE>(p[0], p[1], p[2]))
                {
                    if (contains<EX_NEIGHBORHOOD>(p[0], p[1], p[2]))
                        cur.setLevel(NEIGHBORHOOD);
                    else
                        cur.setLevel(OUTSIDE);
                    _collar.insert(cur);
                    collarComponents.initialize(cur);
                    for (ExtendedNeighborsIterator<Voxel> it(cur);
                         !it.isEnd();
                         it.next())
                    {
                        it.getCurrentValue(x, y, z);
                        collarComponents.merge(cur, Voxel(x, y, z));
                    }
                } else {
                    done = true;
                }
            }
        }
    }
}

template <class Shape, class Voxel>
void InvertedAugmentedShape<Shape, Voxel>::_updateDimensions(Voxel &v){
    if (v[0] > _dimensions[0])
        _dimensions[0] = v[0];
    if (v[1] > _dimensions[1])
        _dimensions[1] = v[1];
    if (v[2] > _dimensions[2])
        _dimensions[2] = v[2];
}

template <class Shape, class Voxel>
void InvertedAugmentedShape<Shape, Voxel>::_updateNumberOfCellsAfterAddingVoxel(
        Voxel v)
{
    _numberOfCells[0]++;
    _numberOfCells[1] += _getNberOfGeneratedEdges(v);
    _numberOfCells[2] += _getNberOfGeneratedSquares(v);
    _numberOfCells[3] += _getNberOfGeneratedCubes(v);
}

template <class Shape, class Voxel>
int InvertedAugmentedShape<Shape, Voxel>::_getNberOfGeneratedEdges(Voxel v){
    int edges = 0;
    int x, y, z;
    for (NeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()){
        it.getCurrentValue(x, y, z);
        if (_components.contains(Voxel(x, y, z)) &&
                contains<MAIN_SHAPE>(x, y, z)) edges++;
    }
    return edges;
}

template <class Shape, class Voxel>
int InvertedAugmentedShape<Shape, Voxel>::_getNberOfGeneratedSquares(Voxel v){
    int squares = 0;
    for (DualSquareIterator<Voxel> it(v); !it.isEnd(); it.next()){
        std::vector<Point> coordinates = it.getCurrentValue();
        auto coorIt = coordinates.begin();
        Point cur = *coorIt;
        while (coorIt != coordinates.end() &&
               ((cur[0] == v[0] && cur[1] == v[1] && cur[2] == v[2]) ||
                (_components.contains(Voxel(cur)) &&
                 contains<MAIN_SHAPE>(cur[0], cur[1], cur[2])))){
            coorIt++;
            cur = *coorIt;
        }
        if (coorIt == coordinates.end()) squares++;
    }
    return squares;
}

template <class Shape, class Voxel>
int InvertedAugmentedShape<Shape, Voxel>::_getNberOfGeneratedCubes(Voxel v){
    int cubes = 0;
    for (DualCubeIterator<Voxel> it(v); !it.isEnd(); it.next()){
        std::vector<Point> coordinates = it.getCurrentValue();
        auto coorIt = coordinates.begin();
        Point cur = *coorIt;
        while (coorIt != coordinates.end() &&
               ((cur[0] == v[0] && cur[1] == v[1] && cur[2] == v[2]) ||
                (_components.contains(Voxel(cur)) &&
                 contains<MAIN_SHAPE>(cur[0], cur[1], cur[2])))){
            coorIt++;
            cur = *coorIt;
        }
        if (coorIt == coordinates.end()) cubes++;
    }
    return cubes;
}

