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
 * @file AdditiveCandidateGenerator.h
 * Contains the declaration of the \ref AdditiveCandidateGenerator class.
 **/

#include <iostream>
#include <unordered_set>
#include <list>
#include <queue>

#include "persistence/PersistentHomology.h"
#include "util/Point.h"
#include "shape/AugmentedShape.h"
#include "shape/ShapeOverlay.h"
#include "shape/ShapeUtilities.h"
#include "shape/DualSkeleton.h"
#include "shape/ConnectedComponents.h"

namespace shape_simplification{ namespace simplifier {

using util::Point;
using shape::Level;
using shape::CORE;
using shape::MAIN_SHAPE;
using shape::NEIGHBORHOOD;
using shape::ConnectedComponents;
using shape::ConnectedComponentsWithRemoval;
using shape::ExtendedNeighborsIterator;
using shape::DualSkeleton;
using persistence::PersistentHomology;

template <class AugmentedShape, class Voxel>
/**
 * class AdditiveCandidateGenerator
 * @brief Generates additive candidates.
 * @tparam AugmentedShape Augmented shape class.
 * @tparam Voxel Voxel class used by the augmented shape.
 **/
class AdditiveCandidateGenerator {
public:
    template <Level level>
    using neighbor_iterator =
        typename AugmentedShape::template neighbor_iterator<level>;
    template <Level level>
    using extended_neighbor_iterator =
        typename AugmentedShape::template extended_neighbor_iterator<level>;
    using voxel_container = typename AugmentedShape::voxel_container;
    using candidate_container = std::vector<voxel_container>;
    using candidate_iterator = typename candidate_container::iterator;

    using CC = ConnectedComponents<Voxel>;

    /**
     * @brief Constructor.
     * @param shape Shape.
     **/
    AdditiveCandidateGenerator(AugmentedShape &shape);
    /**
     * @brief Destructor.
     **/
    ~AdditiveCandidateGenerator();

    template <class RestrictedShape>
    /**
     * @brief Generates candidates using persistence.
     * @tparam RestrictedShape RestrictedShape class
     * @param width Box width. The candidates are generated
     * for each box instead of the whole shape.
     * @param silent If true, no text is displayed in the terminal.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidates(
            int width, bool silent = false);
    template <class RestrictedShape>
    /**
     * @brief Generates candidates using persistence in specific boxes.
     * @tparam RestrictedShape RestrictedShape class.
     * @param width Box width.
     * @param lowerPoints left-most bottom coordinates of the boxes to use.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidates(
            int width,
            std::unordered_set<Point>& lowerPoints,
            bool silent = false);
    template <class RestrictedShape>
    /**
     * @brief Generates candidates using persistence in the given box.
     * @tparam RestrictedShape RestrictedShape class.
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
     * @brief Generates H2-safe candidates using persistence.
     * @tparam ClosedRestrictedShape RestrictedShape class with closed borders.
     * @param width Box width. The candidates are generated
     * for each box instead of the whole shape.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidatesWithGuarantee(
            int width, bool silent = false);
    template <class ClosedRestrictedShape>
    /**
     * @brief Generates H2-safe candidates using persistence in specific boxes.
     * @tparam ClosedRestrictedShape RestrictedShape class with closed borders.
     * @param width Box width. The candidates are generated
     * for each box instead of the whole shape.
     * @param lowerPoints left-most bottom coordinates of the boxes to use.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidatesWithGuarantee(
            int width,
            std::unordered_set<Point>& lowerPoints,
            bool silent = false);
    template <class ClosedRestrictedShape>
    /**
     * @brief Generates H2-safe candidates using persistence in the given box.
     * @tparam ClosedRestrictedShape RestrictedShape class with closed borders.
     * @param width Box width. The candidates are generated
     * for each box instead of the whole shape.
     * @param lowerPoint left-most bottom coordinate of the box to use.
     * @param silent If true, no terminal output is produced.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generatePeristenceCandidatesWithGuarantee(
            int width, Point lowerPoint, bool silent = false);

    /**
     * @brief Computes voxel sets filling the voids in the shape.
     * @return Reference to the resulting sets.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generateVoidFillings();
    /**
     * @brief Generates H0-candidates which contains only one voxel.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generateH0OneVoxelCandidates();
    /**
     * @brief Generates H2-candidates which contains only one voxel.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generateH2OneVoxelCandidates();

    /**
     * @brief Tests if the candidate generated are additive.
     * @return True.
     */
    static constexpr bool isAdditive();

private:
    candidate_container _candidates; ///< Resulting candidates. Reseted by each
                                     ///< generating method call.
    AugmentedShape &_shape;          ///< Underlying shape.

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
     * @brief Generates H2-safe candidates using persistence inside the given
     * box.
     * @tparam ClosedRestrictedShape RestrictedShape class with closed borders.
     * @param lowerCorner Lower left corner of the box.
     * @param upperCorner Upper right corner of the box.
     */
    void _generatePeristenceCandidatesInBoxWithGuarantee(
            Point &lowerCorner, Point &upperCorner);
};

#include "AdditiveCandidateGenerator.txx"

}} // namespace shape_simplification::simplifier
