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
 * @file H2_Candidates_Generator_test.cpp
 * Test file for the H2_Candidates_Generator classes.
 **/

#include <algorithm>
#include <gtest/gtest.h>
#include <random>
#include <vector>

#include "shape/AddableShape.h"
#include "shape/RemovableShape.h"
#include "shape/EditableShape.h"
#include "shape/AugmentedShape.h"
#include "shape/RestrictedShapeCopy.h"
#include "shape/InvertedRestrictedShapeCopy.h"
#include "shape/ClosedInvertedRestrictedShapeCopy.h"
#include "shape/Voxel.h"
#include "simplifier/AdditiveH2CandidateGenerator.h"
#include "simplifier/SubstractiveH2CandidateGenerator.h"

#include "test_examples.h"

using shape_simplification::shape::AddableShape;
using shape_simplification::shape::RemovableShape;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::AugmentedShape;
using shape_simplification::shape::RestrictedShapeCopy;
using shape_simplification::shape::InvertedRestrictedShapeCopy;
using shape_simplification::shape::ClosedInvertedRestrictedShapeCopy;
using shape_simplification::shape::Voxel;
using shape_simplification::simplifier::AdditiveH2CandidateGenerator;
using shape_simplification::simplifier::SubstractiveH2CandidateGenerator;

using AugAddShape = AugmentedShape<AddableShape<Voxel>, Voxel>;
using AugSubShape = AugmentedShape<RemovableShape<Voxel>, Voxel>;
using AugEdiShape = AugmentedShape<EditableShape<Voxel>, Voxel>;

template <class Shape, class Restr, class Voxel>
void verifyH2AdditiveOneVoxelCandidates(std::string name);
template <class Shape, class Restr, class Voxel>
void verifyH2AdditiveDirectVoidFillingCandidates(std::string name);
template <class Shape, class Restr, class ClRestr, class Voxel>
void verifyH2SubstractivePeristenceCandidates(std::string name);
template <class Shape, class Restr, class Voxel>
void verifyH2SubstractiveOneVoxelCandidates(std::string name);

TEST(H2GeneratorTest, AdditiveDirectVoidFilling) {
    verifyH2AdditiveDirectVoidFillingCandidates<
            AugAddShape,
            RestrictedShapeCopy<AugAddShape, Voxel>,
            Voxel>("AddableShape");
    verifyH2AdditiveDirectVoidFillingCandidates<
            AugSubShape,
            RestrictedShapeCopy<AugSubShape, Voxel>,
            Voxel>("RemovableShape");
    verifyH2AdditiveDirectVoidFillingCandidates<
            AugEdiShape,
            RestrictedShapeCopy<AugEdiShape, Voxel>,
            Voxel>("EditableShape");
}

template <class Shape, class Restr, class Voxel>
void verifyH2AdditiveDirectVoidFillingCandidates(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildExample1Extended<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);
    AdditiveH2CandidateGenerator<Shape, Voxel> gen(shape);
    const typename AdditiveH2CandidateGenerator<Shape, Voxel>::candidate_container
            &candidates = gen.generateVoidFillings();

    ASSERT_EQ(1, candidates.size()) << name;
    ASSERT_EQ(8, candidates.begin()->size()) << name;
}

TEST(H2GeneratorTest, SubstractivePeristenceCandidates) {
    verifyH2SubstractivePeristenceCandidates<
            RemovableShape<Voxel>,
            InvertedRestrictedShapeCopy<RemovableShape<Voxel>, Voxel>,
            ClosedInvertedRestrictedShapeCopy<RemovableShape<Voxel>, Voxel>,
            Voxel>("RemovableShape");
    verifyH2SubstractivePeristenceCandidates<
            EditableShape<Voxel>,
            InvertedRestrictedShapeCopy<EditableShape<Voxel>, Voxel>,
            ClosedInvertedRestrictedShapeCopy<EditableShape<Voxel>, Voxel>,
            Voxel>("EditableShape");
}

template <class Shape, class Restr, class ClRestr, class Voxel>
void verifyH2SubstractivePeristenceCandidates(std::string name){
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildExample1Extended<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);
    SubstractiveH2CandidateGenerator<Shape, Voxel> gen(shape);
    const typename SubstractiveH2CandidateGenerator<Shape, Voxel>::candidate_container
            &candidates = gen.template generatePeristenceCandidates<Restr>(10);

    ASSERT_NE(0, candidates.size()) << name;
    ASSERT_NE(0, candidates.begin()->size()) << name;

    const typename SubstractiveH2CandidateGenerator<Shape, Voxel>::candidate_container
            &candidates2 = gen.template generatePeristenceCandidatesWithH0Guarantee<ClRestr>(20);

    ASSERT_NE(0, candidates2.size()) << name;
    ASSERT_NE(0, candidates2.begin()->size()) << name;
}




