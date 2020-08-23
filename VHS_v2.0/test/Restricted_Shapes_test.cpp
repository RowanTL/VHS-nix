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
 * @file Restricted_Shapes_test.cpp
 * Test file for the Restricted Shape classes.
 **/

#include <algorithm>
#include <gtest/gtest.h>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "shape/AddableShape.h"
#include "shape/BaseShape.h"
#include "shape/EditableShape.h"
#include "shape/RemovableShape.h"
#include "shape/RestrictedShapeCopy.h"
#include "shape/ClosedRestrictedShapeCopy.h"
#include "shape/InvertedRestrictedShapeCopy.h"
#include "shape/Voxel.h"
#include "util/Point.h"

#include "test_examples.h"

using shape_simplification::shape::AddableShape;
using shape_simplification::shape::BaseShape;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::RemovableShape;
using shape_simplification::shape::RestrictedShapeCopy;
using shape_simplification::shape::ClosedRestrictedShapeCopy;
using shape_simplification::shape::InvertedRestrictedShapeCopy;
using shape_simplification::shape::Voxel;
using shape_simplification::util::Point;

template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrVoxelNotContained(
        Voxel v,
        RestrictedShape &shape,
        std::string name);
template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrVoxelAppearsInCore(
        Voxel v,
        RestrictedShape &shape,
        std::string name);
template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrVoxelAppearsInMainShape(
        Voxel v,
        RestrictedShape &shape,
        std::string name);
template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrVoxelAppearsInNeighborhood(
        Voxel v,
        RestrictedShape &shape,
        std::string name);

template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrContent(
        std::string name,
        std::vector<Voxel> &voxels,
        std::vector<typename Shape::Level> &minLevels,
        std::unordered_map<Voxel, typename Shape::Level,
        std::hash<Voxel>> &realMinLevel);
template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrContainsMethod(std::string name);
template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrIterators(std::string name);
template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrNonEndIteratorStep(
        typename RestrictedShape::voxel_iterator &it,
        RestrictedShape &shape,
        typename Shape::Level level,
        Voxel &value,
        std::string name);
template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrEndIteratorStep(
        typename RestrictedShape::voxel_iterator &it,
        RestrictedShape &shape,
        typename Shape::Level level,
        std::string name);
template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrMinLevelInformation(std::string name);

template <class Shape, class Voxel>
void verifyInvRestrContent(std::string name);
template <class Shape, class Voxel>
void verifyInvRestrIterators(std::string name);

TEST(RestrictedShapeTest, Constructors) {
    std::mt19937 randomGenerator(0);
    std::uniform_int_distribution<unsigned int> unisize(0, 1000);
    std::vector<Voxel> voxels;
    std::vector<BaseShape<Voxel>::Level> minLevels;
    std::unordered_map<Voxel, BaseShape<Voxel>::Level, std::hash<Voxel>> realMinLevel;

    for (int repetition = 0; repetition < 300; repetition++) {
        if (repetition % 50 == 0) std::cout << "loop: " << repetition << "\n";
        voxels.clear();
        minLevels.clear();
        realMinLevel.clear();
        unsigned int nberOfVoxels = unisize(randomGenerator);

        buildRandomExample<BaseShape<Voxel>,Voxel>(
                    nberOfVoxels, voxels, minLevels, realMinLevel);

        verifyRestrContent<RestrictedShapeCopy<BaseShape<Voxel>, Voxel>,
                BaseShape<Voxel>, Voxel>("base shape copy", voxels,
                                         minLevels, realMinLevel);
        verifyRestrContent<RestrictedShapeCopy<AddableShape<Voxel>, Voxel>,
                AddableShape<Voxel>, Voxel>("addable shape copy", voxels,
                                            minLevels, realMinLevel);
        verifyRestrContent<RestrictedShapeCopy<RemovableShape<Voxel>, Voxel>,
                RemovableShape<Voxel>, Voxel>(
                    "removable shape copy", voxels, minLevels, realMinLevel);
        verifyRestrContent<RestrictedShapeCopy<EditableShape<Voxel>, Voxel>,
                EditableShape<Voxel>, Voxel>(
                    "editable shape copy", voxels, minLevels, realMinLevel);

        verifyRestrContent<ClosedRestrictedShapeCopy<BaseShape<Voxel>, Voxel>,
                BaseShape<Voxel>, Voxel>("base shape closed copy", voxels,
                                         minLevels, realMinLevel);
        verifyRestrContent<ClosedRestrictedShapeCopy<AddableShape<Voxel>, Voxel>,
                AddableShape<Voxel>, Voxel>("addable shape closed copy", voxels,
                                            minLevels, realMinLevel);
        verifyRestrContent<ClosedRestrictedShapeCopy<RemovableShape<Voxel>, Voxel>,
                RemovableShape<Voxel>, Voxel>(
                    "removable shape closed copy", voxels, minLevels, realMinLevel);
        verifyRestrContent<ClosedRestrictedShapeCopy<EditableShape<Voxel>, Voxel>,
                EditableShape<Voxel>, Voxel>(
                    "editable shape closed copy", voxels, minLevels, realMinLevel);
    }
}

