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
 * @file Shape_test.cpp
 * Test file for the Shape classes.
 **/

#include <gtest/gtest.h>
#include <vector>
#include <unordered_set>

#include "shape/BaseShape.h"
#include "shape/AddableShape.h"
#include "shape/EditableShape.h"
#include "shape/RemovableShape.h"
#include "shape/InflationShape.h"
#include "shape/IntensityShape.h"
#include "shape/Voxel.h"
#include "shape/ShapeUtilities.h"

#include "test_examples.h"

using shape_simplification::shape::BaseShape;
using shape_simplification::shape::AddableShape;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::RemovableShape;
using shape_simplification::shape::InflationShape;
using shape_simplification::shape::IntensityShape;
using shape_simplification::shape::Voxel;
using shape_simplification::shape::Level;

template <class Shape, class L>
void verifyShapeConstructor(
        Shape shape, std::unordered_set<Voxel> &voxels, std::string name)
{
    for (Voxel v : voxels) {
        if (v.getLevel() == Level::CORE || v.getLevel() == Level::MAIN_SHAPE)
            ASSERT_TRUE(shape.template contains<L::MAIN_SHAPE>(
                            v[0], v[1], v[2])) << name;
        else
            ASSERT_TRUE(shape.template contains<L::NEIGHBORHOOD>(
                            v[0], v[1], v[2])) << name;
    }

    int count = 0;
    for (auto it = shape.template begin<L::MAIN_SHAPE>();
         it != shape.template end<L::MAIN_SHAPE>();
         it++)
    {
        Voxel v = *it;
        ASSERT_TRUE(voxels.find(v) != voxels.end()) << name;
        ASSERT_TRUE(v.getLevel() == Level::MAIN_SHAPE ||
                    v.getLevel() == Level::CORE ||
                    v.getLevel() == Level::NOT_SPECIFIED) << name;
        count++;
    }
    ASSERT_EQ(11, count) << name;

    count = 0;
    for (auto it = shape.template begin<L::NEIGHBORHOOD>();
         it != shape.template end<L::NEIGHBORHOOD>();
         it++)
    {
        Voxel v = *it;
        ASSERT_TRUE(voxels.find(v) != voxels.end()) << name;
        ASSERT_TRUE(v.getLevel() == Level::NEIGHBORHOOD ||
                    v.getLevel() == Level::MAIN_SHAPE ||
                    v.getLevel() == Level::CORE ||
                    v.getLevel() == Level::NOT_SPECIFIED) << name;
        count++;
    }
    ASSERT_EQ(14, count) << name;

    ASSERT_EQ(1, shape.getShapeDimensions()[0]) << name;
    ASSERT_EQ(4, shape.getShapeDimensions()[1]) << name;
    ASSERT_EQ(3, shape.getShapeDimensions()[2]) << name;

    ASSERT_EQ(11, shape.getShapeSize()) << name;

    shape.clear();
    ASSERT_EQ(0, shape.getShapeSize()) << name;
}

TEST(ShapeTest, Constructor) {
    std::vector<Voxel> voxels;
    std::vector<png::image<png::gray_pixel> > stack;
    std::unordered_set<Voxel> voxelSet;
    ProcessedImageStack proc;

    buildExample1Small<Voxel>(voxels);
    buildExample1SmallAsStack<Voxel>(stack);
    proc.load(stack);

    for (Voxel v : voxels) voxelSet.insert(v);

    verifyShapeConstructor<BaseShape<Voxel>,Level>(
                BaseShape<Voxel>(voxels), voxelSet, "BaseShape");
    verifyShapeConstructor<AddableShape<Voxel>,Level>(
                AddableShape<Voxel>(voxels), voxelSet, "AddableShape");
    verifyShapeConstructor<RemovableShape<Voxel>,Level>(
                RemovableShape<Voxel>(voxels), voxelSet, "RemovableShape");
    verifyShapeConstructor<EditableShape<Voxel>,Level>(
                EditableShape<Voxel>(voxels), voxelSet, "EditableShape");
    verifyShapeConstructor<InflationShape,Level>(
                InflationShape(proc, 0, true), voxelSet, "InflationShape");
    verifyShapeConstructor<IntensityShape,IntensityShape::Level>(
                IntensityShape(proc, 100), voxelSet, "IntensityShape");
}

