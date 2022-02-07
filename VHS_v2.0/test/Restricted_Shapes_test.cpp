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
 * @file Restricted_Shapes_test.cpp
 * Test file for the Restricted Shape classes.
 **/

#include <gtest/gtest.h>

#include "shape/RestrictedShapeCopy.h"
#include "shape/InvertedRestrictedShapeCopy.h"
#include "shape/ClosedRestrictedShapeCopy.h"
#include "shape/ClosedInvertedRestrictedShapeCopy.h"
#include "shape/EditableShape.h"
#include "shape/Voxel.h"
#include "shape/ShapeUtilities.h"

#include "test_examples.h"

using shape_simplification::shape::RestrictedShapeCopy;
using shape_simplification::shape::InvertedRestrictedShapeCopy;
using shape_simplification::shape::ClosedRestrictedShapeCopy;
using shape_simplification::shape::ClosedInvertedRestrictedShapeCopy;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::Voxel;
using shape_simplification::shape::Level;
using shape_simplification::shape::ExtendedNeighborsInIterator;

template <class Shape>
void verifyRestrVoxelNotContained(Voxel v, Shape &shape, std::string name)
{
    ASSERT_FALSE(shape.template contains<Level::CORE>(v[0], v[1], v[2]))
            << v << " Outside voxel contained in " << name << " core.";
    ASSERT_FALSE(shape.template contains<Level::MAIN_SHAPE>(v[0], v[1], v[2]))
            << v << " Outside voxel contained in " << name << " main shape.";
    ASSERT_FALSE(shape.template contains<Level::NEIGHBORHOOD>(v[0], v[1], v[2]))
            << v << " Outside voxel contained in " << name << " neighborhood.";
    ASSERT_TRUE(shape.template contains<Level::OUTSIDE>(v[0], v[1], v[2]))
            << v << " Outside voxel contained in " << name << ".";
    ASSERT_FALSE(shape.template contains<Level::EX_MAIN_SHAPE>(
                     v[0], v[1], v[2]))
            << v << " Outside voxel contained in "
            << name << " main shape (exclusively).";
    ASSERT_FALSE(shape.template contains<Level::EX_NEIGHBORHOOD>(
                     v[0], v[1], v[2]))
            << v << " Outside voxel contained in "
            << name << " neighborhood (exclusively).";
}

template <class Shape>
void verifyRestrVoxelAppearsInCore(Voxel v, Shape &shape, std::string name)
{
    ASSERT_TRUE(shape.template contains<Level::CORE>(v[0], v[1], v[2]))
            << v << " Core voxel not contained in " << name << " core.";
    ASSERT_TRUE(shape.template contains<Level::MAIN_SHAPE>(v[0], v[1], v[2]))
            << v << " Core voxel not contained in " << name << " main shape.";
    ASSERT_TRUE(shape.template contains<Level::NEIGHBORHOOD>(v[0], v[1], v[2]))
            << v << " Core voxel not contained in " << name << " neighborhood.";
    ASSERT_FALSE(shape.template contains<Level::OUTSIDE>(v[0], v[1], v[2]))
            << v << " Core voxel not contained in " << name << ".";
    ASSERT_FALSE(shape.template contains<Level::EX_MAIN_SHAPE>(
                     v[0], v[1], v[2]))
            << v << " Core voxel contained in "
            << name << " main shape (exclusively).";
    ASSERT_FALSE(shape.template contains<Level::EX_NEIGHBORHOOD>(
                     v[0], v[1], v[2]))
            << v << " Core voxel contained in "
            << name << " neighborhood (exclusively).";
}

template <class Shape>
void verifyRestrVoxelAppearsInMainShape(Voxel v, Shape &shape, std::string name)
{
    ASSERT_FALSE(shape.template contains<Level::CORE>(v[0], v[1], v[2]))
            << v << " Main shape voxel contained in " << name << " core.";
    ASSERT_TRUE(shape.template contains<Level::MAIN_SHAPE>(v[0], v[1], v[2]))
            << v << " Main shape voxel not contained in "
            << name << " main shape.";
    ASSERT_TRUE(shape.template contains<Level::NEIGHBORHOOD>(v[0], v[1], v[2]))
            << v << " Main shape voxel not contained in "
            << name << " neighborhood.";
    ASSERT_FALSE(shape.template contains<Level::OUTSIDE>(v[0], v[1], v[2]))
            << v << " Main shape voxel not contained in " << name << ".";
    ASSERT_TRUE(shape.template contains<Level::EX_MAIN_SHAPE>(
                    v[0], v[1], v[2]))
            << v << " Main shape voxel not contained in "
            << name << " main shape (exclusively).";
    ASSERT_FALSE(shape.template contains<Level::EX_NEIGHBORHOOD>(
                     v[0], v[1], v[2]))
            << v << " Main shape voxel contained in "
            << name << " neighborhood (exclusively).";
}

