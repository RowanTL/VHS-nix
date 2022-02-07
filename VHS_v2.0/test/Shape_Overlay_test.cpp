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
 * @file Shape_Overlay_test.cpp
 * Test file for the Shape Overlay classes.
 **/

#include <gtest/gtest.h>

#include "shape/ShapeOverlay.h"
#include "shape/Voxel.h"
#include "shape/AugmentedShape.h"
#include "shape/EditableShape.h"
#include "shape/ShapeUtilities.h"

#include "test_examples.h"

using shape_simplification::shape::ShapeOverlay;
using shape_simplification::shape::Voxel;
using shape_simplification::shape::AugmentedShape;
using shape_simplification::shape::EditableShape;

using Shape = AugmentedShape<EditableShape<Voxel>, Voxel>;

TEST(OverlayTest, Addition) {
    std::vector<Voxel> voxels;
    typename ShapeOverlay<Shape, Voxel>::voxel_container voxelsToAdd;

    buildExample1Small<Voxel>(voxels);

    Voxel v14(0, 4, 2, Level::NEIGHBORHOOD); // N
    Voxel v15(0, 4, 1, Level::NEIGHBORHOOD); // N
    Voxel v16(0, 2, 0, Level::NEIGHBORHOOD); // N
    Voxel v17(0, 3, 0, Level::NEIGHBORHOOD); // N
    Voxel v18(0, 4, 0, Level::NEIGHBORHOOD); // N

    voxels.push_back(v14);
    voxels.push_back(v15);
    voxels.push_back(v16);
    voxels.push_back(v17);
    voxels.push_back(v18);

    voxelsToAdd.insert(v14);
    voxelsToAdd.insert(v15);
    voxelsToAdd.insert(v16);
    voxelsToAdd.insert(v17);
    voxelsToAdd.insert(v18);

    Shape shape(voxels);
    ShapeOverlay<Shape, Voxel> overlay(shape);

    ASSERT_EQ(2, overlay.getInitialEulerCharacteristic())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(0, overlay.getInitialNumberOfLoops())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(2, overlay.getCurrentEulerCharacteristic())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfLoops())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "Initial Shape Overlay value.";

    overlay.addVoxelsInMainShape(voxelsToAdd);

    ASSERT_EQ(2, overlay.getInitialEulerCharacteristic())
            << "Shape Overlay after adding.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "Shape Overlay after adding.";
    ASSERT_EQ(0, overlay.getInitialNumberOfLoops())
            << "Shape Overlay after adding.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "Shape Overlay after adding.";
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic())
            << "Shape Overlay after adding.";
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents())
            << "Shape Overlay after adding.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfLoops())
            << "Shape Overlay after adding.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "Shape Overlay after adding.";

    overlay.addVoxelsInMainShapeWithH0Guarantee(voxelsToAdd);

    ASSERT_EQ(2, overlay.getInitialEulerCharacteristic())
            << "Shape Overlay after adding with H0 Guarantee.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "Shape Overlay after adding with H0 Guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfLoops())
            << "Shape Overlay after adding with H0 Guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "Shape Overlay after adding with H0 Guarantee.";
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic())
            << "Shape Overlay after adding with H0 Guarantee.";
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents())
            << "Shape Overlay after adding with H0 Guarantee.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfLoops())
            << "Shape Overlay after adding with H0 Guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "Shape Overlay after adding with H0 Guarantee.";

    overlay.addVoxelsInMainShapeWithH2Guarantee(voxelsToAdd);

    ASSERT_EQ(2, overlay.getInitialEulerCharacteristic())
            << "Shape Overlay after adding with H2 Guarantee.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "Shape Overlay after adding with H2 Guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfLoops())
            << "Shape Overlay after adding with H2 Guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "Shape Overlay after adding with H2 Guarantee.";
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic())
            << "Shape Overlay after adding with H2 Guarantee.";
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents())
            << "Shape Overlay after adding with H2 Guarantee.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfLoops())
            << "Shape Overlay after adding with H2 Guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "Shape Overlay after adding with H2 Guarantee.";

    overlay.addVoxelsInMainShapeWithH0H2Guarantee(voxelsToAdd);

    ASSERT_EQ(2, overlay.getInitialEulerCharacteristic())
            << "Shape Overlay after adding with H0H2 Guarantee.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "Shape Overlay after adding with H0H2 Guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfLoops())
            << "Shape Overlay after adding with H0H2 Guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "Shape Overlay after adding with H0H2 Guarantee.";
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic())
            << "Shape Overlay after adding with H0H2 Guarantee.";
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents())
            << "Shape Overlay after adding with H0H2 Guarantee.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfLoops())
            << "Shape Overlay after adding with H0H2 Guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "Shape Overlay after adding with H0H2 Guarantee.";

    ASSERT_EQ(1, overlay.addVoxelsAndReturnEulerOnly(voxelsToAdd));
    ASSERT_EQ(2, overlay.addVoxelsAndReturnH0Only(voxelsToAdd));
    ASSERT_EQ(0, overlay.addVoxelsAndReturnH2Only(voxelsToAdd));
}

