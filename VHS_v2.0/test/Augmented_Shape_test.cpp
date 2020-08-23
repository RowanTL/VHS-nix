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
 * @file Augmented_Shape_test.cpp
 * Test file for the Augmented Shape classes.
 **/

#include <algorithm>
#include <gtest/gtest.h>
#include <random>

#include "shape/AddableShape.h"
#include "shape/RemovableShape.h"
#include "shape/EditableShape.h"
#include "shape/AugmentedShape.h"
#include "shape/Voxel.h"

#include "test_examples.h"

using shape_simplification::shape::AddableShape;
using shape_simplification::shape::RemovableShape;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::AugmentedShape;
using shape_simplification::shape::Voxel;

template <class Shape, class Voxel>
void verifyAugConstructor(std::string name);
template <class Shape, class Voxel>
void verifyAugAddVoxelMethod(std::string name);
template <class Shape, class Voxel>
void verifyAugRemoveVoxelMethod(std::string name);
template <class Shape, class Voxel>
void verifyInvAugConstructor(std::string name);

TEST(AugmentedShapeTest, Constructor) {
    verifyAugConstructor<AugmentedShape<AddableShape<Voxel>, Voxel>, Voxel>("AddableShape");
    verifyAugConstructor<AugmentedShape<RemovableShape<Voxel>, Voxel>, Voxel>("RemovableShape");
    verifyAugConstructor<AugmentedShape<EditableShape<Voxel>, Voxel>, Voxel>("EditableShape");
}

template <class Shape, class Voxel>
void verifyAugConstructor(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildExample1<Shape,Voxel>(voxels, minLevels);

    Voxel v1(0, 0, 2);  // S
    Voxel v6(1, 0, 3);  // N
    Voxel v11(0, 3, 1); // N
    Voxel v13(1, 4, 3); // C

    Shape shape(voxels, minLevels);

    ASSERT_EQ(1, shape.getEulerCharacteristic()) << name;
    ASSERT_EQ(2, shape.getNumberOfComponents()) << name;
    ASSERT_EQ(1, shape.getNumberOfLoops()) << name;
    ASSERT_EQ(0, shape.getNumberOfVoids()) << name;

    Voxel rep1 = shape.getComponentRepresentative(v1);
    ASSERT_TRUE(shape.getComponentRepresentative(v13) == v13);
    for (Voxel v : voxels) {
        if (v != v13 && shape.template contains<Shape::MAIN_SHAPE>(v))
            ASSERT_EQ(rep1, shape.getComponentRepresentative(v)) << name;
    }

    ASSERT_TRUE(shape.isInCollar(v11));
    ASSERT_TRUE(shape.isInCollar(v6));
    ASSERT_FALSE(shape.isInCollar(v13));
}

TEST(AugmentedShapeTest, AddVoxelMethod) {
    verifyAugAddVoxelMethod<AugmentedShape<AddableShape<Voxel>, Voxel>, Voxel>("AddableShape");
    verifyAugAddVoxelMethod<AugmentedShape<EditableShape<Voxel>, Voxel>, Voxel>("EditableShape");
}

template <class Shape, class Voxel>
void verifyAugAddVoxelMethod(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildExample1Small<Shape,Voxel>(voxels, minLevels);

    Voxel v1(0, 0, 2);  // S
    Voxel v6(1, 0, 3);  // N
    Voxel v11(0, 3, 1); // N
    Voxel v13(1, 4, 3); // C
    Voxel v14(0, 4, 2); // S
    Voxel v15(0, 4, 1); // S
    Voxel v16(0, 2, 0); // S
    Voxel v17(0, 3, 0); // C
    Voxel v18(0, 4, 0); // S
    Voxel v19(1, 4, 4); // N

    Shape shape(voxels, minLevels);

    ASSERT_EQ(2, shape.getEulerCharacteristic()) << name;

    shape.template addVoxel<Shape::MAIN_SHAPE>(v14);
    shape.template addVoxel<Shape::MAIN_SHAPE>(v15);
    shape.template addVoxel<Shape::MAIN_SHAPE>(v16);
    shape.template addVoxel<Shape::CORE>(v17);
    shape.template addVoxel<Shape::MAIN_SHAPE>(v18);
    shape.template addVoxel<Shape::NEIGHBORHOOD>(v19);

    ASSERT_EQ(1, shape.getEulerCharacteristic()) << name;
    ASSERT_EQ(2, shape.getNumberOfComponents()) << name;
    ASSERT_EQ(1, shape.getNumberOfLoops()) << name;
    ASSERT_EQ(0, shape.getNumberOfVoids()) << name;

    Voxel rep1 = shape.getComponentRepresentative(v1);
    ASSERT_TRUE(shape.getComponentRepresentative(v13) == v13);
    for (Voxel v : voxels) {
        if (v != v13 && shape.template contains<Shape::MAIN_SHAPE>(v))
            ASSERT_EQ(rep1, shape.getComponentRepresentative(v)) << name;
    }

    ASSERT_TRUE(shape.isInCollar(v11));
    ASSERT_TRUE(shape.isInCollar(v6));
    ASSERT_TRUE(shape.isInCollar(v19));
    ASSERT_FALSE(shape.isInCollar(v13));
    ASSERT_FALSE(shape.isInCollar(v17));
}

