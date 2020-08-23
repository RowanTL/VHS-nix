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
 * @file Shape_test.cpp
 * Test file for the Shape classes.
 **/

#include <algorithm>
#include <gtest/gtest.h>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "shape/AddableShape.h"
#include "shape/AugmentedShape.h"
#include "shape/BaseShape.h"
#include "shape/EditableShape.h"
#include "shape/RemovableShape.h"
#include "shape/Voxel.h"
#include "util/Point.h"

#include "test_examples.h"

using shape_simplification::shape::AddableShape;
using shape_simplification::shape::AugmentedShape;
using shape_simplification::shape::BaseShape;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::RemovableShape;
using shape_simplification::shape::Voxel;
using shape_simplification::util::Point;

using AugShape = AugmentedShape<AddableShape<Voxel>, Voxel>;

template <class Shape, class Voxel>
void verifyShapeVoxelNotContained(
        Voxel v, Shape &shape, std::string name);
template <class Shape, class Voxel>
void verifyShapeVoxelAppearsInCore(
        Voxel v, Shape &shape, std::string name);
template <class Shape, class Voxel>
void verifyShapeVoxelAppearsInMainShape(
        Voxel v, Shape &shape, std::string name);
template <class Shape, class Voxel>
void verifyShapeVoxelAppearsInNeighborhood(
        Voxel v, Shape &shape, std::string name);

template <class Shape, class Voxel>
void verifyShapeContent(
        std::string name,
        std::vector<Voxel> &voxels,
        std::vector<typename Shape::Level> &minLevels,
        std::unordered_map<Voxel,typename Shape::Level,std::hash<Voxel>> &realMinLevel);
template <class Shape, class Voxel>
void verifyShapeContainsMethod(std::string name);
template <class Shape, class Voxel>
void verifyShapeIterators(std::string name);
template <class Shape, class Voxel>
void verifyShapeNonEndIteratorStep(
        typename Shape::voxel_iterator &it,
        Shape &shape,
        typename Shape::Level level,
        Voxel &value,
        std::string name);
template <class Shape, class Voxel>
void verifyShapeEndIteratorStep(
        typename Shape::voxel_iterator &it,
        Shape &shape,
        typename Shape::Level level,
        std::string name);
template <class Shape, class Voxel>
void verifyShapeAddibility(std::string name);
template <class Shape, class Voxel>
void verifyShapeNonAddibility(std::string name);
template <class Shape, class Voxel>
void verifyShapeRemovability(std::string name);
template <class Shape, class Voxel>
void verifyShapeNonRemovability(std::string name);
template <class Shape, class Voxel>
void verifyShapeMinLevelInformation(std::string name);

TEST(ShapeTest, Constructors) {
    std::mt19937 randomGenerator(0); // Repeatable seed
    std::uniform_int_distribution<unsigned int> unisize(0, 100);
    std::vector<Voxel> voxels;
    std::vector<BaseShape<Voxel>::Level> minLevels;
    std::unordered_map<Voxel, BaseShape<Voxel>::Level, std::hash<Voxel>> realMinLevel;

    for (int repetition = 0; repetition < 500; repetition++) {
        if (repetition % 100 == 0) std::cout << "loop: " << repetition << "\n";

        voxels.clear();
        minLevels.clear();
        realMinLevel.clear();
        unsigned int nberOfVoxels = unisize(randomGenerator);

        buildRandomExample<BaseShape<Voxel>,Voxel>(
                    nberOfVoxels, voxels, minLevels, realMinLevel);

        verifyShapeContent<BaseShape<Voxel>, Voxel>("base shape", voxels, minLevels,
                                                    realMinLevel);
        verifyShapeContent<AddableShape<Voxel>, Voxel>("addable shape", voxels,
                                                       minLevels, realMinLevel);
        verifyShapeContent<RemovableShape<Voxel>, Voxel>("removable shape", voxels,
                                                         minLevels, realMinLevel);
        verifyShapeContent<EditableShape<Voxel>, Voxel>("editable shape", voxels,
                                                        minLevels, realMinLevel);
        verifyShapeContent<AugShape, Voxel>("augmented shape", voxels, minLevels,
                                            realMinLevel);
    }
}

