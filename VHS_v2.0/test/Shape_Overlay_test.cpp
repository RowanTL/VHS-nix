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
 * @file Shape_Overlay_test.cpp
 * Test file for the Shape Overlay classes.
 **/

#include <algorithm>
#include <gtest/gtest.h>
#include <random>

#include "shape/AddableShape.h"
#include "shape/RemovableShape.h"
#include "shape/EditableShape.h"
#include "shape/AugmentedShape.h"
#include "shape/ShapeOverlay.h"
#include "shape/Voxel.h"

#include "test_examples.h"

using shape_simplification::shape::AddableShape;
using shape_simplification::shape::RemovableShape;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::AugmentedShape;
using shape_simplification::shape::ShapeOverlay;
using shape_simplification::shape::Voxel;

template <class Shape, class Voxel>
void verifyOLConstructor(std::string name);
template <class Shape, class Voxel>
void verifyOLAddVoxelsMethod(std::string name);
template <class Shape, class Voxel>
void verifyOLRemoveVoxelsMethod(std::string name);

TEST(ShapeOverlayTest, Constructor) {
    verifyOLConstructor<
            AugmentedShape<AddableShape<Voxel>, Voxel>,
            Voxel>("AddableShape");
    verifyOLConstructor<
            AugmentedShape<RemovableShape<Voxel>, Voxel>,
            Voxel>("RemovableShape");
    verifyOLConstructor<
            AugmentedShape<EditableShape<Voxel>, Voxel>,
            Voxel>("EditableShape");
}

template <class Shape, class Voxel>
void verifyOLConstructor(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildExample1<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);
    ShapeOverlay<Shape, Voxel> overlay(shape);

    ASSERT_EQ(1, overlay.getInitialEulerCharacteristic()) << name;
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents()) << name;
    ASSERT_EQ(1, overlay.getInitialNumberOfLoops()) << name;
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids()) << name;
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic()) << name;
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents()) << name;
    ASSERT_EQ(1, overlay.getCurrentNumberOfLoops()) << name;
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids()) << name;
}

TEST(ShapeOverlayTest, AddVoxelsMethod) {
    verifyOLAddVoxelsMethod<
            AugmentedShape<AddableShape<Voxel>, Voxel>,
            Voxel>("AddableShape");
    verifyOLAddVoxelsMethod<
            AugmentedShape<RemovableShape<Voxel>, Voxel>,
            Voxel>("RemovableShape");
    verifyOLAddVoxelsMethod<
            AugmentedShape<EditableShape<Voxel>, Voxel>,
            Voxel>("EditableShape");
}

template <class Shape, class Voxel>
void verifyOLAddVoxelsMethod(std::string name) {
    std::vector<Voxel> voxels;
    typename ShapeOverlay<Shape, Voxel>::voxel_container voxelsToAdd;
    std::vector<typename Shape::Level> minLevels;

    buildExample1Small<Shape,Voxel>(voxels, minLevels);

    Voxel v14(0, 4, 2); // N
    Voxel v15(0, 4, 1); // N
    Voxel v16(0, 2, 0); // N
    Voxel v17(0, 3, 0); // N
    Voxel v18(0, 4, 0); // N

    voxels.push_back(v14);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v15);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v16);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v17);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v18);
    minLevels.push_back(Shape::NEIGHBORHOOD);

    voxelsToAdd.insert(v14);
    voxelsToAdd.insert(v15);
    voxelsToAdd.insert(v16);
    voxelsToAdd.insert(v17);
    voxelsToAdd.insert(v18);

    Shape shape(voxels, minLevels);
    ShapeOverlay<Shape, Voxel> overlay(shape);

    ASSERT_EQ(2, overlay.getInitialEulerCharacteristic()) << name;
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents()) << name;
    ASSERT_EQ(0, overlay.getInitialNumberOfLoops()) << name;
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids()) << name;
    ASSERT_EQ(2, overlay.getCurrentEulerCharacteristic()) << name;
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents()) << name;
    ASSERT_EQ(0, overlay.getCurrentNumberOfLoops()) << name;
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids()) << name;

    overlay.addVoxelsInMainShape(voxelsToAdd);

    ASSERT_EQ(2, overlay.getInitialEulerCharacteristic())
            << name << " after adding.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << name << " after adding.";
    ASSERT_EQ(0, overlay.getInitialNumberOfLoops())
            << name << " after adding.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << name << " after adding.";
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic())
            << name << " after adding.";
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents())
            << name << " after adding.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfLoops())
            << name << " after adding.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << name << " after adding.";
}

TEST(ShapeOverlayTest, RemoveVoxelsMethod) {
    verifyOLRemoveVoxelsMethod<
            AugmentedShape<RemovableShape<Voxel>, Voxel>,
            Voxel>("RemovableShape");
    verifyOLRemoveVoxelsMethod<
            AugmentedShape<EditableShape<Voxel>, Voxel>,
            Voxel>("EditableShape");
}

template <class Shape, class Voxel>
void verifyOLRemoveVoxelsMethod(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;
    typename ShapeOverlay<Shape, Voxel>::voxel_container voxelsToRemove;

    buildExample1<Shape,Voxel>(voxels, minLevels);

    voxelsToRemove.insert(Voxel(1, 1, 3));
    voxelsToRemove.insert(Voxel(1, 4, 3));
    voxelsToRemove.insert(Voxel(0, 3, 0));

    Shape shape(voxels, minLevels);
    ShapeOverlay<Shape, Voxel> overlay(shape);

    ASSERT_EQ(1, overlay.getInitialEulerCharacteristic()) << name;
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents()) << name;
    ASSERT_EQ(1, overlay.getInitialNumberOfLoops()) << name;
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids()) << name;
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic()) << name;
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents()) << name;
    ASSERT_EQ(1, overlay.getCurrentNumberOfLoops()) << name;
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids()) << name;

    overlay.removeVoxelsFromMainShape(voxelsToRemove);

    ASSERT_EQ(1, overlay.getInitialEulerCharacteristic())
            << name << " after removing.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << name << " after removing.";
    ASSERT_EQ(1, overlay.getInitialNumberOfLoops())
            << name << " after removing.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << name << " after removing.";
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic())
            << name << " after removing.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfComponents())
            << name << " after removing.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfLoops())
            << name << " after removing.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << name << " after removing.";
}