template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrContent(
        std::string name, std::vector<Voxel> &voxels, std::vector<typename Shape::Level> &minLevels,
        std::unordered_map<Voxel, typename Shape::Level, std::hash<Voxel>> &realMinLevel) {
    Shape shape(voxels, minLevels);

    int maxDim[3] = {0, 0, 0};
    Point lowerBound = {35, 40, 45};
    Point upperBound = {55, 60, 65};
    RestrictedShape resShape(shape, lowerBound, upperBound);

    for (Voxel v : voxels) {
        typename Shape::Level l = realMinLevel.at(v);

        if (v[0] < lowerBound[0] - 1 ||
                v[1] < lowerBound[1] - 1 ||
                v[2] < lowerBound[2] - 1 ||
                v[0] > upperBound[0] + 1 ||
                v[1] > upperBound[1] + 1 ||
                v[2] > upperBound[2] + 1) {
            verifyRestrVoxelNotContained<RestrictedShape, Shape, Voxel>(
                        v, resShape, name);
        } else if (l != Shape::OUTSIDE &&
                   v[0] >= lowerBound[0] &&
                   v[1] >= lowerBound[1] &&
                   v[2] >= lowerBound[2] &&
                   v[0] <= upperBound[0] &&
                   v[1] <= upperBound[1] &&
                   v[2] <= upperBound[2]) {
            if (v[0] > maxDim[0]) maxDim[0] = v[0];
            if (v[1] > maxDim[1]) maxDim[1] = v[1];
            if (v[2] > maxDim[2]) maxDim[2] = v[2];

            if (l == Shape::CORE)
                verifyRestrVoxelAppearsInCore<RestrictedShape, Shape, Voxel>(
                            v, resShape, name);
            else if (l == Shape::MAIN_SHAPE)
                verifyRestrVoxelAppearsInMainShape<RestrictedShape, Shape, Voxel>(
                            v, resShape, name);
            else if (l == Shape::NEIGHBORHOOD)
                verifyRestrVoxelAppearsInNeighborhood<RestrictedShape, Shape, Voxel>(
                            v, resShape, name);
        }
    }
    const std::array<unsigned int, 3>& dimensions = resShape.getShapeDimensions();

    ASSERT_EQ(dimensions[0], maxDim[0]);
    ASSERT_EQ(dimensions[1], maxDim[1]);
    ASSERT_EQ(dimensions[2], maxDim[2]);
}

