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
 * @file InflationShape.cpp
 * Contains the implementation of the methods in the \ref Shape class.
 **/

#include "InflationShape.h"

namespace shape_simplification::shape {

InflationShape::InflationShape() :
    _shapeDimensions{0, 0, 0},
    _shapeThreshold(0),
    _neighborhoodThreshold(0)
{}

InflationShape::InflationShape(ProcessedImageStack &stack,
                               unsigned int inflationValue,
                               bool nonVoidLocalMaxima) :
    _shapeDimensions{0, 0, 0},
    _shapeThreshold(stack.getShapeThreshold()),
    _neighborhoodThreshold(stack.getNeighborhoodThreshold())
{
    if (inflationValue == 0)
        _constructShape(stack);
    else
        _constructShape(stack, inflationValue);

    if (nonVoidLocalMaxima) _computeLocalMaxima();
    else _computeVoidCollar();
}

InflationShape::InflationShape(ProcessedImageStack &stack,
                               bool nonVoidLocalMaxima) :
    _shapeDimensions{0, 0, 0},
    _shapeThreshold(stack.getShapeThreshold()),
    _neighborhoodThreshold(stack.getNeighborhoodThreshold())
{
    _constructShape(stack);

    if (nonVoidLocalMaxima) _computeLocalMaxima();
    else _computeVoidCollar();
}

int InflationShape::_constructShape(ProcessedImageStack &stack,
                                     unsigned int inflationValue)
{
    const std::array<unsigned int, 3> &dimensions = stack.getDimensions();
    _voxels.resize(inflationValue + 1);

    int count = 0;
    int curX, curY, curZ;
    ConnectedComponents<Voxel> collar;
    ConnectedComponents<Voxel> outerCollar;
    std::vector<std::vector<Voxel> > components;
    Voxel maxCollar(0, 0, 0);

    for (unsigned int z = 0; z < dimensions[2]; z++) {
        for (unsigned int y = 0; y < dimensions[1]; y++) {
            for (unsigned int x = 0; x < dimensions[0]; x++) {
                unsigned char level = stack.getLevel(x, y, z);
                if (level >= stack.getShapeThreshold()) {
                    Voxel v(x, y, z);
                    count++;
                    _addAtLevel(v, 0);
                }
                if (level >= stack.getNeighborhoodThreshold()) {
                    Voxel v(x, y, z);

                    for (ExtendedNeighborsIterator<Voxel> it(v);
                         !it.isEnd();
                         it.next())
                    {
                        it.getCurrentValue(curX, curY, curZ);
                        Voxel cur(curX, curY, curZ);
                        if (stack.isWithinBounds(curX, curY, curZ) &&
                                stack.getLevel(curX, curY, curZ) <
                                stack.getNeighborhoodThreshold() &&
                                !collar.contains(cur))
                        {
                            collar.initialize(cur);
                            maxCollar = std::max(maxCollar, cur);
                            for (ExtendedNeighborsIterator<Voxel> it2(cur);
                                 !it2.isEnd();
                                 it2.next())
                            {
                                it2.getCurrentValue(curX, curY, curZ);
                                collar.merge(cur, Voxel(curX, curY, curZ));
                            }
                        }
                    }
                }
            }
        }
    }

    collar.getComponents(components);
    _mergeSameVoidCollarComponents(collar, components, stack);

    Voxel rep = collar.find(maxCollar);
    for (std::vector<Voxel>& comp : components){
        if (collar.find(comp.at(0)) == rep){
            for (Voxel& cur : comp){
                outerCollar.initialize(cur);
                for (ExtendedNeighborsIterator<Voxel> it(cur);
                     !it.isEnd();
                     it.next())
                {
                    it.getCurrentValue(curX, curY, curZ);
                    outerCollar.merge(cur, Voxel(curX, curY, curZ));
                }
            }
        }
    }

    for (unsigned int curDistance = 1;
         curDistance <= inflationValue;
         ++curDistance)
    {
        for (Voxel v : _voxels.at(curDistance - 1)){
            for (ExtendedNeighborsIterator<Voxel> it(v);
                 !it.isEnd();
                 it.next())
            {
                it.getCurrentValue(curX, curY, curZ);
                Voxel cur(curX, curY, curZ);
                if (stack.isWithinBounds(curX, curY, curZ) &&
                        stack.getLevel(curX, curY, curZ) <
                        stack.getShapeThreshold() &&
                        _levels.find(cur) == _levels.end() &&
                        !outerCollar.contains(cur))
                {
                    count++;
                    _addAtLevel(cur, curDistance);
                }
            }
        }
    }

    return count;
}

int InflationShape::_constructShape(ProcessedImageStack &stack)
{
    const std::array<unsigned int, 3> &dimensions = stack.getDimensions();
    _voxels.resize(1);
    std::unordered_set<Voxel> toFind;
    ConnectedComponents<Voxel> collar;
    std::vector<std::vector<Voxel> > components;
    Voxel maxCollar(0, 0, 0);

    int count = 0;
    int curX, curY, curZ;
    for (unsigned int z = 0; z < dimensions[2]; z++) {
        for (unsigned int y = 0; y < dimensions[1]; y++) {
            for (unsigned int x = 0; x < dimensions[0]; x++) {
                unsigned char level = stack.getLevel(x, y, z);
                Voxel v(x, y, z);
                if (level >= stack.getShapeThreshold()) {
                    count++;
                    _addAtLevel(v, 0);
                } else if (level >= stack.getNeighborhoodThreshold()){
                    toFind.insert(v);
                }
                if (level >= stack.getNeighborhoodThreshold()) {
                    for (ExtendedNeighborsIterator<Voxel> it(v);
                         !it.isEnd();
                         it.next())
                    {
                        it.getCurrentValue(curX, curY, curZ);
                        Voxel cur(curX, curY, curZ);
                        if (stack.isWithinBounds(curX, curY, curZ) &&
                                stack.getLevel(curX, curY, curZ) <
                                stack.getNeighborhoodThreshold() &&
                                !collar.contains(cur))
                        {
                            collar.initialize(cur);
                            maxCollar = std::max(maxCollar, cur);
                            for (ExtendedNeighborsIterator<Voxel> it2(cur);
                                 !it2.isEnd();
                                 it2.next())
                            {
                                it2.getCurrentValue(curX, curY, curZ);
                                collar.merge(cur, Voxel(curX, curY, curZ));
                            }
                        }
                    }
                }
            }
        }
    }
    collar.getComponents(components);
    _mergeSameVoidCollarComponents(collar, components, stack);

    Voxel rep = collar.find(maxCollar);
    for (std::vector<Voxel>& comp : components){
        if (collar.find(comp.at(0)) == rep){
            for (Voxel& cur : comp){
                _localMaxima.initialize(cur);
                for (ExtendedNeighborsIterator<Voxel> it(cur);
                     !it.isEnd();
                     it.next())
                {
                    it.getCurrentValue(curX, curY, curZ);
                    _localMaxima.merge(cur, Voxel(curX, curY, curZ));
                }
            }
        }
    }

    unsigned int curDistance = 1;
    while (!toFind.empty() && !_voxels.at(curDistance - 1).empty()) {
        _voxels.push_back(voxel_container());
        for (Voxel v : _voxels.at(curDistance - 1)){
            for (ExtendedNeighborsIterator<Voxel> it(v);
                 !it.isEnd();
                 it.next())
            {
                it.getCurrentValue(curX, curY, curZ);
                Voxel cur(curX, curY, curZ);

                if (stack.isWithinBounds(curX, curY, curZ) &&
                        stack.getLevel(curX, curY, curZ) <
                        stack.getShapeThreshold() &&
                        _levels.find(cur) == _levels.end() &&
                        !_localMaxima.contains(cur))
                {
                    count++;
                    _addAtLevel(cur, curDistance);

                    if (stack.getLevel(curX, curY, curZ) >=
                            stack.getNeighborhoodThreshold())
                        toFind.erase(cur);
                }
            }
        }
        curDistance++;
    }
    if (_voxels.back().empty()) _voxels.pop_back();

    return count;
}

void InflationShape::_mergeSameVoidCollarComponents(
        ConnectedComponents<Voxel>& collar,
        const std::vector<std::vector<Voxel> >& components,
        ProcessedImageStack &stack)
{
    std::vector<Voxel> ccRep;
    int curX, curY, curZ;
    for (const std::vector<Voxel>& comp : components) {
        ccRep.push_back(comp.at(0));
        for (const Voxel& v : comp){
            ccRep.back() = std::max(v, ccRep.back());
        }
    }
    for (Voxel& v : ccRep) {
        std::array<int,3> p = {v[0], v[1], v[2]};

        bool done = false;
        for (unsigned i = 0; (i < 3) && !done; i++) {
            while (!done &&
                   static_cast<unsigned int>(p[i]) <
                   stack.getDimensions()[i] - 1)
            {
                p[i]++;
                Voxel cur(p);
                if (!collar.contains(cur) &&
                        stack.getLevel(cur[0], cur[1], cur[2]) <
                        stack.getNeighborhoodThreshold())
                {
                    collar.initialize(cur);
                    for (ExtendedNeighborsIterator<Voxel> it(cur);
                         !it.isEnd();
                         it.next()) {
                        it.getCurrentValue(curX, curY, curZ);
                        collar.merge(cur, Voxel(curX, curY, curZ));
                    }
                } else {
                    done = true;
                }
            }
        }
    }
}

void InflationShape::_computeVoidCollar()
{
    int curX, curY, curZ;
    for (unsigned int curLevel = 0; curLevel < _voxels.size(); ++curLevel){
        voxel_container& c = _voxels.at(curLevel);

        for (Voxel& v : c){
            for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next())
            {
                it.getCurrentValue(curX, curY, curZ);
                Voxel cur(curX, curY, curZ);

                if (_levels.find(cur) == _levels.end()){
                    _localMaxima.initialize(cur);
                    for (ExtendedNeighborsIterator<Voxel> it2(cur);
                         !it2.isEnd();
                         it2.next())
                    {
                        it2.getCurrentValue(curX, curY, curZ);
                        _localMaxima.merge(cur, Voxel(curX, curY, curZ));
                    }
                }
            }
        }
    }
}

