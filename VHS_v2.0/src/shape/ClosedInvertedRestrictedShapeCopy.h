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

#pragma once

/**
 * @file ClosedInvertedRestrictedShapeCopy.h
 * Contains the declaration of the \ref ClosedInvertedRestrictedShapeCopy class.
 **/

#include <array>
#include <cmath>

#include "shape/InvertedRestrictedShapeCopy.h"
#include "util/Point.h"

namespace shape_simplification{ namespace shape {

using util::Point;

template <class Shape, class Voxel>
/**
 * class ClosedRestrictedShape
 * @brief Shape class which restrict a given shape instanciation to a smaller
 * cube of coordinates and adds a border to the cube.
 * It inherits from \ref RestrictedShapeCopy.
 * @tparam Shape Shape class to be restricted.
 * @tparam Voxel Voxel class used by the shape.
 **/
class ClosedInvertedRestrictedShapeCopy : public InvertedRestrictedShapeCopy<Shape,Voxel> {
public:
    using Level = typename InvertedRestrictedShapeCopy<Shape,Voxel>::Level;
    using InvertedRestrictedShapeCopy<Shape,Voxel>::CORE;
    using InvertedRestrictedShapeCopy<Shape,Voxel>::MAIN_SHAPE;
    using InvertedRestrictedShapeCopy<Shape,Voxel>::NEIGHBORHOOD;
    using InvertedRestrictedShapeCopy<Shape,Voxel>::OUTSIDE;
    using voxel_iterator =
        typename InvertedRestrictedShapeCopy<Shape,Voxel>::voxel_iterator;
    template <Level level>
    using neighbor_iterator =
        typename BaseShape<Voxel>::template neighbor_iterator<level>;
    template <Level level>
    using extended_neighbor_iterator =
        typename BaseShape<Voxel>::template extended_neighbor_iterator<level>;
    using voxel_container =
        typename BaseShape<Voxel>::voxel_container;

    /**
     * @brief Constructor
     * @param shape Shape to be restricted.
     * @param lowerBound Left bottom corner of the restricted area.
     * @param upperBound Right top corner of the restricted area.
     */
    ClosedInvertedRestrictedShapeCopy(const Shape &shape, Point &lowerBound, Point &upperBound);

private:
    /**
     * @brief Tests if the given voxel is inside the restricted area.
     * @param v Voxel to be tested.
     * @return True if it is inside the restricted area, false otherwise.
     */
    bool _isWithinBounds(Voxel &v);
};

#include "ClosedInvertedRestrictedShapeCopy.txx"

}} // namespace shape_simplification::shape
