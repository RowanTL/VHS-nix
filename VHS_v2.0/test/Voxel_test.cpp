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
 * @file Voxel_test.cpp
 * Test file for the Voxel classes.
 **/

#include <gtest/gtest.h>
#include <random>

#include "shape/Voxel.h"
#include "shape/ShapeUtilities.h"
#include "util/Point.h"

using shape_simplification::shape::Voxel;
using shape_simplification::shape::Level;
using shape_simplification::util::Point;

TEST(VoxelTest, Constructor) {
    std::mt19937 randomGenerator(0); // Repeatable seed
    std::uniform_int_distribution<int> uniform(0, 65535);

    for (int repetition = 0; repetition < 1000; repetition++) {
        Point p;
        for (unsigned int coord = 0; coord < 3; coord++)
            p[coord] = uniform(randomGenerator);

        Voxel voxel1(p);
        Voxel voxel2(p[0], p[1], p[2], Level::MAIN_SHAPE);

        for (unsigned int coord = 0; coord < 3; coord++) {
            ASSERT_EQ(p[coord], voxel1[coord])
                    << p[0] << " " << p[1] << " " << p[2] << " : " << voxel1;
            ASSERT_EQ(p[coord], voxel2[coord])
                    << p[0] << " " << p[1] << " " << p[2] << " : " << voxel2;
        }

        ASSERT_EQ(Level::NOT_SPECIFIED, voxel1.getLevel()) << "getLevel()";
        ASSERT_EQ(Level::MAIN_SHAPE, voxel2.getLevel()) << "getLevel()";

        voxel1.setLevel(Level::CORE);
        voxel2.setLevel(Level::NEIGHBORHOOD);

        ASSERT_EQ(Level::CORE, voxel1.getLevel()) << "setLevel()";
        ASSERT_EQ(Level::NEIGHBORHOOD, voxel2.getLevel()) << "setLevel()";

        ASSERT_EQ(65536, voxel1.getMaxCoordinateValue())
                << "getMaxCoordinateValue()";
        ASSERT_EQ(65536, voxel2.getMaxCoordinateValue())
                << "getMaxCoordinateValue()";
    }
}

TEST(VoxelTest, Comparators) {
    Voxel v1(2, 3, 4, Level::NEIGHBORHOOD);
    Voxel v2(2, 3, 4, Level::MAIN_SHAPE);

    ASSERT_TRUE(v1 == v2) << "==";
    ASSERT_FALSE(v1 != v2) << "!=";
    ASSERT_FALSE(v1 < v2) << "<";
    ASSERT_FALSE(v2 < v1) << "<";

    Voxel v3(2, 3, 5, Level::MAIN_SHAPE);
    Voxel v4(2, 3, 4, Level::MAIN_SHAPE);

    ASSERT_FALSE(v3 == v4) << "==";
    ASSERT_TRUE(v3 != v4) << "!=";
    ASSERT_FALSE(v3 < v4) << "<";
    ASSERT_TRUE(v4 < v3) << "<";

    Voxel v5(2, 4, 4, Level::MAIN_SHAPE);
    Voxel v6(2, 3, 4, Level::MAIN_SHAPE);

    ASSERT_FALSE(v5 == v6) << "==";
    ASSERT_TRUE(v5 != v6) << "!=";
    ASSERT_FALSE(v5 < v6) << "<";
    ASSERT_TRUE(v6 < v5) << "<";

    Voxel v7(3, 3, 4, Level::MAIN_SHAPE);
    Voxel v8(2, 3, 4, Level::MAIN_SHAPE);

    ASSERT_FALSE(v7 == v8) << "==";
    ASSERT_TRUE(v7 != v8) << "!=";
    ASSERT_FALSE(v7 < v8) << "<";
    ASSERT_TRUE(v8 < v7) << "<";
}

