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

/**
 * @file CandidateValidator.txx
 * Contains the implementation of the methods in the \ref CandidateValidator
 * class.
 **/

#include "CandidateValidator.h"

using namespace shape_simplification::simplifier;

template <class AugmentedShape, class Voxel>
CandidateValidator<AugmentedShape, Voxel>::CandidateValidator(
        AugmentedShape &shape)
    : _shape(shape), _overlay(shape) {}

template <class AugmentedShape, class Voxel>
CandidateValidator<AugmentedShape, Voxel>::~CandidateValidator() {}

template <class AugmentedShape, class Voxel>
const typename CandidateValidator<AugmentedShape,Voxel>::valid_candidate_container &
CandidateValidator<AugmentedShape, Voxel>::verifyAdditiveCandidatesValidity(
        const candidate_container &candidates) {
    return _verifyCandidatesValidity(
                candidates,
                [this](
                    std::vector<voxel_container>& cur,
                    rem_candidate_container& rem
                ){
        return _chooseFirstComeFirstServedAdditiveValidCandidates(cur, rem); }
    );
}

template <class AugmentedShape, class Voxel>
const typename CandidateValidator<AugmentedShape,Voxel>::valid_candidate_container &
CandidateValidator<AugmentedShape, Voxel>::verifyAdditiveCandidatesValidityWithH2Guarantee(
        const candidate_container &candidates) {
    return _verifyCandidatesValidity(
                candidates,
                [this](
                    std::vector<voxel_container>& cur,
                    rem_candidate_container& rem
                ){
        return _chooseFirstComeFirstServedAdditiveValidCandidatesWithH2Guarantee(
                    cur, rem);
        }
    );
}

template <class AugmentedShape, class Voxel>
const typename CandidateValidator<AugmentedShape,Voxel>::valid_candidate_container &
CandidateValidator<AugmentedShape, Voxel>::verifySubstractiveCandidatesValidity(
        const candidate_container &candidates) {
    return _verifyCandidatesValidity(
                candidates,
                [this](
                    std::vector<voxel_container>& cur,
                    rem_candidate_container& rem
                ){
        return _chooseFirstComeFirstServedSubstractiveValidCandidates(cur, rem);
        }
    );
}

template <class AugmentedShape, class Voxel>
const typename CandidateValidator<AugmentedShape,Voxel>::valid_candidate_container &
CandidateValidator<AugmentedShape, Voxel>::verifySubstractiveCandidatesValidityWithH2Guarantee(
        const candidate_container &candidates) {
    return _verifyCandidatesValidity(
                candidates,
                [this](
                    std::vector<voxel_container>& cur,
                    rem_candidate_container& rem
                ){
        return _chooseFirstComeFirstServedSubstractiveValidCandidatesWithH2Guarantee(
                    cur, rem);
        }
    );
}

template <class AugmentedShape, class Voxel>
const typename CandidateValidator<AugmentedShape,Voxel>::valid_candidate_container &
CandidateValidator<AugmentedShape, Voxel>::verifySubstractiveCandidatesValidityWithH0Guarantee(
        const candidate_container &candidates) {
    return _verifyCandidatesValidity(
                candidates,
                [this](
                    std::vector<voxel_container>& cur,
                    rem_candidate_container& rem
                ){
        return _chooseFirstComeFirstServedSubstractiveValidCandidatesWithH0Guarantee(
                    cur, rem);
        }
    );
}

template <class AugmentedShape, class Voxel>
const typename CandidateValidator<AugmentedShape,Voxel>::valid_candidate_container &
CandidateValidator<AugmentedShape, Voxel>::verifySubstractiveCandidatesValidityWithH0H2Guarantee(
        const candidate_container &candidates) {
    return _verifyCandidatesValidity(
                candidates,
                [this](
                    std::vector<voxel_container>& cur,
                    rem_candidate_container& rem
                ){
        return _chooseFirstComeFirstServedSubstractiveValidCandidatesWithH0H2Guarantee(
                    cur, rem);
        }
    );
}

