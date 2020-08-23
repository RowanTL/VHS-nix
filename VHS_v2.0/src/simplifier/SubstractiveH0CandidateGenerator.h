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
 * @file SubstractiveH0CandidateGenerator.h
 * Contains the declaration of the \ref SubstractiveH0CandidateGenerator class.
 **/

#include <list>
#include <queue>

#include "simplifier/BaseCandidateGenerator.h"
#include "util/Point.h"
#include "shape/VoxelSkeletonIterators.h"

namespace shape_simplification{ namespace simplifier {

using persistence::PersistentHomology;
using shape::ExtendedNeighborsIterator;
using util::Point;

template <class AugmentedShape, class Voxel>
/**
 * class SubstractiveH0CandidateGenerator
 * @brief Generates substractive candidates for H0.
 * @tparam AugmentedShape Augmented shape class.
 * @tparam Voxel Voxel class used by the augmented shape.
 **/
class SubstractiveH0CandidateGenerator :
        private BaseCandidateGenerator<AugmentedShape,Voxel,0> {
public:
    using Base = BaseCandidateGenerator<AugmentedShape,Voxel,0>;
    using Level = typename Base::Level;
    static const Level CORE = Base::CORE;
    static const Level MAIN_SHAPE = Base::MAIN_SHAPE;
    static const Level NEIGHBORHOOD = Base::NEIGHBORHOOD;
    static const Level OUTSIDE = Base::OUTSIDE;
    template <Level level>
    using neighbor_iterator =
        typename Base::template neighbor_iterator<level>;
    template <Level level>
    using extended_neighbor_iterator =
        typename Base::template extended_neighbor_iterator<level>;
    using candidate_container = typename Base::candidate_container;
    using voxel_container = typename Base::voxel_container;

    /**
     * @brief Constructor.
     * @param shape Shape.
     **/
    SubstractiveH0CandidateGenerator(AugmentedShape &shape);
    /**
     * @brief Destructor.
     **/
    ~SubstractiveH0CandidateGenerator();

    /**
     * @brief Retrieves connected components with no core elements
     * and whose size is lower than given maximal size.
     * @param maxSize Maximal size of the connected components.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generateSubstractibleComponents(int maxSize);
    /**
     * @brief Retrieves connected components with no core elements.
     * @return Reference to the resulting candidates.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generateSubstractibleComponents();

    /**
     * @brief Tests if the candidate generated are additive.
     * @return False.
     */
    static constexpr bool isAdditive();

private:
};

#include "SubstractiveH0CandidateGenerator.txx"

}} // namespace shape_simplification::simplifier