TEST(OverlayTest, Removal) {
    std::vector<Voxel> voxels;
    typename ShapeOverlay<Shape, Voxel>::voxel_container voxelsToRemove;

    buildExample1<Voxel>(voxels);

    voxelsToRemove.insert(Voxel(1, 1, 3));
    voxelsToRemove.insert(Voxel(1, 4, 3));
    voxelsToRemove.insert(Voxel(0, 3, 0));

    Shape shape(voxels);
    ShapeOverlay<Shape, Voxel> overlay(shape);

    ASSERT_EQ(1, overlay.getInitialEulerCharacteristic())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(1, overlay.getInitialNumberOfLoops())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfLoops())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "Initial Shape Overlay value.";

    overlay.removeVoxelsFromMainShape(voxelsToRemove);

    ASSERT_EQ(1, overlay.getInitialEulerCharacteristic())
            << "Shape Overlay after removing.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "Shape Overlay after removing.";
    ASSERT_EQ(1, overlay.getInitialNumberOfLoops())
            << "Shape Overlay after removing.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "Shape Overlay after removing.";
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic())
            << "Shape Overlay after removing.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfComponents())
            << "Shape Overlay after removing.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfLoops())
            << "Shape Overlay after removing.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "Shape Overlay after removing.";

    overlay.removeVoxelsFromMainShapeWithH2Guarantee(voxelsToRemove);

    ASSERT_EQ(1, overlay.getInitialEulerCharacteristic())
            << "Shape Overlay after removing with H2 Guarantee.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "Shape Overlay after removing with H2 Guarantee.";
    ASSERT_EQ(1, overlay.getInitialNumberOfLoops())
            << "Shape Overlay after removing with H2 Guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "Shape Overlay after removing with H2 Guarantee.";
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic())
            << "Shape Overlay after removing with H2 Guarantee.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfComponents())
            << "Shape Overlay after removing with H2 Guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfLoops())
            << "Shape Overlay after removing with H2 Guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "Shape Overlay after removing with H2 Guarantee.";

    ASSERT_EQ(1, overlay.removeVoxelsAndReturnEulerOnly(voxelsToRemove));
    ASSERT_EQ(1, overlay.removeVoxelsAndReturnH0Only(voxelsToRemove));
    ASSERT_EQ(0, overlay.removeVoxelsAndReturnH2Only(voxelsToRemove));

    voxelsToRemove.erase(Voxel(1, 4, 3));

    overlay.removeVoxelsFromMainShapeWithH0Guarantee(voxelsToRemove);

    ASSERT_EQ(1, overlay.getInitialEulerCharacteristic())
            << "Shape Overlay after removing with H0 Guarantee.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "Shape Overlay after removing with H0 Guarantee.";
    ASSERT_EQ(1, overlay.getInitialNumberOfLoops())
            << "Shape Overlay after removing with H0 Guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "Shape Overlay after removing with H0 Guarantee.";
    ASSERT_EQ(2, overlay.getCurrentEulerCharacteristic())
            << "Shape Overlay after removing with H0 Guarantee.";
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents())
            << "Shape Overlay after removing with H0 Guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfLoops())
            << "Shape Overlay after removing with H0 Guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "Shape Overlay after removing with H0 Guarantee.";

    overlay.removeVoxelsFromMainShapeWithH0H2Guarantee(voxelsToRemove);

    ASSERT_EQ(1, overlay.getInitialEulerCharacteristic())
            << "Shape Overlay after removing with H0H2 Guarantee.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "Shape Overlay after removing with H0H2 Guarantee.";
    ASSERT_EQ(1, overlay.getInitialNumberOfLoops())
            << "Shape Overlay after removing with H0H2 Guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "Shape Overlay after removing with H0H2 Guarantee.";
    ASSERT_EQ(2, overlay.getCurrentEulerCharacteristic())
            << "Shape Overlay after removing with H0H2 Guarantee.";
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents())
            << "Shape Overlay after removing with H0H2 Guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfLoops())
            << "Shape Overlay after removing with H0H2 Guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "Shape Overlay after removing with H0H2 Guarantee.";
}