template <class AugmentedShape, class Voxel>
const typename CandidateValidator<AugmentedShape,Voxel>::valid_candidate_container &
CandidateValidator<AugmentedShape, Voxel>::_verifyCandidatesValidity(
        const candidate_container &candidates,
        std::function<unsigned int(
            std::vector<voxel_container>&,
            rem_candidate_container&
            )> chooseValidCandidates)
{
    _overlay.clearModifications();
    _validCandidates.clear();
    _collar.clear();

    rem_candidate_container remainingCandidates;
    std::vector<voxel_container> currentGroup;
    unsigned int currentMaxGroupSize = 0;

    _retrieveFirstGroupOfNonOverlappingCandidates(candidates, currentGroup,
                                                  remainingCandidates);
    currentMaxGroupSize = currentGroup.size();

    while (!remainingCandidates.empty()) {
        currentMaxGroupSize = chooseValidCandidates(
                    currentGroup, remainingCandidates);
        _retrieveNextGroupOfNonOverlappingCandidates(
                    currentGroup, currentMaxGroupSize, remainingCandidates);
        currentMaxGroupSize = currentGroup.size();
    }

    return _validCandidates;
}

template <class AugmentedShape, class Voxel>
void CandidateValidator<AugmentedShape, Voxel>::_retrieveFirstGroupOfNonOverlappingCandidates(
        const candidate_container& candidates,
        std::vector<voxel_container>& currentGroup,
        rem_candidate_container& remainingCandidates)
{
    voxel_container region;
    int x, y, z;

    for (voxel_container candidate : candidates) {
        remainingCandidates.insert(candidate);

        auto it = candidate.begin();
        while (it != candidate.end() &&
               region.find(*it) == region.end()) ++it;

        if (it == candidate.end()) {
            currentGroup.push_back(candidate);
            for (Voxel v : candidate) {
                for (extended_neighbor_iterator<NEIGHBORHOOD> it(v, _shape);
                     !it.isEnd(); it.next()) {
                    it.getCurrentValue(x, y, z);
                    region.insert(Voxel(x, y, z));
                }
            }
        }
    }
}

template <class AugmentedShape, class Voxel>
unsigned int CandidateValidator<AugmentedShape, Voxel>::
_chooseFirstComeFirstServedAdditiveValidCandidates(
        std::vector<voxel_container>& currentGroup,
        rem_candidate_container& remainingCandidates)
{
    int x, y, z;

    voxel_container bigSet;
    for (voxel_container candidate : currentGroup){
        for (Voxel v : candidate) bigSet.insert(v);
    }
    unsigned int res = _overlay.addVoxelsAndReturnH2Only(bigSet);
    if (_overlay.getInitialNumberOfVoids() < res) {
        if (currentGroup.size() == 1){
            remainingCandidates.erase(bigSet);
        }
        int diff = res - _overlay.getInitialNumberOfVoids();
        unsigned int newSize = std::max(
            static_cast<unsigned int>(currentGroup.size() / (diff + 1)), 1U);
        return newSize;
    }

    for (voxel_container candidate : currentGroup) {
        _overlay.addVoxelsInMainShapeWithH2Guarantee(candidate);
        remainingCandidates.erase(candidate);
        if (_overlay.getInitialNumberOfLoops() >= _overlay.getCurrentNumberOfLoops())
        {
            for (Voxel v : candidate){
                _validCandidates.insert(v);
                for (ExtendedNeighborsIterator<Voxel> nIt(v);
                     !nIt.isEnd();
                     nIt.next())
                {
                    nIt.getCurrentValue(x, y, z);
                    Voxel n(x, y, z);
                    if (candidate.find(n) == candidate.end())
                        _collar.insert(n);
                }
            }
            _overlay.saveAddedVoxelsWithH2Guarantee(candidate);
        }
    }

    return 2 * (currentGroup.size() + 1);
}

template <class AugmentedShape, class Voxel>
unsigned int CandidateValidator<AugmentedShape, Voxel>::
_chooseFirstComeFirstServedAdditiveValidCandidatesWithH2Guarantee(
        std::vector<voxel_container>& currentGroup,
        rem_candidate_container& remainingCandidates)
{
    int x, y, z;

    for (voxel_container candidate : currentGroup) {
        _overlay.addVoxelsInMainShapeWithH2Guarantee(candidate);
        remainingCandidates.erase(candidate);

        if (_overlay.getInitialNumberOfLoops() <
                _overlay.getCurrentNumberOfLoops()) {
            int diff = _overlay.getCurrentNumberOfLoops() -
                    _overlay.getInitialNumberOfLoops();
            unsigned int newSize = std::max(
                static_cast<unsigned int>(currentGroup.size() / (diff + 1)), 1U);
            return newSize;
        }

        for (Voxel v : candidate){
            _validCandidates.insert(v);
            for (ExtendedNeighborsIterator<Voxel> nIt(v);
                 !nIt.isEnd();
                 nIt.next())
            {
                nIt.getCurrentValue(x, y, z);
                Voxel n(x, y, z);
                if (candidate.find(n) == candidate.end())
                    _collar.insert(n);
            }
        }
        _overlay.saveAddedVoxelsWithH2Guarantee(candidate);
    }

    return 2 * (currentGroup.size() + 1);
}