TEST(RestrictedShapeTest, ContainsMethod) {
    verifyRestrContainsMethod<RestrictedShapeCopy<BaseShape<Voxel>, Voxel>,
            BaseShape<Voxel>, Voxel>("base shape copy");
    verifyRestrContainsMethod<RestrictedShapeCopy<AddableShape<Voxel>, Voxel>,
            AddableShape<Voxel>, Voxel>("addable shape copy");
    verifyRestrContainsMethod<RestrictedShapeCopy<RemovableShape<Voxel>, Voxel>,
            RemovableShape<Voxel>, Voxel>("removable shape copy");
    verifyRestrContainsMethod<RestrictedShapeCopy<EditableShape<Voxel>, Voxel>,
            EditableShape<Voxel>, Voxel>("editable shape copy");

    verifyRestrContainsMethod<ClosedRestrictedShapeCopy<BaseShape<Voxel>, Voxel>,
            BaseShape<Voxel>, Voxel>("base shape closed copy");
    verifyRestrContainsMethod<ClosedRestrictedShapeCopy<AddableShape<Voxel>, Voxel>,
            AddableShape<Voxel>, Voxel>("addable shape closed copy");
    verifyRestrContainsMethod<ClosedRestrictedShapeCopy<RemovableShape<Voxel>, Voxel>,
            RemovableShape<Voxel>, Voxel>("removable shape copy");
    verifyRestrContainsMethod<ClosedRestrictedShapeCopy<EditableShape<Voxel>, Voxel>,
            EditableShape<Voxel>, Voxel>("editable shape closed copy");
}

template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrContainsMethod(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildMiniExample1<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);

    Point lowerBound = {0, 1, 0};
    Point upperBound = {2, 2, 2};

    RestrictedShape resShape(shape, lowerBound, upperBound);

    verifyRestrVoxelAppearsInMainShape<RestrictedShape, Shape, Voxel>(
                Voxel(0, 1, 0), resShape, name);
    verifyRestrVoxelAppearsInCore<RestrictedShape, Shape, Voxel>(
                Voxel(1, 1, 1), resShape, name);
    verifyRestrVoxelNotContained<RestrictedShape, Shape, Voxel>(
                Voxel(0, 0, 4), resShape, name);
}

TEST(RestrictedShapeTest, Iterators) {
    verifyRestrIterators<RestrictedShapeCopy<BaseShape<Voxel>, Voxel>,
            BaseShape<Voxel>, Voxel>("base shape copy");
    verifyRestrIterators<RestrictedShapeCopy<AddableShape<Voxel>, Voxel>,
            AddableShape<Voxel>, Voxel>("addable shape copy");
    verifyRestrIterators<RestrictedShapeCopy<RemovableShape<Voxel>, Voxel>,
            RemovableShape<Voxel>, Voxel>("removable shape copy");
    verifyRestrIterators<RestrictedShapeCopy<EditableShape<Voxel>, Voxel>,
            EditableShape<Voxel>, Voxel>("editable shape copy");
}

template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrIterators(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;
    int count = 0;

    buildMiniExample1<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);

    Point lowerBound = {0, 1, 0};
    Point upperBound = {2, 2, 2};
    RestrictedShape resShape(shape, lowerBound, upperBound);

    for (typename RestrictedShape::voxel_iterator it =
         resShape.template begin<Shape::CORE>();
         it != resShape.template end<Shape::CORE>(); it++)
    {
        ++count;
    }
    ASSERT_EQ(count, 1) << name;
    count = 0;
    for (typename RestrictedShape::voxel_iterator it =
         resShape.template begin<Shape::MAIN_SHAPE>();
         it != resShape.template end<Shape::MAIN_SHAPE>(); it++)
    {
        ++count;
    }
    ASSERT_EQ(count, 2) << name;
    count = 0;
    for (typename RestrictedShape::voxel_iterator it =
         resShape.template begin<Shape::NEIGHBORHOOD>();
         it != resShape.template end<Shape::NEIGHBORHOOD>(); it++)
    {
        ++count;
    }
    ASSERT_EQ(count, 2) << name;
}

