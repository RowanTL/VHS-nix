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
 * @file Skeleton_test.cpp
 * Test file for the Skeleton classes.
 **/

#include <gtest/gtest.h>

#include "shape/DualSkeleton.h"
#include "shape/PrimalSkeleton.h"
#include "shape/VoxelSkeletonIterators.h"
#include "shape/Voxel.h"
#include "shape/EditableShape.h"
#include "shape/ShapeUtilities.h"

#include "test_examples.h"

using shape_simplification::shape::DualSkeleton;
using shape_simplification::shape::PrimalSkeleton;
using shape_simplification::shape::VertexIterator;
using shape_simplification::shape::EdgeIterator;
using shape_simplification::shape::PositiveEdgeIterator;
using shape_simplification::shape::SquareIterator;
using shape_simplification::shape::PositiveSquareIterator;
using shape_simplification::shape::CubeCoordinates;
using shape_simplification::shape::DualEdgeIterator;
using shape_simplification::shape::DualSquareIterator;
using shape_simplification::shape::DualCubeIterator;
using shape_simplification::shape::Voxel;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::Level;

TEST(SkeletonTest, PrimalConstructor) {
    std::vector<Voxel> voxels;

    buildExample1SmallTranslated<Voxel>(voxels);

    EditableShape<Voxel> shape(voxels);
    PrimalSkeleton<EditableShape<Voxel>, Voxel, 0, 3> skeleton(shape);
    PrimalSkeleton<EditableShape<Voxel>, Voxel, 1, 1> restrSkeleton(shape);

    ASSERT_TRUE(skeleton.isPrimal())
            << "primalSkeleton should be primal";
    ASSERT_TRUE(restrSkeleton.isPrimal())
            << "primalRestrSkeleton should be primal";

    ASSERT_FALSE(skeleton.isAddable())
            << "primalSkeleton addability";
    ASSERT_FALSE(restrSkeleton.isAddable())
            << "primalrestrSkeleton addability";
    ASSERT_FALSE(skeleton.isRemovable())
            << "primalSkeleton removability";
    ASSERT_FALSE(restrSkeleton.isRemovable())
            << "primalrestrSkeleton removability";

    ASSERT_EQ(0, skeleton.getMinDim()) << "primalSkeleton max dim";
    ASSERT_EQ(3, skeleton.getMaxDim()) << "primalSkeleton max dim";
    ASSERT_EQ(1, restrSkeleton.getMinDim()) << "primalRestrSkeleton min dim";
    ASSERT_EQ(1, restrSkeleton.getMaxDim()) << "primalRestrSkeleton max dim";

    // Primal skeleton construction not done, so it is just empty.
    ASSERT_EQ(0, skeleton.getEulerCharacteristic())
            << "primalSkeleton Euler Characteristic.";
    ASSERT_EQ(0, restrSkeleton.getEulerCharacteristic())
            << "primalRestrSkeleton Euler Characteristic.";

    ASSERT_EQ(0, skeleton.getNumberOfCells<0>()) << "primalSkeleton.";
    ASSERT_EQ(0, skeleton.getNumberOfCells<1>()) << "primalSkeleton.";
    ASSERT_EQ(0, skeleton.getNumberOfCells<2>()) << "primalSkeleton.";
    ASSERT_EQ(0, skeleton.getNumberOfCells<3>()) << "primalSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCells<0>()) << "primalRestrSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCells<1>()) << "primalRestrSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCells<2>()) << "primalRestrSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCells<3>()) << "primalRestrSkeleton.";

    ASSERT_EQ(0, skeleton.getNumberOfCellsInMainShape<0>())
            << "primalSkeleton.";
    ASSERT_EQ(0, skeleton.getNumberOfCellsInMainShape<1>())
            << "primalSkeleton.";
    ASSERT_EQ(0, skeleton.getNumberOfCellsInMainShape<2>())
            << "primalSkeleton.";
    ASSERT_EQ(0, skeleton.getNumberOfCellsInMainShape<3>())
            << "primalSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCellsInMainShape<0>())
            << "primalRestrSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCellsInMainShape<1>())
            << "primalRestrSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCellsInMainShape<2>())
            << "primalRestrSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCellsInMainShape<3>())
            << "primalRestrSkeleton.";

    ASSERT_TRUE(skeleton.begin<0>() == skeleton.end<0>());
    ASSERT_TRUE(skeleton.begin<1>() == skeleton.end<1>());
    ASSERT_TRUE(skeleton.begin<2>() == skeleton.end<2>());
    ASSERT_TRUE(skeleton.begin<3>() == skeleton.end<3>());
    ASSERT_TRUE(restrSkeleton.begin<0>() == skeleton.end<0>());
    ASSERT_TRUE(restrSkeleton.begin<1>() == skeleton.end<1>());
    ASSERT_TRUE(restrSkeleton.begin<2>() == skeleton.end<2>());
    ASSERT_TRUE(restrSkeleton.begin<3>() == skeleton.end<3>());

    const CellContainer &container = skeleton.getCellContainer();
    ASSERT_EQ(container.getSize<0>(), 0);
    ASSERT_EQ(container.getSize<1>(), 0);
    ASSERT_EQ(container.getSize<2>(), 0);
    ASSERT_EQ(container.getSize<3>(), 0);

    const CellContainer &rcontainer = restrSkeleton.getCellContainer();
    ASSERT_EQ(rcontainer.getSize<0>(), 0);
    ASSERT_EQ(rcontainer.getSize<1>(), 0);
    ASSERT_EQ(rcontainer.getSize<2>(), 0);
    ASSERT_EQ(rcontainer.getSize<3>(), 0);
}