template <class AugmentedShape, class Voxel>
unsigned int CandidateValidator<AugmentedShape, Voxel>::
_chooseFirstComeFirstServedSubstractiveValidCandidates(
        std::vector<voxel_container>& currentGroup,
        rem_candidate_container& remainingCandidates)
{
    int x, y, z;

    voxel_container bigSet;
    for (voxel_container candidate : currentGroup){
        for (Voxel v : candidate) bigSet.insert(v);
    }
    unsigned int res = _overlay.removeVoxelsAndReturnH0Only(bigSet);
    if (_overlay.getInitialNumberOfComponents() < res) {
        if (currentGroup.size() == 1){
            remainingCandidates.erase(bigSet);
        }
        int diff = res - _overlay.getInitialNumberOfComponents();
        unsigned int newSize = std::max(
            static_cast<unsigned int>(currentGroup.size() / (diff + 1)), 1U);
        return newSize;
    }

    for (voxel_container candidate : currentGroup) {
        //_overlay.removeVoxelsFromMainShapeWithH0Guarantee(candidate);
        _overlay.removeVoxelsFromMainShape(candidate);
        remainingCandidates.erase(candidate);

        if (_overlay.getInitialNumberOfLoops() >= _overlay.getCurrentNumberOfLoops())
        {
            for (Voxel v : candidate){
                _validCandidates.insert(v);
                for (ExtendedNeighborsIterator<Voxel> nIt(v);
                     !nIt.isEnd();
                     nIt.next())
                {
                    nIt.getCurrentValue(x, y, z);
                    Voxel n(x, y, z);
                    if (candidate.find(n) == candidate.end())
                        _collar.insert(n);
                }
            }
            //_overlay.saveRemovedVoxelsWithH0Guarantee(candidate);
            _overlay.saveRemovedVoxels(candidate);
        }
    }

    return 2 * (currentGroup.size() + 1);
}

template <class AugmentedShape, class Voxel>
unsigned int CandidateValidator<AugmentedShape, Voxel>::
_chooseFirstComeFirstServedSubstractiveValidCandidatesWithH2Guarantee(
        std::vector<voxel_container>& currentGroup,
        rem_candidate_container& remainingCandidates)
{
    int x, y, z;

    voxel_container bigSet;
    for (voxel_container candidate : currentGroup){
        for (Voxel v : candidate) bigSet.insert(v);
    }
    unsigned int res = _overlay.removeVoxelsAndReturnH0Only(bigSet);
    if (_overlay.getInitialNumberOfComponents() < res) {
        if (currentGroup.size() == 1){
            remainingCandidates.erase(bigSet);
        }
        int diff = res - _overlay.getInitialNumberOfComponents();
        unsigned int newSize = std::max(
            static_cast<unsigned int>(currentGroup.size() / (diff + 1)), 1U);
        return newSize;
    }

    for (voxel_container candidate : currentGroup) {
        //_overlay.removeVoxelsFromMainShapeWithH0H2Guarantee(candidate);
        _overlay.removeVoxelsFromMainShapeWithH2Guarantee(candidate);
        remainingCandidates.erase(candidate);

        if (_overlay.getInitialNumberOfLoops() >= _overlay.getCurrentNumberOfLoops())
        {
            for (Voxel v : candidate){
                _validCandidates.insert(v);
                for (ExtendedNeighborsIterator<Voxel> nIt(v);
                     !nIt.isEnd();
                     nIt.next())
                {
                    nIt.getCurrentValue(x, y, z);
                    Voxel n(x, y, z);
                    if (candidate.find(n) == candidate.end())
                        _collar.insert(n);
                }
            }
            //_overlay.saveRemovedVoxelsWithH0H2Guarantee(candidate);
            _overlay.saveRemovedVoxelsWithH2Guarantee(candidate);
        }
    }

    return 2 * (currentGroup.size() + 1);
}

