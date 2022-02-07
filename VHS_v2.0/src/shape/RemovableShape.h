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
 * @file RemovableShape.h
 * Contains the declaration of the \ref RemovableShape class.
 **/

#include <vector>

#include "shape/BaseShape.h"
#include "shape/ShapeUtilities.h"

namespace shape_simplification{ namespace shape {

template <class Voxel>
/**
 * class RemovableShape
 * @brief Shape class which inherites from \ref BaseShape.
 * It has the additional property that Voxels can be removed from it after
 * instanciation.
 * @tparam Voxel Voxel class used by the shape.
 **/
class RemovableShape : virtual public BaseShape<Voxel> {
public:
    template <Level level>
    using neighbor_iterator =
        typename BaseShape<Voxel>::template neighbor_iterator<level>;
    template <Level level>
    using extended_neighbor_iterator =
        typename BaseShape<Voxel>::template extended_neighbor_iterator<level>;
    using voxel_container = typename BaseShape<Voxel>::voxel_container;

    /**
     * @brief Constructor for an emty shape.
     */
    RemovableShape();
    /**
     * @brief Constructor.
     * @param voxels Vector of voxels to be inside the shape.
     * @param minLevels The minimal level a voxel has to be inserted to.
     * The corresponding voxel is the one with same index in @p voxels.
     */
    RemovableShape(std::vector<Voxel> &voxels);

    /**
     * @brief Test if the class has the property to remove voxels from the
     * shape after instanciation.
     * @return True.
     */
    static constexpr bool isRemovable();
    /**
     * @brief Remove a voxel from the shape from level @p maxLevel and lower.
     * @tparam maxLevel Maximum level the voxel has to be removed from.
     * @param v Voxel to be removed.
     */
    template <Level maxLevel> void removeVoxel(Voxel v);
};

#include "RemovableShape.txx"

}} // namespace shape_simplification::shape