template <class Shape, class Voxel>
void verifyShapeContent(
        std::string name,
        std::vector<Voxel> &voxels,
        std::vector<typename Shape::Level> &minLevels,
        std::unordered_map<Voxel, typename Shape::Level, std::hash<Voxel> > &realMinLevel) {
    int maxDim[3] = {0, 0, 0};
    Shape emptyShape;
    Shape shape(voxels, minLevels);

    ASSERT_EQ(emptyShape.getShapeDimensions()[0], 0);
    ASSERT_EQ(emptyShape.getShapeDimensions()[1], 0);
    ASSERT_EQ(emptyShape.getShapeDimensions()[2], 0);

    for (Voxel v : voxels) {
        typename Shape::Level l = realMinLevel.at(v);

        verifyShapeVoxelNotContained(v, emptyShape, "empty " + name);

        if (l == Shape::CORE)
            verifyShapeVoxelAppearsInCore(v, shape, name);
        else if (l == Shape::MAIN_SHAPE)
            verifyShapeVoxelAppearsInMainShape(v, shape, name);
        else if (l == Shape::NEIGHBORHOOD)
            verifyShapeVoxelAppearsInNeighborhood(v, shape, name);

        if (v[0] > maxDim[0])
            maxDim[0] = v[0];
        if (v[1] > maxDim[1])
            maxDim[1] = v[1];
        if (v[2] > maxDim[2])
            maxDim[2] = v[2];
    }

    ASSERT_EQ(shape.getShapeDimensions()[0], maxDim[0]);
    ASSERT_EQ(shape.getShapeDimensions()[1], maxDim[1]);
    ASSERT_EQ(shape.getShapeDimensions()[2], maxDim[2]);
}

TEST(ShapeTest, ContainsMethod) {
    verifyShapeContainsMethod<BaseShape<Voxel>, Voxel>("base shape");
    verifyShapeContainsMethod<AddableShape<Voxel>, Voxel>("addable shape");
    verifyShapeContainsMethod<RemovableShape<Voxel>, Voxel>("removable shape");
    verifyShapeContainsMethod<EditableShape<Voxel>, Voxel>("editable shape");
    verifyShapeContainsMethod<AugShape, Voxel>("augmented shape");
}

template <class Shape, class Voxel>
void verifyShapeContainsMethod(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildMiniExample2<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);

    verifyShapeVoxelAppearsInCore(Voxel(0, 0, 0), shape, name);
    verifyShapeVoxelAppearsInNeighborhood(Voxel(0, 1, 0), shape, name);
    verifyShapeVoxelAppearsInMainShape(Voxel(2, 0, 3), shape, name);
    verifyShapeVoxelAppearsInCore(Voxel(0, 2, 1), shape, name);
    verifyShapeVoxelNotContained(Voxel(0, 0, 4), shape, name);
}

TEST(ShapeTest, Iterators) {
    verifyShapeIterators<BaseShape<Voxel>, Voxel>("base shape");
    verifyShapeIterators<AddableShape<Voxel>, Voxel>("addable shape");
    verifyShapeIterators<RemovableShape<Voxel>, Voxel>("removable shape");
    verifyShapeIterators<EditableShape<Voxel>, Voxel>("editable shape");
    verifyShapeIterators<AugShape, Voxel>("augmented shape");
}

template <class Shape, class Voxel>
void verifyShapeIterators(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;
    int count = 0;

    buildMiniExample2<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);

    for (typename Shape::voxel_iterator it = shape.template begin<Shape::CORE>();
         it != shape.template end<Shape::CORE>(); it++){
        ++count;
    }
    ASSERT_EQ(count, 2) << name;
    count = 0;
    for (typename Shape::voxel_iterator it = shape.template begin<Shape::MAIN_SHAPE>();
         it != shape.template end<Shape::MAIN_SHAPE>(); it++){
        ++count;
    }
    ASSERT_EQ(count, 3) << name;
    count = 0;
    for (typename Shape::voxel_iterator it = shape.template begin<Shape::NEIGHBORHOOD>();
         it != shape.template end<Shape::NEIGHBORHOOD>(); it++){
        ++count;
    }
    ASSERT_EQ(count, 4) << name;
}

