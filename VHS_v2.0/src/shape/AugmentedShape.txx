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
 * @file AugmentedShape.txx
 * Contains the implementation of the methods in the \ref AugmentedShape class.
 **/

#include "AugmentedShape.h"

using namespace shape_simplification::shape;

template <class Shape, class Voxel>
AugmentedShape<Shape, Voxel>::AugmentedShape() : Shape(),
    _collarModifiedSinceLastCompUpdate(false), _numberOfCells{0, 0, 0, 0} {}

template <class Shape, class Voxel>
AugmentedShape<Shape, Voxel>::AugmentedShape(std::vector<Voxel> &voxels)
    : BaseShape<Voxel>(voxels), Shape(voxels),
      _collarModifiedSinceLastCompUpdate(false), _numberOfCells{0, 0, 0, 0}
{
    for (typename std::vector<Voxel>::size_type i = 0; i < voxels.size(); ++i) {
        Voxel v = voxels.at(i);
        Level l = v.getLevel();
        if ((l == CORE || l == MAIN_SHAPE) && !_components.contains(v)) {
            _updateComponentsAfterAddingVoxel(v);
            _updateCollarAfterAddingVoxel(v);
            _updateNumberOfCellsAfterAddingVoxel(v);
        }
    }
}

template <class Shape, class Voxel>
AugmentedShape<Shape, Voxel>::AugmentedShape(Shape &shape)
    : BaseShape<Voxel>(shape), Shape(shape),
      _collarModifiedSinceLastCompUpdate(false), _numberOfCells{0, 0, 0, 0}
{
    for (auto it = shape.template begin<MAIN_SHAPE>();
         it != shape.template end<MAIN_SHAPE>();
         ++it)
    {
        Voxel v = *it;
        if (!_components.contains(v)) {
            _updateComponentsAfterAddingVoxel(v);
            _updateCollarAfterAddingVoxel(v);
            _updateNumberOfCellsAfterAddingVoxel(v);
        }
    }
}

template <class Shape, class Voxel>
template <Level minLevel>
void AugmentedShape<Shape, Voxel>::addVoxel(Voxel v)
{
    static_assert(Shape::isAddable(), "Insertion of voxel not enabled.");

    v.setLevel(minLevel);
    Shape::template addVoxel<minLevel>(v);

    if constexpr (minLevel == CORE || minLevel == MAIN_SHAPE){
        if (!_components.contains(v)) {
            _updateComponentsAfterAddingVoxel(v);
            _updateCollarAfterAddingVoxel(v);
            _updateNumberOfCellsAfterAddingVoxel(v);
        }
    }
}

template <class Shape, class Voxel>
template <Level maxLevel>
void AugmentedShape<Shape, Voxel>::removeVoxel(Voxel v)
{
    static_assert(Shape::isRemovable(), "Removal of voxel not enabled.");

    Shape::template removeVoxel<maxLevel>(v);

    if constexpr (maxLevel == NEIGHBORHOOD) v.setLevel(OUTSIDE);
    if constexpr (maxLevel == MAIN_SHAPE) v.setLevel(NEIGHBORHOOD);

    if constexpr (maxLevel == NEIGHBORHOOD || maxLevel == MAIN_SHAPE){
        if (_components.contains(v)) {
            _updateComponentsAfterRemovingVoxel(v);
            _updateCollarAfterRemovingVoxel(v);
            _updateNumberOfCellsAfterRemovingVoxel(v);
        }
    }
}

template <class Shape, class Voxel>
inline int AugmentedShape<Shape, Voxel>::getEulerCharacteristic() {
    return _numberOfCells[0] - _numberOfCells[1]
            + _numberOfCells[2] - _numberOfCells[3];
}

template <class Shape, class Voxel>
inline unsigned int AugmentedShape<Shape, Voxel>::getNumberOfComponents() const
{
    return _components.size();
}

template <class Shape, class Voxel>
inline unsigned int AugmentedShape<Shape, Voxel>::getNumberOfLoops() {
    return getNumberOfComponents() - getEulerCharacteristic() +
            getNumberOfVoids();
}

template <class Shape, class Voxel>
inline unsigned int AugmentedShape<Shape, Voxel>::getNumberOfVoids() {
    if (_collarModifiedSinceLastCompUpdate)
        _updateCollarComponents();
    return _numberOfCollarComponents - 1;
}

