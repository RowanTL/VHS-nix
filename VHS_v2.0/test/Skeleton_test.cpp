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
 * @file Skeleton_test.cpp
 * Test file for the Skeleton classes.
 **/

#include <algorithm>
#include <gtest/gtest.h>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "shape/AddableShape.h"
#include "shape/BaseShape.h"
#include "shape/Cell.h"
#include "shape/EditableShape.h"
#include "shape/RemovableShape.h"
#include "shape/DualSkeleton.h"
#include "shape/PrimalSkeleton.h"
#include "shape/Voxel.h"
#include "util/Point.h"

#include "test_examples.h"

using shape_simplification::shape::AddableShape;
using shape_simplification::shape::BaseShape;
using shape_simplification::shape::Cell;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::RemovableShape;
using shape_simplification::shape::DualSkeleton;
using shape_simplification::shape::PrimalSkeleton;
using shape_simplification::shape::Voxel;
using shape_simplification::util::Point;

template <class Shape, class Voxel>
void verifySkeletonConstructor(std::string name);
template <class Shape, class Skeleton, class Voxel>
void verifyPrimalIterators(std::string name);
template <class Skeleton, class Voxel>
void verifyPrimalVertexIterator(Skeleton &skeleton, std::string name);
template <class Skeleton, class Voxel>
void verifyPrimalEdgeIterator(Skeleton &skeleton, std::string name);
template <class Skeleton, class Voxel>
void verifyPrimalSquareIterator(Skeleton &skeleton, std::string name);
template <class Skeleton, class Voxel>
void verifyPrimalCubeIterator(Skeleton &skeleton, std::string name);
template <class Shape, class Skeleton, class Voxel>
void verifyDualIterators(std::string name);
template <class Skeleton, class Voxel>
void verifyDualVertexIterator(Skeleton &skeleton, std::string name);
template <class Skeleton, class Voxel>
void verifyDualEdgeIterator(Skeleton &skeleton, std::string name);
template <class Skeleton, class Voxel>
void verifyDualSquareIterator(Skeleton &skeleton, std::string name);
template <class Skeleton, class Voxel>
void verifyDualCubeIterator(Skeleton &skeleton, std::string name);
template <class Shape, class Skeleton, class Voxel>
void verifyPrimalCellVoxelCorrespondance(std::string name);
template <class Skeleton, class Voxel>
void verifyPrimalSurroundingVoxelsV(std::string name);
template <class Skeleton, class Shape, class Voxel>
void verifyPrimalExistingVoxelsV(Skeleton skeleton, std::string name);
template <class Skeleton, class Voxel>
void verifyPrimalSurroundingVoxelsE(std::string name);
template <class Skeleton, class Shape, class Voxel>
void verifyPrimalExistingVoxelsE(Skeleton skeleton, std::string name);
template <class Skeleton, class Voxel>
void verifyPrimalSurroundingVoxelsS(std::string name);
template <class Skeleton, class Shape, class Voxel>
void verifyPrimalExistingVoxelsS(Skeleton skeleton, std::string name);
template <class Skeleton, class Voxel>
void verifyPrimalSurroundingVoxelsC(std::string name);
template <class Skeleton, class Shape, class Voxel>
void verifyPrimalExistingVoxelsC(Skeleton skeleton, std::string name);
template <class Shape, class Skeleton, class Voxel>
void verifyDualCellVoxelCorrespondance(std::string name);
template <class Skeleton, class Voxel>
void verifyDualSurroundingVoxelsV(std::string name);
template <class Skeleton, class Shape, class Voxel>
void verifyDualExistingVoxelsV(Skeleton skeleton, std::string name);
template <class Skeleton, class Voxel>
void verifyDualSurroundingVoxelsE(std::string name);
template <class Skeleton, class Shape, class Voxel>
void verifyDualExistingVoxelsE(Skeleton skeleton, std::string name);
template <class Skeleton, class Voxel>
void verifyDualSurroundingVoxelsS(std::string name);
template <class Skeleton, class Shape, class Voxel>
void verifyDualExistingVoxelsS(Skeleton skeleton, std::string name);
template <class Skeleton, class Voxel>
void verifyDualSurroundingVoxelsC(std::string name);
template <class Skeleton, class Shape, class Voxel>
void verifyDualExistingVoxelsC(Skeleton skeleton, std::string name);

TEST(SkeletonTest, Constructors) {
  verifySkeletonConstructor<BaseShape<Voxel>, Voxel>("SkeletonBaseShape");
  verifySkeletonConstructor<AddableShape<Voxel>, Voxel>("SkeletonAddableShape");
  verifySkeletonConstructor<RemovableShape<Voxel>, Voxel>(
      "SkeletonRemovableShape");
  verifySkeletonConstructor<EditableShape<Voxel>, Voxel>(
      "SkeletonEditableShape");
}

template <class Shape, class Voxel>
void verifySkeletonConstructor(std::string name) {
  std::vector<Voxel> voxels;
  std::vector<typename Shape::Level> minLevels;

  buildExample1SmallTranslated<Shape,Voxel>(voxels, minLevels);

  Shape shape(voxels, minLevels);
  PrimalSkeleton<Shape, Voxel, 0, 3> primalSkeleton(shape);
  DualSkeleton<Shape, Voxel, 0, 3> dualSkeleton(shape);
  PrimalSkeleton<Shape, Voxel, 1, 1> primalRestrSkeleton(shape);
  DualSkeleton<Shape, Voxel, 0, 2> dualRestrSkeleton(shape);

  ASSERT_TRUE(primalSkeleton.isPrimal())
      << name << "::primalSkeleton should be primal";
  ASSERT_TRUE(primalRestrSkeleton.isPrimal())
      << name << "::primalRestrSkeleton should be primal";
  ASSERT_FALSE(dualSkeleton.isPrimal())
      << name << "::dualSkeleton should be dual";
  ASSERT_FALSE(dualRestrSkeleton.isPrimal())
      << name << "::dualRestrSkeleton should be dual";

  ASSERT_EQ(0, primalSkeleton.getMinDim())
      << name << "::primalSkeleton should have minDim = 0 and not "
      << primalSkeleton.getMinDim() << ".";
  ASSERT_EQ(3, primalSkeleton.getMaxDim())
      << name << "::primalSkeleton should have maxDim = 3 and not "
      << primalSkeleton.getMaxDim() << ".";
  ASSERT_EQ(0, dualSkeleton.getMinDim())
      << name << "::dualSkeleton should have minDim = 0 and not "
      << primalSkeleton.getMinDim() << ".";
  ASSERT_EQ(3, dualSkeleton.getMaxDim())
      << name << "::dualSkeleton should have maxDim = 3 and not "
      << primalSkeleton.getMaxDim() << ".";
  ASSERT_EQ(1, primalRestrSkeleton.getMinDim())
      << name << "::primalRestrSkeleton should have minDim = 1 and not "
      << primalSkeleton.getMinDim() << ".";
  ASSERT_EQ(1, primalRestrSkeleton.getMaxDim())
      << name << "::primalRestrSkeleton should have maxDim = 1 and not "
      << primalSkeleton.getMaxDim() << ".";
  ASSERT_EQ(0, dualRestrSkeleton.getMinDim())
      << name << "::dualRestrSkeleton should have minDim = 0 and not "
      << primalSkeleton.getMinDim() << ".";
  ASSERT_EQ(2, dualRestrSkeleton.getMaxDim())
      << name << "::dualRestrSkeleton should have maxDim = 2 and not "
      << primalSkeleton.getMaxDim() << ".";

  ASSERT_EQ(1, dualSkeleton.getEulerCharacteristic())
      << name << "::dualSkeleton has wrong Euler Characteristic.";
  ASSERT_EQ(1, dualRestrSkeleton.getEulerCharacteristic())
      << name << "::dualRestrSkeleton has wrong Euler Characteristic.";
}

TEST(SkeletonTest, Iterators) {
  verifyDualIterators<
          BaseShape<Voxel>,
          DualSkeleton<BaseShape<Voxel>, Voxel, 0, 3>,
          Voxel>("DualSkeletonBaseShape");
  verifyDualIterators<
          AddableShape<Voxel>,
          DualSkeleton<AddableShape<Voxel>, Voxel, 0, 3>,
          Voxel>("DualSkeletonAddableShape");
  verifyDualIterators<
          RemovableShape<Voxel>,
          DualSkeleton<RemovableShape<Voxel>, Voxel, 0, 3>,
          Voxel>("DualSkeletonRemovableShape");
  verifyDualIterators<
          EditableShape<Voxel>,
          DualSkeleton<EditableShape<Voxel>, Voxel, 0, 3>,
          Voxel>("DualSkeletonEditableShape");
}

