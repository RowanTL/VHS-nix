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
 * @file Augmented_Shape_test.cpp
 * Test file for the Augmented Shape classes.
 **/

#include <gtest/gtest.h>
#include <vector>

#include "shape/AugmentedShape.h"
#include "shape/InvertedAugmentedShape.h"
#include "shape/Voxel.h"
#include "shape/BaseShape.h"
#include "shape/AddableShape.h"
#include "shape/RemovableShape.h"
#include "shape/EditableShape.h"

#include "test_examples.h"

using shape_simplification::shape::AugmentedShape;
using shape_simplification::shape::InvertedAugmentedShape;
using shape_simplification::shape::Voxel;
using shape_simplification::shape::BaseShape;
using shape_simplification::shape::AddableShape;
using shape_simplification::shape::RemovableShape;
using shape_simplification::shape::EditableShape;

template <class Shape>
void verifyAugAddability(std::string name)
{
    std::vector<Voxel> voxels;
    buildExample1Small<Voxel>(voxels);

    Voxel v1(0, 0, 2, Level::MAIN_SHAPE);
    Voxel v6(1, 0, 3, Level::NEIGHBORHOOD);
    Voxel v11(0, 3, 1, Level::NEIGHBORHOOD);
    Voxel v13(1, 4, 3, Level::CORE);
    Voxel v14(0, 4, 2, Level::MAIN_SHAPE);
    Voxel v15(0, 4, 1, Level::MAIN_SHAPE);
    Voxel v16(0, 2, 0, Level::MAIN_SHAPE);
    Voxel v17(0, 3, 0, Level::CORE);
    Voxel v18(0, 4, 0, Level::MAIN_SHAPE);
    Voxel v19(1, 4, 4, Level::NEIGHBORHOOD);

    Shape shape(voxels);

    ASSERT_TRUE(shape.isAddable()) << "Non addable; " << name;

    ASSERT_EQ(2, shape.getEulerCharacteristic())
            << "Euler Characteristic 0; " << name;
    ASSERT_EQ(2, shape.getNumberOfComponents())
            << "Connected Components 0; " << name;
    ASSERT_EQ(0, shape.getNumberOfLoops()) << "Loops 0; " << name;
    ASSERT_EQ(0, shape.getNumberOfVoids()) << "Void 0; " << name;

    shape.template addVoxel<Level::MAIN_SHAPE>(v14);
    shape.template addVoxel<Level::MAIN_SHAPE>(v15);
    shape.template addVoxel<Level::MAIN_SHAPE>(v16);
    shape.template addVoxel<Level::CORE>(v17);
    shape.template addVoxel<Level::MAIN_SHAPE>(v18);
    shape.template addVoxel<Level::NEIGHBORHOOD>(v19);

    ASSERT_EQ(1, shape.getEulerCharacteristic())
            << "Euler Characteristic 1; " << name;
    ASSERT_EQ(2, shape.getNumberOfComponents())
            << "Connected Components 1; " << name;
    ASSERT_EQ(1, shape.getNumberOfLoops()) << "Loops 1; " << name;
    ASSERT_EQ(0, shape.getNumberOfVoids()) << "Void 1; " << name;

    Voxel rep1 = shape.getComponentRepresentative(v1);
    ASSERT_TRUE(shape.getComponentRepresentative(v13) == v13)
            << "v13 Rep; " << name;
    for (Voxel v : voxels) {
        if (v != v13 &&
                shape.template contains<Level::MAIN_SHAPE>(v[0], v[1], v[2]))
            ASSERT_EQ(rep1, shape.getComponentRepresentative(v))
                    << v << " Rep; " << name;
    }

    ASSERT_TRUE(shape.isInCollar(v11)) << "v11 Collar 0; " << name;
    ASSERT_TRUE(shape.isInCollar(v6)) << "v6 Collar 0; " << name;
    ASSERT_TRUE(shape.isInCollar(v19)) << "v19 Collar 0; " << name;
    ASSERT_FALSE(shape.isInCollar(v13)) << "v13 Collar 0; " << name;
    ASSERT_FALSE(shape.isInCollar(v17)) << "v17 Collar 0; " << name;
}

template <class Shape>
void verifyAugNonAddability(std::string name)
{
    std::vector<Voxel> voxels;
    buildExample1Small<Voxel>(voxels);
    Shape shape(voxels);

    ASSERT_FALSE(shape.isAddable()) << "Addable; " << name;
}

