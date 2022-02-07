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
 * @file Persistence_test.cpp
 * Test file for the Persistent Homoly class.
 **/

#include <gtest/gtest.h>
#include <vector>

#include "persistence/PersistentHomology.h"
#include "shape/Voxel.h"
#include "shape/DualSkeleton.h"
#include "shape/AddableShape.h"

#include "test_examples.h"

using shape_simplification::persistence::PersistentHomology;
using shape_simplification::shape::Voxel;
using shape_simplification::shape::AddableShape;
using shape_simplification::shape::DualSkeleton;

TEST(PersistenceTest, Persistence) {
    using Skeleton = DualSkeleton<AddableShape<Voxel>, Voxel, 0, 3>;
    using PH = PersistentHomology<Skeleton, Voxel, 0, 3>;

    std::vector<Voxel> voxels;
    std::vector<Voxel> chain;
    std::vector<PH::persistent_pair> pairsSorted;

    buildExample1<Voxel>(voxels);

    AddableShape<Voxel> shape(voxels);
    Skeleton skeleton(shape);
    PH pers(skeleton);

    pers.reduceMatrix();
    for (PH::persistent_pair p : pers.getPersistentPairs())
        pairsSorted.push_back(p);

    std::sort(pairsSorted.begin(), pairsSorted.end(),
              [&pers](const PH::persistent_pair& p1,
                      const PH::persistent_pair& p2){
                    if (pers.getTimestamp(p1.first) <
                            pers.getTimestamp(p2.first))
                        return true;
                    if (pers.getTimestamp(p1.first) >
                            pers.getTimestamp(p2.first))
                        return false;
                    if (pers.getTimestamp(p1.second) <
                            pers.getTimestamp(p2.second))
                        return true;

                    return false;
              });

    ASSERT_EQ(4, pairsSorted.size()) << "getPersistentPairs";

    ASSERT_EQ(0, pers.getTimestamp(pairsSorted.at(0).first)) << "getTimestamp";
    ASSERT_EQ(1, pers.getTimestamp(pairsSorted.at(0).second)) << "getTimestamp";
    ASSERT_EQ(0, pers.getTimestamp(pairsSorted.at(1).first)) << "getTimestamp";
    ASSERT_EQ(1, pers.getTimestamp(pairsSorted.at(1).second)) << "getTimestamp";
    ASSERT_EQ(0, pers.getTimestamp(pairsSorted.at(2).first)) << "getTimestamp";
    ASSERT_EQ(2, pers.getTimestamp(pairsSorted.at(2).second)) << "getTimestamp";
    ASSERT_EQ(1, pers.getTimestamp(pairsSorted.at(3).first)) << "getTimestamp";
    ASSERT_EQ(2, pers.getTimestamp(pairsSorted.at(3).second)) << "getTimestamp";

    ASSERT_EQ(0, pers.getDimension(pairsSorted.at(0).first)) << "getDimension";
    ASSERT_EQ(1, pers.getDimension(pairsSorted.at(0).second)) << "getDimension";
    ASSERT_EQ(0, pers.getDimension(pairsSorted.at(1).first)) << "getDimension";
    ASSERT_EQ(1, pers.getDimension(pairsSorted.at(1).second)) << "getDimension";
    ASSERT_EQ(0, pers.getDimension(pairsSorted.at(2).first)) << "getDimension";
    ASSERT_EQ(1, pers.getDimension(pairsSorted.at(2).second)) << "getDimension";
    ASSERT_EQ(1, pers.getDimension(pairsSorted.at(3).first)) << "getDimension";
    ASSERT_EQ(2, pers.getDimension(pairsSorted.at(3).second)) << "getDimension";

    //Assert could return false if reduction method changes...
    pers.getCorrespondingVoxelChain(pairsSorted.at(0), chain);
    ASSERT_EQ(10, chain.size()) << "getCorrespondingVoxelChain";
    chain.clear();
    pers.getCorrespondingVoxelChain(pairsSorted.at(1), chain);
    ASSERT_EQ(6, chain.size()) << "getCorrespondingVoxelChain";
    chain.clear();
    pers.getCorrespondingVoxelChain(pairsSorted.at(2), chain);
    ASSERT_EQ(14, chain.size()) << "getCorrespondingVoxelChain";
    chain.clear();
    pers.getCorrespondingVoxelChain(pairsSorted.at(3), chain);
    ASSERT_EQ(16, chain.size()) << "getCorrespondingVoxelChain";
}