template <class Shape, class Skeleton, class Voxel>
void verifyPrimalIterators(std::string name) {
  std::vector<Voxel> voxels;
  std::vector<typename Shape::Level> minLevels;

  buildMiniExample1<Shape,Voxel>(voxels, minLevels);

  Shape shape(voxels, minLevels);
  Skeleton skeleton(shape);

  verifyPrimalVertexIterator<Skeleton, Voxel>(skeleton, name + "::VertexIt");
  verifyPrimalEdgeIterator<Skeleton, Voxel>(skeleton, name + "::EdgeIt");
  verifyPrimalSquareIterator<Skeleton, Voxel>(skeleton, name + "::SquareIt");
  verifyPrimalCubeIterator<Skeleton, Voxel>(skeleton, name + "::CubeIt");
}

template <class Skeleton, class Voxel>
void verifyPrimalVertexIterator(Skeleton &skeleton, std::string name) {
  typename Skeleton::template cell_iterator<0> it = skeleton.template begin<0>();
  std::unordered_set<typename Skeleton::vertex,
                     typename Skeleton::vertex::CellHasher>
      cells;
  std::array<Point, 1> vertices;

  while (it != skeleton.template end<0>()) {
    cells.insert(*it);
    ++it;
  }

  vertices[0] = Point{1, 1, 1};
  Cell<0> v1(vertices, 0);
  vertices[0] = Point{1, 2, 0};
  Cell<0> v2(vertices, 0);
  vertices[0] = Point{2, 1, 2};
  Cell<0> v3(vertices, 0);
  vertices[0] = Point{0, 0, 2};
  Cell<0> v4(vertices, 0);

  ASSERT_NE(cells.find(v1), cells.end()) << name << "::v1 is missing.";
  ASSERT_NE(cells.find(v2), cells.end()) << name << "::v2 is missing.";
  ASSERT_NE(cells.find(v3), cells.end()) << name << "::v3 is missing.";
  ASSERT_EQ(cells.find(v4), cells.end())
      << name << "::v4 should not be contained.";

  ASSERT_EQ(cells.size(), 24);
}

template <class Skeleton, class Voxel>
void verifyPrimalEdgeIterator(Skeleton &skeleton, std::string name) {
  typename Skeleton::template cell_iterator<1> it = skeleton.template begin<1>();
  std::unordered_set<typename Skeleton::edge,
                     typename Skeleton::edge::CellHasher>
      cells;
  std::array<Point, 2> vertices;

  while (it != skeleton.template end<1>()) {
    cells.insert(*it);
    ++it;
  }

  vertices[0] = Point{0, 1, 1};
  vertices[1] = Point{1, 1, 1};
  Cell<1> e1(vertices, 0);
  vertices[0] = Point{1, 2, 0};
  vertices[1] = Point{1, 2, 1};
  Cell<1> e2(vertices, 0);
  vertices[0] = Point{2, 1, 2};
  vertices[1] = Point{1, 1, 2};
  Cell<1> e3(vertices, 0);
  vertices[0] = Point{0, 0, 2};
  vertices[1] = Point{0, 0, 1};
  Cell<1> e4(vertices, 0);

  ASSERT_NE(cells.find(e1), cells.end()) << name << "::e1 is missing.";
  ASSERT_NE(cells.find(e2), cells.end()) << name << "::e2 is missing.";
  ASSERT_NE(cells.find(e3), cells.end()) << name << "::e3 is missing.";
  ASSERT_EQ(cells.find(e4), cells.end())
      << name << "::e4 should not be contained.";

  ASSERT_EQ(cells.size(), 42);
}

template <class Skeleton, class Voxel>
void verifyPrimalSquareIterator(Skeleton &skeleton, std::string name) {
  typename Skeleton::template cell_iterator<2> it = skeleton.template begin<2>();
  std::unordered_set<typename Skeleton::square,
                     typename Skeleton::square::CellHasher>
      cells;
  std::array<Point, 4> vertices;

  while (it != skeleton.template end<2>()) {
    cells.insert(*it);
    ++it;
  }

  vertices[0] = Point{0, 1, 1};
  vertices[1] = Point{1, 1, 1};
  vertices[2] = Point{0, 1, 0};
  vertices[3] = Point{1, 1, 0};
  Cell<2> s1(vertices, 0);
  vertices[0] = Point{1, 2, 0};
  vertices[1] = Point{1, 2, 1};
  vertices[2] = Point{0, 2, 0};
  vertices[3] = Point{0, 2, 1};
  Cell<2> s2(vertices, 0);
  vertices[0] = Point{2, 1, 2};
  vertices[1] = Point{1, 1, 2};
  vertices[2] = Point{1, 1, 3};
  vertices[3] = Point{2, 1, 3};
  Cell<2> s3(vertices, 0);
  vertices[0] = Point{0, 0, 2};
  vertices[1] = Point{0, 0, 1};
  vertices[2] = Point{1, 0, 1};
  vertices[3] = Point{1, 0, 2};
  Cell<2> s4(vertices, 0);

  ASSERT_NE(cells.find(s1), cells.end()) << name << "::s1 is missing.";
  ASSERT_NE(cells.find(s2), cells.end()) << name << "::s2 is missing.";
  ASSERT_NE(cells.find(s3), cells.end()) << name << "::s3 is missing.";
  ASSERT_EQ(cells.find(s4), cells.end())
      << name << "::s4 should not be contained.";

  ASSERT_EQ(cells.size(), 23);
}

template <class Skeleton, class Voxel>
void verifyPrimalCubeIterator(Skeleton &skeleton, std::string name) {
  typename Skeleton::template cell_iterator<3> it = skeleton.template begin<3>();
  std::unordered_set<typename Skeleton::cube,
                     typename Skeleton::cube::CellHasher>
      cells;
  std::array<Point, 8> vertices;

  while (it != skeleton.template end<3>()) {
    cells.insert(*it);
    ++it;
  }

  vertices[0] = Point{0, 0, 0};
  vertices[1] = Point{0, 0, 1};
  vertices[2] = Point{0, 1, 0};
  vertices[3] = Point{0, 1, 1};
  vertices[4] = Point{1, 0, 0};
  vertices[5] = Point{1, 0, 1};
  vertices[6] = Point{1, 1, 0};
  vertices[7] = Point{1, 1, 1};
  Cell<3> c1(vertices, 0);
  vertices[0] = Point{0, 1, 0};
  vertices[1] = Point{0, 1, 1};
  vertices[2] = Point{0, 2, 0};
  vertices[3] = Point{0, 2, 1};
  vertices[4] = Point{1, 1, 0};
  vertices[5] = Point{1, 1, 1};
  vertices[6] = Point{1, 2, 0};
  vertices[7] = Point{1, 2, 1};
  Cell<3> c2(vertices, 0);
  vertices[0] = Point{1, 1, 1};
  vertices[1] = Point{1, 1, 2};
  vertices[2] = Point{1, 2, 1};
  vertices[3] = Point{1, 2, 2};
  vertices[4] = Point{2, 1, 1};
  vertices[5] = Point{2, 1, 2};
  vertices[6] = Point{2, 2, 1};
  vertices[7] = Point{2, 2, 2};
  Cell<3> c3(vertices, 0);
  vertices[0] = Point{1, 0, 2};
  vertices[1] = Point{1, 0, 3};
  vertices[2] = Point{1, 1, 2};
  vertices[3] = Point{1, 1, 3};
  vertices[4] = Point{2, 0, 2};
  vertices[5] = Point{2, 0, 3};
  vertices[6] = Point{2, 1, 2};
  vertices[7] = Point{2, 1, 3};
  Cell<3> c4(vertices, 0);

  ASSERT_NE(cells.find(c1), cells.end()) << name << "::c1 is missing.";
  ASSERT_NE(cells.find(c2), cells.end()) << name << "::c2 is missing.";
  ASSERT_NE(cells.find(c3), cells.end()) << name << "::c3 is missing.";
  ASSERT_NE(cells.find(c4), cells.end()) << name << "::c4 is missing.";

  ASSERT_EQ(cells.size(), 4);
}