template <class Shape>
void verifyAugRemovability(std::string name)
{
    std::vector<Voxel> voxels;
    buildExample1<Voxel>(voxels);

    Voxel v1(0, 0, 2, Level::MAIN_SHAPE);
    Voxel v5(1, 0, 2, Level::MAIN_SHAPE);
    Voxel v8(1, 1, 3, Level::MAIN_SHAPE);
    Voxel v12(0, 3, 2, Level::MAIN_SHAPE);
    Voxel v13(1, 4, 3, Level::CORE);
    Voxel v17(0, 3, 0, Level::CORE);

    Shape shape(voxels);

    ASSERT_TRUE(shape.isRemovable()) << "Non removable; " << name;

    ASSERT_EQ(15, shape.getComponentSize(v1)) << "v1 CC Size 0; " << name;
    ASSERT_EQ(1, shape.getComponentSize(v13)) << "v13 CC Size 0; " << name;

    shape.template removeVoxel<Level::MAIN_SHAPE>(v8);

    ASSERT_EQ(1, shape.getEulerCharacteristic())
            << "Euler Characteristic 2; " << name;
    ASSERT_EQ(2, shape.getNumberOfComponents())
            << "Connected Components 2; " << name;
    ASSERT_EQ(1, shape.getNumberOfLoops()) << "Loops 2; " << name;
    ASSERT_EQ(0, shape.getNumberOfVoids()) << "Void 2; " << name;

    ASSERT_EQ(14, shape.getComponentSize(v1)) << "v1 CC Size 1; " << name;
    ASSERT_EQ(1, shape.getComponentSize(v13)) << "v13 CC Size 1; " << name;

    shape.template removeVoxel<Level::MAIN_SHAPE>(v13);

    ASSERT_EQ(0, shape.getEulerCharacteristic())
            << "Euler Characteristic 3; " << name;
    ASSERT_EQ(1, shape.getNumberOfComponents())
            << "Connected Components 3; " << name;
    ASSERT_EQ(1, shape.getNumberOfLoops()) << "Loops 3; " << name;
    ASSERT_EQ(0, shape.getNumberOfVoids()) << "Void 3; " << name;

    ASSERT_EQ(14, shape.getComponentSize(v1)) << "v1 CC Size 2; " << name;
    ASSERT_FALSE(shape.template contains<Level::MAIN_SHAPE>(v13[0], v13[1], v13[2]))
            << "v13 contained; " << name;

    shape.template removeVoxel<Level::MAIN_SHAPE>(v17);

    ASSERT_EQ(1, shape.getEulerCharacteristic())
            << "Euler Characteristic 4; " << name;
    ASSERT_EQ(1, shape.getNumberOfComponents())
            << "Connected Components 4; " << name;
    ASSERT_EQ(0, shape.getNumberOfLoops()) << "Loops 4; " << name;
    ASSERT_EQ(0, shape.getNumberOfVoids()) << "Void 4; " << name;

    ASSERT_EQ(13, shape.getComponentSize(v1)) << "v1 CC Size 3; " << name;

    ASSERT_TRUE(shape.isInCollar(v8)) << "v18 Collar 1; " << name;
    ASSERT_TRUE(shape.isInCollar(v13)) << "v13 Collar 1; " << name;
    ASSERT_TRUE(shape.isInCollar(v17)) << "v17 Collar 1; " << name;
    ASSERT_FALSE(shape.isInCollar(v12)) << "v12 Collar 1; " << name;
    ASSERT_FALSE(shape.isInCollar(v5)) << "v5 Collar 1; " << name;
}

template <class Shape>
void verifyAugNonRemovability(std::string name)
{
    std::vector<Voxel> voxels;
    buildExample1Small<Voxel>(voxels);
    Shape shape(voxels);

    ASSERT_FALSE(shape.isRemovable()) << "Removable; " << name;
}

TEST(AugmentedShapeTest, Modifier) {
    verifyAugAddability<
            AugmentedShape<AddableShape<Voxel>, Voxel > >("AddableShape");
    verifyAugNonAddability<
            AugmentedShape<RemovableShape<Voxel>, Voxel > >("RemovableShape");
    verifyAugAddability<
            AugmentedShape<EditableShape<Voxel>, Voxel > >("EditableShape");

    verifyAugNonRemovability<
            AugmentedShape<AddableShape<Voxel>, Voxel > >("AddableShape");
    verifyAugRemovability<
            AugmentedShape<RemovableShape<Voxel>, Voxel > >("RemovableShape");
    verifyAugRemovability<
            AugmentedShape<EditableShape<Voxel>, Voxel > >("EditableShape");
}

