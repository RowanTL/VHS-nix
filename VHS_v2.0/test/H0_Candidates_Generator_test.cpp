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
 * @file H0_Candidates_Generator_test.cpp
 * Test file for the H0_Candidates_Generator classes.
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
#include "shape/ClosedRestrictedShapeCopy.h"
#include "shape/Voxel.h"
#include "simplifier/AdditiveH0CandidateGenerator.h"
#include "simplifier/SubstractiveH0CandidateGenerator.h"

#include "test_examples.h"

using shape_simplification::shape::AddableShape;
using shape_simplification::shape::RemovableShape;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::AugmentedShape;
using shape_simplification::shape::RestrictedShapeCopy;
using shape_simplification::shape::ClosedRestrictedShapeCopy;
using shape_simplification::shape::Voxel;
using shape_simplification::simplifier::AdditiveH0CandidateGenerator;
using shape_simplification::simplifier::SubstractiveH0CandidateGenerator;

using AugAddShape = AugmentedShape<AddableShape<Voxel>, Voxel>;
using AugSubShape = AugmentedShape<RemovableShape<Voxel>, Voxel>;
using AugEdiShape = AugmentedShape<EditableShape<Voxel>, Voxel>;

template <class Shape, class Restr, class ClRestr, class Voxel>
void verifyH0AdditivePeristenceCandidates(std::string name);
template <class Shape, class Voxel>
void verifyH0AdditiveOneVoxelCandidates(std::string name);
template <class Shape, class Voxel>
void verifyH0GetSubstractibleComponents(std::string name);
template <class Shape, class Voxel>
void verifyH0SubstractiveOneVoxelCandidates(std::string name);

TEST(H0GeneratorTest, AdditivePeristenceCandidates) {
    verifyH0AdditivePeristenceCandidates<
            AugAddShape,
            RestrictedShapeCopy<AddableShape<Voxel>, Voxel>,
            ClosedRestrictedShapeCopy<AddableShape<Voxel>, Voxel>,
            Voxel>("AddableShape");
    verifyH0AdditivePeristenceCandidates<
            AugSubShape,
            RestrictedShapeCopy<RemovableShape<Voxel>, Voxel>,
            ClosedRestrictedShapeCopy<RemovableShape<Voxel>, Voxel>,
            Voxel>("RemovableShape");
    verifyH0AdditivePeristenceCandidates<
            AugEdiShape,
            RestrictedShapeCopy<EditableShape<Voxel>, Voxel>,
            ClosedRestrictedShapeCopy<EditableShape<Voxel>, Voxel>,
            Voxel>("EditableShape");
}

template <class Shape, class Restr, class ClRestr, class Voxel>
void verifyH0AdditivePeristenceCandidates(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildExample1<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);
    AdditiveH0CandidateGenerator<Shape, Voxel> gen(shape);
    const typename AdditiveH0CandidateGenerator<Shape, Voxel>::candidate_container
            &candidates = gen.template generatePeristenceCandidates<Restr>(10);

    ASSERT_EQ(1, candidates.size()) << name;
    ASSERT_EQ(1, candidates.begin()->size()) << name;

    const typename AdditiveH0CandidateGenerator<Shape, Voxel>::candidate_container
            &candidates2 = gen.template generatePeristenceCandidatesWithH2Guarantee<ClRestr>(20);

    ASSERT_EQ(1, candidates2.size()) << name;
    ASSERT_EQ(1, candidates2.begin()->size()) << name;
}

TEST(H0GeneratorTest, AdditiveOneVoxelCandidates) {
    verifyH0AdditiveOneVoxelCandidates<
            AugAddShape,
            Voxel>("AddableShape");
    verifyH0AdditiveOneVoxelCandidates<
            AugSubShape,
            Voxel>("RemovableShape");
    verifyH0AdditiveOneVoxelCandidates<
            AugEdiShape,
            Voxel>("EditableShape");
}

template <class Shape, class Voxel>
void verifyH0AdditiveOneVoxelCandidates(std::string name) {
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildExample1<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);
    AdditiveH0CandidateGenerator<Shape, Voxel> gen(shape);
    const typename AdditiveH0CandidateGenerator<Shape, Voxel>::candidate_container
            &candidates = gen.generateOneVoxelCandidates();

    ASSERT_EQ(1, candidates.size()) << name;
    ASSERT_EQ(1, candidates.begin()->size()) << name;
}

TEST(H0GeneratorTest, SubstractibleComponentsCandidates) {
    verifyH0GetSubstractibleComponents<
            AugSubShape,
            Voxel>("RemovableShape");
    verifyH0GetSubstractibleComponents<
            AugEdiShape,
            Voxel>("EditableShape");
}

template <class Shape, class Voxel>
void verifyH0GetSubstractibleComponents(std::string name){
    std::vector<Voxel> voxels;
    std::vector<typename Shape::Level> minLevels;

    buildExample1<Shape,Voxel>(voxels, minLevels);

    Shape shape(voxels, minLevels);
    SubstractiveH0CandidateGenerator<Shape, Voxel> gen(shape);

    const typename SubstractiveH0CandidateGenerator<Shape, Voxel>::candidate_container
            &components = gen.generateSubstractibleComponents(10);

    ASSERT_EQ(1, components.size()) << name;
    ASSERT_EQ(1, components.begin()->size()) << name;

    const typename SubstractiveH0CandidateGenerator<Shape, Voxel>::candidate_container
            &components2 = gen.generateSubstractibleComponents(20);

    ASSERT_EQ(2, components2.size()) << name;
    auto it = components2.begin();
    unsigned long s = it->size();
    ASSERT_TRUE(s == 1 || s == 15) << name;
    ++it;
    ASSERT_TRUE(it->size() == 1 || it->size() == 15) << name;
    ASSERT_NE(s, it->size());
}