void InflationShape::_computeLocalMaxima()
{
    ConnectedComponents<Voxel> cc;
    std::vector<std::vector<Voxel> > components;
    int curX, curY, curZ;

    for (Voxel& v : _voxels.at(0)){
        for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next())
        {
            it.getCurrentValue(curX, curY, curZ);
            Voxel cur(curX, curY, curZ);
            if (_levels.find(cur) == _levels.end()){
                _localMaxima.initialize(cur);
                for (ExtendedNeighborsIterator<Voxel> it2(cur);
                     !it2.isEnd();
                     it2.next())
                {
                    it2.getCurrentValue(curX, curY, curZ);
                    _localMaxima.merge(cur, Voxel(curX, curY, curZ));
                }
            }
        }
    }

    for (unsigned int curLevel = 1; curLevel < _voxels.size(); ++curLevel){
        voxel_container& c = _voxels.at(curLevel);
        cc.clear();
        components.clear();

        for (Voxel& v : c){
            cc.initialize(v);
            for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next())
            {
                it.getCurrentValue(curX, curY, curZ);
                Voxel cur(curX, curY, curZ);
                cc.merge(v, cur);

                if (_levels.find(cur) == _levels.end()){
                    _localMaxima.initialize(cur);
                    for (ExtendedNeighborsIterator<Voxel> it2(cur);
                         !it2.isEnd();
                         it2.next())
                    {
                        it2.getCurrentValue(curX, curY, curZ);
                        _localMaxima.merge(cur, Voxel(curX, curY, curZ));
                    }
                }
            }
        }

        cc.getComponents(components);

        for (std::vector<Voxel>& comp : components){
                bool isMaximum = true;
                for (unsigned int i = 0; i < comp.size() && isMaximum; ++i){
                    Voxel& v = comp.at(i);

                    for (ExtendedNeighborsIterator<Voxel> it(v);
                         !it.isEnd() && isMaximum;
                         it.next())
                    {
                        it.getCurrentValue(curX, curY, curZ);
                        Voxel cur(curX, curY, curZ);
                        if (_levels.find(cur) == _levels.end() ||
                                _levels.at(cur) > curLevel)
                            isMaximum = false;
                    }
                }

                if (isMaximum){
                    for (Voxel v : comp){
                        _localMaxima.initialize(v);
                        for (Voxel n : comp){
                            _localMaxima.merge(v, n);
                        }
                    }
                }
        }
    }
}