template <class Shape>
void verifyShapeAddability(Shape shape, std::string name)
{
    ASSERT_TRUE(shape.isAddable()) << name;

    ASSERT_FALSE(shape.template contains<Level::NEIGHBORHOOD>(0, 4, 0)) << name;

    shape.template addVoxel<Level::CORE>(
                Voxel(0, 4, 0, Level::CORE));

    ASSERT_TRUE(shape.template contains<Level::NEIGHBORHOOD>(0, 4, 0)) << name;
    ASSERT_TRUE(shape.template contains<Level::MAIN_SHAPE>(0, 4, 0)) << name;
    ASSERT_TRUE(shape.template contains<Level::CORE>(0, 4, 0)) << name;

    ASSERT_FALSE(shape.template contains<Level::NEIGHBORHOOD>(0, 3, 0)) << name;

    shape.template addVoxel<Level::MAIN_SHAPE>(
                Voxel(0, 3, 0, Level::MAIN_SHAPE));

    ASSERT_TRUE(shape.template contains<Level::NEIGHBORHOOD>(0, 3, 0)) << name;
    ASSERT_TRUE(shape.template contains<Level::MAIN_SHAPE>(0, 3, 0)) << name;
    ASSERT_FALSE(shape.template contains<Level::CORE>(0, 3, 0)) << name;

    ASSERT_TRUE(shape.template contains<Level::NEIGHBORHOOD>(0, 2, 1)) << name;
    ASSERT_FALSE(shape.template contains<Level::CORE>(0, 2, 1)) << name;

    shape.template addVoxel<Level::CORE>(
                Voxel(0, 2, 1, Level::CORE));

    ASSERT_TRUE(shape.template contains<Level::NEIGHBORHOOD>(0, 2, 1)) << name;
    ASSERT_TRUE(shape.template contains<Level::MAIN_SHAPE>(0, 2, 1)) << name;
    ASSERT_TRUE(shape.template contains<Level::CORE>(0, 2, 1)) << name;
}

template <class Shape>
void verifyShapeNonAddability(Shape shape, std::string name)
{
    ASSERT_FALSE(shape.isAddable()) << name;
}

TEST(ShapeTest, Addition) {
    std::vector<Voxel> voxels;
    std::vector<png::image<png::gray_pixel> > stack;
    ProcessedImageStack proc;

    buildExample1Small<Voxel>(voxels);
    buildExample1SmallAsStack<Voxel>(stack);
    proc.load(stack);

    verifyShapeNonAddability<BaseShape<Voxel> >(
                BaseShape<Voxel>(voxels), "BaseShape");
    verifyShapeAddability<AddableShape<Voxel> >(
                AddableShape<Voxel>(voxels), "AddableShape");
    verifyShapeNonAddability<RemovableShape<Voxel> >(
                RemovableShape<Voxel>(voxels), "RemovableShape");
    verifyShapeAddability<EditableShape<Voxel> >(
                EditableShape<Voxel>(voxels), "EditableShape");
    verifyShapeNonAddability<InflationShape>(
                InflationShape(proc, 0, true), "InflationShape");
    verifyShapeNonAddability<IntensityShape>(
                IntensityShape(proc, 100), "IntensityShape");
}

template <class Shape>
void verifyShapeRemovability(Shape shape, std::string name)
{
    ASSERT_TRUE(shape.isRemovable()) << name;

    ASSERT_TRUE(shape.template contains<Level::MAIN_SHAPE>(0, 0, 2)) << name;

    shape.template removeVoxel<Level::MAIN_SHAPE>(
                Voxel(0, 0, 2, Level::MAIN_SHAPE));

    ASSERT_TRUE(shape.template contains<Level::NEIGHBORHOOD>(0, 0, 2)) << name;
    ASSERT_FALSE(shape.template contains<Level::MAIN_SHAPE>(0, 0, 2)) << name;
    ASSERT_FALSE(shape.template contains<Level::CORE>(0, 0, 2)) << name;

    ASSERT_TRUE(shape.template contains<Level::CORE>(0, 1, 2)) << name;

    shape.template removeVoxel<Level::MAIN_SHAPE>(
                Voxel(0, 1, 2, Level::CORE));

    ASSERT_TRUE(shape.template contains<Level::NEIGHBORHOOD>(0, 1, 2)) << name;
    ASSERT_FALSE(shape.template contains<Level::MAIN_SHAPE>(0, 1, 2)) << name;
    ASSERT_FALSE(shape.template contains<Level::CORE>(0, 1, 2)) << name;

    ASSERT_TRUE(shape.template contains<Level::NEIGHBORHOOD>(1, 1, 3)) << name;

    shape.template removeVoxel<Level::CORE>(
                Voxel(1, 1, 3, Level::CORE));

    ASSERT_TRUE(shape.template contains<Level::NEIGHBORHOOD>(1, 1, 3)) << name;
    ASSERT_TRUE(shape.template contains<Level::MAIN_SHAPE>(1, 1, 3)) << name;
    ASSERT_FALSE(shape.template contains<Level::CORE>(1, 1, 3)) << name;
}

