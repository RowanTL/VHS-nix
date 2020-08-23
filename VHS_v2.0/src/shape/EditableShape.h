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
 * @file EditableShape.h
 * Contains the declaration of the \ref EditableShape class.
 **/

#include <vector>

#include "AddableShape.h"
#include "RemovableShape.h"

namespace shape_simplification{ namespace shape {

template <class Voxel>
/**
 * class EditableShape
 * @brief Shape class which inherites from \ref AddableShape and \ref
 * RemovableShape. It has therefore the property to add and to remove Voxels
 * after instanciation.
 * @tparam Voxel Voxel class used by the shape.
 **/
class EditableShape : public AddableShape<Voxel>, public RemovableShape<Voxel> {
public:
    using AddableShape<Voxel>::isAddable;
    using AddableShape<Voxel>::addVoxel;
    using RemovableShape<Voxel>::isRemovable;
    using RemovableShape<Voxel>::removeVoxel;
    using typename BaseShape<Voxel>::Level;
    using BaseShape<Voxel>::CORE;
    using BaseShape<Voxel>::MAIN_SHAPE;
    using BaseShape<Voxel>::NEIGHBORHOOD;
    using BaseShape<Voxel>::OUTSIDE;
    template <Level level>
    using neighbor_iterator =
        typename BaseShape<Voxel>::template neighbor_iterator<level>;
    template <Level level>
    using extended_neighbor_iterator =
        typename BaseShape<Voxel>::template extended_neighbor_iterator<level>;
    using typename BaseShape<Voxel>::voxel_container;

    /**
     * @brief Constructor for an emty shape.
     */
    EditableShape() : BaseShape<Voxel>() {}
    /**
     * @brief Constructor.
     * @param voxels Vector of voxels to be inside the shape.
     * @param minLevels The minimal level a voxel has to be inserted to.
     * The corresponding voxel is the one with same index in @p voxels.
     */
    EditableShape(std::vector<Voxel> &voxels, std::vector<Level> &minLevels)
        : BaseShape<Voxel>(voxels, minLevels) {}
};

}} // namespace shape_simplification::shape