TEST(SkeletonTest, PrimalVoxels) {
    using PS = PrimalSkeleton<EditableShape<Voxel>, Voxel, 0, 3>;

    std::vector<Voxel> voxels;
    std::unordered_set<Voxel> voxelSet;
    std::vector<Point> vertices;

    vertices.push_back({1,1,1});
    Cell<0> vertex(vertices,0);
    PS::getCorrespondingVoxels(vertex, voxels);

    ASSERT_EQ(voxels.size(), 8);
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(0,0,0)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,1,0)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(1,0,0)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,0)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,1)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,1,1)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(1,0,1)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,0,1)) != voxelSet.end())
            << "Primal skeleton";

    vertices.push_back({2,1,1});
    Cell<1> edge(vertices,0);
    PS::getCorrespondingVoxels(edge, voxels);

    ASSERT_EQ(voxels.size(), 4);
    voxelSet.clear();
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(1,0,0)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,0)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(1,0,1)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,1)) != voxelSet.end())
            << "Primal skeleton";

    vertices.push_back({1,1,2});
    vertices.push_back({2,1,2});
    Cell<2> square(vertices,0);
    PS::getCorrespondingVoxels(square, voxels);

    ASSERT_EQ(voxels.size(), 2);
    voxelSet.clear();
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(1,0,1)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,1)) != voxelSet.end())
            << "Primal skeleton";

    vertices.push_back({1,2,2});
    vertices.push_back({2,2,2});
    vertices.push_back({1,2,1});
    vertices.push_back({2,2,1});
    Cell<3> cube(vertices,0);
    PS::getCorrespondingVoxels(cube, voxels);

    ASSERT_EQ(voxels.size(), 1);
    voxelSet.clear();
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,1)) != voxelSet.end())
            << "Primal skeleton";
}

