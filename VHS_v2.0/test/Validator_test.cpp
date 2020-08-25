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
 * @file Validator_test.cpp
 * Test file for the Candidate Validator class.
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
#include "shape/Voxel.h"
#include "simplifier/AdditiveH0CandidateGenerator.h"
#include "simplifier/AdditiveH1CandidateGenerator.h"
#include "simplifier/AdditiveH2CandidateGenerator.h"
#include "simplifier/SubstractiveH0CandidateGenerator.h"
#include "simplifier/SubstractiveH1CandidateGenerator.h"
#include "simplifier/SubstractiveH2CandidateGenerator.h"
#include "simplifier/CandidateValidator.h"

#include "test_examples.h"

using shape_simplification::shape::AddableShape;
using shape_simplification::shape::RemovableShape;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::AugmentedShape;
using shape_simplification::shape::RestrictedShapeCopy;
using shape_simplification::shape::InvertedRestrictedShapeCopy;
using shape_simplification::shape::Voxel;
using shape_simplification::simplifier::AdditiveH0CandidateGenerator;
using shape_simplification::simplifier::AdditiveH1CandidateGenerator;
using shape_simplification::simplifier::AdditiveH2CandidateGenerator;
using shape_simplification::simplifier::SubstractiveH0CandidateGenerator;
using shape_simplification::simplifier::SubstractiveH1CandidateGenerator;
using shape_simplification::simplifier::SubstractiveH2CandidateGenerator;
using shape_simplification::simplifier::CandidateValidator;

using AugAddShape = AugmentedShape<AddableShape<Voxel>, Voxel>;
using AugSubShape = AugmentedShape<RemovableShape<Voxel>, Voxel>;
using AugEdiShape = AugmentedShape<EditableShape<Voxel>, Voxel>;

using RestrAddShape = RestrictedShapeCopy<AugAddShape, Voxel>;
using RestrSubShape = RestrictedShapeCopy<AugSubShape, Voxel>;
using RestrEdiShape = RestrictedShapeCopy<AugEdiShape, Voxel>;
using InvRestrAddShape = InvertedRestrictedShapeCopy<AugAddShape, Voxel>;
using InvRestrSubShape = InvertedRestrictedShapeCopy<AugSubShape, Voxel>;
using InvRestrEdiShape = InvertedRestrictedShapeCopy<AugEdiShape, Voxel>;

template <class AugShape, class RestrShape, class Voxel>
void verifyAdditiveCandidates(std::string name);
template <class AugShape, class RestrShape, class Voxel>
void verifySubstractiveCandidates(std::string name);

TEST(ValidatorTest, AdditiveCandidates) {
    verifyAdditiveCandidates<
            AugAddShape,
            RestrictedShapeCopy<AugAddShape, Voxel>,
            Voxel>("AddableShape");
    verifyAdditiveCandidates<
            AugSubShape,
            RestrictedShapeCopy<AugSubShape, Voxel>,
            Voxel>("RemovableShape");
    verifyAdditiveCandidates<
            AugEdiShape,
            RestrictedShapeCopy<AugEdiShape, Voxel>,
            Voxel>("EditableShape");
}

template <class AugShape, class RestrShape, class Voxel>
void verifyAdditiveCandidates(std::string name){
    using valid_candidate_container =
    typename CandidateValidator<AugShape, Voxel>::valid_candidate_container;

    std::vector<Voxel> voxels;
    std::vector<typename AugShape::Level> minLevels;

    buildExample1Extended<AugShape,Voxel>(voxels, minLevels);

    AugShape shape(voxels, minLevels);
    AdditiveH0CandidateGenerator<AugShape, Voxel> gen0(shape);
    AdditiveH1CandidateGenerator<AugShape, Voxel> gen1(shape);
    AdditiveH2CandidateGenerator<AugShape, Voxel> gen2(shape);
    CandidateValidator<AugShape, Voxel> val(shape);

    const valid_candidate_container &cand00 = val.verifyAdditiveCandidatesValidity(
                gen0.template generatePeristenceCandidates<RestrShape>(10));
    ASSERT_EQ(1, cand00.size()) << name;
    ASSERT_EQ(Voxel(1, 5, 4), *(cand00.begin())) << name;

    const valid_candidate_container &cand01 =
            val.verifyAdditiveCandidatesValidity(gen0.generateOneVoxelCandidates());
    ASSERT_EQ(1, cand01.size()) << name;
    ASSERT_EQ(Voxel(1, 5, 4), *(cand01.begin())) << name;

    const valid_candidate_container &cand10 =
            val.verifyAdditiveCandidatesValidity(
                gen1.template generatePeristenceCandidates<RestrShape>(10));
    ASSERT_EQ(1, cand10.size()) << name;
    ASSERT_EQ(Voxel(1, 4, 2), *(cand10.begin())) << name;
}

TEST(ValidatorTest, SubstractiveCandidates) {
    verifySubstractiveCandidates<
            AugSubShape,
            InvertedRestrictedShapeCopy<RemovableShape<Voxel>, Voxel>,
            Voxel>("RemovableShape");
    verifySubstractiveCandidates<
            AugEdiShape,
            InvertedRestrictedShapeCopy<EditableShape<Voxel>, Voxel>,
            Voxel>("EditableShape");
}

template <class AugShape, class InvRestrShape, class Voxel>
void verifySubstractiveCandidates(std::string name){
    using valid_candidate_container =
    typename CandidateValidator<AugShape, Voxel>::valid_candidate_container;

    std::vector<Voxel> voxels;
    std::vector<typename AugShape::Level> minLevels;

    buildExample1Extended<AugShape,Voxel>(voxels, minLevels);

    AugShape shape(voxels, minLevels);
    SubstractiveH0CandidateGenerator<AugShape, Voxel> gen0(shape);
    SubstractiveH1CandidateGenerator<AugShape, Voxel> gen1(shape);
    SubstractiveH2CandidateGenerator<AugShape, Voxel> gen2(shape);
    CandidateValidator<AugShape, Voxel> val(shape);

    const valid_candidate_container &cand00 = val.verifySubstractiveCandidatesValidity(
                gen0.generateSubstractibleComponents(-1));

    ASSERT_EQ(0, cand00.size()) << name;

    const valid_candidate_container &cand10 =
            val.verifySubstractiveCandidatesValidity(
                gen1.template generatePeristenceCandidates<InvRestrShape>(10));

    ASSERT_NE(0, cand10.size()) << name;

    const valid_candidate_container &cand20 =
            val.verifySubstractiveCandidatesValidity(
                gen2.template generatePeristenceCandidates<InvRestrShape>(10));
    ASSERT_EQ(0, cand20.size()) << name;
}