template <class Shape, class Skeleton, class Voxel>
void verifyDualIterators(std::string name) {
  std::vector<Voxel> voxels;
  std::vector<typename Shape::Level> minLevels;

  buildExample1SmallTranslated<Shape,Voxel>(voxels, minLevels);

  Shape shape(voxels, minLevels);
  Skeleton skeleton(shape);

  verifyDualVertexIterator<Skeleton, Voxel>(skeleton, name + "::VertexIt");
  verifyDualEdgeIterator<Skeleton, Voxel>(skeleton, name + "::EdgeIt");
  verifyDualSquareIterator<Skeleton, Voxel>(skeleton, name + "::SquareIt");
  verifyDualCubeIterator<Skeleton, Voxel>(skeleton, name + "::CubeIt");
}

template <class Skeleton, class Voxel>
void verifyDualVertexIterator(Skeleton &skeleton, std::string name) {
  typename Skeleton::template cell_iterator<0> it = skeleton.template begin<0>();
  std::unordered_set<typename Skeleton::vertex,
                     typename Skeleton::vertex::CellHasher>
      cells;
  std::array<Point, 1> vertices;

  while (it != skeleton.template end<0>()) {
    cells.insert(*it);
    ++it;
  }

  vertices[0] = Point{1, 1, 1};
  Cell<0> v1(vertices, 0);
  vertices[0] = Point{0, 3, 1};
  Cell<0> v2(vertices, 0);
  vertices[0] = Point{0, 0, 2};
  Cell<0> v3(vertices, 0);
  vertices[0] = Point{0, 1, 0};
  Cell<0> v4(vertices, 0);

  ASSERT_NE(cells.find(v1), cells.end()) << name << "::v1 is missing.";
  ASSERT_NE(cells.find(v2), cells.end()) << name << "::v2 is missing.";
  ASSERT_NE(cells.find(v3), cells.end()) << name << "::v3 is missing.";
  ASSERT_EQ(cells.find(v4), cells.end())
      << name << "::v4 should not be contained.";

  ASSERT_EQ(cells.size(), 14);
}

template <class Skeleton, class Voxel>
void verifyDualEdgeIterator(Skeleton &skeleton, std::string name) {
  typename Skeleton::template cell_iterator<1> it = skeleton.template begin<1>();
  std::unordered_set<typename Skeleton::edge,
                     typename Skeleton::edge::CellHasher>
      cells;
  std::array<Point, 2> vertices;

  while (it != skeleton.template end<1>()) {
    cells.insert(*it);
    ++it;
  }

  vertices[0] = Point{0, 0, 0};
  vertices[1] = Point{0, 0, 1};
  Cell<1> e1(vertices, 0);
  vertices[0] = Point{0, 1, 1};
  vertices[1] = Point{0, 1, 2};
  Cell<1> e2(vertices, 0);
  vertices[0] = Point{0, 2, 1};
  vertices[1] = Point{0, 2, 0};
  Cell<1> e3(vertices, 0);
  vertices[0] = Point{0, 0, 0};
  vertices[1] = Point{0, 1, 0};
  Cell<1> e4(vertices, 0);

  ASSERT_TRUE(cells.find(e1) != cells.end()) << name << "::e1 is missing.";
  ASSERT_NE(cells.find(e2), cells.end()) << name << "::e2 is missing.";
  ASSERT_NE(cells.find(e3), cells.end()) << name << "::e3 is missing.";
  ASSERT_EQ(cells.find(e4), cells.end())
      << name << "::e4 should not be contained.";

  ASSERT_EQ(cells.size(), 18);
}

template <class Skeleton, class Voxel>
void verifyDualSquareIterator(Skeleton &skeleton, std::string name) {
  typename Skeleton::template cell_iterator<2> it = skeleton.template begin<2>();
  std::unordered_set<typename Skeleton::square,
                     typename Skeleton::square::CellHasher>
      cells;
  std::array<Point, 4> vertices;

  while (it != skeleton.template end<2>()) {
    cells.insert(*it);
    ++it;
  }

  vertices[0] = Point{0, 0, 1};
  vertices[1] = Point{0, 1, 1};
  vertices[2] = Point{0, 0, 2};
  vertices[3] = Point{0, 1, 2};
  Cell<2> s1(vertices, 0);
  vertices[0] = Point{0, 0, 2};
  vertices[1] = Point{0, 1, 2};
  vertices[2] = Point{1, 0, 2};
  vertices[3] = Point{1, 1, 2};
  Cell<2> s2(vertices, 0);
  vertices[0] = Point{0, 2, 0};
  vertices[1] = Point{0, 3, 0};
  vertices[2] = Point{0, 2, 1};
  vertices[3] = Point{0, 3, 1};
  Cell<2> s3(vertices, 0);
  vertices[0] = Point{0, 1, 1};
  vertices[1] = Point{0, 2, 1};
  vertices[2] = Point{0, 1, 2};
  vertices[3] = Point{0, 2, 2};
  Cell<2> s4(vertices, 0);

  ASSERT_NE(cells.find(s1), cells.end()) << name << "::s1 is missing.";
  ASSERT_NE(cells.find(s2), cells.end()) << name << "::s2 is missing.";
  ASSERT_NE(cells.find(s3), cells.end()) << name << "::s3 is missing.";
  ASSERT_EQ(cells.find(s4), cells.end())
      << name << "::s4 should not be contained.";

  ASSERT_EQ(cells.size(), 7);
}

template <class Skeleton, class Voxel>
void verifyDualCubeIterator(Skeleton &skeleton, std::string name) {
  typename Skeleton::template cell_iterator<3> it = skeleton.template begin<3>();
  std::unordered_set<typename Skeleton::cube,
                     typename Skeleton::cube::CellHasher>
      cells;
  std::array<Point, 8> vertices;

  while (it != skeleton.template end<3>()) {
    cells.insert(*it);
    ++it;
  }

  vertices[0] = Point{0, 0, 1};
  vertices[1] = Point{0, 0, 2};
  vertices[2] = Point{0, 1, 1};
  vertices[3] = Point{0, 1, 2};
  vertices[4] = Point{1, 0, 1};
  vertices[5] = Point{1, 0, 2};
  vertices[6] = Point{1, 1, 1};
  vertices[7] = Point{1, 1, 2};
  Cell<3> c1(vertices, 0);
  vertices[0] = Point{0, 1, 0};
  vertices[1] = Point{0, 1, 1};
  vertices[2] = Point{0, 2, 0};
  vertices[3] = Point{0, 2, 1};
  vertices[4] = Point{1, 1, 0};
  vertices[5] = Point{1, 1, 1};
  vertices[6] = Point{1, 2, 0};
  vertices[7] = Point{1, 2, 1};
  Cell<3> c2(vertices, 0);

  ASSERT_NE(cells.find(c1), cells.end()) << name << "::c1 is missing.";
  ASSERT_EQ(cells.find(c2), cells.end())
      << name << "::c2 should not be contained.";

  ASSERT_EQ(cells.size(), 1);
}

TEST(SkeletonTest, CellVoxelCorrespondance) {
  verifyPrimalCellVoxelCorrespondance<
          BaseShape<Voxel>,
          PrimalSkeleton<BaseShape<Voxel>, Voxel, 0, 3>,
          Voxel>("PrimalSkeletonBaseShape");
  verifyPrimalCellVoxelCorrespondance<
          AddableShape<Voxel>,
          PrimalSkeleton<AddableShape<Voxel>, Voxel, 0, 3>,
          Voxel>("PrimalSkeletonAddableShape");
  verifyPrimalCellVoxelCorrespondance<
          RemovableShape<Voxel>,
          PrimalSkeleton<RemovableShape<Voxel>, Voxel, 0, 3>,
          Voxel>("PrimalSkeletonRemovableShape");
  verifyPrimalCellVoxelCorrespondance<
          EditableShape<Voxel>,
          PrimalSkeleton<EditableShape<Voxel>, Voxel, 0, 3>,
          Voxel>("PrimalSkeletonEditableShape");
  verifyDualCellVoxelCorrespondance<
          BaseShape<Voxel>,
          DualSkeleton<BaseShape<Voxel>, Voxel, 0, 3>,
          Voxel>("DualSkeletonBaseShape");
  verifyDualCellVoxelCorrespondance<
          AddableShape<Voxel>,
          DualSkeleton<AddableShape<Voxel>, Voxel, 0, 3>,
          Voxel>("DualSkeletonAddableShape");
  verifyDualCellVoxelCorrespondance<
          RemovableShape<Voxel>,
          DualSkeleton<RemovableShape<Voxel>, Voxel, 0, 3>,
          Voxel>("DualSkeletonRemovableShape");
  verifyDualCellVoxelCorrespondance<
          EditableShape<Voxel>,
          DualSkeleton<EditableShape<Voxel>, Voxel, 0, 3>,
          Voxel>("DualSkeletonEditableShape");
}