template <class Shape, class Voxel>
void verifyShapeNonEndIteratorStep(typename Shape::voxel_iterator &it,
                                   Shape &shape, typename Shape::Level level,
                                   Voxel &value, std::string name) {
    if (level == Shape::CORE){
        typename Shape::voxel_iterator end = shape.template end<Shape::CORE>();
        ASSERT_NE(it, end) << "Core Iterator " << name << " is at the end unexpectedly.";
    } else if (level == Shape::MAIN_SHAPE){
        typename Shape::voxel_iterator end = shape.template end<Shape::MAIN_SHAPE>();
        ASSERT_NE(it, end) << "Shape Iterator " << name << " is at the end unexpectedly.";
    } else {
        typename Shape::voxel_iterator end = shape.template end<Shape::NEIGHBORHOOD>();
        ASSERT_NE(it, end) << "Neighborhood Iterator " << name << " is at the end unexpectedly.";
    }

    ASSERT_EQ(*it, value) << "Not right value for " << name << ": (" << (*it)[0]
                          << "," << (*it)[1] << "," << (*it)[2]
                          << ") instead of (" << value[0] << "," << value[1]
                          << "," << value[2] << ").";
}

template <class Shape, class Voxel>
void verifyShapeEndIteratorStep(typename Shape::voxel_iterator &it,
                                Shape &shape, typename Shape::Level level,
                                std::string name) {
    if (level == Shape::CORE){
        typename Shape::voxel_iterator end = shape.template end<Shape::CORE>();
        ASSERT_EQ(it, end) << "Core Iterator " << name << " is not at the end unexpectedly.";
    } else if (level == Shape::MAIN_SHAPE){
        typename Shape::voxel_iterator end = shape.template end<Shape::MAIN_SHAPE>();
        ASSERT_EQ(it, end) << "Shape Iterator " << name << " is not at the end unexpectedly.";
    } else {
        typename Shape::voxel_iterator end = shape.template end<Shape::NEIGHBORHOOD>();
        ASSERT_EQ(it, end) << "Neighborhood Iterator " << name << " is not at the end unexpectedly.";
    }
}

TEST(ShapeTest, Addibility) {
    verifyShapeNonAddibility<BaseShape<Voxel>, Voxel>("base shape");
    verifyShapeAddibility<AddableShape<Voxel>, Voxel>("addable shape");
    verifyShapeNonAddibility<RemovableShape<Voxel>, Voxel>("removable shape");
    verifyShapeAddibility<EditableShape<Voxel>, Voxel>("editable shape");
    verifyShapeAddibility<AugShape, Voxel>("augmented shape");
}

template <class Shape, class Voxel>
void verifyShapeNonAddibility(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildMiniExample2<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);

    ASSERT_FALSE(shape.isAddable()) << name << " should not be addable";
}

template <class Shape, class Voxel>
void verifyShapeAddibility(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildMiniExample2<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);

    ASSERT_TRUE(shape.isAddable()) << name << " should be addable";

    shape.template addVoxel<Shape::CORE>(Voxel(1, 1, 1));
    shape.template addVoxel<Shape::NEIGHBORHOOD>(Voxel(1, 2, 3));
    shape.template addVoxel<Shape::CORE>(Voxel(0, 1, 0));

    verifyShapeVoxelAppearsInCore<Shape, Voxel>(Voxel(1, 1, 1), shape,
                                                name + "::(1,1,1)");
    verifyShapeVoxelAppearsInNeighborhood<Shape, Voxel>(Voxel(1, 2, 3), shape,
                                                        name + "::(1,2,3)");
    verifyShapeVoxelAppearsInCore<Shape, Voxel>(Voxel(0, 1, 0), shape, name + "::v1");
}

TEST(ShapeTest, Removability) {
    verifyShapeNonRemovability<BaseShape<Voxel>, Voxel>("base shape");
    verifyShapeNonRemovability<AddableShape<Voxel>, Voxel>("addable shape");
    verifyShapeRemovability<RemovableShape<Voxel>, Voxel>("removable shape");
    verifyShapeRemovability<EditableShape<Voxel>, Voxel>("editable shape");
    verifyShapeNonRemovability<AugShape, Voxel>("augmented shape");
}

template <class Shape, class Voxel>
void verifyShapeNonRemovability(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildMiniExample2<Shape,Voxel>(voxels, minLevels);

    Voxel v4(1, 1, 1);
    Voxel v5(1, 2, 3);

    voxels.push_back(v4);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v5);
    minLevels.push_back(Shape::NEIGHBORHOOD);

    Shape shape(voxels, minLevels);

    ASSERT_FALSE(shape.isRemovable()) << name << " should not be removable";
}