TEST(SkeletonTest, PrimalVoxelsIn) {
    std::vector<Voxel> voxels;
    std::unordered_set<Voxel> voxelSet;
    std::vector<Point> vertices;

    buildExample1SmallTranslated<Voxel>(voxels);

    EditableShape<Voxel> shape(voxels);
    PrimalSkeleton<EditableShape<Voxel>, Voxel, 0, 3> skeleton(shape);

    vertices.push_back({1,2,1});
    Cell<0> vertex(vertices,0);
    skeleton.getCorrespondingVoxelsIn<0,Level::NEIGHBORHOOD>(vertex, voxels);

    ASSERT_EQ(voxels.size(), 4);
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,1)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,1,1)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,2,0)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,2,1)) != voxelSet.end())
            << "Primal skeleton";

    skeleton.getCorrespondingVoxelsIn<0,Level::MAIN_SHAPE>(vertex, voxels);

    ASSERT_EQ(voxels.size(), 4);
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,1)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,1,1)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,2,0)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,2,1)) != voxelSet.end())
            << "Primal skeleton";

    skeleton.getCorrespondingVoxelsIn<0,Level::CORE>(vertex, voxels);

    ASSERT_EQ(voxels.size(), 2);
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(0,1,1)) != voxelSet.end())
            << "Primal skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,2,1)) != voxelSet.end())
            << "Primal skeleton";
}

TEST(SkeletonTest, DualConstructor) {
    std::vector<Voxel> voxels;

    buildExample1SmallTranslated<Voxel>(voxels);

    EditableShape<Voxel> shape(voxels);
    DualSkeleton<EditableShape<Voxel>, Voxel, 0, 3> skeleton(shape);
    DualSkeleton<EditableShape<Voxel>, Voxel, 1, 1> restrSkeleton(shape);

    ASSERT_FALSE(skeleton.isPrimal())
            << "dualSkeleton should not be primal";
    ASSERT_FALSE(restrSkeleton.isPrimal())
            << "dualRestrSkeleton should not be primal";

    ASSERT_FALSE(skeleton.isAddable())
            << "dualSkeleton addability";
    ASSERT_FALSE(restrSkeleton.isAddable())
            << "dualrestrSkeleton addability";
    ASSERT_FALSE(skeleton.isRemovable())
            << "dualSkeleton removability";
    ASSERT_FALSE(restrSkeleton.isRemovable())
            << "dualrestrSkeleton removability";

    ASSERT_EQ(0, skeleton.getMinDim()) << "dualSkeleton max dim";
    ASSERT_EQ(3, skeleton.getMaxDim()) << "dualSkeleton max dim";
    ASSERT_EQ(1, restrSkeleton.getMinDim()) << "dualRestrSkeleton min dim";
    ASSERT_EQ(1, restrSkeleton.getMaxDim()) << "dualRestrSkeleton max dim";

    ASSERT_EQ(2, skeleton.getEulerCharacteristic())
            << "dualSkeleton Euler Characteristic.";
    ASSERT_EQ(-12, restrSkeleton.getEulerCharacteristic())
            << "dualRestrSkeleton Euler Characteristic.";

    ASSERT_EQ(14, skeleton.getNumberOfCells<0>()) << "dualSkeleton.";
    ASSERT_EQ(18, skeleton.getNumberOfCells<1>()) << "dualSkeleton.";
    ASSERT_EQ(7, skeleton.getNumberOfCells<2>()) << "dualSkeleton.";
    ASSERT_EQ(1, skeleton.getNumberOfCells<3>()) << "dualSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCells<0>()) << "dualRestrSkeleton.";
    ASSERT_EQ(18, restrSkeleton.getNumberOfCells<1>()) << "dualRestrSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCells<2>()) << "dualRestrSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCells<3>()) << "dualRestrSkeleton.";

    ASSERT_EQ(11, skeleton.getNumberOfCellsInMainShape<0>())
            << "dualSkeleton.";
    ASSERT_EQ(12, skeleton.getNumberOfCellsInMainShape<1>())
            << "dualSkeleton.";
    ASSERT_EQ(3, skeleton.getNumberOfCellsInMainShape<2>())
            << "dualSkeleton.";
    ASSERT_EQ(0, skeleton.getNumberOfCellsInMainShape<3>())
            << "dualSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCellsInMainShape<0>())
            << "dualRestrSkeleton.";
    ASSERT_EQ(12, restrSkeleton.getNumberOfCellsInMainShape<1>())
            << "dualRestrSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCellsInMainShape<2>())
            << "dualRestrSkeleton.";
    ASSERT_EQ(0, restrSkeleton.getNumberOfCellsInMainShape<3>())
            << "dualRestrSkeleton.";

    int count = 0;
    for (auto it = skeleton.begin<0>(); it != skeleton.end<0>(); it++)
        count++;
    ASSERT_EQ(14, count);
    count = 0;
    for (auto it = skeleton.begin<1>(); it != skeleton.end<1>(); it++)
        count++;
    ASSERT_EQ(18, count);
    count = 0;
    for (auto it = skeleton.begin<2>(); it != skeleton.end<2>(); it++)
        count++;
    ASSERT_EQ(7, count);
    count = 0;
    for (auto it = skeleton.begin<3>(); it != skeleton.end<3>(); it++)
        count++;
    ASSERT_EQ(1, count);

    count = 0;
    for (auto it = restrSkeleton.begin<0>(); it != restrSkeleton.end<0>(); it++)
        count++;
    ASSERT_EQ(0, count);
    count = 0;
    for (auto it = restrSkeleton.begin<1>(); it != restrSkeleton.end<1>(); it++)
        count++;
    ASSERT_EQ(18, count);
    count = 0;
    for (auto it = restrSkeleton.begin<2>(); it != restrSkeleton.end<2>(); it++)
        count++;
    ASSERT_EQ(0, count);
    count = 0;
    for (auto it = restrSkeleton.begin<3>(); it != restrSkeleton.end<3>(); it++)
        count++;
    ASSERT_EQ(0, count);

    const CellContainer &container = skeleton.getCellContainer();
    ASSERT_EQ(container.getSize<0>(), 14);
    ASSERT_EQ(container.getSize<1>(), 18);
    ASSERT_EQ(container.getSize<2>(), 7);
    ASSERT_EQ(container.getSize<3>(), 1);

    const CellContainer &rcontainer = restrSkeleton.getCellContainer();
    ASSERT_EQ(rcontainer.getSize<0>(), 0);
    ASSERT_EQ(rcontainer.getSize<1>(), 18);
    ASSERT_EQ(rcontainer.getSize<2>(), 0);
    ASSERT_EQ(rcontainer.getSize<3>(), 0);
}