TEST(OverlayTest, Save) {
    std::vector<Voxel> voxels;
    typename ShapeOverlay<Shape, Voxel>::voxel_container voxelsToRemove;
    typename ShapeOverlay<Shape, Voxel>::voxel_container voxelsToAdd;

    buildExample1<Voxel>(voxels);

    for (int x = 10; x < 14; x++) {
        for (int y = 10; y < 14; y++) {
            for (int z = 10; z < 14; z++) {
                voxels.push_back(Voxel(x, y, z, Level::NEIGHBORHOOD));
                if (!(x > 10 && x < 13 && y > 10 && y < 13 && z > 10 && z < 13))
                    voxelsToAdd.insert(Voxel(x, y, z));
            }
        }
    }
    voxelsToAdd.insert(Voxel(0, 3, 1));

    voxelsToRemove.insert(Voxel(1, 1, 3));
    voxelsToRemove.insert(Voxel(1, 4, 3));
    voxelsToRemove.insert(Voxel(0, 3, 0));

    Shape shape(voxels);
    ShapeOverlay<Shape, Voxel> overlay(shape);

    ASSERT_EQ(1, overlay.getInitialEulerCharacteristic())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(1, overlay.getInitialNumberOfLoops())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfLoops())
            << "Initial Shape Overlay value.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "Initial Shape Overlay value.";

    overlay.saveAddedVoxels(voxelsToAdd);

    ASSERT_EQ(4, overlay.getInitialEulerCharacteristic())
            << "After saving additions.";
    ASSERT_EQ(3, overlay.getInitialNumberOfComponents())
            << "After saving additions.";
    ASSERT_EQ(0, overlay.getInitialNumberOfLoops())
            << "After saving additions.";
    ASSERT_EQ(1, overlay.getInitialNumberOfVoids())
            << "After saving additions.";
    ASSERT_EQ(4, overlay.getCurrentEulerCharacteristic())
            << "After saving additions.";
    ASSERT_EQ(3, overlay.getCurrentNumberOfComponents())
            << "After saving additions.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfLoops())
            << "After saving additions.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfVoids())
            << "After saving additions.";

    overlay.clearModifications();
    voxelsToAdd.erase(Voxel(10, 10, 10));

    overlay.saveAddedVoxelsWithH2Guarantee(voxelsToAdd);

    ASSERT_EQ(3, overlay.getInitialEulerCharacteristic())
            << "After saving additions with H2 guarantee.";
    ASSERT_EQ(3, overlay.getInitialNumberOfComponents())
            << "After saving additions with H2 guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfLoops())
            << "After saving additions with H2 guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "After saving additions with H2 guarantee.";
    ASSERT_EQ(3, overlay.getCurrentEulerCharacteristic())
            << "After saving additions with H2 guarantee.";
    ASSERT_EQ(3, overlay.getCurrentNumberOfComponents())
            << "After saving additions with H2 guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfLoops())
            << "After saving additions with H2 guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "After saving additions with H2 guarantee.";

    overlay.clearModifications();

    overlay.saveRemovedVoxels(voxelsToRemove);

    ASSERT_EQ(1, overlay.getInitialEulerCharacteristic())
            << "After saving removals.";
    ASSERT_EQ(1, overlay.getInitialNumberOfComponents())
            << "After saving removals.";
    ASSERT_EQ(0, overlay.getInitialNumberOfLoops())
            << "After saving removals.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "After saving removals.";
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic())
            << "After saving removals.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfComponents())
            << "After saving removals.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfLoops())
            << "After saving removals.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "After saving removals.";

    overlay.clearModifications();

    overlay.saveRemovedVoxelsWithH2Guarantee(voxelsToRemove);

    ASSERT_EQ(1, overlay.getInitialEulerCharacteristic())
            << "After saving removals with H2 guarantee.";
    ASSERT_EQ(1, overlay.getInitialNumberOfComponents())
            << "After saving removals with H2 guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfLoops())
            << "After saving removals with H2 guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "After saving removals with H2 guarantee.";
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic())
            << "After saving removals with H2 guarantee.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfComponents())
            << "After saving removals with H2 guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfLoops())
            << "After saving removals with H2 guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "After saving removals with H2 guarantee.";

    overlay.clearModifications();
    voxelsToRemove.erase(Voxel(1, 4, 3));

    overlay.saveRemovedVoxelsWithH0Guarantee(voxelsToRemove);

    ASSERT_EQ(2, overlay.getInitialEulerCharacteristic())
            << "After saving removals with H0 guarantee.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "After saving removals with H0 guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfLoops())
            << "After saving removals with H0 guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "After saving removals with H0 guarantee.";
    ASSERT_EQ(2, overlay.getCurrentEulerCharacteristic())
            << "After saving removals with H0 guarantee.";
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents())
            << "After saving removals with H0 guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfLoops())
            << "After saving removals with H0 guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "After saving removals with H0 guarantee.";

    overlay.clearModifications();

    overlay.saveRemovedVoxelsWithH0H2Guarantee(voxelsToRemove);

    ASSERT_EQ(2, overlay.getInitialEulerCharacteristic())
            << "After saving removals with H0H2 guarantee.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "After saving removals with H0H2 guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfLoops())
            << "After saving removals with H0H2 guarantee.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "After saving removals with H0H2 guarantee.";
    ASSERT_EQ(2, overlay.getCurrentEulerCharacteristic())
            << "After saving removals with H0H2 guarantee.";
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents())
            << "After saving removals with H0H2 guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfLoops())
            << "After saving removals with H0H2 guarantee.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "After saving removals with H0H2 guarantee.";

    overlay.clearModifications();

    ASSERT_EQ(1, overlay.getInitialEulerCharacteristic())
            << "After clearance.";
    ASSERT_EQ(2, overlay.getInitialNumberOfComponents())
            << "After clearance.";
    ASSERT_EQ(1, overlay.getInitialNumberOfLoops())
            << "After clearance.";
    ASSERT_EQ(0, overlay.getInitialNumberOfVoids())
            << "After clearance.";
    ASSERT_EQ(1, overlay.getCurrentEulerCharacteristic())
            << "After clearance.";
    ASSERT_EQ(2, overlay.getCurrentNumberOfComponents())
            << "After clearance.";
    ASSERT_EQ(1, overlay.getCurrentNumberOfLoops())
            << "After clearance.";
    ASSERT_EQ(0, overlay.getCurrentNumberOfVoids())
            << "After clearance.";
}

