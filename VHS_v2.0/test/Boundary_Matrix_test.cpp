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

/**
 * @file Boundary_Matrix_test.cpp
 * Test file for the Boundary Matrix classes.
 **/

#include <gtest/gtest.h>
#include <vector>

#include "persistence/BoundaryMatrix.h"

using shape_simplification::persistence::BoundaryMatrix;

TEST(BoundaryMatrixTest, Modifier) {
    BoundaryMatrix matrix;
    std::vector<BoundaryMatrix::index> col;

    matrix.setNumberOfColumns(6);

    // vertex 0
    matrix.addColumn(0, 0, col);
    // vertex 1
    matrix.addColumn(1, 0, col);
    // vertex 2
    matrix.addColumn(2, 0, col);
    // edge 3 : (0,1)
    col.push_back(0);
    col.push_back(1);
    matrix.addColumn(3, 1, col);
    // edge 4 : (0,2)
    col.at(1) = 2;
    matrix.addColumn(4, 1, col);
    // edge 5 : (1,2)
    col.at(0) = 1;
    matrix.addColumn(5, 1, col);

    matrix.addColTo(4, 5);
    ASSERT_EQ(2, matrix.getPivot(4));
    ASSERT_EQ(1, matrix.getPivot(5));

    col.clear();
    matrix.getColumn(4, col);
    ASSERT_EQ(2, col.size());
    ASSERT_EQ(0, col.at(0));
    ASSERT_EQ(2, col.at(1));

    col.clear();
    matrix.getColumn(5, col);
    ASSERT_EQ(2, col.size());
    ASSERT_EQ(0, col.at(0));
    ASSERT_EQ(1, col.at(1));

    col.clear();
    matrix.getInverseColumn(4, col);
    ASSERT_EQ(1, col.size());
    ASSERT_EQ(4, col.at(0));

    col.clear();
    matrix.getInverseColumn(5, col);
    ASSERT_EQ(2, col.size());
    ASSERT_EQ(4, col.at(0));
    ASSERT_EQ(5, col.at(1));

    matrix.clearColumn(3);
    matrix.getColumn(3, col);
    ASSERT_EQ(0, col.size());
}

TEST(BoundaryMatrixTest, Access) {
    BoundaryMatrix matrix;
    std::vector<BoundaryMatrix::index> col;

    matrix.setNumberOfColumns(6);
    ASSERT_EQ(6, matrix.getNumberOfColumns());

    // vertex 0
    matrix.addColumn(0, 0, col);
    // vertex 1
    matrix.addColumn(1, 0, col);
    // vertex 2
    matrix.addColumn(2, 0, col);
    // edge 3 : (0,1)
    col.push_back(0);
    col.push_back(1);
    matrix.addColumn(3, 1, col);
    // edge 4 : (0,2)
    col.at(1) = 2;
    matrix.addColumn(4, 1, col);
    // edge 5 : (1,2)
    col.at(0) = 1;
    matrix.addColumn(5, 1, col);

    ASSERT_EQ(-1, matrix.getPivot(0));
    ASSERT_EQ(-1, matrix.getPivot(1));
    ASSERT_EQ(-1, matrix.getPivot(2));
    ASSERT_EQ(1, matrix.getPivot(3));
    ASSERT_EQ(2, matrix.getPivot(4));
    ASSERT_EQ(2, matrix.getPivot(5));

    ASSERT_EQ(0, matrix.getDimension(0));
    ASSERT_EQ(0, matrix.getDimension(1));
    ASSERT_EQ(0, matrix.getDimension(2));
    ASSERT_EQ(1, matrix.getDimension(3));
    ASSERT_EQ(1, matrix.getDimension(4));
    ASSERT_EQ(1, matrix.getDimension(5));

    col.clear();
    matrix.getColumn(4, col);
    ASSERT_EQ(2, col.size());
    ASSERT_EQ(0, col.at(0));
    ASSERT_EQ(2, col.at(1));

    col.clear();
    matrix.getColumn(5, col);
    ASSERT_EQ(2, col.size());
    ASSERT_EQ(1, col.at(0));
    ASSERT_EQ(2, col.at(1));

    col.clear();
    matrix.getInverseColumn(4, col);
    ASSERT_EQ(1, col.size());
    ASSERT_EQ(4, col.at(0));

    col.clear();
    matrix.getInverseColumn(5, col);
    ASSERT_EQ(1, col.size());
    ASSERT_EQ(5, col.at(0));
}

