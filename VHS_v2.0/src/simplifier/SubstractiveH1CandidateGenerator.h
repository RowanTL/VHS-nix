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

#pragma once

/**
 * @file SubstractiveH1CandidateGenerator.h
 * Contains the declaration of the \ref SubstractiveH1CandidateGenerator class.
 **/

#include <unordered_set>

#include "simplifier/BaseCandidateGenerator.h"
#include "util/Point.h"

namespace shape_simplification{ namespace simplifier {

using util::Point;

template <class AugmentedShape, class Voxel>
/**
 * class SubstractiveH1CandidateGenerator
 * @brief Generates substractive candidates for H1.
 * @tparam AugmentedShape Augmented shape class.
 * @tparam Voxel Voxel class used by the augmented shape.
 **/
class SubstractiveH1CandidateGenerator :
        private BaseCandidateGenerator<AugmentedShape,Voxel,1> {
public:
    using Base = BaseCandidateGenerator<AugmentedShape,Voxel,1>;
    using Level = typename Base::Level;
    static const Level CORE = Base::CORE;
    static const Level MAIN_SHAPE = Base::MAIN_SHAPE;
    static const Level NEIGHBORHOOD = Base::NEIGHBORHOOD;
    static const Level OUTSIDE = Base::OUTSIDE;
    template <Level level>
    using neighbor_iterator = typename Base::template neighbor_iterator<level>;
    using candidate_container = typename Base::candidate_container;
    using voxel_container = typename Base::voxel_container;

    /**
     * @brief Constructor.
     * @param shape Shape.
     **/
    SubstractiveH1CandidateGenerator(AugmentedShape &shape);
    /**
     * @brief Destructor.
     **/
    ~SubstractiveH1CandidateGenerator();

    template <class InvertedRestrictedShape>
    /**
     * @brief Generates candidates using persistence.
     * @tparam InvertedRestrictedShape InvertedRestrictedShape class
     * @param width Box width. The candidates are generated
     * for each box instead of the whole shape.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidates(
            int width, bool silent = false);
    template <class InvertedRestrictedShape>
    /**
     * @brief Generates candidates using persistence in specific boxes.
     * @tparam InvertedRestrictedShape InvertedRestrictedShape class.
     * @param width Box width.
     * @param lowerPoints left-most bottom coordinates of the boxes to use.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidates(
            int width, std::unordered_set<Point>& lowerPoints, bool silent = false);
    template <class InvertedRestrictedShape>
    /**
     * @brief Generates candidates using persistence in the given box.
     * @tparam InvertedRestrictedShape InvertedRestrictedShape class.
     * @param width Box width.
     * @param lowerPoint left-most bottom coordinate of the box to use.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidates(
            int width, Point lowerPoint, bool silent = false);
    template <class ClosedInvertedRestrictedShape>
    /**
     * @brief Generates H0-safe candidates using persistence.
     * @tparam ClosedInvertedRestrictedShape
     * InvertedRestrictedShape class with closed borders.
     * @param width Box width. The candidates are generated
     * for each box instead of the whole shape.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidatesWithH0Guarantee(
            int width, bool silent = false);
    template <class ClosedInvertedRestrictedShape>
    /**
     * @brief Generates H0-safe candidates using persistence in specific boxes.
     * @tparam ClosedInvertedRestrictedShape ClosedInvertedRestrictedShape class.
     * @param width Box width.
     * @param lowerPoints left-most bottom coordinates of the boxes to use.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidatesWithH0Guarantee(
            int width, std::unordered_set<Point>& lowerPoints, bool silent = false);
    template <class ClosedInvertedRestrictedShape>
    /**
     * @brief Generates H0-safe candidates using persistence in the given box.
     * @tparam ClosedInvertedRestrictedShape ClosedInvertedRestrictedShape class.
     * @param width Box width.
     * @param lowerPoint left-most bottom coordinate of the box to use.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidatesWithH0Guarantee(
            int width, Point lowerPoint, bool silent = false);

    /**
     * @brief Tests if the candidate generated are additive.
     * @return False.
     */
    static constexpr bool isAdditive();

private:
};

#include "SubstractiveH1CandidateGenerator.txx"

}} // namespace shape_simplification::simplifier