template <class Shape, class Skeleton, class Voxel>
void verifyPrimalCellVoxelCorrespondance(std::string name) {
  std::vector<Voxel> voxels;
  std::vector<typename Shape::Level> minLevels;

  buildMiniExample1<Shape,Voxel>(voxels, minLevels);

  Shape shape(voxels, minLevels);
  Skeleton skeleton(shape);

  verifyPrimalSurroundingVoxelsV<Skeleton, Voxel>(name + "::Vertex");
  verifyPrimalExistingVoxelsV<Skeleton, Shape, Voxel>(skeleton,
                                                      name + "::Vertex");
  verifyPrimalSurroundingVoxelsE<Skeleton, Voxel>(name + "::Edge");
  verifyPrimalExistingVoxelsE<Skeleton, Shape, Voxel>(skeleton,
                                                      name + "::Edge");
  verifyPrimalSurroundingVoxelsS<Skeleton, Voxel>(name + "::Square");
  verifyPrimalExistingVoxelsS<Skeleton, Shape, Voxel>(skeleton,
                                                      name + "::Square");
  verifyPrimalSurroundingVoxelsC<Skeleton, Voxel>(name + "::Cube");
  verifyPrimalExistingVoxelsC<Skeleton, Shape, Voxel>(skeleton,
                                                      name + "::Cube");
}

template <class Skeleton, class Voxel>
void verifyPrimalSurroundingVoxelsV(std::string name) {
  std::array<Point, 1> vertices;
  vertices[0] = Point{1, 1, 1};
  Cell<0> v(vertices, 0);

  std::array<Voxel, 8> voxels;
  Skeleton::getCorrespondingVoxels(v, voxels);
  ASSERT_TRUE(Voxel(Point{0, 0, 0}) == voxels[0] ||
              Voxel(Point{0, 0, 0}) == voxels[1] ||
              Voxel(Point{0, 0, 0}) == voxels[2] ||
              Voxel(Point{0, 0, 0}) == voxels[3] ||
              Voxel(Point{0, 0, 0}) == voxels[4] ||
              Voxel(Point{0, 0, 0}) == voxels[5] ||
              Voxel(Point{0, 0, 0}) == voxels[6] ||
              Voxel(Point{0, 0, 0}) == voxels[7])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 0}) == voxels[0] ||
              Voxel(Point{0, 1, 0}) == voxels[1] ||
              Voxel(Point{0, 1, 0}) == voxels[2] ||
              Voxel(Point{0, 1, 0}) == voxels[3] ||
              Voxel(Point{0, 1, 0}) == voxels[4] ||
              Voxel(Point{0, 1, 0}) == voxels[5] ||
              Voxel(Point{0, 1, 0}) == voxels[6] ||
              Voxel(Point{0, 1, 0}) == voxels[7])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 0, 1}) == voxels[0] ||
              Voxel(Point{0, 0, 1}) == voxels[1] ||
              Voxel(Point{0, 0, 1}) == voxels[2] ||
              Voxel(Point{0, 0, 1}) == voxels[3] ||
              Voxel(Point{0, 0, 1}) == voxels[4] ||
              Voxel(Point{0, 0, 1}) == voxels[5] ||
              Voxel(Point{0, 0, 1}) == voxels[6] ||
              Voxel(Point{0, 0, 1}) == voxels[7])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 1}) == voxels[0] ||
              Voxel(Point{0, 1, 1}) == voxels[1] ||
              Voxel(Point{0, 1, 1}) == voxels[2] ||
              Voxel(Point{0, 1, 1}) == voxels[3] ||
              Voxel(Point{0, 1, 1}) == voxels[4] ||
              Voxel(Point{0, 1, 1}) == voxels[5] ||
              Voxel(Point{0, 1, 1}) == voxels[6] ||
              Voxel(Point{0, 1, 1}) == voxels[7])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 0, 1}) == voxels[0] ||
              Voxel(Point{1, 0, 1}) == voxels[1] ||
              Voxel(Point{1, 0, 1}) == voxels[2] ||
              Voxel(Point{1, 0, 1}) == voxels[3] ||
              Voxel(Point{1, 0, 1}) == voxels[4] ||
              Voxel(Point{1, 0, 1}) == voxels[5] ||
              Voxel(Point{1, 0, 1}) == voxels[6] ||
              Voxel(Point{1, 0, 1}) == voxels[7])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1] ||
              Voxel(Point{1, 1, 1}) == voxels[2] ||
              Voxel(Point{1, 1, 1}) == voxels[3] ||
              Voxel(Point{1, 1, 1}) == voxels[4] ||
              Voxel(Point{1, 1, 1}) == voxels[5] ||
              Voxel(Point{1, 1, 1}) == voxels[6] ||
              Voxel(Point{1, 1, 1}) == voxels[7])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 0, 0}) == voxels[0] ||
              Voxel(Point{1, 0, 0}) == voxels[1] ||
              Voxel(Point{1, 0, 0}) == voxels[2] ||
              Voxel(Point{1, 0, 0}) == voxels[3] ||
              Voxel(Point{1, 0, 0}) == voxels[4] ||
              Voxel(Point{1, 0, 0}) == voxels[5] ||
              Voxel(Point{1, 0, 0}) == voxels[6] ||
              Voxel(Point{1, 0, 0}) == voxels[7])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 0}) == voxels[0] ||
              Voxel(Point{1, 1, 0}) == voxels[1] ||
              Voxel(Point{1, 1, 0}) == voxels[2] ||
              Voxel(Point{1, 1, 0}) == voxels[3] ||
              Voxel(Point{1, 1, 0}) == voxels[4] ||
              Voxel(Point{1, 1, 0}) == voxels[5] ||
              Voxel(Point{1, 1, 0}) == voxels[6] ||
              Voxel(Point{1, 1, 0}) == voxels[7])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
}

template <class Skeleton, class Shape, class Voxel>
void verifyPrimalExistingVoxelsV(Skeleton skeleton, std::string name) {
  std::array<Point, 1> vertices;
  vertices[0] = Point{1, 1, 1};
  Cell<0> v(vertices, 0);

  std::vector<Voxel> voxels;
  skeleton.template getCorrespondingVoxelsIn<0,Shape::CORE>(v, voxels);
  ASSERT_EQ(2, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 0, 0}) == voxels[0] ||
              Voxel(Point{0, 0, 0}) == voxels[1])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<0,Shape::MAIN_SHAPE>(v, voxels);
  ASSERT_EQ(3, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 0, 0}) == voxels[0] ||
              Voxel(Point{0, 0, 0}) == voxels[1] ||
              Voxel(Point{0, 0, 0}) == voxels[2])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1] ||
              Voxel(Point{1, 1, 1}) == voxels[2])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 0}) == voxels[0] ||
              Voxel(Point{0, 1, 0}) == voxels[1] ||
              Voxel(Point{0, 1, 0}) == voxels[2])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << ").";
  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<0,Shape::NEIGHBORHOOD>(v, voxels);
  ASSERT_EQ(3, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 0, 0}) == voxels[0] ||
              Voxel(Point{0, 0, 0}) == voxels[1] ||
              Voxel(Point{0, 0, 0}) == voxels[2])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1] ||
              Voxel(Point{1, 1, 1}) == voxels[2])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 0}) == voxels[0] ||
              Voxel(Point{0, 1, 0}) == voxels[1] ||
              Voxel(Point{0, 1, 0}) == voxels[2])
      << name << "::v wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << ").";
}

template <class Skeleton, class Voxel>
void verifyPrimalSurroundingVoxelsE(std::string name) {
  std::array<Point, 2> vertices;
  vertices[0] = Point{0, 1, 1};
  vertices[1] = Point{1, 1, 1};
  Cell<1> e(vertices, 0);

  std::array<Voxel, 4> voxels;
  Skeleton::getCorrespondingVoxels(e, voxels);
  ASSERT_TRUE(Voxel(Point{0, 0, 0}) == voxels[0] ||
              Voxel(Point{0, 0, 0}) == voxels[1] ||
              Voxel(Point{0, 0, 0}) == voxels[2] ||
              Voxel(Point{0, 0, 0}) == voxels[3])
      << name << "::e wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 0}) == voxels[0] ||
              Voxel(Point{0, 1, 0}) == voxels[1] ||
              Voxel(Point{0, 1, 0}) == voxels[2] ||
              Voxel(Point{0, 1, 0}) == voxels[3])
      << name << "::e wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 0, 1}) == voxels[0] ||
              Voxel(Point{0, 0, 1}) == voxels[1] ||
              Voxel(Point{0, 0, 1}) == voxels[2] ||
              Voxel(Point{0, 0, 1}) == voxels[3])
      << name << "::e wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 1}) == voxels[0] ||
              Voxel(Point{0, 1, 1}) == voxels[1] ||
              Voxel(Point{0, 1, 1}) == voxels[2] ||
              Voxel(Point{0, 1, 1}) == voxels[3])
      << name << "::e wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << ").";
}