TEST(SkeletonTest, DualVoxels) {
    using DS = DualSkeleton<EditableShape<Voxel>, Voxel, 0, 3>;

    std::vector<Voxel> voxels;
    std::unordered_set<Voxel> voxelSet;
    std::vector<Point> vertices;

    vertices.push_back({1,1,1});
    Cell<0> vertex(vertices,0);
    DS::getCorrespondingVoxels(vertex, voxels);

    ASSERT_EQ(voxels.size(), 1);
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,1)) != voxelSet.end())
            << "Dual skeleton";

    vertices.push_back({2,1,1});
    Cell<1> edge(vertices,0);
    DS::getCorrespondingVoxels(edge, voxels);

    ASSERT_EQ(voxels.size(), 2);
    voxelSet.clear();
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,1)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(2,1,1)) != voxelSet.end())
            << "Dual skeleton";

    vertices.push_back({1,1,2});
    vertices.push_back({2,1,2});
    Cell<2> square(vertices,0);
    DS::getCorrespondingVoxels(square, voxels);

    ASSERT_EQ(voxels.size(), 4);
    voxelSet.clear();
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,1)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(2,1,1)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,2)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(2,1,2)) != voxelSet.end())
            << "Dual skeleton";

    vertices.push_back({1,2,2});
    vertices.push_back({2,2,2});
    vertices.push_back({1,2,1});
    vertices.push_back({2,2,1});
    Cell<3> cube(vertices,0);
    DS::getCorrespondingVoxels(cube, voxels);

    ASSERT_EQ(voxels.size(), 8);
    voxelSet.clear();
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,1)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(2,1,1)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,2)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(2,1,2)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(1,2,2)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(2,2,2)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(1,2,1)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(2,2,1)) != voxelSet.end())
            << "Dual skeleton";
}

