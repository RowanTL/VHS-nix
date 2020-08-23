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
 * @file RestrictedShapeCopy.txx
 * Contains the implementation of the methods in the \ref RestrictedShapeCopy
 * class.
 **/

#include "RestrictedShapeCopy.h"

using namespace shape_simplification::shape;

template <class Shape, class Voxel>
RestrictedShapeCopy<Shape, Voxel>::RestrictedShapeCopy(const Shape &shape,
                                                       Point &lowerBound,
                                                       Point &upperBound)
    : BaseShape<Voxel>(), _lowerBound(lowerBound), _upperBound(upperBound)
{
    std::array<voxel_container, 3> &voxels = BaseShape<Voxel>::_getVoxels();
    for (auto it = shape.template begin<NEIGHBORHOOD>();
         it != shape.template end<NEIGHBORHOOD>();
         ++it)
    {
        Voxel v = *it;
        if (_isWithinBounds(v)) {
            if (shape.template contains<CORE>(v)) {
                voxels.at(0).insert(v);
                voxels.at(1).insert(v);
                voxels.at(2).insert(v);
            } else if (shape.template contains<MAIN_SHAPE>(v)) {
                voxels.at(1).insert(v);
                voxels.at(2).insert(v);
            } else {
                voxels.at(2).insert(v);
            }
            std::array<unsigned int, 3> &dimensions = BaseShape<Voxel>::_getShapeDimensions();
            if (v[0] > dimensions[0])
                dimensions[0] = v[0];
            if (v[1] > dimensions[1])
                dimensions[1] = v[1];
            if (v[2] > dimensions[2])
                dimensions[2] = v[2];
        }
    }
}

template <class Shape, class Voxel>
constexpr Point RestrictedShapeCopy<Shape, Voxel>::getLowerBound() {
    return _lowerBound;
}

template <class Shape, class Voxel>
constexpr Point RestrictedShapeCopy<Shape, Voxel>::getUpperBound() {
    return _upperBound;
}

template <class Shape, class Voxel>
bool RestrictedShapeCopy<Shape, Voxel>::_isWithinBounds(Voxel &v) {
    return (v[0] >= _lowerBound[0] && v[1] >= _lowerBound[1] &&
            v[2] >= _lowerBound[2] && v[0] <= _upperBound[0] &&
            v[1] <= _upperBound[1] && v[2] <= _upperBound[2]);
}