template <class Skeleton, class Shape, class Voxel>
void verifyPrimalExistingVoxelsE(Skeleton skeleton, std::string name) {
  std::array<Point, 2> vertices;
  vertices[0] = Point{0, 1, 1};
  vertices[1] = Point{1, 1, 1};
  Cell<1> e(vertices, 0);

  std::vector<Voxel> voxels;
  skeleton.template getCorrespondingVoxelsIn<1,Shape::CORE>(e, voxels);
  ASSERT_EQ(1, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 0, 0}) == voxels[0])
      << name << "::e wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << ").";

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<1,Shape::MAIN_SHAPE>(e, voxels);
  ASSERT_EQ(2, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 0, 0}) == voxels[0] ||
              Voxel(Point{0, 0, 0}) == voxels[1])
      << name << "::e wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 0}) == voxels[0] ||
              Voxel(Point{0, 1, 0}) == voxels[1])
      << name << "::e wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<1,Shape::NEIGHBORHOOD>(e, voxels);
  ASSERT_EQ(2, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 0, 0}) == voxels[0] ||
              Voxel(Point{0, 0, 0}) == voxels[1])
      << name << "::e wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 0}) == voxels[0] ||
              Voxel(Point{0, 1, 0}) == voxels[1])
      << name << "::e wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
}

template <class Skeleton, class Voxel>
void verifyPrimalSurroundingVoxelsS(std::string name) {
  std::array<Point, 4> vertices;
  vertices[0] = Point{0, 1, 0};
  vertices[1] = Point{1, 1, 0};
  vertices[2] = Point{0, 1, 1};
  vertices[3] = Point{1, 1, 1};
  Cell<2> s1(vertices, 0);
  vertices[0] = Point{1, 1, 2};
  vertices[1] = Point{2, 1, 2};
  vertices[2] = Point{1, 1, 3};
  vertices[3] = Point{2, 1, 3};
  Cell<2> s2(vertices, 0);

  std::array<Voxel, 2> voxels;
  Skeleton::getCorrespondingVoxels(s1, voxels);
  ASSERT_TRUE(Voxel(Point{0, 0, 0}) == voxels[0] ||
              Voxel(Point{0, 0, 0}) == voxels[1])
      << name << "::s1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 0}) == voxels[0] ||
              Voxel(Point{0, 1, 0}) == voxels[1])
      << name << "::s1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";

  Skeleton::getCorrespondingVoxels(s2, voxels);
  ASSERT_TRUE(Voxel(Point{1, 0, 2}) == voxels[0] ||
              Voxel(Point{1, 0, 2}) == voxels[1])
      << name << "::s2 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 2}) == voxels[0] ||
              Voxel(Point{1, 1, 2}) == voxels[1])
      << name << "::s2 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
}

template <class Skeleton, class Shape, class Voxel>
void verifyPrimalExistingVoxelsS(Skeleton skeleton, std::string name) {
  std::array<Point, 4> vertices;
  vertices[0] = Point{0, 1, 0};
  vertices[1] = Point{1, 1, 0};
  vertices[2] = Point{0, 1, 1};
  vertices[3] = Point{1, 1, 1};
  Cell<2> s1(vertices, 0);
  vertices[0] = Point{1, 1, 2};
  vertices[1] = Point{2, 1, 2};
  vertices[2] = Point{1, 1, 3};
  vertices[3] = Point{2, 1, 3};
  Cell<2> s2(vertices, 0);

  std::vector<Voxel> voxels;
  skeleton.template getCorrespondingVoxelsIn<2,Shape::CORE>(s1, voxels);
  ASSERT_EQ(1, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 0, 0}) == voxels[0])
      << name << "::s1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << ").";

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<2,Shape::MAIN_SHAPE>(s1, voxels);
  ASSERT_EQ(2, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 0, 0}) == voxels[0] ||
              Voxel(Point{0, 0, 0}) == voxels[1])
      << name << "::s1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 0}) == voxels[0] ||
              Voxel(Point{0, 1, 0}) == voxels[1])
      << name << "::s1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<2,Shape::NEIGHBORHOOD>(s1, voxels);
  ASSERT_EQ(2, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 0, 0}) == voxels[0] ||
              Voxel(Point{0, 0, 0}) == voxels[1])
      << name << "::s1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 0}) == voxels[0] ||
              Voxel(Point{0, 1, 0}) == voxels[1])
      << name << "::s1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<2,Shape::CORE>(s2, voxels);
  ASSERT_EQ(0, voxels.size());

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<2,Shape::MAIN_SHAPE>(s2, voxels);
  ASSERT_EQ(0, voxels.size());

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<2,Shape::NEIGHBORHOOD>(s2, voxels);
  ASSERT_EQ(1, voxels.size());
  ASSERT_TRUE(Voxel(Point{1, 0, 2}) == voxels[0])
      << name << "::s2 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << ").";
}

template <class Skeleton, class Voxel>
void verifyPrimalSurroundingVoxelsC(std::string name) {
  std::array<Point, 8> vertices;
  vertices[0] = Point{0, 1, 0};
  vertices[1] = Point{0, 2, 0};
  vertices[2] = Point{0, 1, 1};
  vertices[3] = Point{0, 2, 1};
  vertices[4] = Point{1, 1, 0};
  vertices[5] = Point{1, 2, 0};
  vertices[6] = Point{1, 1, 1};
  vertices[7] = Point{1, 2, 1};
  Cell<3> c1(vertices, 0);
  vertices[0] = Point{0, 0, 1};
  vertices[1] = Point{0, 1, 1};
  vertices[2] = Point{0, 0, 2};
  vertices[3] = Point{0, 1, 2};
  vertices[4] = Point{1, 0, 1};
  vertices[5] = Point{1, 1, 1};
  vertices[6] = Point{1, 0, 2};
  vertices[7] = Point{1, 1, 2};
  Cell<3> c2(vertices, 0);

  std::array<Voxel, 1> voxels;
  Skeleton::getCorrespondingVoxels(c1, voxels);
  ASSERT_TRUE(Voxel(Point{0, 1, 0}) == voxels[0])
      << name << "::c1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << ").";

  Skeleton::getCorrespondingVoxels(c2, voxels);
  ASSERT_TRUE(Voxel(Point{0, 0, 1}) == voxels[0])
      << name << "::c2 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << ").";
}

template <class Skeleton, class Shape, class Voxel>
void verifyPrimalExistingVoxelsC(Skeleton skeleton, std::string name) {
  std::array<Point, 8> vertices;
  vertices[0] = Point{0, 1, 0};
  vertices[1] = Point{0, 2, 0};
  vertices[2] = Point{0, 1, 1};
  vertices[3] = Point{0, 2, 1};
  vertices[4] = Point{1, 1, 0};
  vertices[5] = Point{1, 2, 0};
  vertices[6] = Point{1, 1, 1};
  vertices[7] = Point{1, 2, 1};
  Cell<3> c1(vertices, 0);
  vertices[0] = Point{0, 0, 1};
  vertices[1] = Point{0, 1, 1};
  vertices[2] = Point{0, 0, 2};
  vertices[3] = Point{0, 1, 2};
  vertices[4] = Point{1, 0, 1};
  vertices[5] = Point{1, 1, 1};
  vertices[6] = Point{1, 0, 2};
  vertices[7] = Point{1, 1, 2};
  Cell<3> c2(vertices, 0);

  std::vector<Voxel> voxels;
  skeleton.template getCorrespondingVoxelsIn<3,Shape::CORE>(c1, voxels);
  ASSERT_EQ(0, voxels.size());

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<3,Shape::MAIN_SHAPE>(c1, voxels);
  ASSERT_EQ(1, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 1, 0}) == voxels[0])
      << name << "::c1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << ").";

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<3,Shape::NEIGHBORHOOD>(c1, voxels);
  ASSERT_EQ(1, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 1, 0}) == voxels[0])
      << name << "::c1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << ").";

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<3,Shape::CORE>(c2, voxels);
  ASSERT_EQ(0, voxels.size());

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<3,Shape::MAIN_SHAPE>(c2, voxels);
  ASSERT_EQ(0, voxels.size());

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<3,Shape::NEIGHBORHOOD>(c2, voxels);
  ASSERT_EQ(0, voxels.size());
}