template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrNonEndIteratorStep(typename RestrictedShape::voxel_iterator &it,
                                   RestrictedShape &shape,
                                   typename Shape::Level level, Voxel &value,
                                   std::string name) {
    if (level == Shape::CORE) {
        typename RestrictedShape::voxel_iterator end =
                shape.template end<Shape::CORE>();
        ASSERT_NE(it, end) << "Iterator " << name << " is at the end unexpectedly.";
    } else if (level == Shape::MAIN_SHAPE) {
        typename RestrictedShape::voxel_iterator end =
                shape.template end<Shape::MAIN_SHAPE>();
        ASSERT_NE(it, end) << "Iterator " << name << " is at the end unexpectedly.";
    } else {
        typename RestrictedShape::voxel_iterator end =
                shape.template end<Shape::NEIGHBORHOOD>();
        ASSERT_NE(it, end) << "Iterator " << name << " is at the end unexpectedly.";
    }

    Voxel v = *it;
    ASSERT_EQ(v, value) << "Not right value for " << name << ": (" << v[0] << ","
                        << v[1] << "," << v[2] << ") instead of (" << value[0]
                        << "," << value[1] << "," << value[2] << ").";
    it++;
}

template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrEndIteratorStep(typename RestrictedShape::voxel_iterator &it,
                                RestrictedShape &shape,
                                typename Shape::Level level, std::string name) {
    if (level == Shape::CORE) {
        typename RestrictedShape::voxel_iterator end =
                shape.template end<Shape::CORE>();
        ASSERT_EQ(it, end) << "Iterator " << name
                           << " is not at the end unexpectedly: " << (*end)[0]
                           << "," << (*end)[0] << "," << (*end)[0] << ".";
    } else if (level == Shape::MAIN_SHAPE) {
        typename RestrictedShape::voxel_iterator end =
                shape.template end<Shape::MAIN_SHAPE>();
        ASSERT_EQ(it, end) << "Iterator " << name
                           << " is not at the end unexpectedly: " << (*end)[0]
                           << "," << (*end)[0] << "," << (*end)[0] << ".";
    } else {
        typename RestrictedShape::voxel_iterator end =
                shape.template end<Shape::NEIGHBORHOOD>();
        ASSERT_EQ(it, end) << "Iterator " << name
                           << " is not at the end unexpectedly: " << (*end)[0]
                           << "," << (*end)[0] << "," << (*end)[0] << ".";
    }
}

TEST(RestrictedShapeTest, MinLevelInformation) {
    verifyRestrMinLevelInformation<RestrictedShapeCopy<BaseShape<Voxel>, Voxel>,
            BaseShape<Voxel>, Voxel>("base shape copy");
    verifyRestrMinLevelInformation<
            RestrictedShapeCopy<AddableShape<Voxel>, Voxel>, AddableShape<Voxel>,
            Voxel>("addable shape copy");
    verifyRestrMinLevelInformation<
            RestrictedShapeCopy<RemovableShape<Voxel>, Voxel>, RemovableShape<Voxel>,
            Voxel>("removable shape copy");
    verifyRestrMinLevelInformation<
            RestrictedShapeCopy<EditableShape<Voxel>, Voxel>, EditableShape<Voxel>,
            Voxel>("editable shape copy");

    verifyRestrMinLevelInformation<ClosedRestrictedShapeCopy<BaseShape<Voxel>, Voxel>,
            BaseShape<Voxel>, Voxel>("base shape closed copy");
    verifyRestrMinLevelInformation<
            ClosedRestrictedShapeCopy<AddableShape<Voxel>, Voxel>, AddableShape<Voxel>,
            Voxel>("addable shape closed copy");
    verifyRestrMinLevelInformation<
            ClosedRestrictedShapeCopy<RemovableShape<Voxel>, Voxel>, RemovableShape<Voxel>,
            Voxel>("removable shape closed copy");
    verifyRestrMinLevelInformation<
            ClosedRestrictedShapeCopy<EditableShape<Voxel>, Voxel>, EditableShape<Voxel>,
            Voxel>("editable shape closed copy");
}

