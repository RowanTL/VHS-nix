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
 * @file InvertedRestrictedShapeCopy.txx
 * Contains the implementation of the methods in the
 * \ref InvertedRestrictedShapeCopy class.
 **/

#include "InvertedRestrictedShapeCopy.h"

using namespace shape_simplification::shape;

template <class Shape, class Voxel>
InvertedRestrictedShapeCopy<Shape, Voxel>::InvertedRestrictedShapeCopy(
        const Shape &shape,
        Point &lowerBound,
        Point &upperBound)
    : BaseShape<Voxel>(), _lowerBound(lowerBound), _upperBound(upperBound)
{
    int x, y, z;
    std::array<voxel_container, 3> &voxels = BaseShape<Voxel>::_getVoxels();
    for (auto it = shape.template begin<NEIGHBORHOOD>();
         it != shape.template end<NEIGHBORHOOD>();
         ++it)
    {
        Voxel v = *it;
        Level l = v.getLevel();
        if (_isWithinBounds(v)) {
            if (l == NEIGHBORHOOD) {
                v.setLevel(MAIN_SHAPE);
                voxels.at(1).insert(v);
                voxels.at(2).insert(v);
            } else if (l == MAIN_SHAPE) {
                v.setLevel(NEIGHBORHOOD);
                voxels.at(2).insert(v);
            }
            v.setLevel(l);
            for (ExtendedNeighborsInIterator<Shape,Voxel,OUTSIDE> it(v, shape);
                 !it.isEnd();
                 it.next()){
                it.getCurrentValue(x, y, z);
                Voxel cur(x, y, z);
                cur.setLevel(CORE);
                voxels.at(0).insert(cur);
                voxels.at(1).insert(cur);
                voxels.at(2).insert(cur);
                _updateDimensions(cur);
            }

            if (l != CORE) _updateDimensions(v);
        }
    }
}

template <class Shape, class Voxel>
constexpr Point InvertedRestrictedShapeCopy<Shape, Voxel>::getLowerBound() {
    return _lowerBound;
}

template <class Shape, class Voxel>
constexpr Point InvertedRestrictedShapeCopy<Shape, Voxel>::getUpperBound() {
    return _upperBound;
}

// Carefull about new core voxels which can be out of bound by 1,
// but still in the restricted shape!
template <class Shape, class Voxel>
bool InvertedRestrictedShapeCopy<Shape, Voxel>::_isWithinBounds(Voxel &v) {
    return (v[0] >= _lowerBound[0] && v[1] >= _lowerBound[1] &&
            v[2] >= _lowerBound[2] && v[0] <= _upperBound[0] &&
            v[1] <= _upperBound[1] && v[2] <= _upperBound[2]);
}

template <class Shape, class Voxel>
void InvertedRestrictedShapeCopy<Shape, Voxel>::_updateDimensions(Voxel &v){
    std::array<unsigned int, 3> &dimensions = BaseShape<Voxel>::_getShapeDimensions();
    if (v[0] > dimensions[0]) dimensions[0] = v[0];
    if (v[1] > dimensions[1]) dimensions[1] = v[1];
    if (v[2] > dimensions[2]) dimensions[2] = v[2];
}


