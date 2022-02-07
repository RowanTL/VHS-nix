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

#pragma once

/**
 * @file CandidateValidator.h
 * Contains the declaration of the \ref CandidateValidator class.
 **/

#include <functional>
#include <algorithm>
#include <vector>
#include <unordered_set>

#include "shape/ShapeOverlay.h"
#include "shape/ShapeUtilities.h"

namespace shape_simplification{ namespace simplifier {

using shape::ExtendedNeighborsIterator;
using shape::ShapeOverlay;
using shape::Level;
using shape::CORE;
using shape::MAIN_SHAPE;
using shape::NEIGHBORHOOD;

template <class AugmentedShape, class Voxel>
/**
 * class CandidateValidator
 * @brief Sorts out a list of voxels from lists of candidates, which are
 * guaranteed to simplify the given shape.
 **/
class CandidateValidator {
public:
    template <Level level>
    using neighbor_iterator =
        typename AugmentedShape::template neighbor_iterator<level>;
    template <Level level>
    using extended_neighbor_iterator =
        typename AugmentedShape::template extended_neighbor_iterator<level>;
    using voxel_container = typename AugmentedShape::voxel_container;
    // Has to correspond to the container of the Generator classes:
    using candidate_container = std::vector<voxel_container>;
    using candidate_iterator = typename candidate_container::iterator;
    using valid_candidate_container = std::unordered_set<Voxel>;
    using valid_candidate_iterator =
        typename valid_candidate_container::iterator;
    using collar_container = std::unordered_set<Voxel>;
    using collar_iterator = typename std::unordered_set<Voxel>::iterator;
    using rem_candidate_container = std::unordered_set<voxel_container>;

    /**
     * @brief Constructor.
     * @param shape Shape to simplify.
     */
    CandidateValidator(AugmentedShape &shape);
    /**
     * @brief Destructor.
     */
    ~CandidateValidator();

    /**
     * @brief Sorts out a list of voxels which are guaranteed to
     * simplify the stored shape from vectors of additive candidates.
     * @param candidates Vector of candidates.
     * @return A reference to the resulting voxels.
     * @pre Assumes that voids are already filled when possible.
     */
    const valid_candidate_container &
    verifyAdditiveCandidatesValidity(const candidate_container &candidates);
    /**
     * @brief Sorts out a list of voxels which are guaranteed to
     * simplify the stored shape from vectors of H2-safe additive candidates.
     * @param candidates Vectors of candidates which does not affect H2.
     * @return A reference to the resulting voxels.
     * @pre Assumes that voids are already filled when possible.
     */
    const valid_candidate_container &
    verifyAdditiveCandidatesValidityWithH2Guarantee(
            const candidate_container &candidates);
    /**
     * @brief Sorts out a list of voxels which are guaranteed to
     * simplify the stored shape from vectors of substractive candidates.
     * @param candidates Vectors of candidates.
     * @return A reference to the resulting voxels.
     * @pre Assumes that non-core components were removed.
     */
    const valid_candidate_container &
    verifySubstractiveCandidatesValidity(
            const candidate_container &candidates);
    /**
     * @brief Sorts out a list of voxels which are guaranteed to
     * simplify the stored shape from vectors of H2-safe substractive
     * candidates.
     * @param candidates Vectors of candidates which does not affect H2.
     * @return A reference to the resulting voxels.
     * @pre Assumes that non-core components were removed.
     */
    const valid_candidate_container &
    verifySubstractiveCandidatesValidityWithH2Guarantee(
            const candidate_container &candidates);
    /**
     * @brief Sorts out a list of voxels which are guaranteed to
     * simplify the stored shape from vectors of H0 and H2-safe
     * substractive candidates.
     * @param candidates Vectors of candidates which does not affect
     * H0 and H2.
     * @return A reference to the resulting voxels.
     * @pre Assumes that non-core components were removed.
     */
    const valid_candidate_container &
    verifySubstractiveCandidatesValidityWithH0H2Guarantee(
            const candidate_container &candidates);
    /**
     * @brief Sorts out a list of voxels which are guaranteed to
     * simplify the stored shape from vectors of H0-safe substractive
     * candidates.
     * @param candidates Vectors of candidates which does not affect H0.
     * @return A reference to the resulting voxels.
     * @pre Assumes that non-core components were removed.
     */
    const valid_candidate_container &
    verifySubstractiveCandidatesValidityWithH0Guarantee(
            const candidate_container &candidates);

private:
    valid_candidate_container _validCandidates;     ///< Resulting voxels. Reseted by
                                                    ///< each verifying method call.
    AugmentedShape &_shape;                         ///< Underlying shape.
    ShapeOverlay<AugmentedShape, Voxel> _overlay;   ///< Shape overlay to tests candidates.
    collar_container _collar;                       ///< Collar of the candidates.