void InflationShape::_addAtLevel(Voxel& v, unsigned int level)
{
    if (level == 0) v.setLevel(MAIN_SHAPE);
    else v.setLevel(NEIGHBORHOOD);

    _voxels.at(level).push_back(v);
    _levels.emplace(v, level);

    if (v[0] > static_cast<int>(_shapeDimensions[0]))
        _shapeDimensions[0] = v[0];
    if (v[1] > static_cast<int>(_shapeDimensions[1]))
        _shapeDimensions[1] = v[1];
    if (v[2] > static_cast<int>(_shapeDimensions[2]))
        _shapeDimensions[2] = v[2];
}

template <>
bool InflationShape::contains<MAIN_SHAPE>(int x, int y, int z) const {
    auto it = _levels.find(Voxel(x, y, z));

    if (it == _levels.end()) return false;

    return it->second == 0;
}

template <>
bool InflationShape::contains<NEIGHBORHOOD>(int x, int y, int z) const {
    auto it = _levels.find(Voxel(x, y, z));

    return it != _levels.end();
}

template <>
bool InflationShape::contains<EX_NEIGHBORHOOD>(int x, int y, int z) const {
    auto it = _levels.find(Voxel(x, y, z));

    if (it == _levels.end()) return false;

    return it->second > 0;
}