template <class Shape, class Skeleton, class Voxel>
void verifyDualCellVoxelCorrespondance(std::string name) {
  std::vector<Voxel> voxels;
  std::vector<typename Shape::Level> minLevels;

  buildExample1SmallTranslated<Shape,Voxel>(voxels, minLevels);

  Shape shape(voxels, minLevels);
  Skeleton skeleton(shape);

  verifyDualSurroundingVoxelsV<Skeleton, Voxel>(name + "::Vertex");
  verifyDualExistingVoxelsV<Skeleton, Shape, Voxel>(skeleton,
                                                    name + "::Vertex");
  verifyDualSurroundingVoxelsE<Skeleton, Voxel>(name + "::Edge");
  verifyDualExistingVoxelsE<Skeleton, Shape, Voxel>(skeleton, name + "::Edge");
  verifyDualSurroundingVoxelsS<Skeleton, Voxel>(name + "::Square");
  verifyDualExistingVoxelsS<Skeleton, Shape, Voxel>(skeleton,
                                                    name + "::Square");
  verifyDualSurroundingVoxelsC<Skeleton, Voxel>(name + "::Cube");
  verifyDualExistingVoxelsC<Skeleton, Shape, Voxel>(skeleton, name + "::Cube");
}

template <class Skeleton, class Voxel>
void verifyDualSurroundingVoxelsV(std::string name) {
  std::array<Point, 1> vertices;
  vertices[0] = Point{1, 1, 1};
  Cell<0> v1(vertices, 0);
  vertices[0] = Point{0, 1, 0};
  Cell<0> v2(vertices, 0);

  std::array<Voxel, 1> voxels;
  Skeleton::getCorrespondingVoxels(v1, voxels);
  ASSERT_EQ(Voxel(Point{1, 1, 1}), voxels[0])
      << name << "::v1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << ").";

  Skeleton::getCorrespondingVoxels(v2, voxels);
  ASSERT_EQ(Voxel(Point{0, 1, 0}), voxels[0])
      << name << "::v2 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << ").";
}

template <class Skeleton, class Shape, class Voxel>
void verifyDualExistingVoxelsV(Skeleton skeleton, std::string name) {
  std::array<Point, 1> vertices;
  vertices[0] = Point{1, 1, 1};
  Cell<0> v1(vertices, 0);
  vertices[0] = Point{0, 1, 0};
  Cell<0> v2(vertices, 0);

  std::vector<Voxel> voxels;
  skeleton.template getCorrespondingVoxelsIn<0,Shape::CORE>(v1, voxels);
  ASSERT_EQ(Voxel(Point{1, 1, 1}), voxels[0])
      << name << "::v1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << ").";
  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<0,Shape::MAIN_SHAPE>(v1, voxels);
  ASSERT_EQ(Voxel(Point{1, 1, 1}), voxels[0])
      << name << "::v1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << ").";
  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<0,Shape::NEIGHBORHOOD>(v1, voxels);
  ASSERT_EQ(Voxel(Point{1, 1, 1}), voxels[0])
      << name << "::v1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << ").";

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<0,Shape::CORE>(v2, voxels);
  ASSERT_EQ(0, voxels.size())
      << name << "::v2 should not have existing corresponding voxels).";
}

template <class Skeleton, class Voxel>
void verifyDualSurroundingVoxelsE(std::string name) {
  std::array<Point, 2> vertices;
  vertices[0] = Point{0, 1, 1};
  vertices[1] = Point{0, 1, 2};
  Cell<1> e1(vertices, 0);
  vertices[0] = Point{0, 1, 1};
  vertices[1] = Point{1, 1, 1};
  Cell<1> e2(vertices, 0);

  std::array<Voxel, 2> voxels;
  Skeleton::getCorrespondingVoxels(e1, voxels);
  ASSERT_TRUE(Voxel(Point{0, 1, 1}) == voxels[0] ||
              Voxel(Point{0, 1, 1}) == voxels[1])
      << name << "::e1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 2}) == voxels[0] ||
              Voxel(Point{0, 1, 2}) == voxels[1])
      << name << "::e1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";

  Skeleton::getCorrespondingVoxels(e2, voxels);
  ASSERT_TRUE(Voxel(Point{0, 1, 1}) == voxels[0] ||
              Voxel(Point{0, 1, 1}) == voxels[1])
      << name << "::e2 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1])
      << name << "::e2 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
}

template <class Skeleton, class Shape, class Voxel>
void verifyDualExistingVoxelsE(Skeleton skeleton, std::string name) {
  std::array<Point, 2> vertices;
  vertices[0] = Point{0, 1, 1};
  vertices[1] = Point{0, 1, 2};
  Cell<1> e1(vertices, 0);
  vertices[0] = Point{0, 1, 1};
  vertices[1] = Point{1, 1, 1};
  Cell<1> e2(vertices, 0);

  std::vector<Voxel> voxels;
  skeleton.template getCorrespondingVoxelsIn<1,Shape::CORE>(e1, voxels);
  ASSERT_EQ(1, voxels.size()) << name << "::e1 has " << voxels.size()
                              << " voxels in core instead of 1.";
  ASSERT_EQ(Voxel(Point{0, 1, 1}), voxels[0])
      << name << "::e1 wrong corresponding voxel in core: (" << voxels[0][0]
      << ", " << voxels[0][1] << ", " << voxels[0][2] << ").";
  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<1,Shape::MAIN_SHAPE>(e1, voxels);
  ASSERT_EQ(2, voxels.size()) << name << "::e1 has " << voxels.size()
                              << " voxels in main shape instead of 2.";
  ASSERT_TRUE(Voxel(Point{0, 1, 1}) == voxels[0] ||
              Voxel(Point{0, 1, 1}) == voxels[1])
      << name << "::e1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 2}) == voxels[0] ||
              Voxel(Point{0, 1, 2}) == voxels[1])
      << name << "::e1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<1,Shape::NEIGHBORHOOD>(e1, voxels);
  ASSERT_EQ(2, voxels.size()) << name << "::e1 has " << voxels.size()
                              << " voxels in neighborhood instead of 2.";
  ASSERT_TRUE(Voxel(Point{0, 1, 1}) == voxels[0] ||
              Voxel(Point{0, 1, 1}) == voxels[1])
      << name << "::e1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 2}) == voxels[0] ||
              Voxel(Point{0, 1, 2}) == voxels[1])
      << name << "::e1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<1,Shape::CORE>(e2, voxels);
  ASSERT_EQ(2, voxels.size()) << name << "::e1 has " << voxels.size()
                              << " voxels in core instead of 1.";
  ASSERT_TRUE(Voxel(Point{0, 1, 1}) == voxels[0] ||
              Voxel(Point{0, 1, 1}) == voxels[1])
      << name << "::e1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1])
      << name << "::e1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<1,Shape::MAIN_SHAPE>(e2, voxels);
  ASSERT_EQ(2, voxels.size()) << name << "::e1 has " << voxels.size()
                              << " voxels in main shape instead of 2.";
  ASSERT_TRUE(Voxel(Point{0, 1, 1}) == voxels[0] ||
              Voxel(Point{0, 1, 1}) == voxels[1])
      << name << "::e1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1])
      << name << "::e1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<1,Shape::NEIGHBORHOOD>(e2, voxels);
  ASSERT_EQ(2, voxels.size()) << name << "::e1 has " << voxels.size()
                              << " voxels in neighborhood instead of 2.";
  ASSERT_TRUE(Voxel(Point{0, 1, 1}) == voxels[0] ||
              Voxel(Point{0, 1, 1}) == voxels[1])
      << name << "::e1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1])
      << name << "::e1 wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
}

template <class Skeleton, class Voxel>
void verifyDualSurroundingVoxelsS(std::string name) {
  std::array<Point, 4> vertices;
  vertices[0] = Point{1, 0, 1};
  vertices[1] = Point{1, 1, 1};
  vertices[2] = Point{1, 0, 2};
  vertices[3] = Point{1, 1, 2};
  Cell<2> s(vertices, 0);

  std::array<Voxel, 4> voxels;
  Skeleton::getCorrespondingVoxels(s, voxels);
  ASSERT_TRUE(Voxel(Point{1, 0, 1}) == voxels[0] ||
              Voxel(Point{1, 0, 1}) == voxels[1] ||
              Voxel(Point{1, 0, 1}) == voxels[2] ||
              Voxel(Point{1, 0, 1}) == voxels[3])
      << name << "::s wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1] ||
              Voxel(Point{1, 1, 1}) == voxels[2] ||
              Voxel(Point{1, 1, 1}) == voxels[3])
      << name << "::s wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 0, 2}) == voxels[0] ||
              Voxel(Point{1, 0, 2}) == voxels[1] ||
              Voxel(Point{1, 0, 2}) == voxels[2] ||
              Voxel(Point{1, 0, 2}) == voxels[3])
      << name << "::s wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 2}) == voxels[0] ||
              Voxel(Point{1, 1, 2}) == voxels[1] ||
              Voxel(Point{1, 1, 2}) == voxels[2] ||
              Voxel(Point{1, 1, 2}) == voxels[3])
      << name << "::s wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << ").";
}

