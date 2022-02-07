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
 * @file Validator_test.cpp
 * Test file for the Candidate Validator class.
 **/

#include <gtest/gtest.h>

#include "simplifier/CandidateValidator.h"
#include "simplifier/AdditiveCandidateGenerator.h"
#include "simplifier/SubstractiveCandidateGenerator.h"
#include "shape/Voxel.h"
#include "shape/EditableShape.h"
#include "shape/AugmentedShape.h"
#include "shape/RestrictedShapeCopy.h"
#include "shape/InvertedRestrictedShapeCopy.h"
#include "shape/ClosedRestrictedShapeCopy.h"

#include "test_examples.h"

using shape_simplification::simplifier::CandidateValidator;
using shape_simplification::simplifier::AdditiveCandidateGenerator;
using shape_simplification::simplifier::SubstractiveCandidateGenerator;
using shape_simplification::shape::AugmentedShape;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::Voxel;
using shape_simplification::shape::RestrictedShapeCopy;
using shape_simplification::shape::InvertedRestrictedShapeCopy;
using shape_simplification::shape::ClosedRestrictedShapeCopy;

using Shape = EditableShape<Voxel>;
using AugShape = AugmentedShape<Shape,Voxel>;
using RestrShape = RestrictedShapeCopy<Shape,Voxel>;
using InvRestrShape = InvertedRestrictedShapeCopy<Shape,Voxel>;
using ClRestrShape = ClosedRestrictedShapeCopy<Shape,Voxel>;

TEST(ValidatorTest, AdditiveCandidates) {
    using vcc = CandidateValidator<AugShape,Voxel>::valid_candidate_container;

    std::vector<Voxel> voxels;
    buildExample1Extended<Voxel>(voxels);

    AugShape shape(voxels);
    AdditiveCandidateGenerator<AugShape,Voxel> gen(shape);
    CandidateValidator<AugShape,Voxel> val(shape);

    const vcc &res = val.verifyAdditiveCandidatesValidity(
                gen.generatePeristenceCandidates<RestrShape>(20, true));
    ASSERT_EQ(res.size(), 2);
    ASSERT_TRUE(res.find(Voxel(1,4,2)) != res.end());
    ASSERT_TRUE(res.find(Voxel(1,5,4)) != res.end());

    val.verifyAdditiveCandidatesValidity(
                gen.generateH0OneVoxelCandidates());
    ASSERT_EQ(res.size(), 1);
    ASSERT_TRUE(res.find(Voxel(1,5,4)) != res.end());

    val.verifyAdditiveCandidatesValidity(
                gen.generateH2OneVoxelCandidates());
    ASSERT_EQ(res.size(), 0);
}

TEST(ValidatorTest, SubstractiveCandidates) {
    using vcc = CandidateValidator<AugShape,Voxel>::valid_candidate_container;

    std::vector<Voxel> voxels;
    buildExample1Extended<Voxel>(voxels);

    AugShape shape(voxels);
    SubstractiveCandidateGenerator<AugShape,Voxel> gen(shape);
    CandidateValidator<AugShape,Voxel> val(shape);

    const vcc &res = val.verifySubstractiveCandidatesValidity(
                gen.generatePeristenceCandidates<InvRestrShape>(20, true));
    ASSERT_EQ(res.size(), 57);

    val.verifySubstractiveCandidatesValidity(
                gen.generateH0OneVoxelCandidates());
    ASSERT_EQ(res.size(), 1);
    ASSERT_TRUE(res.find(Voxel(2,5,4)) != res.end());

    val.verifySubstractiveCandidatesValidity(
                gen.generateH2OneVoxelCandidates());
    ASSERT_EQ(res.size(), 1);
    ASSERT_TRUE((*res.begin())[0] >= 10);
    ASSERT_TRUE((*res.begin())[1] >= 10);
    ASSERT_TRUE((*res.begin())[2] >= 10);
}

TEST(ValidatorTest, SubstractiveCandidatesWithH2Guarantee) {
    using vcc = CandidateValidator<AugShape,Voxel>::valid_candidate_container;

    std::vector<Voxel> voxels;
    buildExample1Translated<Voxel>(voxels);

    AugShape shape(voxels);
    SubstractiveCandidateGenerator<AugShape,Voxel> gen(shape);
    CandidateValidator<AugShape,Voxel> val(shape);

    const vcc &res = val.verifySubstractiveCandidatesValidityWithH2Guarantee(
                gen.generatePeristenceCandidates<InvRestrShape>(20, true));
    ASSERT_EQ(res.size(), 1);

    val.verifySubstractiveCandidatesValidityWithH2Guarantee(
                gen.generateH0OneVoxelCandidates());
    ASSERT_EQ(res.size(), 1);
    ASSERT_TRUE(res.find(Voxel(2,5,4)) != res.end());

    val.verifySubstractiveCandidatesValidityWithH2Guarantee(
                gen.generateH2OneVoxelCandidates());
    ASSERT_EQ(res.size(), 0);
}

