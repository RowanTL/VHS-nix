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
 * @file AddableShape.h
 * Contains the declaration of the \ref AddableShape class.
 **/

#include "BaseShape.h"
#include "shape/ShapeUtilities.h"

namespace shape_simplification{ namespace shape {

template <class Voxel>
/**
 * class AddableShape
 * @brief Shape class which inherits from \ref BaseShape.
 * It has the additional property that new Voxels can be added to it after
 * instantiation.
 * @tparam Voxel Voxel class used by the shape.
 **/
class AddableShape : virtual public BaseShape<Voxel> {
public:    
    template <Level level>
    using neighbor_iterator =
        typename BaseShape<Voxel>::template neighbor_iterator<level>;
    template <Level level>
    using extended_neighbor_iterator =
        typename BaseShape<Voxel>::template extended_neighbor_iterator<level>;
    using voxel_container = typename BaseShape<Voxel>::voxel_container;

    /**
     * @brief Constructor for an empty shape.
     */
    AddableShape();
    /**
     * @brief Constructor.
     * @param voxels Vector of voxels to be inside the shape.
     * The voxels' level has to be correctly setted.
     */
    AddableShape(std::vector<Voxel> &voxels);

    /**
     * @brief Test if the class has the property to add voxels to the shape
     * after instantiation.
     * @return True.
     */
    static constexpr bool isAddable();

    template <Level minLevel>
    /**
     * @brief Add a voxel to the shape to level @p minLevel and higher.
     * @tparam minLevel Minimum level the voxel has to be added to.
     * @param v Voxel to be added.
     */
    void addVoxel(Voxel v);
};

#include "AddableShape.txx"

}} // namespace shape_simplification::shape