TEST(AugmentedShapeTest, RemoveVoxelMethod) {
    verifyAugRemoveVoxelMethod<AugmentedShape<RemovableShape<Voxel>, Voxel>, Voxel>("Removable");
    verifyAugRemoveVoxelMethod<AugmentedShape<EditableShape<Voxel>, Voxel>, Voxel>("EditableShape");
}

template <class Shape, class Voxel>
void verifyAugRemoveVoxelMethod(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildExample1<Shape,Voxel>(voxels, minLevels);

    Voxel v1(0, 0, 2);  // S
    Voxel v5(1, 0, 2);  // S
    Voxel v8(1, 1, 3);  // S
    Voxel v12(0, 3, 2); // S
    Voxel v13(1, 4, 3); // C
    Voxel v17(0, 3, 0); // S

    Shape shape(voxels, minLevels);

    ASSERT_EQ(1, shape.getEulerCharacteristic()) << name;
    ASSERT_EQ(2, shape.getNumberOfComponents()) << name;
    ASSERT_EQ(1, shape.getNumberOfLoops()) << name;
    ASSERT_EQ(0, shape.getNumberOfVoids()) << name;

    ASSERT_EQ(15, shape.getComponentSize(v1));
    ASSERT_EQ(1, shape.getComponentSize(v13));

    shape.template removeVoxel<Shape::MAIN_SHAPE>(v8);

    ASSERT_EQ(1, shape.getEulerCharacteristic()) << name;
    ASSERT_EQ(2, shape.getNumberOfComponents()) << name;
    ASSERT_EQ(1, shape.getNumberOfLoops()) << name;
    ASSERT_EQ(0, shape.getNumberOfVoids()) << name;

    ASSERT_EQ(14, shape.getComponentSize(v1));
    ASSERT_EQ(1, shape.getComponentSize(v13));

    shape.template removeVoxel<Shape::MAIN_SHAPE>(v13);

    ASSERT_EQ(0, shape.getEulerCharacteristic()) << name;
    ASSERT_EQ(1, shape.getNumberOfComponents()) << name;
    ASSERT_EQ(1, shape.getNumberOfLoops()) << name;
    ASSERT_EQ(0, shape.getNumberOfVoids()) << name;

    ASSERT_EQ(14, shape.getComponentSize(v1));

    shape.template removeVoxel<Shape::MAIN_SHAPE>(v17);

    ASSERT_EQ(1, shape.getEulerCharacteristic()) << name;
    ASSERT_EQ(1, shape.getNumberOfComponents()) << name;
    ASSERT_EQ(0, shape.getNumberOfLoops()) << name;
    ASSERT_EQ(0, shape.getNumberOfVoids()) << name;

    ASSERT_EQ(13, shape.getComponentSize(v1)) << name;

    ASSERT_TRUE(shape.isInCollar(v8)) << name;
    ASSERT_TRUE(shape.isInCollar(v13)) << name;
    ASSERT_TRUE(shape.isInCollar(v17)) << name;
    ASSERT_FALSE(shape.isInCollar(v12)) << name;
    ASSERT_FALSE(shape.isInCollar(v5)) << name;
}

