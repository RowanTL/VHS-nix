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
 * @file AdditiveH1CandidateGenerator.txx
 * Contains the implementation of the methods in the \ref
 *AdditiveH1CandidateGenerator class.
 **/

#include "AdditiveH1CandidateGenerator.h"

using namespace shape_simplification::simplifier;

template <class AugmentedShape, class Voxel>
AdditiveH1CandidateGenerator<AugmentedShape, Voxel>::
AdditiveH1CandidateGenerator(AugmentedShape &shape)
    : BaseCandidateGenerator<AugmentedShape,Voxel,1>(shape) {}

template <class AugmentedShape, class Voxel>
AdditiveH1CandidateGenerator<AugmentedShape, Voxel>::
~AdditiveH1CandidateGenerator() {}

template <class AugmentedShape, class Voxel>
template <class RestrictedShape>
const typename AdditiveH1CandidateGenerator<AugmentedShape, Voxel>::
candidate_container &
AdditiveH1CandidateGenerator<AugmentedShape, Voxel>::
generatePeristenceCandidates(int width, bool silent) {
    return Base::template
            generatePeristenceCandidates<RestrictedShape>(width, silent);
}

template <class AugmentedShape, class Voxel>
template <class RestrictedShape>
const typename AdditiveH1CandidateGenerator<AugmentedShape, Voxel>::
candidate_container &
AdditiveH1CandidateGenerator<AugmentedShape,Voxel>::
generatePeristenceCandidates(int width,
                             std::unordered_set<Point>& lowerPoints,
                             bool silent)
{
    return Base::template
            generatePeristenceCandidates<RestrictedShape>(width, lowerPoints, silent);
}

template <class AugmentedShape, class Voxel>
template <class RestrictedShape>
const typename AdditiveH1CandidateGenerator<AugmentedShape, Voxel>::
candidate_container &
AdditiveH1CandidateGenerator<AugmentedShape,Voxel>::
generatePeristenceCandidates(int width,
                             Point lowerPoint,
                             bool silent)
{
    return Base::template
            generatePeristenceCandidates<RestrictedShape>(width, lowerPoint, silent);
}

template <class AugmentedShape, class Voxel>
template <class ClosedRestrictedShape>
const typename AdditiveH1CandidateGenerator<AugmentedShape, Voxel>::
candidate_container &
AdditiveH1CandidateGenerator<AugmentedShape, Voxel>::
generatePeristenceCandidatesWithH2Guarantee(int width, bool silent) {
    return Base::template
            generatePeristenceCandidatesWithGuarantee<ClosedRestrictedShape>(
                width, silent);
}

template <class AugmentedShape, class Voxel>
template <class ClosedRestrictedShape>
const typename AdditiveH1CandidateGenerator<AugmentedShape, Voxel>::
candidate_container &
AdditiveH1CandidateGenerator<AugmentedShape, Voxel>::
generatePeristenceCandidatesWithH2Guarantee(
        int width,
        std::unordered_set<Point>& lowerPoints,
        bool silent)
{
    return Base::template
            generatePeristenceCandidatesWithGuarantee<ClosedRestrictedShape>(
                width, lowerPoints, silent);
}

template <class AugmentedShape, class Voxel>
template <class ClosedRestrictedShape>
const typename AdditiveH1CandidateGenerator<AugmentedShape,  Voxel>::
candidate_container &
AdditiveH1CandidateGenerator<AugmentedShape, Voxel>::
generatePeristenceCandidatesWithH2Guarantee(int width,
                                            Point lowerPoint,
                                            bool silent)
{
    return Base::template
            generatePeristenceCandidatesWithGuarantee<ClosedRestrictedShape>(
                width, lowerPoint, silent);
}

template <class AugmentedShape, class Voxel>
inline constexpr bool
AdditiveH1CandidateGenerator<AugmentedShape,Voxel>::isAdditive(){
    return true;
}


