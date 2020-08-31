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
 * @file TheoreticalOptimum.h
 * Contains the declaration of the \ref TheoreticalOptimum class.
 **/

#include <ctime>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "phat/boundary_matrix.h"
#include "phat/compute_persistence_pairs.h"
#include "phat/representations/bit_tree_pivot_column.h"
#include "phat/algorithms/twist_reduction.h"
#include "phat/helpers/dualize.h"

#include "util/FileIO.h"
#include "util/BaseImageStack.h"
#include "util/ProcessedImageStack.h"
#include "shape/Voxel.h"
#include "shape/EditableShape.h"
#include "shape/DualSkeleton.h"
#include "shape/Cell.h"

namespace shape_simplification { namespace optimum {

using shape_simplification::util::BaseImageStack;
using shape_simplification::util::ProcessedImageStack;
using shape_simplification::shape::Voxel;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::DualSkeleton;

/**
 * class TheoreticalOptimum
 * @brief Computes the theoretical optimum for the betti numbers of the inclusion
 * from the core to the neighborhood.
 **/
class TheoreticalOptimum {
public:
    using Shape = EditableShape<Voxel>;
    using Skeleton = DualSkeleton<Shape, Voxel, 0, 3>;
    using index = phat::index;
    using timestamp_type = Cell<0>::timestamp_type;
    using dim_type = phat::dimension;
    using matrix_type = phat::boundary_matrix<phat::bit_tree_pivot_column>;
    template <int dim>
    using cell_iterator = CellContainer::cell_iterator<dim>;

    /**
     * @brief Constructor.
     */
    TheoreticalOptimum();
    /**
     * @brief Destructor.
     */
    ~TheoreticalOptimum();

    /**
     * @brief Loads shape from a stack of png images.
     * The images are loaded 1-to-1, so, they should already be processed as desired.
     * Recalling this method will overwrite the previous shape.
     * @param inputFileNameBase Base for the input file names.
     * Each image should end with 'xxxx.png', were 'xxxx' is the position of the image in
     * the stack written with four digits. The base is was preceed this number
     * (with the absolute or relative path).
     * @param numberOfImages The number of images in the stack to load.
     */
    void loadPreprocessedImage(std::string inputFileNameBase,
                               unsigned int numberOfImages,
                               unsigned char coreThreshold = 200,
                               unsigned char shapeThreshold = 150,
                               unsigned char neighborhoodThreshold = 100);
    /**
     * @brief Prints out the theoretical optimum for the latest loaded shape.
     */
    void printTheoreticalOptimum();

private:
    std::array<unsigned int, 3> _minBettyNumbers;   ///< Optimal Betty numbers.
    Shape _shape;                                   ///< Latest loaded shape.
    unsigned int _nberOfCoreCells;                  ///< Number of cells in the core of @p _shape.

    /**
     * @brief computes the optimum.
     */
    void _computeOptimum();
    /**
     * @brief Constructs the boundary matrix to compute the optimum.
     * @param skeleton Dual skeleton of the shape.
     * @param matrix Matrix to fill.
     * @param indexToTimestamp Map from cell index to cell filtration value.
     */
    void _constructMatrix(Skeleton &skeleton,
                          matrix_type& matrix,
                          std::vector<timestamp_type>& indexToTimestamp);
    template <unsigned int dim>
    /**
     * @brief Constructs the filtration to compute the optimum.
     * @tparam dim Dimension of the cells.
     * @param skeleton
     * @param cells
     */
    void _constructFiltration(Skeleton &skeleton, CellContainer &cells);
    /**
     * @brief Inserts 0-dimensional columns into the boundary matrix.
     * @param cellIt
     * @param endIt
     * @param timestamp
     * @param current
     * @param cellToIndex
     * @param matrix
     * @param indexToTimestamp
     */
    void _addVertexColumns(
            cell_iterator<0> &cellIt,
            cell_iterator<0> &endIt,
            int timestamp, index &current,
            std::unordered_map<Cell<0>, index, Cell<0>::CellHasher> &cellToIndex,
            matrix_type &matrix,
            std::vector<timestamp_type>& indexToTimestamp);
    template <unsigned int dim>
    /**
     * @brief Inserts @p dim-dimensional columns into the boundary matrix.
     * @tparam dim Dimension of the cells.
     * @param cellIt
     * @param endIt
     * @param timestamp
     * @param current
     * @param cellToIndex
     * @param boundaryCellToIndex
     * @param matrix
     * @param indexToTimestamp
     */
    void
    _addColumns(cell_iterator<dim> &cellIt,
                cell_iterator<dim> &endIt,
                int timestamp, index &current,
                std::unordered_map<Cell<dim>, index,
                typename Cell<dim>::CellHasher> &cellToIndex,
                std::unordered_map<Cell<dim - 1>, index,
                typename Cell<dim - 1>::CellHasher>
                &boundaryCellToIndex,
                matrix_type &matrix,
                std::vector<timestamp_type>& indexToTimestamp);

    /**
     * @brief Prints out the theoretical optimum for the latest loaded shape.
     */
    void _printTheoreticalOptimum();

    /**
     * @brief Prints given message.
     * @param message Message to print.
     */
    void _printMessage(std::string message);
};

}} // shape_simplification::optimum