template <class Shape>
void verifyAugHomology(std::string name)
{
    std::vector<Voxel> voxels;
    buildExample1Extended<Voxel>(voxels);
    Shape shape1(voxels);

    ASSERT_EQ(3, shape1.getEulerCharacteristic())
            << "Euler Characteristic 0; " << name;
    ASSERT_EQ(3, shape1.getNumberOfComponents())
            << "Connected Components 0; " << name;
    ASSERT_EQ(1, shape1.getNumberOfLoops()) << "Loops 0; " << name;
    ASSERT_EQ(1, shape1.getNumberOfVoids()) << "Void 0; " << name;

    voxels.clear();
    buildExample2<Voxel>(voxels);
    Shape shape2(voxels);

    ASSERT_EQ(1, shape2.getEulerCharacteristic())
            << "Euler Characteristic 1; " << name;
    ASSERT_EQ(2, shape2.getNumberOfComponents())
            << "Connected Components 1; " << name;
    ASSERT_EQ(1, shape2.getNumberOfLoops()) << "Loops 1; " << name;
    ASSERT_EQ(0, shape2.getNumberOfVoids()) << "Void 1; " << name;
}

TEST(AugmentedShapeTest, Homology) {
    verifyAugHomology<
            AugmentedShape<AddableShape<Voxel>, Voxel> >("AddableShape");
    verifyAugHomology<
            AugmentedShape<RemovableShape<Voxel>, Voxel> >("RemovableShape");
    verifyAugHomology<
            AugmentedShape<EditableShape<Voxel>, Voxel> >("EditableShape");
}

template <class Shape>
void verifyAugComponents(std::string name)
{
    std::vector<Voxel> voxels;
    std::vector<Voxel> comp;
    buildExample1<Voxel>(voxels);

    Voxel v1(0, 0, 2, Level::MAIN_SHAPE);
    Voxel v13(1, 4, 3, Level::CORE);

    Shape shape(voxels);

    Voxel rep1 = shape.getComponentRepresentative(v1);
    ASSERT_TRUE(shape.getComponentRepresentative(v13) == v13)
            << "v13 Rep; " << name;
    for (Voxel v : voxels) {
        if (v != v13 &&
                shape.template contains<Level::MAIN_SHAPE>(v[0], v[1], v[2]))
            ASSERT_EQ(rep1, shape.getComponentRepresentative(v))
                    << v << " Rep; " << name;
    }

    if constexpr (shape.isRemovable()){
        ASSERT_EQ(15, shape.getComponentSize(v1)) << "v1 CC Size; " << name;
        ASSERT_EQ(1, shape.getComponentSize(v13)) << "v13 CC Size; " << name;

        for (Voxel v : voxels){
            if (v.getLevel() != Level::NEIGHBORHOOD)
                ASSERT_TRUE(shape.isInCoreComponent(v))
                        << "not in core; " << name;
            else
                ASSERT_FALSE(shape.isInCoreComponent(v))
                        << "in core; " << name;
        }

        shape.getComponent(v13, comp);
        ASSERT_EQ(1, comp.size()) << "v13 Comp size; " << name;
        ASSERT_EQ(v13, comp.at(0)) << "v13 Comp; " << name;

        comp.clear();
        shape.getComponent(v1, comp);
        ASSERT_EQ(15, comp.size()) << "v1 Comp size; " << name;
    }
}

TEST(AugmentedShapeTest, Components) {
    verifyAugComponents<
            AugmentedShape<AddableShape<Voxel>, Voxel> >("AddableShape");
    verifyAugComponents<
            AugmentedShape<RemovableShape<Voxel>, Voxel> >("RemovableShape");
    verifyAugComponents<
            AugmentedShape<EditableShape<Voxel>, Voxel> >("EditableShape");
}