template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrMinLevelInformation(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildMiniExample1<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);

    Point lowerBound = {0, 1, 0};
    Point upperBound = {2, 2, 2};
    RestrictedShape resShape(shape, lowerBound, upperBound);

    ASSERT_EQ(Shape::MAIN_SHAPE, resShape.getMinLevel(Voxel(0, 1, 0)))
            << "Neighborhood voxel does have neighborhood as min level in " << name
            << ".";
    ASSERT_EQ(Shape::CORE, resShape.getMinLevel(Voxel(1, 1, 1)))
            << "Main shape voxel does have main shape as min level in " << name
            << ".";
}

template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrVoxelNotContained(Voxel v, RestrictedShape &shape,
                                  std::string name) {
    ASSERT_FALSE(shape.template contains<Shape::CORE>(v))
            << "Outside voxel contained in " << name << " core.";
    ASSERT_FALSE(shape.template contains<Shape::MAIN_SHAPE>(v))
            << "Outside voxel contained in " << name << " main shape.";
    ASSERT_FALSE(shape.template contains<Shape::NEIGHBORHOOD>(v))
            << "Outside voxel contained in " << name << " neighborhood.";
    ASSERT_TRUE(shape.template contains<Shape::OUTSIDE>(v))
            << "Outside voxel contained in " << name << ".";
}

template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrVoxelAppearsInCore(Voxel v, RestrictedShape &shape,
                                   std::string name) {
    ASSERT_TRUE(shape.template contains<Shape::CORE>(v))
            << "Core voxel not contained in " << name << " core.";
    ASSERT_TRUE(shape.template contains<Shape::MAIN_SHAPE>(v))
            << "Core voxel not contained in " << name << " main shape.";
    ASSERT_TRUE(shape.template contains<Shape::NEIGHBORHOOD>(v))
            << "Core voxel not contained in " << name << " neighborhood.";
    ASSERT_FALSE(shape.template contains<Shape::OUTSIDE>(v))
            << "Core voxel not contained in " << name << ".";
}

template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrVoxelAppearsInMainShape(Voxel v, RestrictedShape &shape,
                                        std::string name) {
    ASSERT_FALSE(shape.template contains<Shape::CORE>(v))
            << "Main shape voxel contained in " << name << " core.";
    ASSERT_TRUE(shape.template contains<Shape::MAIN_SHAPE>(v))
            << "Main shape voxel not contained in " << name << " main shape.";
    ASSERT_TRUE(shape.template contains<Shape::NEIGHBORHOOD>(v))
            << "Main shape voxel not contained in " << name << " neighborhood.";
    ASSERT_FALSE(shape.template contains<Shape::OUTSIDE>(v))
            << "Main shape voxel not contained in " << name << ".";
}

template <class RestrictedShape, class Shape, class Voxel>
void verifyRestrVoxelAppearsInNeighborhood(Voxel v, RestrictedShape &shape,
                                           std::string name) {
    ASSERT_FALSE(shape.template contains<Shape::CORE>(v))
            << "Neighborhood voxel contained in " << name << " core.";
    ASSERT_FALSE(shape.template contains<Shape::MAIN_SHAPE>(v))
            << "Neighborhood voxel contained in " << name << " main shape.";
    ASSERT_TRUE(shape.template contains<Shape::NEIGHBORHOOD>(v))
            << "Neighborhood voxel not contained in " << name << " neighborhood.";
    ASSERT_FALSE(shape.template contains<Shape::OUTSIDE>(v))
            << "Neighborhood voxel not contained in " << name << ".";
}

TEST(RestrictedShapeTest, InvConstructor) {
    verifyInvRestrContent<BaseShape<Voxel>, Voxel>("base shape");
    verifyInvRestrContent<AddableShape<Voxel>, Voxel>("addable shape");
    verifyInvRestrContent<RemovableShape<Voxel>, Voxel>("removable shape");
    verifyInvRestrContent<EditableShape<Voxel>, Voxel>("editable shape");
}