unsigned int InflationShape::getMinLevel(Voxel v) {
    auto it = _levels.find(v);

    if (it == _levels.end()) return _voxels.size();

    return it->second;
}

unsigned int InflationShape::getMinLevel(int x, int y, int z) {
    auto it = _levels.find(Voxel(x, y, z));

    if (it == _levels.end()) return _voxels.size();

    return it->second;
}

const std::array<unsigned int, 3>& InflationShape::getShapeDimensions() const
{
    return _shapeDimensions;
}

void InflationShape::clear(){
    _levels.clear();
    _voxels.clear();
    _shapeDimensions[0] = 0;
    _shapeDimensions[1] = 0;
    _shapeDimensions[2] = 0;
}

unsigned int InflationShape::getShapeThreshold() const
{
    return _shapeThreshold;
}

unsigned int InflationShape::getNeighborhoodThreshold() const
{
    return _neighborhoodThreshold;
}

unsigned int InflationShape::getNeighborhoodSize(){
    unsigned int res = 0;

    for (voxel_container& c : _voxels){
        res += c.size();
    }

    return res;
}

unsigned int InflationShape::getShapeSize(){
    if (_voxels.empty()) return 0;
    return _voxels.at(0).size();
}

const InflationShape::voxel_container& InflationShape::getVoxelsAtDistance(
        unsigned int distance) const
{
    return _voxels.at(distance);
}

unsigned int InflationShape::getInflationValue() const
{
    return _voxels.size() - 1;
}

const ConnectedComponents<Voxel>& InflationShape::getLocalMaxima() const
{
    return _localMaxima;
}

unsigned int InflationShape::getNumberOfLevels()
{
    return _voxels.size();
}

} // namespace pseudo_void_filler