template <class Shape>
void verifyShapeNonRemovability(Shape shape, std::string name)
{
    ASSERT_FALSE(shape.isRemovable()) << name;
}

TEST(ShapeTest, Removal) {
    std::vector<Voxel> voxels;
    std::vector<png::image<png::gray_pixel> > stack;
    ProcessedImageStack proc;

    buildExample1Small<Voxel>(voxels);
    buildExample1SmallAsStack<Voxel>(stack);
    proc.load(stack);

    verifyShapeNonRemovability<BaseShape<Voxel> >(
                BaseShape<Voxel>(voxels), "BaseShape");
    verifyShapeNonRemovability<AddableShape<Voxel> >(
                AddableShape<Voxel>(voxels), "AddableShape");
    verifyShapeRemovability<RemovableShape<Voxel> >(
                RemovableShape<Voxel>(voxels), "RemovableShape");
    verifyShapeRemovability<EditableShape<Voxel> >(
                EditableShape<Voxel>(voxels), "EditableShape");
    verifyShapeNonRemovability<InflationShape>(
                InflationShape(proc, 0, true), "InflationShape");
    verifyShapeNonRemovability<IntensityShape>(
                IntensityShape(proc, 100), "IntensityShape");
}

TEST(ShapeTest, Inflation) {
    std::vector<Voxel> voxels;
    std::vector<png::image<png::gray_pixel> > stack;
    std::unordered_set<Voxel> voxelSet;
    ProcessedImageStack proc;

    buildExample1Small<Voxel>(voxels);
    buildExample1SmallAsStack<Voxel>(stack);
    proc.load(stack);
    InflationShape shape(proc, 0, true);

    for (Voxel v : voxels) voxelSet.insert(v);

    int count = 0;
    for (auto it = shape.rbegin<Level::MAIN_SHAPE>();
         it != shape.template rend<Level::MAIN_SHAPE>();
         it++)
    {
        Voxel v = *it;
        ASSERT_TRUE(voxelSet.find(v) != voxelSet.end()) << "Reverse Iterator";
        ASSERT_TRUE(v.getLevel() == Level::MAIN_SHAPE ||
                    v.getLevel() == Level::CORE) << "Reverse Iterator";
        ASSERT_EQ(0, shape.getMinLevel(v)) << "Reverse Iterator";
        count++;
    }
    ASSERT_EQ(11, count) << "Reverse Iterator";

    count = 0;
    for (auto it = shape.template rbegin<Level::NEIGHBORHOOD>();
         it != shape.template rend<Level::NEIGHBORHOOD>();
         it++)
    {
        Voxel v = *it;
        ASSERT_TRUE(voxelSet.find(v) != voxelSet.end()) << "Reverse Iterator";
        ASSERT_TRUE(v.getLevel() == Level::NEIGHBORHOOD ||
                    v.getLevel() == Level::MAIN_SHAPE ||
                    v.getLevel() == Level::CORE) << "Reverse Iterator";
        if (v.getLevel() == Level::NEIGHBORHOOD)
            ASSERT_TRUE(0 < shape.getMinLevel(v)) << "Reverse Iterator";
        count++;
    }
    ASSERT_EQ(14, count) << "Reverse Iterator";

    ASSERT_EQ(150, shape.getShapeThreshold()) << "Threshold";
    ASSERT_EQ(100, shape.getNeighborhoodThreshold()) << "Threshold";

    ASSERT_EQ(14, shape.getNeighborhoodSize()) << "Neighborhood size";

    ASSERT_EQ(shape.getInflationValue() + 1, shape.getNumberOfLevels())
            << "Number of levels and inflation value";

    const InflationShape::voxel_container &container =
            shape.getVoxelsAtDistance(1);

    ASSERT_EQ(3, container.size()) << "Voxels at distance 1";

    const ConnectedComponents<Voxel> &maxima = shape.getLocalMaxima();

    ASSERT_EQ(1, maxima.size()) << "Voxels at distance 1";
}

