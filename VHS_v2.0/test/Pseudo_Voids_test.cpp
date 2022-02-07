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
 * @file Pseudo_Voids_test.cpp
 * Test file for the Pseudo-Voids classes.
 **/

#include <gtest/gtest.h>

#include "simplifier/DistancePseudoVoidsCreator.h"
#include "shape/Voxel.h"
#include "shape/ShapeUtilities.h"
#include "shape/AugmentedShape.h"
#include "shape/EditableShape.h"
#include "util/FileIO.h"

#include "test_examples.h"

using shape_simplification::simplifier::DistancePseudoVoidsCreator;
using shape_simplification::shape::Voxel;
using shape_simplification::shape::Level;
using shape_simplification::shape::AugmentedShape;
using shape_simplification::shape::EditableShape;

TEST(PseudoVoidsTest, CloseOpenTop) {
    ProcessedImageStack proc;
    std::vector<png::image<png::gray_pixel> > images;

    buildPseudoVoidExampleAsStack<Voxel>(images);

    proc.load(images);
    proc.setCoreThreshold(200);
    proc.setShapeThreshold(150);
    proc.setNeighborhoodThreshold(100);
    const std::array<unsigned int, 3> &dim = proc.getDimensions();

    DistancePseudoVoidsCreator pvc(proc, 0);

    ASSERT_EQ(56, dim[2]);
    ASSERT_EQ(36, dim[1]);
    ASSERT_EQ(36, dim[0]);

    for (unsigned int x = 0; x < dim[1]; x++){
        for (unsigned int y = 0; y < dim[0]; y++){
            ASSERT_EQ(0, static_cast<int>(proc.getLevel(x, y, dim[2] - 1)));
        }
    }

    pvc.closeTop();

    ASSERT_EQ(57, dim[2]);
    ASSERT_EQ(36, dim[1]);
    ASSERT_EQ(36, dim[0]);

    for (unsigned int x = 0; x < dim[1]; x++){
        for (unsigned int y = 0; y < dim[0]; y++){
            ASSERT_EQ(0, static_cast<int>(proc.getLevel(x, y, dim[2] - 1)));
        }
    }

    pvc.removeTop();

    ASSERT_EQ(56, dim[2]);
    ASSERT_EQ(36, dim[1]);
    ASSERT_EQ(36, dim[0]);

    for (unsigned int x = 0; x < dim[1]; x++){
        for (unsigned int y = 0; y < dim[0]; y++){
            ASSERT_EQ(0, static_cast<int>(proc.getLevel(x, y, dim[2] - 1)));
        }
    }

    proc.setValue(150, 8, 8, dim[2] - 1);
    proc.setValue(150, 28, 28, dim[2] - 1);

    ASSERT_EQ(56, dim[2]);
    ASSERT_EQ(36, dim[1]);
    ASSERT_EQ(36, dim[0]);

    for (unsigned int x = 0; x < dim[1]; x++){
        for (unsigned int y = 0; y < dim[0]; y++){
            if ((x == 8 && y == 8) || (x == 28 && y == 28))
                ASSERT_EQ(150, static_cast<int>(
                              proc.getLevel(x, y, dim[2] - 1)));
            else
                ASSERT_EQ(0, static_cast<int>(
                              proc.getLevel(x, y, dim[2] - 1)));
        }
    }

    pvc.closeTop();

    ASSERT_EQ(57, dim[2]);
    ASSERT_EQ(36, dim[1]);
    ASSERT_EQ(36, dim[0]);

    for (unsigned int x = 0; x < dim[1]; x++){
        for (unsigned int y = 0; y < dim[0]; y++){
            if (x >= 8 && x <= 28 && y >= 8 && y <= 28)
                ASSERT_EQ(150, static_cast<int>(
                              proc.getLevel(x, y, dim[2] - 1)));
            else
                ASSERT_EQ(0, static_cast<int>(
                              proc.getLevel(x, y, dim[2] - 1)));
        }
    }

    pvc.removeTop();

    ASSERT_EQ(56, dim[2]);
    ASSERT_EQ(36, dim[1]);
    ASSERT_EQ(36, dim[0]);

    for (unsigned int x = 0; x < dim[1]; x++){
        for (unsigned int y = 0; y < dim[0]; y++){
            if ((x == 8 && y == 8) || (x == 28 && y == 28))
                ASSERT_EQ(150, static_cast<int>(
                              proc.getLevel(x, y, dim[2] - 1)));
            else
                ASSERT_EQ(0, static_cast<int>(
                              proc.getLevel(x, y, dim[2] - 1)));
        }
    }
}

TEST(PseudoVoidsTest, PVCreation) {
    ProcessedImageStack proc;
    std::vector<png::image<png::gray_pixel> > images;
    AugmentedShape<EditableShape<Voxel>, Voxel > shape1;
    AugmentedShape<EditableShape<Voxel>, Voxel > shape2;

    buildPseudoVoidExampleAsStack<Voxel>(images);

    proc.load(images);
    proc.setCoreThreshold(200);
    proc.setShapeThreshold(150);
    proc.setNeighborhoodThreshold(100);

    proc >> shape1;

    ASSERT_EQ(1, shape1.getNumberOfComponents());
    ASSERT_EQ(0, shape1.getNumberOfLoops());
    ASSERT_EQ(0, shape1.getNumberOfVoids());

    DistancePseudoVoidsCreator pvc(proc, 0, true);

    pvc.createPseudoVoids();

    proc >> shape2;

    ASSERT_EQ(1, shape2.getNumberOfComponents());
    ASSERT_EQ(0, shape2.getNumberOfLoops());
    ASSERT_EQ(1, shape2.getNumberOfVoids());
}

TEST(PseudoVoidsTest, VoidFill) {
    ProcessedImageStack proc;
    std::vector<png::image<png::gray_pixel> > images;
    AugmentedShape<EditableShape<Voxel>, Voxel > shape1;
    AugmentedShape<EditableShape<Voxel>, Voxel > shape2;
    AugmentedShape<EditableShape<Voxel>, Voxel > shape3;

    buildPseudoVoidExampleAsStack<Voxel>(images);

    proc.load(images);
    proc.setCoreThreshold(200);
    proc.setShapeThreshold(150);
    proc.setNeighborhoodThreshold(100);

    DistancePseudoVoidsCreator pvc(proc, 0, true);

    pvc.fillVoids();

    proc >> shape1;

    ASSERT_EQ(1, shape1.getNumberOfComponents());
    ASSERT_EQ(0, shape1.getNumberOfLoops());
    ASSERT_EQ(0, shape1.getNumberOfVoids());

    pvc.createPseudoVoids();

    proc >> shape2;

    ASSERT_EQ(1, shape2.getNumberOfComponents());
    ASSERT_EQ(0, shape2.getNumberOfLoops());
    ASSERT_EQ(1, shape2.getNumberOfVoids());

    pvc.fillVoids();

    proc >> shape3;

    ASSERT_EQ(1, shape3.getNumberOfComponents());
    ASSERT_EQ(0, shape3.getNumberOfLoops());
    ASSERT_EQ(0, shape3.getNumberOfVoids());
}