    /**
     * @brief Sorts out a list of voxels which are guaranteed to
     * simplify the stored shape.
     * @param candidates Vectors of candidates.
     * @param chooseValidCandidates Validation method.
     * @return A reference to the resulting voxels.
     */
    const valid_candidate_container &
    _verifyCandidatesValidity(
            const candidate_container &candidates,
            std::function<unsigned int(
                std::vector<voxel_container>&,
                rem_candidate_container&
                )> chooseValidCandidates);
    /**
     * @brief Retrieves a group of non overlapping candidates
     * from the complete list of candidates.
     * @param candidates Complete list of candidates.
     * @param currentGroup For the resulting group.
     * @param remainingCandidates Will become a copy of @p candidates.
     */
    void _retrieveFirstGroupOfNonOverlappingCandidates(
            const candidate_container& candidates,
            std::vector<voxel_container>& currentGroup,
            rem_candidate_container& remainingCandidates);
    /**
     * @brief Chooses valid candidates from the given group of
     * additive candidates with a first-come-first-served strategy.
     * @param currentGroup Current group of non overlapping candidates.
     * @param remainingCandidates Non processed candidates.
     * @return A new maximal for the current group size.
     */
    unsigned int _chooseFirstComeFirstServedAdditiveValidCandidates(
            std::vector<voxel_container>& currentGroup,
            rem_candidate_container& remainingCandidates);
    /**
     * @brief Chooses valid candidates from the given group of
     * H2-safe additive candidates with a first-come-first-served strategy.
     * @param currentGroup Current group of non overlapping candidates.
     * @param remainingCandidates Non processed candidates.
     * @return A new maximal for the current group size.
     */
    unsigned int _chooseFirstComeFirstServedAdditiveValidCandidatesWithH2Guarantee(
            std::vector<voxel_container>& currentGroup,
            rem_candidate_container& remainingCandidates);

    /**
     * @brief Chooses valid candidates from the given group of
     * substractive candidates with a first-come-first-served strategy.
     * @param currentGroup Current group of non overlapping candidates.
     * @param remainingCandidates Non processed candidates.
     * @return A new maximal for the current group size.
     */
    unsigned int _chooseFirstComeFirstServedSubstractiveValidCandidates(
            std::vector<voxel_container>& currentGroup,
            rem_candidate_container& remainingCandidates);
    /**
     * @brief Chooses valid candidates from the given group of
     * H2-safe substractive candidates with a first-come-first-served strategy.
     * @param currentGroup Current group of non overlapping candidates.
     * @param remainingCandidates Non processed candidates.
     * @return A new maximal for the current group size.
     */
    unsigned int _chooseFirstComeFirstServedSubstractiveValidCandidatesWithH2Guarantee(
            std::vector<voxel_container>& currentGroup,
            rem_candidate_container& remainingCandidates);
    /**
     * @brief Chooses valid candidates from the given group of
     * H0 and h2-safe substractive candidates with a first-come-first-served
     * strategy.
     * @param currentGroup Current group of non overlapping candidates.
     * @param remainingCandidates Non processed candidates.
     * @return A new maximal for the current group size.
     */
    unsigned int _chooseFirstComeFirstServedSubstractiveValidCandidatesWithH0H2Guarantee(
            std::vector<voxel_container>& currentGroup,
            rem_candidate_container& remainingCandidates);
    /**
     * @brief Chooses valid candidates from the given group of
     * H0-safe substractive candidates with a first-come-first-served strategy.
     * @param currentGroup Current group of non overlapping candidates.
     * @param remainingCandidates Non processed candidates.
     * @return A new maximal for the current group size.
     */
    unsigned int _chooseFirstComeFirstServedSubstractiveValidCandidatesWithH0Guarantee(
            std::vector<voxel_container>& currentGroup,
            rem_candidate_container& remainingCandidates);

    /**
     * @brief Retrieves a group of non overlapping candidates
     * from current group of candidates.
     * @param currentGroup For the resulting group.
     * @param currentMaxGroupSize Maximal size the new group can have.
     * @param remainingCandidates Non processed candidates.
     */
    void _retrieveNextGroupOfNonOverlappingCandidates(
            std::vector<voxel_container>& currentGroup,
            unsigned int currentMaxGroupSize,
            rem_candidate_container& remainingCandidates);
};

#include "CandidateValidator.txx"

}} // namespace shape_simplification::simplifier