template <class Shape, class Voxel>
void verifyInvRestrContent(std::string name){
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;
    using InvResShape = InvertedRestrictedShapeCopy<Shape,Voxel>;

    buildExample2<Shape,Voxel>(voxels, minLevels);

    Point lowerBound = {0, 2, 7};
    Point upperBound = {3, 7, 12};

    Shape shape(voxels, minLevels);
    InvResShape inv(shape, lowerBound, upperBound);

    ASSERT_EQ(lowerBound, inv.getLowerBound()) << name;
    ASSERT_EQ(upperBound, inv.getUpperBound()) << name;

    ASSERT_FALSE(inv.isAddable()) << name;
    ASSERT_FALSE(inv.isRemovable()) << name;

    ASSERT_EQ(3, inv.getShapeDimensions()[0]) << name;
    ASSERT_EQ(7, inv.getShapeDimensions()[1]) << name;
    ASSERT_EQ(12, inv.getShapeDimensions()[2]) << name;

    ASSERT_EQ(138, inv.getNeighborhoodSize()) << name;

    for (int x = 0; x < 3; x++){
        for (int y = 2; y < 8; y++){
            for (int z = 9; z < 13; z++){
                Voxel v(x, y, z);
                voxels.push_back(v);
                if ((x == 1 || x == 2) && (y == 4 || y == 5) && (z == 9 || z == 10)){
                    verifyRestrVoxelAppearsInNeighborhood<
                            InvResShape, Shape, Voxel
                            >(v, inv, name);
                    ASSERT_EQ(Shape::NEIGHBORHOOD, inv.getMinLevel(v));
                } else if (x == 3 && z == 13) {
                    verifyRestrVoxelNotContained<
                            InvResShape, Shape, Voxel
                            >(v, inv, name);
                    ASSERT_EQ(Shape::OUTSIDE, inv.getMinLevel(v));
                } else if (x > -1 && x < 3 && y > 2 && y < 7 && z > 7  && z < 12 &&
                           (x != 2 || z != 11)) {
                    verifyRestrVoxelAppearsInMainShape<
                            InvResShape, Shape, Voxel
                            >(v, inv, name);
                    ASSERT_EQ(Shape::MAIN_SHAPE, inv.getMinLevel(v));
                } else {
                    verifyRestrVoxelAppearsInCore<
                            InvResShape, Shape, Voxel
                            >(v, inv, name);
                    ASSERT_EQ(Shape::CORE, inv.getMinLevel(v));
                }
            }
        }
    }
}

TEST(RestrictedShapeTest, InvIterators) {
    verifyInvRestrIterators<BaseShape<Voxel>, Voxel>("base shape");
    verifyInvRestrIterators<AddableShape<Voxel>, Voxel>("addable shape");
    verifyInvRestrIterators<RemovableShape<Voxel>, Voxel>("removable shape");
    verifyInvRestrIterators<EditableShape<Voxel>, Voxel>("editable shape");
}

template <class Shape, class Voxel>
void verifyInvRestrIterators(std::string name){
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;
    using InvResShape = InvertedRestrictedShapeCopy<Shape,Voxel>;

    buildExample2<Shape,Voxel>(voxels, minLevels);

    Point lowerBound = {0, 2, 7};
    Point upperBound = {3, 7, 12};

    Shape shape(voxels, minLevels);
    InvResShape inv(shape, lowerBound, upperBound);

    int count = 0;
    for (auto it = inv.template begin<Shape::CORE>();
         it != inv.template end<Shape::CORE>();
         ++it) count++;
    ASSERT_EQ(count, 94) << name;

    count = 0;
    for (auto it = inv.template begin<Shape::MAIN_SHAPE>();
         it != inv.template end<Shape::MAIN_SHAPE>();
         ++it) count++;
    ASSERT_EQ(count, 130) << name;

    count = 0;
    for (auto it = inv.template begin<Shape::NEIGHBORHOOD>();
         it != inv.template end<Shape::NEIGHBORHOOD>();
         ++it) count++;
    ASSERT_EQ(count, 138) << name;
}

