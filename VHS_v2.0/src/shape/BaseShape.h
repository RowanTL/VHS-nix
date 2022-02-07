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
 * @file BaseShape.h
 * Contains the declaration of the \ref BaseShape class.
 **/

#include <array>
#include <vector>
#include <unordered_set>

#include "shape/ShapeUtilities.h"

namespace shape_simplification{ namespace shape {

template <class Voxel>
/**
 * class BaseShape
 * @brief The most basic shape class.
 * @tparam Voxel Voxel class used by the shape.
 **/
class BaseShape {
public:
    using voxel_container = std::unordered_set<Voxel>;
    template <Level level = NEIGHBORHOOD>
    using voxel_iterator = typename voxel_container::const_iterator;

    template <Level level>
    using neighbor_iterator =
        NeighborsInIterator<BaseShape<Voxel>, Voxel, level>;
    template <Level level>
    using extended_neighbor_iterator =
        ExtendedNeighborsInIterator<BaseShape<Voxel>, Voxel, level>;

    /**
     * @brief Constructor for an empty shape.
     */
    BaseShape();
    /**
     * @brief Constructor.
     * @param voxels Vector of voxels to be inside the shape.
     * The voxels' level has to be correctly setted.
     */
    BaseShape(std::vector<Voxel> &voxels);

    /**
     * @brief Test if the class has the property to add voxels to the shape
     * after instanciation.
     * @return False.
     */
    static constexpr bool isAddable();

    /**
     * @brief Test if the class has the property to remove voxels from the
     * shape after instanciation.
     * @return False.
     */
    static constexpr bool isRemovable();

    /**
     * @brief Add a voxel to the shape to level @p minLevel and higher.
     * *It will fail compiling if called for this class.*
     * @tparam minLevel Minimum level the voxel has to be added to.
     * @param v Voxel to be added.
     */
    template <Level minLevel> void addVoxel(Voxel v);

    /**
     * @brief Remove a voxel from the shape from level @p maxLevel and lower.
     * *It will fail compiling if called for this class.*
     * @tparam maxLevel Maximum level the voxel has to be removed from.
     * @param v Voxel to be removed.
     */
    template <Level maxLevel> void removeVoxel(Voxel v);

    /**
     * @brief Test if the shape contains a voxel at level @p level.
     * @tparam level Level to be tested.
     * @param x X-coordinate of the voxel.
     * @param y Y-coordinate of the voxel.
     * @param z Z-coordinate of the voxel.
     * @return True if the shape contains the given voxel at level @p level,
     * false otherwise.
     */
    template <Level level = NEIGHBORHOOD>
    bool contains(int x, int y, int z) const;

    /**
     * @brief Returns an iterator at the start of the voxels contained at level
     * @p level.
     * @tparam level Level of the voxels to iterate over.
     * @return Starting iterator for level @p level.
     */
    template <Level level = NEIGHBORHOOD>
    voxel_iterator<level> begin() const;

    /**
     * @brief Returns the end iterator of the voxels contained at level
     * @p level.
     * @tparam level Level of the voxels to iterate over.
     * @return End iterator for level @p level.
     */
    template <Level level = NEIGHBORHOOD>
    voxel_iterator<level> end() const;

    /**
     * @brief Returns the lowest level the given voxel is contained in.
     * @param x X-coordinate of the voxel.
     * @param y Y-coordinate of the voxel.
     * @param z Z-coordinate of the voxel.
     * @return The lowest level the given voxel is contained in.
     */
    Level getMinLevel(int x, int y, int z);

    /**
     * @brief Returns an array width all three maximal coordinate
     * values of a voxel in the shape.
     * @return An array where [0] is the maximal x coordinate,
     * [1] the maximal y coordinate and [2] the maximal z coordinate.
     */
    const std::array<unsigned int, 3> &getShapeDimensions() const;

    /**
     * @brief Returns total number of voxels.
     * @return Total number of voxels contained in the class.
     */
    unsigned int getNeighborhoodSize();
    /**
     * @brief Returns number of voxels contained in the shape,
     * excluding neighborhood.
     * @return Number of voxels contained in the shape.
     */
    unsigned int getShapeSize();
    /**
     * @brief Returns number of voxels in the core of the shape.
     * @return Number of voxels contained in the core.
     */
    unsigned int getCoreSize();

    /**
     * @brief Clears the shape.
     */
    void clear();

    /**
     * @brief Returns the number of voxel levels that are enabled.
     * @return 3.
     */
    static constexpr unsigned int getNumberOfLevels();

protected:
    /**
     * @brief Returns the voxel container.
     * @return A reference to the voxel container.
     */
    std::array<voxel_container, 3> &_getVoxels();
    /**
     * @brief Returns an array width all three maximal coordinate
     * values of a voxel in the shape.
     * @return An array where [0] is the maximal x coordinate,
     * [1] the maximal y coordinate and [2] the maximal z coordinate.
     */
    std::array<unsigned int, 3> &_getShapeDimensions();

private:
    std::array<voxel_container, 3>
    _voxels;                        ///< Voxels contained in the shape.
                                    ///< @p _voxels[0] corresponds to the core.
                                    ///< @p _voxels[1] corresponds to the main shape.
                                    ///< @p _voxels[2] corresponds to the neighborhood.
    std::array<unsigned int, 3>
    _shapeDimensions;               ///< Maximal value of a Voxel coordinate.
};

#include "BaseShape.txx"

}} // namespace shape_simplification::shape
