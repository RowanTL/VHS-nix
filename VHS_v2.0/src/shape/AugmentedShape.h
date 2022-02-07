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
 * @file AugmentedShape.h
 * Contains the declaration of the \ref AugmentedShape class.
 **/

#include <array>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

#include "shape/BaseShape.h"
#include "shape/ConnectedComponents.h"
#include "shape/VoxelSkeletonIterators.h"
#include "shape/ShapeUtilities.h"
#include "util/Point.h"

namespace shape_simplification{ namespace shape {

using util::Point;

template <class Shape, class Voxel>
/**
 * class AugmentedShape
 * @brief Shape class which derives from its template parameter Shape.
 * It additionally keeps track of its connected components, loops and voids,
 * as well as its collar and related properties.
 * @tparam Shape Shape class it has to derive from.
 * @tparam Voxel Voxel class used by the shape.
 **/
class AugmentedShape : public Shape {
public:
    using voxel_container = typename Shape::voxel_container;
    template <Level level = NEIGHBORHOOD>
    using voxel_iterator = typename Shape::template voxel_iterator<level>;
    using collar_container = std::unordered_set<Voxel>;
    using collar_iterator = typename std::unordered_set<Voxel>::iterator;
    template <Level level>
    using neighbor_iterator =
        typename Shape::template neighbor_iterator<level>;
    template <Level level>
    using extended_neighbor_iterator =
        typename Shape::template extended_neighbor_iterator<level>;

    using CC = ConnectedComponents<Voxel>;
    typedef typename std::conditional<
        Shape::isRemovable(),
        ConnectedComponentsWithRemoval<Voxel>,
        ConnectedComponents<Voxel>
    >::type CCR;

    /**
     * @brief Constructor for an empty shape.
     */
    AugmentedShape();

    /**
     * @brief Constructor.
     * @param voxels Vector of voxels to be inside the shape.
     * The voxels' level has to be correctly setted.
     */
    AugmentedShape(std::vector<Voxel> &voxels);
    /**
     * @brief Constructor.
     * @param shape Shape to use as base.
     */
    AugmentedShape(Shape &shape);

    template <Level minLevel>
    /**
     * @brief Add a voxel to the shape to level @p minLevel and higher.
     * *Calling it will fail compiling if the given shape is not addable.*
     * @tparam minLevel Minimum level the voxel has to be added to.
     * @param v Voxel to be added.
     */
    void addVoxel(Voxel v);

    template <Level maxLevel>
    /**
     * @brief Remove a voxel from the shape from level @p maxLevel and lower.
     * *Calling it will fail compiling if the given shape is not removable.*
     * @tparam maxLevel Maximum level the voxel has to be removed from.
     * @param v Voxel to be removed.
     * @pre Removing @p v has to guarantee that the number of connected
     * components does not increase.
     */
    void removeVoxel(Voxel v);

    /**
     * @brief Returns the shape's Euler Characteristic.
     * @return The shape's Euler Characteristic.
     */
    int getEulerCharacteristic();

    /**
     * @brief Returns the number of connected components of the shape.
     * @return The number of connected components.
     */
    unsigned int getNumberOfComponents() const;

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
     * @brief Returns the size of the connected component
     * containing the given voxel.
     * @param v Voxel contained in the connected component.
     * @return The size of the connected component
     * containing @p v.
     */
    unsigned long long int getComponentSize(Voxel v);
    /**
     * @brief Returns the connected component
     * containing the given voxel.
     * @param v Voxel contained in the connected component.
     * @param container Reference to the container to fill.
     */
    void getComponent(Voxel v, std::vector<Voxel> &container);
    /**
     * @brief Tests if the given voxel is contained in a connected
     * component with core voxels.
     * @param v Given voxel.
     * @return True if @p v is contained in a connected component
     * with core voxels, false otherwise.
     */
    bool isInCoreComponent(Voxel v);

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

    template<unsigned int dim>
    /**
     * @brief Returns the number of dual cells of the shape in the given
     * dimension.
     * @tparam dim Dimension of the cells.
     * @return The number of dual cells of the shape in dimension @p dim.
     */
    long long getNumberOfDualCellsInDim();

    /**
     * @brief Clears the shape.
     */
    void clear();

private:
    CCR _components;                            ///< Structure for the connected components.
    collar_container _collar;                   ///< Voxels contained in the collar.
    unsigned int _numberOfCollarComponents;     ///< Number of connected components in the
                                                ///< collar. Not always up-to-date.
    bool _collarModifiedSinceLastCompUpdate;    ///< True if the collar was modified,
                                                ///< but the number of components was
                                                ///< not updated.
    std::array<long long, 4> _numberOfCells;    ///< Number of cells in dual skeleton of the shape.

    /**
     * @brief Updates the connected components after a voxel was added into
     * the shape.
     * @param v Added voxel.
     */
    void _updateComponentsAfterAddingVoxel(Voxel v);
    /**
     * @brief Updates the connected components after a voxel was removed from
     * the shape.
     * @param v Removed voxel.
     */
    void _updateComponentsAfterRemovingVoxel(Voxel v);

    /**
     * @brief Updates the collar after a voxel was added into the shape.
     * @param v Added voxel.
     */
    void _updateCollarAfterAddingVoxel(Voxel v);
    /**
     * @brief Updates the collar after a voxel was removed from the shape.
     * @param v Removed voxel.
     */
    void _updateCollarAfterRemovingVoxel(Voxel v);
    /**
     * @brief Tests if the given voxel is a collar voxel which was not
     * generated to connect collar components together.
     * @param v Voxel to test.
     * @return True or false.
     */
    bool _isRealCollarVoxel(Voxel v);
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
     * @brief Updates the number of cells in dual skeleton of
     * the shape after adding a voxel to shape.
     * @param v Voxel added to shape.
     */
    void _updateNumberOfCellsAfterAddingVoxel(Voxel v);
    /**
     * @brief Updates the number of cells in dual skeleton of
     * the shape after removing a voxel from shape.
     * @param v Voxel removed from shape.
     */
    void _updateNumberOfCellsAfterRemovingVoxel(Voxel v);
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

#include "AugmentedShape.txx"

}} // namespace shape_simplification::shape