template <class AugmentedShape, class Voxel>
unsigned int CandidateValidator<AugmentedShape, Voxel>::
_chooseFirstComeFirstServedSubstractiveValidCandidatesWithH0Guarantee(
        std::vector<voxel_container>& currentGroup,
        rem_candidate_container& remainingCandidates)
{
    int x, y, z;

    for (voxel_container candidate : currentGroup) {
        _overlay.removeVoxelsFromMainShapeWithH0Guarantee(candidate);
        remainingCandidates.erase(candidate);

        if (_overlay.getInitialNumberOfLoops() <
                _overlay.getCurrentNumberOfLoops()) {
            int diff = _overlay.getCurrentNumberOfLoops() -
                    _overlay.getInitialNumberOfLoops();
            unsigned int newSize = std::max(
                static_cast<unsigned int>(currentGroup.size() / (diff + 1)), 1U);
            return newSize;
        }

        for (Voxel v : candidate){
            _validCandidates.insert(v);
            for (ExtendedNeighborsIterator<Voxel> nIt(v);
                 !nIt.isEnd();
                 nIt.next())
            {
                nIt.getCurrentValue(x, y, z);
                Voxel n(x, y, z);
                if (candidate.find(n) == candidate.end())
                    _collar.insert(n);
            }
        }
        _overlay.saveRemovedVoxelsWithH0Guarantee(candidate);
    }

    return 2 * (currentGroup.size() + 1);
}

template <class AugmentedShape, class Voxel>
unsigned int CandidateValidator<AugmentedShape, Voxel>::
_chooseFirstComeFirstServedSubstractiveValidCandidatesWithH0H2Guarantee(
        std::vector<voxel_container>& currentGroup,
        rem_candidate_container& remainingCandidates)
{
    int x, y, z;

    for (voxel_container candidate : currentGroup) {
        _overlay.removeVoxelsFromMainShapeWithH0H2Guarantee(candidate);
        remainingCandidates.erase(candidate);

        if (_overlay.getInitialNumberOfLoops() <
                _overlay.getCurrentNumberOfLoops()) {
            int diff = _overlay.getCurrentNumberOfLoops() -
                    _overlay.getInitialNumberOfLoops();
            unsigned int newSize = std::max(
                static_cast<unsigned int>(currentGroup.size() / (diff + 1)), 1U);
            return newSize;
        }

        for (Voxel v : candidate){
            _validCandidates.insert(v);
            for (ExtendedNeighborsIterator<Voxel> nIt(v);
                 !nIt.isEnd();
                 nIt.next())
            {
                nIt.getCurrentValue(x, y, z);
                Voxel n(x, y, z);
                if (candidate.find(n) == candidate.end())
                    _collar.insert(n);
            }
        }
        _overlay.saveRemovedVoxelsWithH0H2Guarantee(candidate);
    }

    return 2 * (currentGroup.size() + 1);
}

template <class AugmentedShape, class Voxel>
void CandidateValidator<AugmentedShape, Voxel>::
_retrieveNextGroupOfNonOverlappingCandidates(
        std::vector<voxel_container>& currentGroup,
        unsigned int currentMaxGroupSize,
        rem_candidate_container& remainingCandidates)
{
    int x, y, z;
    voxel_container region;
    currentGroup.clear();

    auto it = remainingCandidates.begin();
    while (it != remainingCandidates.end() &&
           currentGroup.size() < currentMaxGroupSize) {
        const voxel_container candidate = *it;

        auto canIt = candidate.begin();
        bool toContinue = true;
        while (canIt != candidate.end() && toContinue) {
            Voxel v = *canIt;
            if (_validCandidates.find(v) != _validCandidates.end() ||
                    _collar.find(v) != _collar.end()){
                it = remainingCandidates.erase(it);
                toContinue = false;
            } else if (region.find(v) != region.end()) {
                ++it;
                toContinue = false;
            }
            canIt++;
        }

        if (toContinue) {
            currentGroup.push_back(candidate);
            for (Voxel v : candidate) {
                for (extended_neighbor_iterator<NEIGHBORHOOD> it(v, _shape);
                     !it.isEnd(); it.next()) {
                    it.getCurrentValue(x, y, z);
                    region.insert(Voxel(x, y, z));
                }
            }
            ++it;
        }
    }
}
