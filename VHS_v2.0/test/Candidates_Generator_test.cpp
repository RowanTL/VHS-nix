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
 * @file Candidates_Generator_test.cpp
 * Test file for the H0_Candidates_Generator classes.
 **/

#include <gtest/gtest.h>
#include <vector>
#include <unordered_set>

#include "simplifier/AdditiveCandidateGenerator.h"
#include "simplifier/SubstractiveCandidateGenerator.h"
#include "shape/AugmentedShape.h"
#include "shape/EditableShape.h"
#include "shape/Voxel.h"
#include "shape/RestrictedShapeCopy.h"
#include "shape/InvertedRestrictedShapeCopy.h"
#include "shape/ClosedRestrictedShapeCopy.h"
#include "shape/ClosedInvertedRestrictedShapeCopy.h"

#include "test_examples.h"

using shape_simplification::simplifier::AdditiveCandidateGenerator;
using shape_simplification::simplifier::SubstractiveCandidateGenerator;
using shape_simplification::shape::AugmentedShape;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::Voxel;
using shape_simplification::shape::RestrictedShapeCopy;
using shape_simplification::shape::InvertedRestrictedShapeCopy;
using shape_simplification::shape::ClosedRestrictedShapeCopy;
using shape_simplification::shape::ClosedInvertedRestrictedShapeCopy;

using Shape = EditableShape<Voxel>;
using AugShape = AugmentedShape<Shape,Voxel>;
using RestrShape = RestrictedShapeCopy<Shape,Voxel>;
using InvRestrShape = InvertedRestrictedShapeCopy<Shape,Voxel>;
using ClRestrShape = ClosedRestrictedShapeCopy<Shape,Voxel>;
using ClInvRestrShape = ClosedInvertedRestrictedShapeCopy<Shape,Voxel>;

template <class Generator, class RestShape>
void verifyPersistenceCandidate(std::string name)
{
    using candidate_container = const typename Generator::candidate_container;

    std::vector<Voxel> voxels;
    buildExample1Extended<Voxel>(voxels);

    AugShape shape(voxels);
    Generator gen(shape);

    candidate_container &cand =
            gen.template generatePeristenceCandidates<RestShape>(20, true);

    if constexpr (Generator::isAdditive()){
        ASSERT_EQ(2, cand.size()) << name;
    } else {
        ASSERT_EQ(9, cand.size()) << name;
    }

    std::unordered_set<Point> lowerPoints;
    lowerPoints.insert({0,0,0});
    lowerPoints.insert({10,10,10});
    gen.template generatePeristenceCandidates<RestShape>(9, lowerPoints, true);

    ASSERT_EQ(2, cand.size()) << name;

    Point lowerPoint{0,0,0};
    gen.template generatePeristenceCandidates<RestShape>(9, lowerPoint, true);

    if constexpr (Generator::isAdditive()){
        ASSERT_EQ(2, cand.size()) << name;
    } else {
        ASSERT_EQ(1, cand.size()) << name;
    }
}

TEST(CandidateGeneratorTest, PersistenceCandidates) {
    verifyPersistenceCandidate<
            AdditiveCandidateGenerator<AugShape,Voxel>,
            RestrShape>("Addiditive Candidates");
    verifyPersistenceCandidate<
            SubstractiveCandidateGenerator<AugShape,Voxel>,
            InvRestrShape>("Substractive Candidates");
}

template <class Generator, class RestShape>
void verifyPersistenceWGCandidate(std::string name)
{
    using candidate_container = const typename Generator::candidate_container;

    std::vector<Voxel> voxels;
    buildExample1Extended<Voxel>(voxels);

    AugShape shape(voxels);
    Generator gen(shape);

    candidate_container &cand =
            gen.template generatePeristenceCandidatesWithGuarantee<RestShape>(
                20, true);

    if constexpr (Generator::isAdditive()){
        ASSERT_EQ(2, cand.size()) << name;
    } else {
        ASSERT_EQ(9, cand.size()) << name;
    }

    std::unordered_set<Point> lowerPoints;
    lowerPoints.insert({0,0,0});
    lowerPoints.insert({10,10,10});
    gen.template generatePeristenceCandidatesWithGuarantee<RestShape>(
                9, lowerPoints, true);

    ASSERT_EQ(2, cand.size()) << name;

    Point lowerPoint{0,0,0};
    gen.template generatePeristenceCandidatesWithGuarantee<RestShape>(
                9, lowerPoint, true);

    if constexpr (Generator::isAdditive()){
        ASSERT_EQ(2, cand.size()) << name;
    } else {
        ASSERT_EQ(1, cand.size()) << name;
    }
}

TEST(CandidateGeneratorTest, PersistenceWithGuaranteeCandidates) {
    verifyPersistenceWGCandidate<
            AdditiveCandidateGenerator<AugShape,Voxel>,
            ClRestrShape>("Addiditive Candidates");
    verifyPersistenceWGCandidate<
            SubstractiveCandidateGenerator<AugShape,Voxel>,
            ClInvRestrShape>("Substractive Candidates");
}

template <class Generator>
void verifyExactCandidate(std::string name)
{
    using candidate_container = const typename Generator::candidate_container;

    std::vector<Voxel> voxels;
    buildExample1Extended<Voxel>(voxels);

    AugShape shape(voxels);
    Generator gen(shape);

    if constexpr (Generator::isAdditive()){
        const candidate_container &cand = gen.generateVoidFillings();

        ASSERT_EQ(1, cand.size()) << name;
    } else {
        const candidate_container &cand = gen.generateSubstractibleComponents(3);

        ASSERT_EQ(0, cand.size()) << name;

        gen.generateSubstractibleComponents();

        ASSERT_EQ(1, cand.size()) << name;
    }
}

TEST(CandidateGeneratorTest, ExactCandidates) {
    verifyExactCandidate<
            AdditiveCandidateGenerator<AugShape,Voxel>
            >("Addiditive Candidates");
    verifyExactCandidate<
            SubstractiveCandidateGenerator<AugShape,Voxel>
            >("Substractive Candidates");
}

template <class Generator>
void verifyOneVoxelCandidate(std::string name)
{
    using candidate_container = const typename Generator::candidate_container;

    std::vector<Voxel> voxels;
    buildExample1Extended<Voxel>(voxels);

    AugShape shape(voxels);
    Generator gen(shape);

    const candidate_container &cand = gen.generateH0OneVoxelCandidates();

    if constexpr (Generator::isAdditive()){
        ASSERT_EQ(1, cand.size()) << "H0 " << name;
    } else {
        ASSERT_EQ(1, cand.size()) << "H0 " << name;
    }

    gen.generateH2OneVoxelCandidates();

    if constexpr (Generator::isAdditive()){
        ASSERT_EQ(0, cand.size()) << "H2 " << name;
    } else {
        ASSERT_EQ(24, cand.size()) << "H2 " << name;
    }
}

TEST(CandidateGeneratorTest, OneVoxelCandidates) {
    verifyOneVoxelCandidate<
            AdditiveCandidateGenerator<AugShape,Voxel>
            >("Addiditive Candidates");
    verifyOneVoxelCandidate<
            SubstractiveCandidateGenerator<AugShape,Voxel>
            >("Substractive Candidates");
}