TEST(ShapeTest, Intensity) {
    std::vector<Voxel> voxels;
    std::vector<png::image<png::gray_pixel> > stack;
    std::unordered_set<Voxel> voxelSet;
    ProcessedImageStack proc;

    buildExample1Small<Voxel>(voxels);
    buildExample1SmallAsStack<Voxel>(stack);
    proc.load(stack);
    IntensityShape shape(proc, 100);

    for (Voxel v : voxels) voxelSet.insert(v);

    int count = 0;
    for (auto it = shape.rbegin<IntensityShape::MAIN_SHAPE>();
         it != shape.template rend<IntensityShape::MAIN_SHAPE>();
         it++)
    {
        Voxel v = *it;
        ASSERT_TRUE(voxelSet.find(v) != voxelSet.end()) << "Reverse Iterator";
        ASSERT_TRUE(105 >= shape.getMinLevel(v)) << "Reverse Iterator";
        count++;
    }
    ASSERT_EQ(11, count) << "Reverse Iterator";

    count = 0;
    for (auto it = shape.template rbegin<IntensityShape::NEIGHBORHOOD>();
         it != shape.template rend<IntensityShape::NEIGHBORHOOD>();
         it++)
    {
        Voxel v = *it;
        ASSERT_TRUE(voxelSet.find(v) != voxelSet.end()) << "Reverse Iterator";
        ASSERT_TRUE(155 >= shape.getMinLevel(v)) << "Reverse Iterator";
        count++;
    }
    ASSERT_EQ(14, count) << "Reverse Iterator";

    ASSERT_EQ(14, shape.getNeighborhoodSize()) << "Neighborhood size";
    ASSERT_EQ(4, shape.getCoreSize()) << "Core size";

    const IntensityShape::voxel_container &container =
            shape.getVoxelsWithGrayscale(150);

    ASSERT_EQ(7, container.size()) << "Voxels with grayscale value 150";

    ASSERT_EQ(55, shape.getCoreThreshold()) << "Threshold";
    ASSERT_EQ(105, shape.getShapeThreshold()) << "Threshold";
    ASSERT_EQ(155, shape.getNeighborhoodThreshold()) << "Threshold";
    ASSERT_EQ(155, shape.getExtendedNeighborhoodThreshold()) << "Threshold";

    shape.setCoreThreshold(60);
    shape.setShapeThreshold(95);
    shape.setNeighborhoodThreshold(125);
    shape.setExtendedNeighborhoodThreshold(150);

    ASSERT_EQ(60, shape.getCoreThreshold()) << "Threshold";
    ASSERT_EQ(95, shape.getShapeThreshold()) << "Threshold";
    ASSERT_EQ(125, shape.getNeighborhoodThreshold()) << "Threshold";
    ASSERT_EQ(150, shape.getExtendedNeighborhoodThreshold()) << "Threshold";
}