template <class Skeleton, class Shape, class Voxel>
void verifyDualExistingVoxelsS(Skeleton skeleton, std::string name) {
  std::array<Point, 4> vertices;
  vertices[0] = Point{1, 0, 1};
  vertices[1] = Point{1, 1, 1};
  vertices[2] = Point{1, 0, 2};
  vertices[3] = Point{1, 1, 2};
  Cell<2> s(vertices, 0);

  std::vector<Voxel> voxels;
  skeleton.template getCorrespondingVoxelsIn<2,Shape::CORE>(s, voxels);
  ASSERT_EQ(1, voxels.size());
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0])
      << name << "::s wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << ").";

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<2,Shape::MAIN_SHAPE>(s, voxels);
  ASSERT_EQ(3, voxels.size());
  ASSERT_TRUE(Voxel(Point{1, 0, 1}) == voxels[0] ||
              Voxel(Point{1, 0, 1}) == voxels[1] ||
              Voxel(Point{1, 0, 1}) == voxels[2])
      << name << "::s wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1] ||
              Voxel(Point{1, 1, 1}) == voxels[2])
      << name << "::s wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 2}) == voxels[0] ||
              Voxel(Point{1, 1, 2}) == voxels[1] ||
              Voxel(Point{1, 1, 2}) == voxels[2])
      << name << "::s wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << ").";

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<2,Shape::NEIGHBORHOOD>(s, voxels);
  ASSERT_EQ(4, voxels.size());
  ASSERT_TRUE(Voxel(Point{1, 0, 1}) == voxels[0] ||
              Voxel(Point{1, 0, 1}) == voxels[1] ||
              Voxel(Point{1, 0, 1}) == voxels[2] ||
              Voxel(Point{1, 0, 1}) == voxels[3])
      << name << "::s wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1] ||
              Voxel(Point{1, 1, 1}) == voxels[2] ||
              Voxel(Point{1, 1, 1}) == voxels[3])
      << name << "::s wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 0, 2}) == voxels[0] ||
              Voxel(Point{1, 0, 2}) == voxels[1] ||
              Voxel(Point{1, 0, 2}) == voxels[2] ||
              Voxel(Point{1, 0, 2}) == voxels[3])
      << name << "::s wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 2}) == voxels[0] ||
              Voxel(Point{1, 1, 2}) == voxels[1] ||
              Voxel(Point{1, 1, 2}) == voxels[2] ||
              Voxel(Point{1, 1, 2}) == voxels[3])
      << name << "::s wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << ").";
}

template <class Skeleton, class Voxel>
void verifyDualSurroundingVoxelsC(std::string name) {
  std::array<Point, 8> vertices;
  vertices[0] = Point{0, 0, 1};
  vertices[1] = Point{0, 1, 1};
  vertices[2] = Point{0, 0, 2};
  vertices[3] = Point{0, 1, 2};
  vertices[4] = Point{1, 0, 1};
  vertices[5] = Point{1, 1, 1};
  vertices[6] = Point{1, 0, 2};
  vertices[7] = Point{1, 1, 2};
  Cell<3> c(vertices, 0);

  std::array<Voxel, 8> voxels;
  Skeleton::getCorrespondingVoxels(c, voxels);
  ASSERT_TRUE(Voxel(Point{0, 0, 1}) == voxels[0] ||
              Voxel(Point{0, 0, 1}) == voxels[1] ||
              Voxel(Point{0, 0, 1}) == voxels[2] ||
              Voxel(Point{0, 0, 1}) == voxels[3] ||
              Voxel(Point{0, 0, 1}) == voxels[4] ||
              Voxel(Point{0, 0, 1}) == voxels[5] ||
              Voxel(Point{0, 0, 1}) == voxels[6] ||
              Voxel(Point{0, 0, 1}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 1}) == voxels[0] ||
              Voxel(Point{0, 1, 1}) == voxels[1] ||
              Voxel(Point{0, 1, 1}) == voxels[2] ||
              Voxel(Point{0, 1, 1}) == voxels[3] ||
              Voxel(Point{0, 1, 1}) == voxels[4] ||
              Voxel(Point{0, 1, 1}) == voxels[5] ||
              Voxel(Point{0, 1, 1}) == voxels[6] ||
              Voxel(Point{0, 1, 1}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 0, 2}) == voxels[0] ||
              Voxel(Point{0, 0, 2}) == voxels[1] ||
              Voxel(Point{0, 0, 2}) == voxels[2] ||
              Voxel(Point{0, 0, 2}) == voxels[3] ||
              Voxel(Point{0, 0, 2}) == voxels[4] ||
              Voxel(Point{0, 0, 2}) == voxels[5] ||
              Voxel(Point{0, 0, 2}) == voxels[6] ||
              Voxel(Point{0, 0, 2}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 2}) == voxels[0] ||
              Voxel(Point{0, 1, 2}) == voxels[1] ||
              Voxel(Point{0, 1, 2}) == voxels[2] ||
              Voxel(Point{0, 1, 2}) == voxels[3] ||
              Voxel(Point{0, 1, 2}) == voxels[4] ||
              Voxel(Point{0, 1, 2}) == voxels[5] ||
              Voxel(Point{0, 1, 2}) == voxels[6] ||
              Voxel(Point{0, 1, 2}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 0, 1}) == voxels[0] ||
              Voxel(Point{1, 0, 1}) == voxels[1] ||
              Voxel(Point{1, 0, 1}) == voxels[2] ||
              Voxel(Point{1, 0, 1}) == voxels[3] ||
              Voxel(Point{1, 0, 1}) == voxels[4] ||
              Voxel(Point{1, 0, 1}) == voxels[5] ||
              Voxel(Point{1, 0, 1}) == voxels[6] ||
              Voxel(Point{1, 0, 1}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1] ||
              Voxel(Point{1, 1, 1}) == voxels[2] ||
              Voxel(Point{1, 1, 1}) == voxels[3] ||
              Voxel(Point{1, 1, 1}) == voxels[4] ||
              Voxel(Point{1, 1, 1}) == voxels[5] ||
              Voxel(Point{1, 1, 1}) == voxels[6] ||
              Voxel(Point{1, 1, 1}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 0, 2}) == voxels[0] ||
              Voxel(Point{1, 0, 2}) == voxels[1] ||
              Voxel(Point{1, 0, 2}) == voxels[2] ||
              Voxel(Point{1, 0, 2}) == voxels[3] ||
              Voxel(Point{1, 0, 2}) == voxels[4] ||
              Voxel(Point{1, 0, 2}) == voxels[5] ||
              Voxel(Point{1, 0, 2}) == voxels[6] ||
              Voxel(Point{1, 0, 2}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 2}) == voxels[0] ||
              Voxel(Point{1, 1, 2}) == voxels[1] ||
              Voxel(Point{1, 1, 2}) == voxels[2] ||
              Voxel(Point{1, 1, 2}) == voxels[3] ||
              Voxel(Point{1, 1, 2}) == voxels[4] ||
              Voxel(Point{1, 1, 2}) == voxels[5] ||
              Voxel(Point{1, 1, 2}) == voxels[6] ||
              Voxel(Point{1, 1, 2}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
}