template <class Shape>
void verifyRestrVoxelAppearsInNeighborhood(
        Voxel v, Shape &shape, std::string name)
{
    ASSERT_FALSE(shape.template contains<Level::CORE>(v[0], v[1], v[2]))
            << v << " Neighborhood voxel contained in " << name << " core.";
    ASSERT_FALSE(shape.template contains<Level::MAIN_SHAPE>(v[0], v[1], v[2]))
            << v << " Neighborhood voxel contained in "
            << name << " main shape.";
    ASSERT_TRUE(shape.template contains<Level::NEIGHBORHOOD>(v[0], v[1], v[2]))
            << v << " Neighborhood voxel not contained in "
            << name << " neighborhood.";
    ASSERT_FALSE(shape.template contains<Level::OUTSIDE>(v[0], v[1], v[2]))
            << v << " Neighborhood voxel not contained in " << name << ".";
    ASSERT_FALSE(shape.template contains<Level::EX_MAIN_SHAPE>(
                     v[0], v[1], v[2]))
            << v << " Neighborhood voxel contained in "
            << name << " main shape (exclusively).";
    ASSERT_TRUE(shape.template contains<Level::EX_NEIGHBORHOOD>(
                    v[0], v[1], v[2]))
            << v << " Neighborhood voxel not contained in "
            << name << " neighborhood (exclusively).";
}

TEST(RestrictedShapesTest, Standart) {
    using ResShape = RestrictedShapeCopy<EditableShape<Voxel>, Voxel >;

    std::mt19937 randomGenerator(0); // Repeatable seed
    std::uniform_int_distribution<unsigned int> unisize(0, 1000);
    std::vector<Voxel> voxels;

    for (int repetition = 0; repetition < 300; repetition++) {
        voxels.clear();
        unsigned int nberOfVoxels = unisize(randomGenerator);

        buildRandomExample<Voxel>(nberOfVoxels, voxels);

        int maxDim[3] = {0, 0, 0};
        Point lowerBound = {35, 40, 45};
        Point upperBound = {55, 60, 65};
        EditableShape<Voxel> shape(voxels);
        ResShape resShape(shape, lowerBound, upperBound);

        ASSERT_EQ(35, resShape.getLowerBound()[0]);
        ASSERT_EQ(40, resShape.getLowerBound()[1]);
        ASSERT_EQ(45, resShape.getLowerBound()[2]);
        ASSERT_EQ(55, resShape.getUpperBound()[0]);
        ASSERT_EQ(60, resShape.getUpperBound()[1]);
        ASSERT_EQ(65, resShape.getUpperBound()[2]);

        for (Voxel v : voxels) {
            Level l = v.getLevel();

            if (l == Level::OUTSIDE ||
                    v[0] < lowerBound[0] ||
                    v[1] < lowerBound[1] ||
                    v[2] < lowerBound[2] ||
                    v[0] > upperBound[0] ||
                    v[1] > upperBound[1] ||
                    v[2] > upperBound[2])
            {
                verifyRestrVoxelNotContained<ResShape>(
                            v, resShape, "RestrictedShapeCopy");
            } else {
                if (v[0] > maxDim[0]) maxDim[0] = v[0];
                if (v[1] > maxDim[1]) maxDim[1] = v[1];
                if (v[2] > maxDim[2]) maxDim[2] = v[2];

                if (l == Level::CORE)
                    verifyRestrVoxelAppearsInCore<ResShape>(
                                v, resShape, "RestrictedShapeCopy");
                else if (l == Level::MAIN_SHAPE)
                    verifyRestrVoxelAppearsInMainShape<ResShape>(
                                v, resShape, "RestrictedShapeCopy");
                else
                    verifyRestrVoxelAppearsInNeighborhood<ResShape>(
                                v, resShape, "RestrictedShapeCopy");
            }
        }

        const std::array<unsigned int, 3>& dimensions =
                resShape.getShapeDimensions();

        ASSERT_EQ(dimensions[0], maxDim[0]);
        ASSERT_EQ(dimensions[1], maxDim[1]);
        ASSERT_EQ(dimensions[2], maxDim[2]);
    }
}