TEST(ShapeTest, Neighborhoods) {
    std::vector<Voxel> voxels;
    buildExample1Small<Voxel>(voxels);
    EditableShape<Voxel> shape(voxels);

    std::unordered_set<Voxel> voxelSet;
    int count = 0;
    int x, y, z;

    for (NeighborsIterator<Voxel> it(Voxel(0, 2, 2)); !it.isEnd(); it.next())
    {
        count++;
        it.getCurrentValue(x, y, z);
        voxelSet.insert(Voxel(x, y, z));
    }

    ASSERT_EQ(count, 5);
    ASSERT_EQ(count, voxelSet.size());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 2, 3)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 2, 1)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 3, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 1, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 2, 2)) != voxelSet.end());

    voxelSet.clear();
    count = 0;

    for (ExtendedNeighborsIterator<Voxel> it(Voxel(0, 2, 2));
         !it.isEnd();
         it.next())
    {
        count++;
        it.getCurrentValue(x, y, z);
        voxelSet.insert(Voxel(x, y, z));
    }

    ASSERT_EQ(count, 17);
    ASSERT_EQ(count, voxelSet.size());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 1, 1)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 1, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 1, 3)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 2, 1)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 2, 3)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 3, 1)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 3, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 3, 3)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 1, 1)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 1, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 1, 3)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 2, 1)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 2, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 2, 3)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 3, 1)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 3, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 3, 3)) != voxelSet.end());

    voxelSet.clear();
    count = 0;

    for (NeighborsInIterator<EditableShape<Voxel>, Voxel, NEIGHBORHOOD> it(
             Voxel(0, 2, 2), shape);
         !it.isEnd(); it.next())
    {
        count++;
        it.getCurrentValue(x, y, z);
        voxelSet.insert(Voxel(x, y, z));
        ASSERT_TRUE(shape.contains<NEIGHBORHOOD>(x, y, z));
    }

    ASSERT_EQ(count, 3);
    ASSERT_EQ(count, voxelSet.size());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 1, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 3, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 2, 1)) != voxelSet.end());

    voxelSet.clear();
    count = 0;

    for (NeighborsInIterator<EditableShape<Voxel>, Voxel, MAIN_SHAPE> it(
             Voxel(0, 2, 2), shape);
         !it.isEnd(); it.next())
    {
        count++;
        it.getCurrentValue(x, y, z);
        voxelSet.insert(Voxel(x, y, z));
        ASSERT_TRUE(shape.contains<MAIN_SHAPE>(x, y, z));
    }

    ASSERT_EQ(count, 3);
    ASSERT_EQ(count, voxelSet.size());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 1, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 3, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 2, 1)) != voxelSet.end());

    voxelSet.clear();
    count = 0;

    for (NeighborsInIterator<EditableShape<Voxel>, Voxel, CORE> it(
             Voxel(0, 2, 2), shape);
         !it.isEnd(); it.next())
    {
        count++;
        it.getCurrentValue(x, y, z);
        voxelSet.insert(Voxel(x, y, z));
        ASSERT_TRUE(shape.contains<CORE>(x, y, z));
    }

    ASSERT_EQ(count, 1);
    ASSERT_EQ(count, voxelSet.size());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 1, 2)) != voxelSet.end());

    voxelSet.clear();
    count = 0;

    for (ExtendedNeighborsInIterator<EditableShape<Voxel>, Voxel, NEIGHBORHOOD> it(
             Voxel(0, 2, 2), shape);
         !it.isEnd(); it.next())
    {
        count++;
        it.getCurrentValue(x, y, z);
        voxelSet.insert(Voxel(x, y, z));
        ASSERT_TRUE(shape.contains<NEIGHBORHOOD>(x, y, z));
    }

    ASSERT_EQ(count, 7);
    ASSERT_EQ(count, voxelSet.size());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 1, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 3, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 2, 1)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 1, 3)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 3, 1)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 1, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 1, 3)) != voxelSet.end());

    voxelSet.clear();
    count = 0;

    for (ExtendedNeighborsInIterator<EditableShape<Voxel>, Voxel, MAIN_SHAPE> it(
             Voxel(0, 2, 2), shape);
         !it.isEnd(); it.next())
    {
        count++;
        it.getCurrentValue(x, y, z);
        voxelSet.insert(Voxel(x, y, z));
        ASSERT_TRUE(shape.contains<MAIN_SHAPE>(x, y, z));
    }

    ASSERT_EQ(count, 6);
    ASSERT_EQ(count, voxelSet.size());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 1, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 3, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 1, 3)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 2, 1)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 1, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 1, 3)) != voxelSet.end());

    voxelSet.clear();
    count = 0;

    for (ExtendedNeighborsInIterator<EditableShape<Voxel>, Voxel, CORE> it(
             Voxel(0, 2, 2), shape);
         !it.isEnd(); it.next())
    {
        count++;
        it.getCurrentValue(x, y, z);
        voxelSet.insert(Voxel(x, y, z));
        ASSERT_TRUE(shape.contains<CORE>(x, y, z));
    }

    ASSERT_EQ(count, 2);
    ASSERT_EQ(count, voxelSet.size());
    ASSERT_TRUE(voxelSet.find(Voxel(0, 1, 2)) != voxelSet.end());
    ASSERT_TRUE(voxelSet.find(Voxel(1, 1, 3)) != voxelSet.end());
}

