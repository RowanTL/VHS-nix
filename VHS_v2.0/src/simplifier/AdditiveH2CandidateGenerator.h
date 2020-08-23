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
 * @file AdditiveH2CandidateGenerator.h
 * Contains the declaration of the \ref AdditiveH2CandidateGenerator class.
 **/

#include <list>
#include <queue>

#include "simplifier/BaseCandidateGenerator.h"
#include "util/Point.h"
#include "shape/VoxelSkeletonIterators.h"
#include "shape/ConnectedComponents.h"

namespace shape_simplification{ namespace simplifier {

using shape::ConnectedComponents;
using shape::ConnectedComponentsWithRemoval;
using shape::ExtendedNeighborsIterator;
using util::Point;

template <class AugmentedShape, class Voxel>
/**
 * class AdditiveH2CandidateGenerator
 * @brief Generates additive candidates for H2.
 * @tparam AugmentedShape Augmented shape class.
 * @tparam Voxel Voxel class used by the augmented shape.
 **/
class AdditiveH2CandidateGenerator :
        private BaseCandidateGenerator<AugmentedShape,Voxel,2> {
public:
    using Base = BaseCandidateGenerator<AugmentedShape,Voxel,2>;
    using Level = typename Base::Level;
    static const Level CORE = Base::CORE;
    static const Level MAIN_SHAPE = Base::MAIN_SHAPE;
    static const Level NEIGHBORHOOD = Base::NEIGHBORHOOD;
    static const Level OUTSIDE = Base::OUTSIDE;
    template <Level level>
    using neighbor_iterator = typename Base::template neighbor_iterator<level>;
    template <Level level>
    using extended_neighbor_iterator = typename Base::template extended_neighbor_iterator<level>;
    using candidate_container = typename Base::candidate_container;
    using voxel_container = typename Base::voxel_container;

    using CC = ConnectedComponents<Voxel>;

    /**
     * @brief Constructor.
     * @param shape Shape.
     **/
    AdditiveH2CandidateGenerator(AugmentedShape &shape);
    /**
     * @brief Destructor.
     **/
    ~AdditiveH2CandidateGenerator();

    /**
     * @brief Computes voxels sets filling the voids in the shape.
     * @return Reference to the resulting sets.
     * Valid only until the next generating method from this class is called.
     */
    const candidate_container &generateVoidFillings();

    /**
     * @brief Tests if the candidate generated are additive.
     * @return True.
     */
    static constexpr bool isAdditive();

private:
};

#include "AdditiveH2CandidateGenerator.txx"

}} // namespace shape_simplification::simplifier
