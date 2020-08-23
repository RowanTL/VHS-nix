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

/**
 * @file BoundaryMatrix.cpp
 * Contains the implementation of the methods in the \ref BoundaryMatrix class.
 **/

#include "BoundaryMatrix.h"

using namespace shape_simplification::persistence;

BoundaryMatrix::BoundaryMatrix() {}

BoundaryMatrix::~BoundaryMatrix() {}

void BoundaryMatrix::addColumn(index colIndex, dim_type dim,
                               std::vector<index> &col) {
    std::vector<index> inverseCol(1, colIndex);
    _boundaryMatrix.set_dim(colIndex, dim);
    _boundaryMatrix.set_col(colIndex, col);
    _inverseBoundaryMatrix.set_col(colIndex, inverseCol);
}

void BoundaryMatrix::setNumberOfColumns(int nber) {
    _boundaryMatrix.set_num_cols(nber);
    _inverseBoundaryMatrix.set_num_cols(nber);
}

BoundaryMatrix::index BoundaryMatrix::getNumberOfColumns() const {
    return _boundaryMatrix.get_num_cols();
}

BoundaryMatrix::index BoundaryMatrix::getPivot(index colIndex) {
    return _boundaryMatrix.get_max_index(colIndex);
}

void BoundaryMatrix::getColumn(index colIndex, std::vector<index> &col) {
    _boundaryMatrix.get_col(colIndex, col);
}

void BoundaryMatrix::getInverseColumn(index colIndex, std::vector<index> &col) {
    return _inverseBoundaryMatrix.get_col(colIndex, col);
}

BoundaryMatrix::dim_type BoundaryMatrix::getDimension(index colIndex) {
    return _boundaryMatrix.get_dim(colIndex);
}

void BoundaryMatrix::addColTo(index toAddIndex, index targetIndex) {
    _boundaryMatrix.add_to(toAddIndex, targetIndex);
    _inverseBoundaryMatrix.add_to(toAddIndex, targetIndex);
}

void BoundaryMatrix::finalize(index colIndex) {
    _boundaryMatrix.finalize(colIndex);
    _inverseBoundaryMatrix.finalize(colIndex);
}