template <class Shape, class Voxel>
inline Voxel AugmentedShape<Shape, Voxel>::getComponentRepresentative(Voxel v)
{
    return _components.find(v);
}

template <class Shape, class Voxel>
inline unsigned long long int
AugmentedShape<Shape, Voxel>::getComponentSize(Voxel v){
    static_assert(Shape::isRemovable(), "Removal of voxel not enabled.");
    return _components.getComponentSize(v);
}

template <class Shape, class Voxel>
inline void AugmentedShape<Shape, Voxel>::getComponent(
        Voxel v, std::vector<Voxel> &container)
{
    static_assert(Shape::isRemovable(), "Removal of voxel not enabled.");
    return _components.getComponent(v, container);
}

template <class Shape, class Voxel>
bool AugmentedShape<Shape, Voxel>::isInCoreComponent(Voxel v)
{
    static_assert(Shape::isRemovable(), "Removal of voxel not enabled.");

    if (v.getLevel() == NEIGHBORHOOD) return false;

    typename CCR::component_iterator it = _components.begin(v);
    typename CCR::component_iterator end = _components.end(v);

    while (it != end && (*it).getLevel() != CORE) {
        it++;
    }

    if (it == end && (*it).getLevel() == CORE) return true;
    return it != end;
}

template <class Shape, class Voxel>
inline const typename AugmentedShape<Shape, Voxel>::collar_container&
AugmentedShape<Shape, Voxel>::getCollar() const {
    return _collar;
}

template <class Shape, class Voxel>
inline bool AugmentedShape<Shape, Voxel>::isInCollar(Voxel v) const {
    return _collar.find(v) != _collar.end();
}

template <class Shape, class Voxel>
template<unsigned int dim>
inline long long AugmentedShape<Shape, Voxel>::getNumberOfDualCellsInDim(){
    return _numberOfCells[dim];
}

template <class Shape, class Voxel>
void AugmentedShape<Shape, Voxel>::clear(){
    Shape::clear();
    _components.clear();
    _collar.clear();
    _numberOfCollarComponents = 0;
    _collarModifiedSinceLastCompUpdate = false;
    _numberOfCells[0] = 0;
    _numberOfCells[1] = 0;
    _numberOfCells[2] = 0;
    _numberOfCells[3] = 0;
}

template <class Shape, class Voxel>
void AugmentedShape<Shape, Voxel>::_updateComponentsAfterAddingVoxel(Voxel v)
{
    int x, y, z;
    _components.initialize(v);
    for (neighbor_iterator<MAIN_SHAPE> it(v, *this); !it.isEnd(); it.next()) {
        it.getCurrentValue(x, y, z);
        _components.merge(v, Voxel(x,y,z));
    }
}

template <class Shape, class Voxel>
inline void AugmentedShape<Shape, Voxel>::_updateComponentsAfterRemovingVoxel(
        Voxel v) {
    _components.remove(v);  //because guaranteed to not increase H0.
}

template <class Shape, class Voxel>
void AugmentedShape<Shape, Voxel>::_updateCollarAfterAddingVoxel(Voxel v)
{
    int x, y, z;
    _collar.erase(v);
    for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
        it.getCurrentValue(x, y, z);
        if (!(Shape::template contains<MAIN_SHAPE>(x, y, z))){
            Voxel c(x, y, z);
            if (Shape::template contains<EX_NEIGHBORHOOD>(x, y, z))
                c.setLevel(NEIGHBORHOOD);
            else
                c.setLevel(OUTSIDE);
            _collar.insert(c);
        }
    }
    _collarModifiedSinceLastCompUpdate = true;
}

template <class Shape, class Voxel>
void AugmentedShape<Shape, Voxel>::_updateCollarAfterRemovingVoxel(Voxel v)
{
    bool toAdd = false;
    int x, y, z;
    for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
        it.getCurrentValue(x, y, z);
        Voxel cur(x, y, z);
        if (Shape::template contains<MAIN_SHAPE>(x, y, z)) toAdd = true;
        else if (_collar.find(cur) != _collar.end()){
            ExtendedNeighborsIterator<Voxel> nIt(cur);
            nIt.getCurrentValue(x, y, z);
            while (!nIt.isEnd() &&
                   !(Shape::template contains<MAIN_SHAPE>(x, y, z))) {
                nIt.next();
                nIt.getCurrentValue(x, y, z);
            }
            if (nIt.isEnd()){
                _collar.erase(cur);
            }
        }
    }
    if (toAdd) _collar.insert(v);
    _collarModifiedSinceLastCompUpdate = true;
}

