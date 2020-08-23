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
 * @file AdditiveH2CandidateGenerator.txx
 * Contains the implementation of the methods in the \ref
 *AdditiveH2CandidateGenerator class.
 **/

#include "AdditiveH2CandidateGenerator.h"

using namespace shape_simplification::simplifier;

template <class AugmentedShape, class Voxel>
AdditiveH2CandidateGenerator<AugmentedShape, Voxel>::
AdditiveH2CandidateGenerator(AugmentedShape &shape)
    : BaseCandidateGenerator<AugmentedShape,Voxel,2>(shape) {}

template <class AugmentedShape, class Voxel>
AdditiveH2CandidateGenerator<AugmentedShape, Voxel>::
~AdditiveH2CandidateGenerator() {}

template <class AugmentedShape, class Voxel>
const typename AdditiveH2CandidateGenerator<AugmentedShape, Voxel>::
candidate_container &
AdditiveH2CandidateGenerator<AugmentedShape, Voxel>::
generateVoidFillings()
{
    Base::_candidates.clear();

    std::list<Voxel> seedPoints;
    CC collarComponents;
    Voxel maxCollar(0, 0, 0);

    for (Voxel v : Base::_shape.getCollar()) {
        collarComponents.initialize(v);
        for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
            Voxel cur = it.getCurrentValue();
            collarComponents.merge(v, cur);
        }
        maxCollar = std::max(maxCollar, v);
    }

    for (Voxel v : Base::_shape.getCollar()) {
        Voxel rep = collarComponents.find(v);
        if (v == rep && rep != collarComponents.find(maxCollar))
            seedPoints.push_back(v);
    }

    for (Voxel seed : seedPoints) {
        voxel_container filling;
        std::queue<Voxel> queue;
        queue.push(seed);
        voxel_container found;
        int nberOfNeighborsNotOutside = 26;
        while (!queue.empty() && nberOfNeighborsNotOutside == 26) {
            Voxel cur = queue.front();
            queue.pop();
            filling.insert(cur);
            nberOfNeighborsNotOutside = 0;
            for (extended_neighbor_iterator<NEIGHBORHOOD> it(cur, Base::_shape);
                 !it.isEnd(); it.next()) {
                nberOfNeighborsNotOutside++;
                Voxel n = it.getCurrentValue();
                if (!(Base::_shape.template contains<MAIN_SHAPE>(n)) &&
                        filling.find(n) == filling.end() && found.find(n) == found.end()) {
                    queue.push(n);
                    found.insert(n);
                }
            }
        }
        if (nberOfNeighborsNotOutside == 26)
            Base::_candidates.push_back(filling);
    }

    return Base::_candidates;
}

template <class AugmentedShape, class Voxel>
inline constexpr bool
AdditiveH2CandidateGenerator<AugmentedShape,Voxel>::isAdditive(){
    return true;
}

