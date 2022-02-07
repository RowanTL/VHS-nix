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
 * @file InvertedAugmentedShape.h
 * Contains the declaration of the \ref InvertedAugmentedShape class.
 **/

#include <array>
#include <unordered_set>
#include <unordered_map>

#include "shape/BaseShape.h"
#include "shape/ConnectedComponents.h"
#include "shape/VoxelSkeletonIterators.h"
#include "shape/ShapeUtilities.h"
#include "util/Point.h"

namespace shape_simplification{ namespace shape {

using util::Point;

template <class Shape, class Voxel>
/**
 * class InvertedAugmentedShape
 * @brief Augmented shape that inverts core/outside, shape/neighborhood.
 * /!\ Not all outside voxels are inverted. Only outside voxels directly
 * adjacent to the neighborhood are inverted into core voxels. Therefore,
 * the homology and the collar are not the one of the real inverted shape.
 * /!\ Contrary to the \ref AugmentedShape class, this shape is not editable.
 * @tparam Shape Shape class it has to derive from.
 * @tparam Voxel Voxel class used by the shape.
 **/
class InvertedAugmentedShape {
public:
    using voxel_container = typename Shape::voxel_container;
    template <Level level = NEIGHBORHOOD>
    using voxel_iterator = typename Shape::template voxel_iterator<level>;
    using collar_container = std::unordered_set<Voxel>;
    using collar_iterator = typename std::unordered_set<Voxel>::iterator;
    template <Level level>
    using neighbor_iterator =
        NeighborsInIterator<InvertedAugmentedShape<Shape,Voxel>, Voxel, level>;
    template <Level level>
    using extended_neighbor_iterator =
        ExtendedNeighborsInIterator<
            InvertedAugmentedShape<Shape,Voxel>, Voxel, level>;

    using CC = ConnectedComponents<Voxel>;

    /**
     * @brief Constructor.
     * @param shape
     */
    InvertedAugmentedShape(Shape &shape);

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
     * @brief Tests if the shape contains a voxel at level @p level.
     * @tparam level Level to be tested.
     * @param v Voxel to be tested.
     * @return True if the shape contains @p v at level @p level, false
     * otherwise.
     */
    template <Level level> bool contains(int x, int y, int z) const;

    /**
     * @brief Returns an array width all three maximal coordinate
     * values of a voxel in the shape.
     * @return An array where [0] is the maximal x coordinate,
     * [1] the maximal y coordinate and [2] the maximal z coordinate.
     */
    const std::array<unsigned int, 3> &getShapeDimensions() const;

    /**
     * @brief Returns the shape's Euler Characteristic.
     * @return The shape's Euler Characteristic.
     */
    int getEulerCharacteristic();

    /**
     * @brief Returns the number of connected components of the shape.
     * @return The number of connected components.
     */
    int getNumberOfComponents() const;

    /**
     * @brief Returns the number of loops in the shape.
     * @return The number of loops.
     */
    unsigned int getNumberOfLoops();

    /**
     * @brief Returns the number of voids in the shape.
     * @return The number of voids.
     */
    unsigned int getNumberOfVoids();

    /**
     * @brief Returns the representative of the connected
     * component the given voxel is contained in.
     * @param v Voxel
     * @return The representative of the connected
     * component @p v is contained in.
     */
    Voxel getComponentRepresentative(Voxel v);

    /**
     * @brief Returns a reference to the current collar.
     * @return A reference to the current collar.
     */
    const collar_container &getCollar() const;

    /**
     * @brief Tests if the given voxel is in the collar.
     * @param v Voxel to test.
     * @return True if @p v is in the collar, false otherwise.
     */
    bool isInCollar(Voxel v) const;

    /**
     * @brief Retruns reference to underying shape.
     * @return Reference to underying shape.
     */
    const Shape& getUnderlyingShape();

private:
    Shape &_shape;                              ///< Underlying shape.
    CC _components;                             ///< Structure for the connected components.
    collar_container _collar;                   ///< Voxels contained in the collar.
    unsigned int _numberOfCollarComponents;     ///< Number of connected components in the
                                                ///< collar. Not always up-to-date.
    bool _collarModifiedSinceLastCompUpdate;    ///< True if the collar was modified,
                                                ///< but the number of components was
                                                ///< not updated.
    std::array<unsigned int, 3> _dimensions;    ///< Maximal value of a Voxel coordinate.
    std::array<long long, 4> _numberOfCells;    ///< Number of cells in dual skeleton of the shape.

    /**
     * @brief Updates the shape information for the addition of
     * the given voxel in the shape.
     * @param v Voxel to be added.
     */
    void _addVoxel(Voxel v);

    /**
     * @brief Updates the Euler Characteristic.
     */
    void _updateEulerCharacteristic();

    /**
     * @brief Updates the connected components after a voxel was added
     * into the shape.
     * @param v Added voxel.
     */
    void _updateComponentsAfterAddingVoxel(Voxel v);

    /**
     * @brief Updates the collar after a voxel was added into the shape.
     * @param v Added voxel.
     */
    void _updateCollarAfterAddingVoxel(Voxel v);
    /**
     * @brief Updates the number of connected components in the collar.
     */
    void _updateCollarComponents();
    /**
     * @brief Auxiliary function of _updateCollarComponents().
     * @param collarComponents
     */
    void _updateCollarComponentsFindCollarComponent(
            CC &collarComponents);
    /**
     * @brief Auxiliary function of _updateCollarComponents().
     * @param collarComponents
     * @param componentRep
     */
    void _updateCollarComponentsFindComponentRepresentative(
            CC &collarComponents,
            std::unordered_map<Voxel,Voxel>& componentRep);
    /**
     * @brief Auxiliary function of _updateCollarComponents().
     * @param collarComponents
     * @param componentRep
     */
    void _updateCollarComponentsMergeSameVoidComponents(
            CC &collarComponents,
            std::unordered_map<Voxel,Voxel>& componentRep);

    /**
     * @brief Updates dimensions of shape after adding given Voxel.
     * @param v Voxel added in the shape.
     */
    void _updateDimensions(Voxel &v);

    /**
     * @brief Updates the number of cells in dual skeleton of
     * the shape after adding a voxel to shape.
     * @param v Voxel added to shape.
     */
    void _updateNumberOfCellsAfterAddingVoxel(Voxel v);
    /**
     * @brief Returns the number of edges corresponding to
     * the given Voxel in the dual skeleton.
     * @param v Voxel generating the edges.
     * @return Number of edges corresponding to
     * @p v in the dual skeleton.
     */
    int _getNberOfGeneratedEdges(Voxel v);
    /**
     * @brief Returns the number of squares corresponding to
     * the given Voxel in the dual skeleton.
     * @param v Voxel generating the squares.
     * @return Number of edges corresponding to
     * @p v in the dual skeleton.
     */
    int _getNberOfGeneratedSquares(Voxel v);
    /**
     * @brief Returns the number of cubes corresponding to
     * the given Voxel in the dual skeleton.
     * @param v Voxel generating the cubes.
     * @return Number of edges corresponding to
     * @p v in the dual skeleton.
     */
    int _getNberOfGeneratedCubes(Voxel v);
};

#include "InvertedAugmentedShape.txx"

}} // namespace shape_simplification::shape