template <class Skeleton, class Shape, class Voxel>
void verifyDualExistingVoxelsC(Skeleton skeleton, std::string name) {
  std::array<Point, 8> vertices;
  vertices[0] = Point{0, 0, 1};
  vertices[1] = Point{0, 1, 1};
  vertices[2] = Point{0, 0, 2};
  vertices[3] = Point{0, 1, 2};
  vertices[4] = Point{1, 0, 1};
  vertices[5] = Point{1, 1, 1};
  vertices[6] = Point{1, 0, 2};
  vertices[7] = Point{1, 1, 2};
  Cell<3> c(vertices, 0);

  std::vector<Voxel> voxels;
  skeleton.template getCorrespondingVoxelsIn<3,Shape::CORE>(c, voxels);
  ASSERT_EQ(2, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 1, 1}) == voxels[0] ||
              Voxel(Point{0, 1, 1}) == voxels[1])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << ").";

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<3,Shape::MAIN_SHAPE>(c, voxels);
  ASSERT_EQ(7, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 0, 1}) == voxels[0] ||
              Voxel(Point{0, 0, 1}) == voxels[1] ||
              Voxel(Point{0, 0, 1}) == voxels[2] ||
              Voxel(Point{0, 0, 1}) == voxels[3] ||
              Voxel(Point{0, 0, 1}) == voxels[4] ||
              Voxel(Point{0, 0, 1}) == voxels[5] ||
              Voxel(Point{0, 0, 1}) == voxels[6])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 1}) == voxels[0] ||
              Voxel(Point{0, 1, 1}) == voxels[1] ||
              Voxel(Point{0, 1, 1}) == voxels[2] ||
              Voxel(Point{0, 1, 1}) == voxels[3] ||
              Voxel(Point{0, 1, 1}) == voxels[4] ||
              Voxel(Point{0, 1, 1}) == voxels[5] ||
              Voxel(Point{0, 1, 1}) == voxels[6])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 0, 2}) == voxels[0] ||
              Voxel(Point{0, 0, 2}) == voxels[1] ||
              Voxel(Point{0, 0, 2}) == voxels[2] ||
              Voxel(Point{0, 0, 2}) == voxels[3] ||
              Voxel(Point{0, 0, 2}) == voxels[4] ||
              Voxel(Point{0, 0, 2}) == voxels[5] ||
              Voxel(Point{0, 0, 2}) == voxels[6])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 2}) == voxels[0] ||
              Voxel(Point{0, 1, 2}) == voxels[1] ||
              Voxel(Point{0, 1, 2}) == voxels[2] ||
              Voxel(Point{0, 1, 2}) == voxels[3] ||
              Voxel(Point{0, 1, 2}) == voxels[4] ||
              Voxel(Point{0, 1, 2}) == voxels[5] ||
              Voxel(Point{0, 1, 2}) == voxels[6])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 0, 1}) == voxels[0] ||
              Voxel(Point{1, 0, 1}) == voxels[1] ||
              Voxel(Point{1, 0, 1}) == voxels[2] ||
              Voxel(Point{1, 0, 1}) == voxels[3] ||
              Voxel(Point{1, 0, 1}) == voxels[4] ||
              Voxel(Point{1, 0, 1}) == voxels[5] ||
              Voxel(Point{1, 0, 1}) == voxels[6])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1] ||
              Voxel(Point{1, 1, 1}) == voxels[2] ||
              Voxel(Point{1, 1, 1}) == voxels[3] ||
              Voxel(Point{1, 1, 1}) == voxels[4] ||
              Voxel(Point{1, 1, 1}) == voxels[5] ||
              Voxel(Point{1, 1, 1}) == voxels[6])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 2}) == voxels[0] ||
              Voxel(Point{1, 1, 2}) == voxels[1] ||
              Voxel(Point{1, 1, 2}) == voxels[2] ||
              Voxel(Point{1, 1, 2}) == voxels[3] ||
              Voxel(Point{1, 1, 2}) == voxels[4] ||
              Voxel(Point{1, 1, 2}) == voxels[5] ||
              Voxel(Point{1, 1, 2}) == voxels[6])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << ").";

  voxels.clear();
  skeleton.template getCorrespondingVoxelsIn<3,Shape::NEIGHBORHOOD>(c, voxels);
  ASSERT_EQ(8, voxels.size());
  ASSERT_TRUE(Voxel(Point{0, 0, 1}) == voxels[0] ||
              Voxel(Point{0, 0, 1}) == voxels[1] ||
              Voxel(Point{0, 0, 1}) == voxels[2] ||
              Voxel(Point{0, 0, 1}) == voxels[3] ||
              Voxel(Point{0, 0, 1}) == voxels[4] ||
              Voxel(Point{0, 0, 1}) == voxels[5] ||
              Voxel(Point{0, 0, 1}) == voxels[6] ||
              Voxel(Point{0, 0, 1}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 1}) == voxels[0] ||
              Voxel(Point{0, 1, 1}) == voxels[1] ||
              Voxel(Point{0, 1, 1}) == voxels[2] ||
              Voxel(Point{0, 1, 1}) == voxels[3] ||
              Voxel(Point{0, 1, 1}) == voxels[4] ||
              Voxel(Point{0, 1, 1}) == voxels[5] ||
              Voxel(Point{0, 1, 1}) == voxels[6] ||
              Voxel(Point{0, 1, 1}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 0, 2}) == voxels[0] ||
              Voxel(Point{0, 0, 2}) == voxels[1] ||
              Voxel(Point{0, 0, 2}) == voxels[2] ||
              Voxel(Point{0, 0, 2}) == voxels[3] ||
              Voxel(Point{0, 0, 2}) == voxels[4] ||
              Voxel(Point{0, 0, 2}) == voxels[5] ||
              Voxel(Point{0, 0, 2}) == voxels[6] ||
              Voxel(Point{0, 0, 2}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{0, 1, 2}) == voxels[0] ||
              Voxel(Point{0, 1, 2}) == voxels[1] ||
              Voxel(Point{0, 1, 2}) == voxels[2] ||
              Voxel(Point{0, 1, 2}) == voxels[3] ||
              Voxel(Point{0, 1, 2}) == voxels[4] ||
              Voxel(Point{0, 1, 2}) == voxels[5] ||
              Voxel(Point{0, 1, 2}) == voxels[6] ||
              Voxel(Point{0, 1, 2}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 0, 1}) == voxels[0] ||
              Voxel(Point{1, 0, 1}) == voxels[1] ||
              Voxel(Point{1, 0, 1}) == voxels[2] ||
              Voxel(Point{1, 0, 1}) == voxels[3] ||
              Voxel(Point{1, 0, 1}) == voxels[4] ||
              Voxel(Point{1, 0, 1}) == voxels[5] ||
              Voxel(Point{1, 0, 1}) == voxels[6] ||
              Voxel(Point{1, 0, 1}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 1}) == voxels[0] ||
              Voxel(Point{1, 1, 1}) == voxels[1] ||
              Voxel(Point{1, 1, 1}) == voxels[2] ||
              Voxel(Point{1, 1, 1}) == voxels[3] ||
              Voxel(Point{1, 1, 1}) == voxels[4] ||
              Voxel(Point{1, 1, 1}) == voxels[5] ||
              Voxel(Point{1, 1, 1}) == voxels[6] ||
              Voxel(Point{1, 1, 1}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 0, 2}) == voxels[0] ||
              Voxel(Point{1, 0, 2}) == voxels[1] ||
              Voxel(Point{1, 0, 2}) == voxels[2] ||
              Voxel(Point{1, 0, 2}) == voxels[3] ||
              Voxel(Point{1, 0, 2}) == voxels[4] ||
              Voxel(Point{1, 0, 2}) == voxels[5] ||
              Voxel(Point{1, 0, 2}) == voxels[6] ||
              Voxel(Point{1, 0, 2}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
  ASSERT_TRUE(Voxel(Point{1, 1, 2}) == voxels[0] ||
              Voxel(Point{1, 1, 2}) == voxels[1] ||
              Voxel(Point{1, 1, 2}) == voxels[2] ||
              Voxel(Point{1, 1, 2}) == voxels[3] ||
              Voxel(Point{1, 1, 2}) == voxels[4] ||
              Voxel(Point{1, 1, 2}) == voxels[5] ||
              Voxel(Point{1, 1, 2}) == voxels[6] ||
              Voxel(Point{1, 1, 2}) == voxels[7])
      << name << "::c wrong corresponding voxel: (" << voxels[0][0] << ", "
      << voxels[0][1] << ", " << voxels[0][2] << "),(" << voxels[1][0] << ", "
      << voxels[1][1] << ", " << voxels[1][2] << "),(" << voxels[2][0] << ", "
      << voxels[2][1] << ", " << voxels[2][2] << "),(" << voxels[3][0] << ", "
      << voxels[3][1] << ", " << voxels[3][2] << "),(" << voxels[4][0] << ", "
      << voxels[4][1] << ", " << voxels[4][2] << "),(" << voxels[5][0] << ", "
      << voxels[5][1] << ", " << voxels[5][2] << "),(" << voxels[6][0] << ", "
      << voxels[6][1] << ", " << voxels[6][2] << "),(" << voxels[7][0] << ", "
      << voxels[7][1] << ", " << voxels[7][2] << ").";
}
