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
 * @file AdditiveH0CandidateGenerator.txx
 * Contains the implementation of the methods in the
 * \ref AdditiveH0CandidateGenerator class.
 **/

#include "AdditiveH0CandidateGenerator.h"

using namespace shape_simplification::simplifier;

template <class AugmentedShape, class Voxel>
AdditiveH0CandidateGenerator<AugmentedShape, Voxel>::
AdditiveH0CandidateGenerator(AugmentedShape &shape)
    : BaseCandidateGenerator<AugmentedShape,Voxel,0>(shape) {}

template <class AugmentedShape, class Voxel>
AdditiveH0CandidateGenerator<AugmentedShape, Voxel>::
~AdditiveH0CandidateGenerator() {}

template <class AugmentedShape, class Voxel>
template <class RestrictedShape>
const typename AdditiveH0CandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
AdditiveH0CandidateGenerator<AugmentedShape, Voxel>::
generatePeristenceCandidates(int width, bool silent) {
    return Base::template
            generatePeristenceCandidates<RestrictedShape>(width, silent);
}

template <class AugmentedShape, class Voxel>
template <class RestrictedShape>
const typename AdditiveH0CandidateGenerator<AugmentedShape,Voxel>::
candidate_container &
AdditiveH0CandidateGenerator<AugmentedShape,Voxel>::
generatePeristenceCandidates(int width,
                             std::unordered_set<Point>& lowerPoints,
                             bool silent)
{
    return Base::template
            generatePeristenceCandidates<RestrictedShape>(width, lowerPoints, silent);
}

template <class AugmentedShape, class Voxel>
template <class RestrictedShape>
const typename AdditiveH0CandidateGenerator<AugmentedShape,Voxel>::
candidate_container &
AdditiveH0CandidateGenerator<AugmentedShape,Voxel>::
generatePeristenceCandidates(int width,
                             Point lowerPoint,
                             bool silent)
{
    return Base::template
            generatePeristenceCandidates<RestrictedShape>(width, lowerPoint, silent);
}

template <class AugmentedShape, class Voxel>
template <class ClosedRestrictedShape>
const typename AdditiveH0CandidateGenerator<AugmentedShape,Voxel>::
candidate_container &
AdditiveH0CandidateGenerator<AugmentedShape, Voxel>::
generatePeristenceCandidatesWithH2Guarantee(int width, bool silent) {
    return Base::template
            generatePeristenceCandidatesWithGuarantee<ClosedRestrictedShape>(
                width, silent);
}

template <class AugmentedShape, class Voxel>
template <class ClosedRestrictedShape>
const typename AdditiveH0CandidateGenerator<AugmentedShape, Voxel>::
candidate_container &
AdditiveH0CandidateGenerator<AugmentedShape, Voxel>::
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
const typename AdditiveH0CandidateGenerator<AugmentedShape, Voxel>::
candidate_container &
AdditiveH0CandidateGenerator<AugmentedShape, Voxel>::
generatePeristenceCandidatesWithH2Guarantee(int width,
                                            Point lowerPoint,
                                            bool silent)
{
    return Base::template
            generatePeristenceCandidatesWithGuarantee<ClosedRestrictedShape>(
                width, lowerPoint, silent);
}

template <class AugmentedShape, class Voxel>
const typename AdditiveH0CandidateGenerator<AugmentedShape, Voxel>::
candidate_container &
AdditiveH0CandidateGenerator<AugmentedShape, Voxel>::
generateOneVoxelCandidates()
{
    Base::_candidates.clear();

    for (Voxel v : Base::_shape.getCollar()) {
        if (Base::_shape.template contains<NEIGHBORHOOD>(v)) {
            voxel_container rep;
            for (neighbor_iterator<MAIN_SHAPE> it(v, Base::_shape); !it.isEnd();
                 it.next()) {
                rep.insert(Base::_shape.getComponentRepresentative(it.getCurrentValue()));
            }
            if (rep.size() > 1) {
                voxel_container candidate;
                candidate.insert(v);
                Base::_candidates.push_back(candidate);
            }
        }
    }

    return Base::_candidates;
}

template <class AugmentedShape, class Voxel>
inline constexpr bool
AdditiveH0CandidateGenerator<AugmentedShape,Voxel>::isAdditive(){
    return true;
}