template <class Shape, class Voxel>
bool AugmentedShape<Shape, Voxel>::_isRealCollarVoxel(Voxel v){
    if (!isInCollar(v)) return false;
    int x, y, z;

    ExtendedNeighborsIterator<Voxel> it(v);
    it.getCurrentValue(x, y, z);
    while (!it.isEnd() &&
           !Shape::template contains<MAIN_SHAPE>(x, y, z))
    {
        it.next();
        if (!it.isEnd()) it.getCurrentValue(x, y, z);
    }
    if (it.isEnd()) return false;
    return true;
}

template <class Shape, class Voxel>
void AugmentedShape<Shape, Voxel>::_updateCollarComponents() {
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
void AugmentedShape<Shape, Voxel>::_updateCollarComponentsFindCollarComponent(
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
void AugmentedShape<Shape, Voxel>::
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
void AugmentedShape<Shape, Voxel>::
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
            while (!done && p[i] < Shape::getShapeDimensions()[i]) {
                p[i]++;
                Voxel cur(p);
                if (!collarComponents.contains(cur) &&
                      !(Shape::template contains<MAIN_SHAPE>(p[0], p[1], p[2])))
                {
                    collarComponents.initialize(cur);
                    for (ExtendedNeighborsIterator<Voxel> it(cur); !it.isEnd();
                         it.next()) {
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
void AugmentedShape<Shape,Voxel>::_updateNumberOfCellsAfterAddingVoxel(Voxel v)
{
    _numberOfCells[0]++;
    _numberOfCells[1] += _getNberOfGeneratedEdges(v);
    _numberOfCells[2] += _getNberOfGeneratedSquares(v);
    _numberOfCells[3] += _getNberOfGeneratedCubes(v);
}

template <class Shape, class Voxel>
void AugmentedShape<Shape,Voxel>::_updateNumberOfCellsAfterRemovingVoxel(Voxel v)
{
    _numberOfCells[0]--;
    _numberOfCells[1] -= _getNberOfGeneratedEdges(v);
    _numberOfCells[2] -= _getNberOfGeneratedSquares(v);
    _numberOfCells[3] -= _getNberOfGeneratedCubes(v);
}

template <class Shape, class Voxel>
int AugmentedShape<Shape, Voxel>::_getNberOfGeneratedEdges(Voxel v){
    int edges = 0;
    int x, y, z;
    for (NeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()){
        it.getCurrentValue(x, y, z);
        if (_components.contains(Voxel(x, y, z)) &&
                Shape::template contains<MAIN_SHAPE>(x, y, z))
            edges++;
    }
    return edges;
}

template <class Shape, class Voxel>
int AugmentedShape<Shape, Voxel>::_getNberOfGeneratedSquares(Voxel v){
    int squares = 0;
    for (DualSquareIterator<Voxel> it(v); !it.isEnd(); it.next()){
        std::vector<Point> coordinates = it.getCurrentValue();
        auto coorIt = coordinates.begin();
        Point cur = *coorIt;
        while (coorIt != coordinates.end() &&
               ((cur[0] == v[0] && cur[1] == v[1] && cur[2] == v[2]) ||
                (_components.contains(Voxel(cur)) &&
                 Shape::template contains<MAIN_SHAPE>(cur[0], cur[1], cur[2]))))
        {
            coorIt++;
            cur = *coorIt;
        }
        if (coorIt == coordinates.end()) squares++;
    }
    return squares;
}

template <class Shape, class Voxel>
int AugmentedShape<Shape, Voxel>::_getNberOfGeneratedCubes(Voxel v){
    int cubes = 0;
    for (DualCubeIterator<Voxel> it(v); !it.isEnd(); it.next()){
        std::vector<Point> coordinates = it.getCurrentValue();
        auto coorIt = coordinates.begin();
        Point cur = *coorIt;
        while (coorIt != coordinates.end() &&
               ((cur[0] == v[0] && cur[1] == v[1] && cur[2] == v[2]) ||
                (_components.contains(Voxel(cur)) &&
                 Shape::template contains<MAIN_SHAPE>(cur[0], cur[1], cur[2]))))
        {
            coorIt++;
            cur = *coorIt;
        }
        if (coorIt == coordinates.end()) cubes++;
    }
    return cubes;
}


