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
 * @file BaseSkeleton.h
 * Contains the declaration of the \ref BaseSkeleton class.
 **/

#include <array>

#include "shape/Cell.h"
#include "shape/ShapeUtilities.h"

namespace shape_simplification{ namespace shape {

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
/**
 * class BaseSkeleton
 * @brief The most basic skeleton class.
 * @tparam Shape Shape class used to construct the skeleton.
 * @tparam Voxel Voxel class of the shape.
 * @tparam minDim Minimal dimension of the skeleton.
 * @tparam maxDim Maximal dimension of the skeleton.
 **/
class BaseSkeleton {
public:
    using vertex = Cell<0>;
    using vertex_container = CellContainer::vertex_container;
    using vertex_iterator = typename vertex_container::iterator;
    using edge = Cell<1>;
    using edge_container = CellContainer::edge_container;
    using edge_iterator = typename edge_container::iterator;
    using square = Cell<2>;
    using square_container = CellContainer::square_container;
    using square_iterator = typename square_container::iterator;
    using cube = Cell<3>;
    using cube_container = CellContainer::cube_container;
    using cube_iterator = typename cube_container::iterator;
    template <int dim>
    using cell_container = CellContainer::cell_container<dim>;
    template <int dim>
    using cell_iterator = CellContainer::cell_iterator<dim>;

    /**
     * @brief Tests if the class has the property to add
     * voxels to the skeleton after instanciation.
     * @return True if addable, false otherwise.
     */
    static constexpr bool isAddable();

    /**
     * @brief Tests if the class has the property to remove
     * voxels from the skeleton after instanciation.
     * @return True if removable, false otherwise.
     */
    static constexpr bool isRemovable();

    /**
     * @brief Returns the minimal dimension the skeleton has been build for.
     * @return The minimal dimension of the skeleton.
     */
    static constexpr unsigned int getMinDim();

    /**
     * @brief Returns the minimal dimension the skeleton has been build for.
     * @return The minimal dimension of the skeleton.
     */
    static constexpr unsigned int getMaxDim();

    template<unsigned int dim>
    /**
     * @brief Returns the begin iterator of the container
     * for cells of dimension @p dim.
     * @tparam dim Dimension of the cell to iterate.
     * @return Begin iterator.
     */
    cell_iterator<dim> begin();

    template<unsigned int dim>
    /**
     * @brief Returns the end iterator of the container
     * for cells of dimension @p dim.
     * @tparam dim Dimension of the cell to iterate.
     * @return End iterator.
     */
    cell_iterator<dim> end();

    template<unsigned int dim>
    /**
     * @brief Returns the number of cells of dimension @p dim
     * contained in the skeleton.
     * @tparam dim Dimension of the cells.
     * @return Number of cells of dimension @p dim.
     */
    unsigned int getNumberOfCells() const;

    template<unsigned int dim>
    /**
     * @brief Returns the number of cells of dimension @p dim
     * contained in the skeleton of the underlying main shape.
     * @tparam dim Dimension of the cells.
     * @return Number of cells of dimension @p dim.
     */
    unsigned int getNumberOfCellsInMainShape() const;

    /**
     * @brief Returns the Euler Characteristic of the underlying cubical
     * complex which is contained in the main shape.
     * @return The Euler Characteristic of the underlying cubical complex
     * in the main shape.
     */
    int getEulerCharacteristic() const;

    /**
     * @brief Returns the skeleton's cell container.
     * @return Reference to the cell container.
     */
    const CellContainer& getCellContainer() const;

protected:
    /**
     * @brief Constructor.
     * @param shape Shape associated to the skeleton.
     */
    BaseSkeleton(Shape &shape);
    /**
     * @brief Empty Skeleton Constructor.
     */
    BaseSkeleton();
    /**
     * @brief Destructor.
     */
    ~BaseSkeleton();

    template<unsigned int dim>
    /**
     * @brief Insert a cell in the skeleton.
     * @param cell Cell to be inserted.
     * @pre Given cell is not already contained in the skeleton.
     */
    void _insertCell(Cell<dim>& cell);
    /**
     * @brief Returns shape.
     * @return Reference to the shape.
     */
    Shape &_getShape();

private:
    Shape &_shape;              ///< Reference to the shape the skeleton is build from.
    CellContainer _cells;       ///< Cell container for all dimensions.
    std::array<unsigned int, 4>
    _nberOfCellsInMainShape;    ///< In main shape:
                                ///< [0]: number of vertices,
                                ///< [1]: number of edges,
                                ///< [2]: number of squares,
                                ///< [3]: number of cubes
};

#include "BaseSkeleton.txx"

}} // namespace shape_simplification::shape
