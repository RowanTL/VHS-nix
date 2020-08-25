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
 * @file SubstractiveH0CandidateGenerator.txx
 * Contains the implementation of the methods in the
 * \ref SubstractiveH0CandidateGenerator class.
 **/

#include "SubstractiveH0CandidateGenerator.h"

using namespace shape_simplification::simplifier;

template <class AugmentedShape, class Voxel>
SubstractiveH0CandidateGenerator<AugmentedShape, Voxel>::
SubstractiveH0CandidateGenerator(AugmentedShape &shape)
    : BaseCandidateGenerator<AugmentedShape,Voxel,0>(shape) {}

template <class AugmentedShape, class Voxel>
SubstractiveH0CandidateGenerator<AugmentedShape, Voxel>::
~SubstractiveH0CandidateGenerator()
{}

template <class AugmentedShape, class Voxel>
const typename SubstractiveH0CandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveH0CandidateGenerator<AugmentedShape, Voxel>::
generateSubstractibleComponents(int maxSize)
{
    if (maxSize < 0) return generateSubstractibleComponents();

    Base::_candidates.clear();

    if (maxSize == 0) return Base::_candidates;

    voxel_container doneRep;

    for (auto it = Base::_shape.template begin<MAIN_SHAPE>();
         it != Base::_shape.template end<MAIN_SHAPE>();
         ++it){
        Voxel cur = *it;
        Voxel rep = Base::_shape.getComponentRepresentative(cur);
        if (doneRep.find(rep) == doneRep.end() && Base::_shape.getComponentSize(rep) <= maxSize){
            doneRep.insert(rep);
            std::vector<Voxel> component;
            Base::_shape.getComponent(rep, component);
            voxel_container candidate;
            auto compIt = component.begin();
            while (compIt != component.end() && !Base::_shape.template contains<CORE>(*compIt)) {
                candidate.insert(*compIt);
                compIt++;
            }
            if (compIt == component.end()) Base::_candidates.push_back(candidate);
        }
    }

    return Base::_candidates;
}

template <class AugmentedShape, class Voxel>
const typename SubstractiveH0CandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveH0CandidateGenerator<AugmentedShape, Voxel>::
generateSubstractibleComponents()
{
    Base::_candidates.clear();

    voxel_container doneRep;

    for (auto it = Base::_shape.template begin<MAIN_SHAPE>();
         it != Base::_shape.template end<MAIN_SHAPE>();
         ++it){
        Voxel cur = *it;
        Voxel rep = Base::_shape.getComponentRepresentative(cur);
        if (doneRep.find(rep) == doneRep.end() && !Base::_shape.isInCoreComponent(rep)){
            doneRep.insert(rep);
            std::vector<Voxel> component;
            Base::_shape.getComponent(rep, component);
            voxel_container candidate;
            for (Voxel v : component) candidate.insert(v);
            Base::_candidates.push_back(candidate);
        }
    }

    return Base::_candidates;
}

template <class AugmentedShape, class Voxel>
inline constexpr bool
SubstractiveH0CandidateGenerator<AugmentedShape,Voxel>::isAdditive(){
    return false;
}