template <class Shape, class Voxel>
void verifyShapeRemovability(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildMiniExample2<Shape,Voxel>(voxels, minLevels);

    Voxel v1(0, 1, 0);
    Voxel v3(0, 2, 1);
    Voxel v4(1, 1, 1);
    Voxel v5(1, 2, 3);

    voxels.push_back(v4);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v5);
    minLevels.push_back(Shape::NEIGHBORHOOD);

    Shape shape(voxels, minLevels);

    ASSERT_TRUE(shape.isRemovable()) << name << " should be removable";

    shape.template removeVoxel<Shape::CORE>(v4);
    shape.template removeVoxel<Shape::NEIGHBORHOOD>(v1);
    shape.template removeVoxel<Shape::NEIGHBORHOOD>(v3);

    verifyShapeVoxelAppearsInMainShape<Shape, Voxel>(v4, shape, name + "::v4");
    verifyShapeVoxelNotContained<Shape, Voxel>(v1, shape, name + "::v1");
    verifyShapeVoxelNotContained<Shape, Voxel>(v3, shape, name + "::v3");
}

TEST(ShapeTest, MinLevelInformation) {
    verifyShapeMinLevelInformation<BaseShape<Voxel>, Voxel>("base shape");
    verifyShapeMinLevelInformation<AddableShape<Voxel>, Voxel>("addable shape");
    verifyShapeMinLevelInformation<RemovableShape<Voxel>, Voxel>(
                "removable shape");
    verifyShapeMinLevelInformation<EditableShape<Voxel>, Voxel>("editable shape");
    verifyShapeMinLevelInformation<AugShape, Voxel>("augmented shape");
}

template <class Shape, class Voxel>
void verifyShapeMinLevelInformation(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildMiniExample2<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);

    ASSERT_EQ(Shape::CORE, shape.getMinLevel(Voxel(0, 0, 0)))
            << "Core voxel does have core as min level in " << name << ".";
    ASSERT_EQ(Shape::NEIGHBORHOOD, shape.getMinLevel(Voxel(0, 1, 0)))
            << "Neighborhood voxel does have neighborhood as min level in " << name
            << ".";
    ASSERT_EQ(Shape::MAIN_SHAPE, shape.getMinLevel(Voxel(2, 0, 3)))
            << "Main shape voxel does have main shape as min level in " << name
            << ".";
    ASSERT_EQ(Shape::CORE, shape.getMinLevel(Voxel(0, 2, 1)))
            << "Core voxel does have core as min level in " << name << ".";
}

template <class Shape, class Voxel>
void verifyShapeVoxelNotContained(Voxel v, Shape &shape, std::string name) {
    ASSERT_FALSE(shape.template contains<Shape::CORE>(v))
            << "Outside voxel contained in " << name << " core.";
    ASSERT_FALSE(shape.template contains<Shape::MAIN_SHAPE>(v))
            << "Outside voxel contained in " << name << " main shape.";
    ASSERT_FALSE(shape.template contains<Shape::NEIGHBORHOOD>(v))
            << "Outside voxel contained in " << name << " neighborhood.";
    ASSERT_TRUE(shape.template contains<Shape::OUTSIDE>(v))
            << "Outside voxel contained in " << name << ".";
}

template <class Shape, class Voxel>
void verifyShapeVoxelAppearsInCore(Voxel v, Shape &shape, std::string name) {
    ASSERT_TRUE(shape.template contains<Shape::CORE>(v))
            << "Core voxel not contained in " << name << " core.";
    ASSERT_TRUE(shape.template contains<Shape::MAIN_SHAPE>(v))
            << "Core voxel not contained in " << name << " main shape.";
    ASSERT_TRUE(shape.template contains<Shape::NEIGHBORHOOD>(v))
            << "Core voxel not contained in " << name << " neighborhood.";
    ASSERT_FALSE(shape.template contains<Shape::OUTSIDE>(v))
            << "Core voxel not contained in " << name << ".";
}

template <class Shape, class Voxel>
void verifyShapeVoxelAppearsInMainShape(Voxel v, Shape &shape,
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

template <class Shape, class Voxel>
void verifyShapeVoxelAppearsInNeighborhood(Voxel v, Shape &shape,
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