template <class Shape>
void verifyAugCollar(std::string name)
{
    std::vector<Voxel> voxels;
    buildExample1<Voxel>(voxels);

    Voxel v0(0, 0, 1, Level::NEIGHBORHOOD);  // N
    Voxel v6(1, 0, 3, Level::NEIGHBORHOOD);  // N
    Voxel v11(0, 3, 1, Level::NEIGHBORHOOD); // N
    Voxel v19(1, 4, 4, Level::NEIGHBORHOOD); // N
    Voxel v20(0, 4, 3, Level::NEIGHBORHOOD); // N

    Voxel v4(0, 1, 3, Level::MAIN_SHAPE);  // S
    Voxel v8(1, 1, 3, Level::CORE);  // C
    Voxel v13(1, 4, 3, Level::CORE); // C
    Voxel v16(0, 2, 0, Level::MAIN_SHAPE); // S
    Voxel v18(0, 4, 0, Level::MAIN_SHAPE); // S

    Shape shape(voxels);

    const typename Shape::collar_container& collar = shape.getCollar();
    int count = 0;

    for (Voxel v : collar){
        if (v.getLevel() == Level::NEIGHBORHOOD) count++;
    }

    ASSERT_EQ(5, count) << "Neighborhood Collar size; " << name;
    ASSERT_TRUE(collar.find(v0) != collar.end())
            << "v0 not in given collar; " << name;
    ASSERT_TRUE(collar.find(v6) != collar.end())
            << "v6 not in given collar; " << name;
    ASSERT_TRUE(collar.find(v11) != collar.end())
            << "v11 not in given collar; " << name;
    ASSERT_TRUE(collar.find(v19) != collar.end())
            << "v19 not in given collar; " << name;
    ASSERT_TRUE(collar.find(v20) != collar.end())
            << "v20 not in given collar; " << name;

    ASSERT_TRUE(shape.isInCollar(v0)) << "v0 not in collar; " << name;
    ASSERT_TRUE(shape.isInCollar(v6)) << "v6 not in collar; " << name;
    ASSERT_TRUE(shape.isInCollar(v11)) << "v11 not in collar; " << name;
    ASSERT_TRUE(shape.isInCollar(v19)) << "v19 not in collar; " << name;
    ASSERT_TRUE(shape.isInCollar(v20)) << "v20 not in collar; " << name;

    ASSERT_FALSE(shape.isInCollar(v4)) << "v4 in collar; " << name;
    ASSERT_FALSE(shape.isInCollar(v8)) << "v8 in collar; " << name;
    ASSERT_FALSE(shape.isInCollar(v13)) << "v13 in collar; " << name;
    ASSERT_FALSE(shape.isInCollar(v16)) << "v16 in collar; " << name;
    ASSERT_FALSE(shape.isInCollar(v18)) << "v18 in collar; " << name;
}

TEST(AugmentedShapeTest, Collar) {
    verifyAugCollar<
            AugmentedShape<AddableShape<Voxel>, Voxel> >("AddableShape");
    verifyAugCollar<
            AugmentedShape<RemovableShape<Voxel>, Voxel> >("RemovableShape");
    verifyAugCollar<
            AugmentedShape<EditableShape<Voxel>, Voxel> >("EditableShape");
}

template <class Shape>
void verifyAugOther(std::string name)
{
    std::vector<Voxel> voxels;
    buildExample1Small<Voxel>(voxels);
    Shape shape(voxels);

    ASSERT_EQ(11, shape.template getNumberOfDualCellsInDim<0>())
            << "Cells in dim 0; " << name;
    ASSERT_EQ(12, shape.template getNumberOfDualCellsInDim<1>())
            << "Cells in dim 1; " << name;
    ASSERT_EQ(3, shape.template getNumberOfDualCellsInDim<2>())
            << "Cells in dim 2; " << name;
    ASSERT_EQ(0, shape.template getNumberOfDualCellsInDim<3>())
            << "Cells in dim 3; " << name;

    shape.clear();

    ASSERT_EQ(0, shape.template getNumberOfDualCellsInDim<0>())
            << "Empty Shape Cells in dim 0; " << name;
    ASSERT_EQ(0, shape.template getNumberOfDualCellsInDim<1>())
            << "Empty Shape Cells in dim 1; " << name;
    ASSERT_EQ(0, shape.template getNumberOfDualCellsInDim<2>())
            << "Empty Shape Cells in dim 2; " << name;
    ASSERT_EQ(0, shape.template getNumberOfDualCellsInDim<3>())
            << "Empty Shape Cells in dim 3; " << name;
}

TEST(AugmentedShapeTest, Other) {
    verifyAugOther<
            AugmentedShape<AddableShape<Voxel>, Voxel> >("AddableShape");
    verifyAugOther<
            AugmentedShape<RemovableShape<Voxel>, Voxel> >("RemovableShape");
    verifyAugOther<
            AugmentedShape<EditableShape<Voxel>, Voxel> >("EditableShape");
}

