/*
 * Copyright 2020 St. Louis University
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
 * @file BaseShape.txx
 * Contains the implementation of the methods in the \ref BaseShape class.
 **/

#include "BaseShape.h"

using namespace shape_simplification::shape;

template <class Voxel>
BaseShape<Voxel>::BaseShape() : _shapeDimensions{0, 0, 0} {}

template <class Voxel>
BaseShape<Voxel>::BaseShape(std::vector<Voxel> &voxels,
                            std::vector<Level> &minLevels)
    : _shapeDimensions{0, 0, 0}
{
    for (typename std::vector<Voxel>::size_type i = 0; i < voxels.size(); ++i) {
        Level l = minLevels.at(i);
        Voxel v = voxels.at(i);
        if (l == CORE) {
            _voxels.at(0).insert(v);
            _voxels.at(1).insert(v);
            _voxels.at(2).insert(v);
        } else if (l == MAIN_SHAPE) {
            _voxels.at(1).insert(v);
            _voxels.at(2).insert(v);
        } else if (l == NEIGHBORHOOD) {
            _voxels.at(2).insert(v);
        }
        if (v[0] > _shapeDimensions[0])
            _shapeDimensions[0] = v[0];
        if (v[1] > _shapeDimensions[1])
            _shapeDimensions[1] = v[1];
        if (v[2] > _shapeDimensions[2])
            _shapeDimensions[2] = v[2];
    }
}

template <class Voxel>
constexpr bool BaseShape<Voxel>::isAddable() {
    return false;
}

template <class Voxel>
constexpr bool BaseShape<Voxel>::isRemovable() {
    return false;
}

template <class Voxel>
template <typename BaseShape<Voxel>::Level minLevel>
void BaseShape<Voxel>::addVoxel(Voxel v) {
    static_assert(minLevel == -1, "Insertion of voxel not enabled.");
}

template <class Voxel>
template <typename BaseShape<Voxel>::Level maxLevel>
void BaseShape<Voxel>::removeVoxel(Voxel v) {
    static_assert(maxLevel == -1, "Removal of voxel not enabled.");
}

template <class Voxel>
template <typename BaseShape<Voxel>::Level level>
bool BaseShape<Voxel>::contains(Voxel v) const {
    if constexpr (level == OUTSIDE){
        return !contains<NEIGHBORHOOD>(v);
    } else {
        return _voxels.at(level).find(v) != _voxels.at(level).end();
    }
}

template <class Voxel>
bool BaseShape<Voxel>::containsOnlyInNeighborhood(Voxel v) const{
    if (_voxels.at(NEIGHBORHOOD).find(v) == _voxels.at(NEIGHBORHOOD).end())
        return false;
    return _voxels.at(MAIN_SHAPE).find(v) == _voxels.at(MAIN_SHAPE).end();
}

template <class Voxel>
template <typename BaseShape<Voxel>::Level level>
typename BaseShape<Voxel>::voxel_iterator BaseShape<Voxel>::begin() const {
    static_assert(level != OUTSIDE,
            "It is not possible to iterate voxels not contained in Shape.");
    return _voxels.at(level).begin();
}

template <class Voxel>
template <typename BaseShape<Voxel>::Level level>
typename BaseShape<Voxel>::voxel_iterator BaseShape<Voxel>::end() const {
    static_assert(level != OUTSIDE,
            "It is not possible to iterate voxels not contained in Shape.");
    return _voxels.at(level).end();
}

template <class Voxel>
typename BaseShape<Voxel>::Level BaseShape<Voxel>::getMinLevel(Voxel v) {
    if (contains<CORE>(v))
        return CORE;
    if (contains<MAIN_SHAPE>(v))
        return MAIN_SHAPE;
    if (contains<NEIGHBORHOOD>(v))
        return NEIGHBORHOOD;
    return OUTSIDE;
}

template <class Voxel>
const std::array<unsigned int, 3>& BaseShape<Voxel>::getShapeDimensions() const {
    return _shapeDimensions;
}

template <class Voxel>
void BaseShape<Voxel>::clear(){
    _voxels[0].clear();
    _voxels[1].clear();
    _voxels[2].clear();
    _shapeDimensions[0] = 0;
    _shapeDimensions[1] = 0;
    _shapeDimensions[2] = 0;
}

template <class Voxel>
std::array<unsigned int, 3>& BaseShape<Voxel>::_getShapeDimensions() {
    return _shapeDimensions;
}

template <class Voxel>
std::array<typename BaseShape<Voxel>::voxel_container, 3>& BaseShape<Voxel>::_getVoxels(){
    return _voxels;
}

template <class Voxel>
inline unsigned int BaseShape<Voxel>::getNeighborhoodSize(){
    return _voxels.at(2).size();
}

template <class Voxel>
inline unsigned int BaseShape<Voxel>::getShapeSize(){
    return _voxels.at(1).size();
}

template <class Voxel>
inline unsigned int BaseShape<Voxel>::getCoreSize(){
    return _voxels.at(0).size();
}


