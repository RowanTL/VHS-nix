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
 * @file BaseCandidateGenerator.h
 * Contains the declaration of the \ref BaseCandidateGenerator class.
 **/

#include <iostream>
#include <vector>
#include <unordered_set>

#include "persistence/PersistentHomology.h"
#include "shape/DualSkeleton.h"
#include "shape/AugmentedShape.h"
#include "shape/ShapeOverlay.h"
#include "util/Point.h"

namespace shape_simplification{ namespace simplifier {

using persistence::PersistentHomology;
using shape::DualSkeleton;
using shape::ShapeOverlay;
using util::Point;

template <class AugmentedShape, class Voxel, int dim>
/**
 * class BaseCandidateGenerator
 * @brief Generates additive candidates for H0.
 * @tparam AugmentedShape Augmented shape class.
 * @tparam Voxel Voxel class used by the augmented shape.
 * @tparam dim Dimension of the generators.
 **/
class BaseCandidateGenerator {
public:
    using Level = typename AugmentedShape::Level;
    static const Level CORE = AugmentedShape::CORE;
    static const Level MAIN_SHAPE = AugmentedShape::MAIN_SHAPE;
    static const Level NEIGHBORHOOD = AugmentedShape::NEIGHBORHOOD;
    static const Level OUTSIDE = AugmentedShape::OUTSIDE;
    template <Level level>
    using neighbor_iterator =
        typename AugmentedShape::template neighbor_iterator<level>;
    template <Level level>
    using extended_neighbor_iterator =
        typename AugmentedShape::template extended_neighbor_iterator<level>;
    using voxel_container = typename AugmentedShape::voxel_container;
    using candidate_container = std::vector<voxel_container>;
    using candidate_iterator = typename candidate_container::iterator;

    template <class RestrictedShape>
    /**
     * @brief Generates candidates using persistence.
     * @tparam RestrictedShape If additive: RestrictedShape class;
     * if substractive: InvertedRestrictedShape class.
     * @param width Box width. The candidates are generated
     * for each box instead of the whole shape.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidates(
            int width, bool silent = false);
    template <class RestrictedShape>
    /**
     * @brief Generates candidates using persistence in specific boxes.
     * @tparam RestrictedShape If additive: RestrictedShape class;
     * if substractive: InvertedRestrictedShape class.
     * @param width Box width.
     * @param lowerPoints left-most bottom coordinates of the boxes to use.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidates(
            int width, std::unordered_set<Point>& lowerPoints, bool silent = false);
    template <class RestrictedShape>
    /**
     * @brief Generates candidates using persistence in the given box.
     * @tparam RestrictedShape If additive: RestrictedShape class;
     * if substractive: InvertedRestrictedShape class.
     * @param width Box width.
     * @param lowerPoint left-most bottom coordinate of the box to use.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidates(
            int width, Point lowerPoint, bool silent = false);
    template <class ClosedRestrictedShape>
    /**
     * @brief If additive: generates H2-safe candidates using persistence;
     * if substractive: generates H0-safe candidates using persistence.
     * @tparam ClosedRestrictedShape If additive: RestrictedShape class
     * with closed borders; if substractive: InvertedRestrictedShape class
     * with closed borders.
     * @param width Box width. The candidates are generated
     * for each box instead of the whole shape.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidatesWithGuarantee(
            int width, bool silent = false);
    template <class ClosedRestrictedShape>
    /**
     * @brief If additive: generates H2-safe candidates using persistence;
     * if substractive: generates H0-safe candidates using persistence
     * in specific boxes.
     * @tparam ClosedRestrictedShape If additive: RestrictedShape class
     * with closed borders; if substractive: InvertedRestrictedShape class
     * with closed borders.
     * @param width Box width.
     * @param lowerPoints left-most bottom coordinates of the boxes to use.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidatesWithGuarantee(
            int width, std::unordered_set<Point>& lowerPoints, bool silent = false);
    template <class ClosedRestrictedShape>
    /**
     * @brief If additive: generates H2-safe candidates using persistence;
     * if substractive: generates H0-safe candidates using persistence
     * in the given box.
     * @tparam ClosedRestrictedShape If additive: RestrictedShape class
     * with closed borders; if substractive: InvertedRestrictedShape class
     * with closed borders.
     * @param width Box width.
     * @param lowerPoint left-most bottom coordinate of the box to use.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidatesWithGuarantee(
            int width, Point lowerPoint, bool silent = false);

    /**
     * @brief Tests if the candidate generated are additive.
     * @return True.
     */
    static constexpr bool isAdditive();

protected:
    /**
     * @brief Constructor.
     * @param shape Shape.
     **/
    BaseCandidateGenerator(AugmentedShape &shape);
    /**
     * @brief Destructor.
     **/
    ~BaseCandidateGenerator();

    candidate_container _candidates;    ///< Resulting candidates. Reseted by each
                                        ///< generating method call.
    AugmentedShape &_shape;             ///< Underlying shape.

private:
    /**
     * @brief Generates peristence candidates.
     * @param width Box width.
     * @param generateCandidates Generation method.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     */
    const candidate_container& _generatePeristenceCandidates(
            int width,
            std::function<void(Point&,Point&)> generateCandidates,
            bool silent = false);
    /**
     * @brief Generates peristence candidates in specified boxes.
     * @param width Box width.
     * @param lowerPoints left-most bottom coordinates of the boxes to use.
     * @param generateCandidates Generation method.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     */
    const candidate_container& _generatePeristenceCandidates(
            int width,
            std::unordered_set<Point>& lowerPoints,
            std::function<void(Point&,Point&)> generateCandidates,
            bool silent = false);
    /**
     * @brief Generates peristence candidates in given box.
     * @param width Box width.
     * @param lower left-most bottom coordinate of the box to use.
     * @param generateCandidates Generation method.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     */
    const candidate_container& _generatePeristenceCandidates(
            int width,
            Point& lower,
            std::function<void(Point&,Point&)> generateCandidates,
            bool silent = false);

    template <class RestrictedShape>
    /**
     * @brief Generates candidates using persistence inside the given box.
     * @tparam RestrictedShape RestrictedShape class.
     * @param lowerCorner Lower left corner of the box.
     * @param upperCorner Upper right corner of the box.
     */
    void _generatePeristenceCandidatesInBox(Point &lowerCorner,
                                            Point &upperCorner);

    template <class ClosedRestrictedShape>
    /**
     * @brief Generates guaranteeded candidates using persistence inside the given box.
     * @tparam ClosedRestrictedShape RestrictedShape class with closed borders.
     * @param lowerCorner Lower left corner of the box.
     * @param upperCorner Upper right corner of the box.
     */
    void _generatePeristenceCandidatesInBoxWithGuarantee(
            Point &lowerCorner, Point &upperCorner);
};

#include "BaseCandidateGenerator.txx"

}} // namespace shape_simplification::simplifier
