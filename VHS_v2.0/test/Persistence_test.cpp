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
 * @file Persistence_test.cpp
 * Test file for the Persistent Homoly class.
 **/

#include <algorithm>
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <unordered_set>

#include "persistence/PersistentHomology.h"
#include "shape/AddableShape.h"
#include "shape/DualSkeleton.h"
#include "shape/Voxel.h"

#include "test_examples.h"

using shape_simplification::persistence::PersistentHomology;
using shape_simplification::shape::AddableShape;
using shape_simplification::shape::DualSkeleton;
using shape_simplification::shape::Voxel;

template <class Shape, class Skeleton, class Voxel>
void verifyPersReduction(std::string name);

TEST(MatrixTest, Reduction) {
    verifyPersReduction<AddableShape<Voxel>,
            DualSkeleton<AddableShape<Voxel>, Voxel, 0, 3>, Voxel>(
                "Addable Shape");
}

template <class Shape, class Skeleton, class Voxel>
void verifyPersReduction(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildExample1<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);
    Skeleton skeleton(shape);
    PersistentHomology<Skeleton, Voxel, 0, 3> pers(skeleton);

    pers.reduceMatrix();
    const std::vector<typename PersistentHomology<
            Skeleton,
            Voxel,
            0,
            3>::persistent_pair> &pairs = pers.getPersistentPairs();
    std::vector<Voxel> chain;

    ASSERT_EQ(4, pairs.size());
    ASSERT_EQ(0, (int)(pers.getTimestamp(pairs.at(0).first))) << name;
    ASSERT_EQ(1, (int)(pers.getTimestamp(pairs.at(0).second))) << name;
    ASSERT_EQ(0, (int)(pers.getTimestamp(pairs.at(1).first))) << name;
    ASSERT_EQ(1, (int)(pers.getTimestamp(pairs.at(1).second))) << name;
    ASSERT_EQ(0, (int)(pers.getTimestamp(pairs.at(2).first))) << name;
    ASSERT_EQ(2, (int)(pers.getTimestamp(pairs.at(2).second))) << name;
    ASSERT_EQ(1, (int)(pers.getTimestamp(pairs.at(3).first))) << name;
    ASSERT_EQ(2, (int)(pers.getTimestamp(pairs.at(3).second))) << name;
    ASSERT_EQ(0, pers.getDimension(pairs.at(0).first)) << name;
    ASSERT_EQ(1, pers.getDimension(pairs.at(0).second)) << name;
    ASSERT_EQ(0, pers.getDimension(pairs.at(1).first)) << name;
    ASSERT_EQ(1, pers.getDimension(pairs.at(1).second)) << name;
    ASSERT_EQ(0, pers.getDimension(pairs.at(2).first)) << name;
    ASSERT_EQ(1, pers.getDimension(pairs.at(2).second)) << name;
    ASSERT_EQ(1, pers.getDimension(pairs.at(3).first)) << name;
    ASSERT_EQ(2, pers.getDimension(pairs.at(3).second)) << name;

    for (auto pair : pairs) {
        pers.getCorrespondingVoxelChain(pair, chain);
        chain.clear();
    }
}
