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
 * @file BoundaryMatrix.h
 * Contains the declaration of the \ref BoundaryMatrix class.
 **/

#include "phat/representations/bit_tree_pivot_column.h"
#include "phat/boundary_matrix.h"

#include "util/Point.h"

namespace shape_simplification{ namespace persistence {

using util::Point;

/**
 * class BoundaryMatrix
 * @brief Boundary Matrix used by the \ref PersistentHomology class.
 * It consists of two matrices: the boundary matrix and the identity matrix,
 * such that the inverse of the boundary matrix is computed when the boundary
 * matrix is reduced.
 **/
class BoundaryMatrix {
public:
    using boundary_matrix_type =
        phat::boundary_matrix<phat::bit_tree_pivot_column>;
    using index = phat::index;
    using dim_type = phat::dimension;

    /**
     * @brief Constructor.
     */
    BoundaryMatrix();
    /**
     * @brief Destructor.
     */
    ~BoundaryMatrix();

    /**
     * @brief Sets a column of the matrices.
     * @param colIndex Index of the column to be set.
     * @param dim Dimension of the column.
     * @param col The new values of the column.
     * @pre \ref setNumberOfColumns was correctly called beforehand.
     */
    void addColumn(index colIndex, dim_type dim, std::vector<index> &col);
    /**
     * @brief Sets the number of columns in the matrices.
     * @param nber The number of columns.
     */
    void setNumberOfColumns(int nber);
    /**
     * @brief Returns the number of columns as they were set.
     * @return The number of columns.
     */
    index getNumberOfColumns() const;

    /**
     * @brief Returns the last non zero index of the given column.
     * @param colIndex Index of the column.
     * @return The pivot of @p colIndex.
     */
    index getPivot(index colIndex);
    /**
     * @brief Returns the column at the given index.
     * @param colIndex Column index.
     * @param col Reference to an empty column which will
     * be filled with the column values.
     */
    void getColumn(index colIndex, std::vector<index> &col);
    /**
     * @brief Returns the column at the given index in the inverse matrix.
     * @param colIndex Column index.
     * @param col Reference to an empty column which will
     * be filled with the column values.
     */
    void getInverseColumn(index colIndex, std::vector<index> &col);
    /**
     * @brief Returns the dimension of the given column.
     * @param colIndex Column index.
     * @return The dimension of @p colIndex.
     */
    dim_type getDimension(index colIndex);
    /**
     * @brief Adds the first given column to the second given column.
     * @param toAddIndex Column index.
     * @param targetIndex Column index.
     */
    void addColTo(index toAddIndex, index targetIndex);
    /**
     * @brief Finalizes given function.
     * Depending on PHAT implementations.
     * @param colIndex Column index.
     */
    void finalize(index colIndex);

private:
    boundary_matrix_type _boundaryMatrix;        ///< Boundary matrix.
    boundary_matrix_type _inverseBoundaryMatrix; ///< Identity matrix.
};

}} // namespace shape_simplification::persistence