TEST(RestrictedShapesTest, Inverted) {
    using ResShape = InvertedRestrictedShapeCopy<EditableShape<Voxel>, Voxel >;

    std::mt19937 randomGenerator(0); // Repeatable seed
    std::uniform_int_distribution<unsigned int> unisize(0, 1000);
    std::vector<Voxel> voxels;

    for (int repetition = 0; repetition < 300; repetition++) {
        voxels.clear();
        unsigned int nberOfVoxels = unisize(randomGenerator);

        buildRandomExample<Voxel>(nberOfVoxels, voxels);

        Point lowerBound = {35, 40, 45};
        Point upperBound = {55, 60, 65};
        EditableShape<Voxel> shape(voxels);
        ResShape resShape(shape, lowerBound, upperBound);

        ASSERT_EQ(35, resShape.getLowerBound()[0]);
        ASSERT_EQ(40, resShape.getLowerBound()[1]);
        ASSERT_EQ(45, resShape.getLowerBound()[2]);
        ASSERT_EQ(55, resShape.getUpperBound()[0]);
        ASSERT_EQ(60, resShape.getUpperBound()[1]);
        ASSERT_EQ(65, resShape.getUpperBound()[2]);

        for (Voxel v : voxels) {
            Level l = v.getLevel();

            if (l == Level::CORE ||
                    v[0] < lowerBound[0] ||
                    v[1] < lowerBound[1] ||
                    v[2] < lowerBound[2] ||
                    v[0] > upperBound[0] ||
                    v[1] > upperBound[1] ||
                    v[2] > upperBound[2])
            {
                verifyRestrVoxelNotContained<ResShape>(
                            v, resShape, "InvertedRestrictedShapeCopy");
            } else {
                if (l == Level::OUTSIDE){
                    ExtendedNeighborsInIterator<
                            EditableShape<Voxel>,
                            Voxel,
                            NEIGHBORHOOD> it(v, shape);
                    if (!it.isEnd())
                        verifyRestrVoxelAppearsInCore<ResShape>(
                                    v, resShape, "InvertedRestrictedShapeCopy");
                    else
                        verifyRestrVoxelNotContained<ResShape>(
                                    v, resShape, "InvertedRestrictedShapeCopy");
                }
                else if (l == Level::NEIGHBORHOOD)
                    verifyRestrVoxelAppearsInMainShape<ResShape>(
                                v, resShape, "InvertedRestrictedShapeCopy");
                else
                    verifyRestrVoxelAppearsInNeighborhood<ResShape>(
                                v, resShape, "InvertedRestrictedShapeCopy");
            }
        }
    }
}

TEST(RestrictedShapesTest, Closed) {
    using ResShape = ClosedRestrictedShapeCopy<EditableShape<Voxel>, Voxel >;

    std::mt19937 randomGenerator(0); // Repeatable seed
    std::uniform_int_distribution<unsigned int> unisize(0, 1000);
    std::vector<Voxel> voxels;

    for (int repetition = 0; repetition < 300; repetition++) {
        voxels.clear();
        unsigned int nberOfVoxels = unisize(randomGenerator);

        buildRandomExample<Voxel>(nberOfVoxels, voxels);

        Point lowerBound = {35, 40, 45};
        Point upperBound = {55, 60, 65};
        EditableShape<Voxel> shape(voxels);
        ResShape resShape(shape, lowerBound, upperBound);

        ASSERT_EQ(35, resShape.getLowerBound()[0]);
        ASSERT_EQ(40, resShape.getLowerBound()[1]);
        ASSERT_EQ(45, resShape.getLowerBound()[2]);
        ASSERT_EQ(55, resShape.getUpperBound()[0]);
        ASSERT_EQ(60, resShape.getUpperBound()[1]);
        ASSERT_EQ(65, resShape.getUpperBound()[2]);

        for (Voxel v : voxels) {
            Level l = v.getLevel();

            if (v[0] < lowerBound[0] - 1 ||
                    v[1] < lowerBound[1] - 1 ||
                    v[2] < lowerBound[2] - 1 ||
                    v[0] > upperBound[0] + 1 ||
                    v[1] > upperBound[1] + 1 ||
                    v[2] > upperBound[2] + 1)
            {
                verifyRestrVoxelNotContained<ResShape>(
                            v, resShape, "RestrictedShapeCopy");
            } else {
                if (l == Level::CORE)
                    verifyRestrVoxelAppearsInCore<ResShape>(
                                v, resShape, "RestrictedShapeCopy");
                else if (l == Level::MAIN_SHAPE)
                    verifyRestrVoxelAppearsInMainShape<ResShape>(
                                v, resShape, "RestrictedShapeCopy");
                else if (l == Level::NEIGHBORHOOD)
                    verifyRestrVoxelAppearsInNeighborhood<ResShape>(
                                v, resShape, "RestrictedShapeCopy");
                else if (l == Level::OUTSIDE &&
                         (v[0] == lowerBound[0] - 1 ||
                          v[1] == lowerBound[1] - 1 ||
                          v[2] == lowerBound[2] - 1 ||
                          v[0] == upperBound[0] + 1 ||
                          v[1] == upperBound[1] + 1 ||
                          v[2] == upperBound[2] + 1))
                    verifyRestrVoxelAppearsInCore<ResShape>(
                                v, resShape, "RestrictedShapeCopy");
                else
                    verifyRestrVoxelNotContained<ResShape>(
                                v, resShape, "RestrictedShapeCopy");
            }
        }

        const std::array<unsigned int, 3>& dimensions =
                resShape.getShapeDimensions();

        ASSERT_EQ(dimensions[0], 56);
        ASSERT_EQ(dimensions[1], 61);
        ASSERT_EQ(dimensions[2], 66);
    }
}

