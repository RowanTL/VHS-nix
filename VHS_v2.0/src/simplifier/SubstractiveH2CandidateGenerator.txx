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
 * @file SubstractiveH2CandidateGenerator.txx
 * Contains the implementation of the methods in the
 * \ref SubstractiveH2CandidateGenerator class.
 **/

#include "SubstractiveH2CandidateGenerator.h"

using namespace shape_simplification::simplifier;

template <class AugmentedShape, class Voxel>
SubstractiveH2CandidateGenerator<AugmentedShape, Voxel>::
SubstractiveH2CandidateGenerator(AugmentedShape &shape)
    : BaseCandidateGenerator<AugmentedShape,Voxel,2>(shape) {}

template <class AugmentedShape, class Voxel>
SubstractiveH2CandidateGenerator<AugmentedShape, Voxel>::
~SubstractiveH2CandidateGenerator() {}

template <class AugmentedShape, class Voxel>
template <class InvertedRestrictedShape>
const typename SubstractiveH2CandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveH2CandidateGenerator<AugmentedShape, Voxel>::
generatePeristenceCandidates(int width, bool silent)
{
    return Base::template
            generatePeristenceCandidates<InvertedRestrictedShape>(width, silent);
}

template <class AugmentedShape, class Voxel>
template <class InvertedRestrictedShape>
const typename SubstractiveH2CandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveH2CandidateGenerator<AugmentedShape,Voxel>::
generatePeristenceCandidates(int width,
                             std::unordered_set<Point>& lowerPoints,
                             bool silent)
{
    return Base::template
            generatePeristenceCandidates<InvertedRestrictedShape>(
                width, lowerPoints, silent);
}

template <class AugmentedShape, class Voxel>
template <class InvertedRestrictedShape>
const typename SubstractiveH2CandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveH2CandidateGenerator<AugmentedShape,Voxel>::
generatePeristenceCandidates(int width,
                             Point lowerPoint,
                             bool silent)
{
    return Base::template
            generatePeristenceCandidates<InvertedRestrictedShape>(
                width, lowerPoint, silent);
}

template <class AugmentedShape, class Voxel>
template <class ClosedInvertedRestrictedShape>
const typename SubstractiveH2CandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveH2CandidateGenerator<AugmentedShape, Voxel>::
generatePeristenceCandidatesWithH0Guarantee(int width, bool silent)
{
    return Base::template
            generatePeristenceCandidatesWithGuarantee<ClosedInvertedRestrictedShape>(
                width, silent);
}

template <class AugmentedShape, class Voxel>
template <class ClosedInvertedRestrictedShape>
const typename SubstractiveH2CandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveH2CandidateGenerator<AugmentedShape,Voxel>::
generatePeristenceCandidatesWithH0Guarantee(
        int width,
        std::unordered_set<Point>& lowerPoints,
        bool silent)
{
    return Base::template
            generatePeristenceCandidatesWithGuarantee<ClosedInvertedRestrictedShape>(
                width, lowerPoints, silent);
}

template <class AugmentedShape, class Voxel>
template <class ClosedInvertedRestrictedShape>
const typename SubstractiveH2CandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveH2CandidateGenerator<AugmentedShape,Voxel>::
generatePeristenceCandidatesWithH0Guarantee(int width,
                                            Point lowerPoint,
                                            bool silent)
{
    return Base::template
            generatePeristenceCandidatesWithGuarantee<ClosedInvertedRestrictedShape>(
                width, lowerPoint, silent);
}

template <class AugmentedShape, class Voxel>
inline constexpr bool
SubstractiveH2CandidateGenerator<AugmentedShape,Voxel>::isAdditive(){
    return false;
}