template <class InvShape, class Shape>
void verifyInvAugConstructor(std::string name)
{
    std::vector<Voxel> voxels;
    buildExample2<Voxel>(voxels);

    Shape shape(voxels);
    InvShape inv(shape);

    ASSERT_TRUE(inv.template contains<Level::MAIN_SHAPE>(14,0,0)) << name;
    ASSERT_TRUE(inv.template contains<Level::MAIN_SHAPE>(1,5,11)) << name;
    ASSERT_TRUE(inv.template contains<Level::NEIGHBORHOOD>(11,1,7)) << name;
    ASSERT_TRUE(inv.template contains<Level::NEIGHBORHOOD>(2,4,9)) << name;
    ASSERT_TRUE(inv.template contains<Level::CORE>(2,5,11)) << name;
    ASSERT_TRUE(inv.template contains<Level::CORE>(15,0,0)) << name;

    ASSERT_FALSE(inv.template contains<Level::MAIN_SHAPE>(11,3,7)) << name;
    ASSERT_FALSE(inv.template contains<Level::MAIN_SHAPE>(2,5,9)) << name;
    ASSERT_FALSE(inv.template contains<Level::NEIGHBORHOOD>(8,2,5)) << name;
    ASSERT_FALSE(inv.template contains<Level::NEIGHBORHOOD>(16,0,0)) << name;
    ASSERT_FALSE(inv.template contains<Level::CORE>(13,0,0)) << name;
    ASSERT_FALSE(inv.template contains<Level::CORE>(1,6,11)) << name;

    ASSERT_EQ(15, inv.getShapeDimensions()[0]) << name;
    ASSERT_EQ(7, inv.getShapeDimensions()[1]) << name;
    ASSERT_EQ(12, inv.getShapeDimensions()[2]) << name;
}

TEST(InvertedAugmentedShapeTest, Constructor) {
    verifyInvAugConstructor<
            InvertedAugmentedShape<AddableShape<Voxel>, Voxel>,
            AddableShape<Voxel>
            >("AddableShape");
    verifyInvAugConstructor<
            InvertedAugmentedShape<RemovableShape<Voxel>, Voxel>,
            RemovableShape<Voxel>
            >("RemovableShape");
    verifyInvAugConstructor<
            InvertedAugmentedShape<EditableShape<Voxel>, Voxel>,
            EditableShape<Voxel>
            >("EditableShape");
}

template <class InvShape, class Shape>
void verifyInvAugModifier(std::string name)
{
    std::vector<Voxel> voxels;
    buildExample1Small<Voxel>(voxels);

    Shape shape(voxels);
    InvShape inv(shape);

    ASSERT_FALSE(inv.isAddable()) << name;
    ASSERT_FALSE(inv.isRemovable()) << name;
}

TEST(InvertedAugmentedShapeTest, Modifier) {
    verifyInvAugModifier<
            InvertedAugmentedShape<AddableShape<Voxel>, Voxel>,
            AddableShape<Voxel>
            >("AddableShape");
    verifyInvAugModifier<
            InvertedAugmentedShape<RemovableShape<Voxel>, Voxel>,
            RemovableShape<Voxel>
            >("RemovableShape");
    verifyInvAugModifier<
            InvertedAugmentedShape<EditableShape<Voxel>, Voxel>,
            EditableShape<Voxel>
            >("EditableShape");
}

template <class InvShape, class Shape>
void verifyInvAugCollar(std::string name)
{
    std::vector<Voxel> voxels;
    buildExample2<Voxel>(voxels);

    Shape shape(voxels);
    InvShape inv(shape);

    const typename InvShape::collar_container& collar = inv.getCollar();
    int count = 0;

    for (Voxel v : collar){
        if (v.getLevel() == Level::NEIGHBORHOOD) count++;
    }

    ASSERT_EQ(184, count) << "Neighborhood Collar size; " << name;

    ASSERT_TRUE(inv.isInCollar(Voxel(13,1,1))) << name;
    ASSERT_TRUE(inv.isInCollar(Voxel(2,4,13))) << name;
    ASSERT_TRUE(inv.isInCollar(Voxel(16,0,0))) << name;
    ASSERT_FALSE(inv.isInCollar(Voxel(17,0,0))) << name;
    ASSERT_FALSE(inv.isInCollar(Voxel(8,2,5))) << name;
}

TEST(InvertedAugmentedShapeTest, Collar) {
    verifyInvAugCollar<
            InvertedAugmentedShape<AddableShape<Voxel>, Voxel>,
            AddableShape<Voxel>
            >("AddableShape");
    verifyInvAugCollar<
            InvertedAugmentedShape<RemovableShape<Voxel>, Voxel>,
            RemovableShape<Voxel>
            >("RemovableShape");
    verifyInvAugCollar<
            InvertedAugmentedShape<EditableShape<Voxel>, Voxel>,
            EditableShape<Voxel>
            >("EditableShape");
}