TEST(RestrictedShapesTest, ClosedInverted) {
    using ResShape = ClosedInvertedRestrictedShapeCopy<EditableShape<Voxel>,Voxel>;

    std::mt19937 randomGenerator(0); // Repeatable seed
    std::uniform_int_distribution<unsigned int> unisize(0, 1000);
    std::vector<Voxel> voxels;

    for (int repetition = 0; repetition < 300; repetition++) {
        voxels.clear();
        unsigned int nberOfVoxels = unisize(randomGenerator);

        buildRandomExample<Voxel>(nberOfVoxels, voxels);

        Point lowerBound = {35, 40, 45};
        Point upperBound = {55, 60, 65};
        EditableShape<Voxel> shape(voxels);
        ResShape resShape(shape, lowerBound, upperBound);

        ASSERT_EQ(35, resShape.getLowerBound()[0]);
        ASSERT_EQ(40, resShape.getLowerBound()[1]);
        ASSERT_EQ(45, resShape.getLowerBound()[2]);
        ASSERT_EQ(55, resShape.getUpperBound()[0]);
        ASSERT_EQ(60, resShape.getUpperBound()[1]);
        ASSERT_EQ(65, resShape.getUpperBound()[2]);

        for (Voxel v : voxels) {
            Level l = v.getLevel();

            if (v[0] < lowerBound[0] - 1 ||
                    v[1] < lowerBound[1] - 1 ||
                    v[2] < lowerBound[2] - 1 ||
                    v[0] > upperBound[0] + 1 ||
                    v[1] > upperBound[1] + 1 ||
                    v[2] > upperBound[2] + 1)
            {
                verifyRestrVoxelNotContained<ResShape>(
                            v, resShape, "InvertedRestrictedShapeCopy");
            } else {
                if (v[0] == lowerBound[0] - 1 ||
                        v[1] == lowerBound[1] - 1 ||
                        v[2] == lowerBound[2] - 1 ||
                        v[0] == upperBound[0] + 1 ||
                        v[1] == upperBound[1] + 1 ||
                        v[2] == upperBound[2] + 1)
                    verifyRestrVoxelAppearsInCore<ResShape>(
                                v, resShape, "InvertedRestrictedShapeCopy");
                else if (l == Level::OUTSIDE){
                    ExtendedNeighborsInIterator<
                            EditableShape<Voxel>,
                            Voxel,
                            NEIGHBORHOOD> it(v, shape);
                    if (!it.isEnd())
                        verifyRestrVoxelAppearsInCore<ResShape>(
                                    v, resShape, "InvertedRestrictedShapeCopy");
                    else
                        verifyRestrVoxelNotContained<ResShape>(
                                    v, resShape, "InvertedRestrictedShapeCopy");
                }
                else if (l == Level::NEIGHBORHOOD)
                    verifyRestrVoxelAppearsInMainShape<ResShape>(
                                v, resShape, "InvertedRestrictedShapeCopy");
                else if (l == Level::MAIN_SHAPE)
                    verifyRestrVoxelAppearsInNeighborhood<ResShape>(
                                v, resShape, "InvertedRestrictedShapeCopy");
                else
                    verifyRestrVoxelNotContained<ResShape>(
                                v, resShape, "InvertedRestrictedShapeCopy");
            }
        }

        const std::array<unsigned int, 3>& dimensions =
                resShape.getShapeDimensions();

        ASSERT_EQ(dimensions[0], 56);
        ASSERT_EQ(dimensions[1], 61);
        ASSERT_EQ(dimensions[2], 66);
    }
}

