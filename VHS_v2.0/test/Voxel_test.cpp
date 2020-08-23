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
 * @file Voxel_test.cpp
 * Test file for the Voxel classes.
 **/

#include <algorithm>
#include <gtest/gtest.h>
#include <random>

#include "shape/Voxel.h"
#include "util/Point.h"

using shape_simplification::shape::Point;
using shape_simplification::shape::Voxel;

TEST(VoxelTest, CoordinateAgreement) {
    std::mt19937 randomGenerator(0);
    std::uniform_int_distribution<int> uniform(0, 65535);

    for (int repetition = 0; repetition < 100000; repetition++) {
        Point p;
        for (unsigned int coord = 0; coord < 3; coord++)
            p[coord] = uniform(randomGenerator);

        Voxel voxel1(p);
        Voxel voxel2(p[0], p[1], p[2]);

        for (int coord = 0; coord < 3; coord++) {
            ASSERT_EQ(p[static_cast<unsigned int>(coord)], voxel1[coord])
                    << p[0] << " " << p[1] << " " << p[2] << " : " << voxel1[0] << " "
                            << voxel1[1] << " " << voxel1[2];
            ASSERT_EQ(p[static_cast<unsigned int>(coord)], voxel2[coord])
                    << p[0] << " " << p[1] << " " << p[2] << " : " << voxel2[0] << " "
                            << voxel2[1] << " " << voxel2[2];
        }
        ASSERT_EQ(static_cast<uint64_t>(p[0]) +
                (static_cast<uint64_t>(p[1]) << 17U) +
                (static_cast<uint64_t>(p[2]) << 34U),
                voxel1.rawData())
                << p[0] << " " << p[1] << " " << p[2] << " : " << voxel1[0] << " "
                        << voxel1[1] << " " << voxel1[2];
        ASSERT_EQ(static_cast<uint64_t>(p[0]) +
                (static_cast<uint64_t>(p[1]) << 17U) +
                (static_cast<uint64_t>(p[2]) << 34U),
                voxel2.rawData())
                << p[0] << " " << p[1] << " " << p[2] << " : " << voxel2[0] << " "
                        << voxel2[1] << " " << voxel2[2];
    }
}

TEST(VoxelTest, LessThanOperator) {
    std::mt19937 randomGenerator(0);
    std::uniform_int_distribution<int> uniform(0, 255);

    for (int repetition = 0; repetition < 100000; repetition++) {
        Point p1;
        Point p2;
        for (unsigned int coord = 0; coord < 3; coord++) {
            p1[coord] = uniform(randomGenerator);
            p2[coord] = uniform(randomGenerator);
        }

        Voxel voxel1(p1);
        Voxel voxel2(p2);

        ASSERT_EQ(p1[2] < p2[2] || (p1[2] == p2[2] && p1[1] < p2[1]) ||
                (p1[2] == p2[2] && p1[1] == p2[1] && p1[0] < p2[0]),
                voxel1 < voxel2)
                << "p1 " << p1[0] << " " << p1[1] << " " << p1[2] << " : p2 " << p2[0]
                << " " << p2[1] << " " << p2[2] << " : voxel1 " << voxel1.rawData()
                << " : voxel2 " << voxel2.rawData();
    }
}