TEST(SkeletonTest, DualVoxelsIn) {
    std::vector<Voxel> voxels;
    std::unordered_set<Voxel> voxelSet;
    std::vector<Point> vertices;

    buildExample1SmallTranslated<Voxel>(voxels);

    EditableShape<Voxel> shape(voxels);
    DualSkeleton<EditableShape<Voxel>, Voxel, 0, 3> skeleton(shape);

    vertices.push_back({0,1,0});
    vertices.push_back({0,1,1});
    vertices.push_back({0,2,0});
    vertices.push_back({0,2,1});
    vertices.push_back({1,1,0});
    vertices.push_back({1,1,1});
    vertices.push_back({1,2,0});
    vertices.push_back({1,2,1});
    Cell<3> cube(vertices,0);
    skeleton.getCorrespondingVoxelsIn<3,Level::NEIGHBORHOOD>(cube, voxels);

    ASSERT_EQ(voxels.size(), 4);
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,1)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,1,1)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,2,0)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,2,1)) != voxelSet.end())
            << "Dual skeleton";

    skeleton.getCorrespondingVoxelsIn<3,Level::MAIN_SHAPE>(cube, voxels);

    ASSERT_EQ(voxels.size(), 4);
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(1,1,1)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,1,1)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,2,0)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,2,1)) != voxelSet.end())
            << "Dual skeleton";

    skeleton.getCorrespondingVoxelsIn<3,Level::CORE>(cube, voxels);

    ASSERT_EQ(voxels.size(), 2);
    for (Voxel v : voxels) voxelSet.insert(v);
    ASSERT_TRUE(voxelSet.find(Voxel(0,1,1)) != voxelSet.end())
            << "Dual skeleton";
    ASSERT_TRUE(voxelSet.find(Voxel(0,2,1)) != voxelSet.end())
            << "Dual skeleton";
}

TEST(SkeletonTest, VoxelIterators) {
    Voxel v(0,0,0);
    Voxel v2(2,2,2);
    int count = 0;

    for (VertexIterator<Voxel> it(v); !it.isEnd(); it.next()){
        it.getCurrentValue();
        count++;
    }
    ASSERT_EQ(8, count);

    count = 0;
    for (EdgeIterator<Voxel> it(v); !it.isEnd(); it.next()){
        it.getCurrentValue();
        count++;
    }
    ASSERT_EQ(12, count);

    count = 0;
    for (PositiveEdgeIterator<Voxel> it(v); !it.isEnd(); it.next()){
        it.getCurrentValue();
        count++;
    }
    ASSERT_EQ(3, count);

    count = 0;
    for (SquareIterator<Voxel> it(v); !it.isEnd(); it.next()){
        it.getCurrentValue();
        count++;
    }
    ASSERT_EQ(6, count);

    count = 0;
    for (PositiveSquareIterator<Voxel> it(v); !it.isEnd(); it.next()){
        it.getCurrentValue();
        count++;
    }
    ASSERT_EQ(3, count);

    count = 0;
    for (DualEdgeIterator<Voxel> it(v); !it.isEnd(); it.next()){
        it.getCurrentValue();
        count++;
    }
    ASSERT_EQ(3, count);

    count = 0;
    for (DualEdgeIterator<Voxel> it(v2); !it.isEnd(); it.next()){
        it.getCurrentValue();
        count++;
    }
    ASSERT_EQ(6, count);

    count = 0;
    for (DualSquareIterator<Voxel> it(v); !it.isEnd(); it.next()){
        it.getCurrentValue();
        count++;
    }
    ASSERT_EQ(3, count);

    count = 0;
    for (DualSquareIterator<Voxel> it(v2); !it.isEnd(); it.next()){
        it.getCurrentValue();
        count++;
    }
    ASSERT_EQ(12, count);

    count = 0;
    for (DualCubeIterator<Voxel> it(v); !it.isEnd(); it.next()){
        it.getCurrentValue();
        count++;
    }
    ASSERT_EQ(1, count);

    count = 0;
    for (DualCubeIterator<Voxel> it(v2); !it.isEnd(); it.next()){
        it.getCurrentValue();
        count++;
    }
    ASSERT_EQ(8, count);

    std::vector<Point> coo = CubeCoordinates<Voxel>()(v);
    ASSERT_EQ(8, coo.size());
}
