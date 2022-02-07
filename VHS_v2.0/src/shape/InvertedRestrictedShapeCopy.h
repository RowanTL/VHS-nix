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

#pragma once

/**
 * @file InvertedRestrictedShapeCopy.h
 * Contains the declaration of the \ref InvertedRestrictedShapeCopy class.
 **/

#include <array>

#include "shape/BaseShape.h"
#include "shape/ShapeUtilities.h"
#include "util/Point.h"

namespace shape_simplification{ namespace shape {

using util::Point;

template <class Shape, class Voxel>
/**
 * class InvertedRestrictedShapeCopy
 * @brief Shape class which restrict a given shape instanciation to a smaller
 * cube of coordinates and then inverts the domains (core/outside,
 * shape/neighborhood). It copies the corresponding voxel from a given shape
 * and inherits from \ref BaseShape.
 * /!\ Not all outside voxels are inverted. Only outside voxels directly
 * adjacent to the neighborhood are inverted into core voxels.
 * @tparam Shape Shape class to be restricted.
 * @tparam Voxel Voxel class used by the shape.
 **/
class InvertedRestrictedShapeCopy : virtual public BaseShape<Voxel> {
public:
    template <Level level = NEIGHBORHOOD>
    using voxel_iterator =
        typename BaseShape<Voxel>::template voxel_iterator<level>;
    template <Level level>
    using neighbor_iterator =
        typename BaseShape<Voxel>::template neighbor_iterator<level>;
    template <Level level>
    using extended_neighbor_iterator =
        typename BaseShape<Voxel>::template extended_neighbor_iterator<level>;
    using voxel_container = typename BaseShape<Voxel>::voxel_container;

    /**
     * @brief Constructor
     * @param shape Shape to be restricted.
     * @param lowerBound Left bottom corner of the restricted area.
     * @param upperBound Right top corner of the restricted area.
     */
    InvertedRestrictedShapeCopy(
            const Shape &shape, Point &lowerBound, Point &upperBound);

    /**
     * @brief Returns the left bottom corner of the restricted area.
     * @return Left bottom corner of the restricted area.
     */
    constexpr Point getLowerBound();
    /**
     * @brief Returns the right top corner of the restricted area.
     * @return Right top corner of the restricted area.
     */
    constexpr Point getUpperBound();

private:
    const Point _lowerBound; ///< Left bottom corner of the restricted area.
    const Point _upperBound; ///< Right top corner of the restricted area.

    /**
     * @brief Tests if the given voxel is inside the restricted area.
     * @param v Voxel to be tested.
     * @return True if it is inside the restricted area, false otherwise.
     */
    bool _isWithinBounds(Voxel &v);

    /**
     * @brief Updates dimensions of shape after adding given Voxel.
     * @param v Voxel added in the shape.
     */
    void _updateDimensions(Voxel &v);
};

#include "InvertedRestrictedShapeCopy.txx"

}} // namespace shape_simplification::shape
