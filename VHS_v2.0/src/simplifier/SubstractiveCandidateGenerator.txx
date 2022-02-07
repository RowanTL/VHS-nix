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
 * @file SubstractiveCandidateGenerator.txx
 * Contains the implementation of the methods in the
 * \ref SubstractiveCandidateGenerator class.
 **/

#include "SubstractiveCandidateGenerator.h"

using namespace shape_simplification::simplifier;

template <class AugmentedShape, class Voxel>
SubstractiveCandidateGenerator<AugmentedShape, Voxel>::
SubstractiveCandidateGenerator(AugmentedShape &shape) : _shape(shape)
{}

template <class AugmentedShape, class Voxel>
SubstractiveCandidateGenerator<AugmentedShape, Voxel>::
~SubstractiveCandidateGenerator()
{}

template <class AugmentedShape, class Voxel>
const typename SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveCandidateGenerator<AugmentedShape, Voxel>::
generateH0OneVoxelCandidates()
{
    _candidates.clear();
    int x, y, z;

    for (auto it = _shape.template begin<MAIN_SHAPE>();
         it != _shape.template end<MAIN_SHAPE>();
         ++it){
        Voxel cur = *it;
        NeighborsIterator<Voxel> neigIt(cur);
        neigIt.getCurrentValue(x, y, z);
        while (!neigIt.isEnd() &&
               !(_shape.template contains<MAIN_SHAPE>(x, y, z))) {
            neigIt.next();
            if (!neigIt.isEnd()) neigIt.getCurrentValue(x, y, z);
        }
        if (neigIt.isEnd()) {
            voxel_container candidate;
            candidate.insert(cur);
            _candidates.push_back(candidate);
        }
    }

    return _candidates;
}

template <class AugmentedShape, class Voxel>
const typename SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveCandidateGenerator<AugmentedShape, Voxel>::
generateH2OneVoxelCandidates()
{
    using neigh_it =
        typename shape::InvertedAugmentedShape<AugmentedShape,Voxel>::template
        neighbor_iterator<MAIN_SHAPE>;

    _candidates.clear();
    int x, y, z;

    shape::InvertedAugmentedShape<AugmentedShape,Voxel> invShape(_shape);

    for (Voxel v : invShape.getCollar()) {
        if (v.getLevel() == NEIGHBORHOOD) {
            voxel_container rep;
            for (neigh_it it(v, invShape); !it.isEnd(); it.next()) {
                it.getCurrentValue(x, y, z);
                rep.insert(invShape.getComponentRepresentative(Voxel(x, y, z)));
            }
            if (rep.size() > 1) {
                voxel_container candidate;
                candidate.insert(v);
                _candidates.push_back(candidate);
            }
        }
    }

    return _candidates;
}

template <class AugmentedShape, class Voxel>
const typename SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveCandidateGenerator<AugmentedShape, Voxel>::
generateSubstractibleComponents(int maxSize)
{
    if (maxSize < 0) return generateSubstractibleComponents();

    _candidates.clear();

    if (maxSize == 0) return _candidates;

    voxel_container doneRep;

    for (auto it = _shape.template begin<MAIN_SHAPE>();
         it != _shape.template end<MAIN_SHAPE>();
         it++)
    {
        Voxel cur = *it;
        Voxel rep = _shape.getComponentRepresentative(cur);
        if (doneRep.find(rep) == doneRep.end() &&
                _shape.getComponentSize(rep) <= maxSize){
            doneRep.insert(rep);
            std::vector<Voxel> component;
            _shape.getComponent(rep, component);
            voxel_container candidate;
            auto compIt = component.begin();
            while (compIt != component.end() && (*compIt).getLevel() != CORE) {
                candidate.insert(*compIt);
                compIt++;
            }
            if (compIt == component.end()) _candidates.push_back(candidate);
        }
    }

    return _candidates;
}

template <class AugmentedShape, class Voxel>
const typename SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveCandidateGenerator<AugmentedShape, Voxel>::
generateSubstractibleComponents()
{
    _candidates.clear();

    voxel_container doneRep;

    for (auto it = _shape.template begin<MAIN_SHAPE>();
         it != _shape.template end<MAIN_SHAPE>();
         ++it)
    {
        Voxel cur = *it;
        Voxel rep = _shape.getComponentRepresentative(cur);
        if (doneRep.find(rep) == doneRep.end()
                && !_shape.isInCoreComponent(rep))
        {
            doneRep.insert(rep);
            std::vector<Voxel> component;
            _shape.getComponent(rep, component);
            voxel_container candidate;
            for (Voxel v : component) candidate.insert(v);
            _candidates.push_back(candidate);
        }
    }

    return _candidates;
}

template <class AugmentedShape, class Voxel>
template <class InvertedRestrictedShape>
const typename SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveCandidateGenerator<AugmentedShape, Voxel>::
generatePeristenceCandidates(int width, bool silent)
{
    return _generatePeristenceCandidates(
                width,
                [this](
                    Point& lower,
                    Point& upper
                ){
        _generatePeristenceCandidatesInBox<InvertedRestrictedShape>(
                    lower, upper); },
                silent
    );
}

template <class AugmentedShape, class Voxel>
template <class InvertedRestrictedShape>
const typename SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
generatePeristenceCandidates(int width,
                             std::unordered_set<Point>& lowerPoints,
                             bool silent)
{
    return _generatePeristenceCandidates(
                width,
                lowerPoints,
                [this](
                    Point& lower,
                    Point& upper
                ){
        _generatePeristenceCandidatesInBox<InvertedRestrictedShape>(
                    lower, upper); },
                silent
    );
}

template <class AugmentedShape, class Voxel>
template <class InvertedRestrictedShape>
const typename SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
generatePeristenceCandidates(int width,
                             Point lowerPoint,
                             bool silent)
{
    return _generatePeristenceCandidates(
                width,
                lowerPoint,
                [this](
                    Point& lower,
                    Point& upper
                ){
        _generatePeristenceCandidatesInBox<InvertedRestrictedShape>(
                    lower, upper); },
                silent
    );
}

template <class AugmentedShape, class Voxel>
template <class ClosedInvertedRestrictedShape>
const typename SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveCandidateGenerator<AugmentedShape, Voxel>::
generatePeristenceCandidatesWithGuarantee(int width, bool silent)
{
    return _generatePeristenceCandidates(
                width,
                [this](
                    Point& lower,
                    Point& upper
                ){
        _generatePeristenceCandidatesInBoxWithGuarantee<ClosedInvertedRestrictedShape>(
                    lower, upper); },
                silent
    );
}

template <class AugmentedShape, class Voxel>
template <class ClosedInvertedRestrictedShape>
const typename SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
generatePeristenceCandidatesWithGuarantee(
        int width,
        std::unordered_set<Point>& lowerPoints,
        bool silent)
{
    return _generatePeristenceCandidates(
                width,
                lowerPoints,
                [this](
                    Point& lower,
                    Point& upper
                ){
        _generatePeristenceCandidatesInBoxWithGuarantee<ClosedInvertedRestrictedShape>(
                    lower, upper); },
                silent
    );
}

template <class AugmentedShape, class Voxel>
template <class ClosedInvertedRestrictedShape>
const typename SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
generatePeristenceCandidatesWithGuarantee(int width,
                                            Point lowerPoint,
                                            bool silent)
{
    return _generatePeristenceCandidates(
                width,
                lowerPoint,
                [this](
                    Point& lower,
                    Point& upper
                ){
        _generatePeristenceCandidatesInBoxWithGuarantee<ClosedInvertedRestrictedShape>(
                    lower, upper); },
                silent
    );
}

template <class AugmentedShape, class Voxel>
const typename SubstractiveCandidateGenerator<
    AugmentedShape,Voxel>::candidate_container&
SubstractiveCandidateGenerator<AugmentedShape,Voxel>::_generatePeristenceCandidates(
        int width,
        std::function<void(Point&,Point&)> generateCandidates,
        bool silent) {
    _candidates.clear();

    const std::array<unsigned int, 3> &dimensions = _shape.getShapeDimensions();

    Point lower;
    int shift = width/2;

    for (lower[0] = 0; lower[0] <= dimensions[0]; lower[0] += shift) {
        for (lower[1] = 0; lower[1] <= dimensions[1]; lower[1] += shift) {
            for (lower[2] = 0; lower[2] <= dimensions[2]; lower[2] += shift) {
                if (!silent){
                    std::cout << "Looking for simplifications in box "
                              << lower[0] << " " << lower[1] << " " << lower[2]
                              << "; " << _candidates.size() << " so far"
                              << std::endl;
                }

                Point upper{lower[0] + width,
                            lower[1] + width,
                            lower[2] + width};

                generateCandidates(lower, upper);
            }
        }
    }

    if (!silent){
        std::cout << "Finished looking for candidates: "
                  << _candidates.size() << " in total"
                  << std::endl;
    }

    return _candidates;
}

template <class AugmentedShape, class Voxel>
const typename SubstractiveCandidateGenerator<
    AugmentedShape,Voxel>::candidate_container&
SubstractiveCandidateGenerator<AugmentedShape,Voxel>::_generatePeristenceCandidates(
        int width,
        std::unordered_set<Point>& lowerPoints,
        std::function<void(Point&,Point&)> generateCandidates,
        bool silent) {
    _candidates.clear();

    for (Point lower : lowerPoints){
        if (!silent){
            std::cout << "Looking for simplifications in box "
                      << lower[0] << " " << lower[1] << " " << lower[2]
                      << "; " << _candidates.size() << " so far" << std::endl;
        }

        Point upper{lower[0] + width, lower[1] + width, lower[2] + width};

        generateCandidates(lower, upper);
    }

    if (!silent){
        std::cout << "Finished looking for candidates: "
                  << _candidates.size() << " in total"
                  << std::endl;
    }

    return _candidates;
}

template <class AugmentedShape, class Voxel>
const typename SubstractiveCandidateGenerator<
AugmentedShape,Voxel>::candidate_container&
SubstractiveCandidateGenerator<AugmentedShape,Voxel>::_generatePeristenceCandidates(
        int width,
        Point& lower,
        std::function<void(Point&,Point&)> generateCandidates,
        bool silent) {
    _candidates.clear();

    if (!silent){
        std::cout << "Looking for simplifications in box "
                  << lower[0] << " " << lower[1] << " " << lower[2]
                  << "." << std::endl;
    }

    Point upper{lower[0] + width, lower[1] + width, lower[2] + width};

    generateCandidates(lower, upper);

    if (!silent){
        std::cout << "Found " << _candidates.size()
                  << " in this box." << std::endl;
    }

    return _candidates;
}

template <class AugmentedShape, class Voxel>
template <class InvertedRestrictedShape>
void SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
_generatePeristenceCandidatesInBox(Point &lowerCorner, Point &upperCorner) {
    using Skeleton = DualSkeleton<InvertedRestrictedShape, Voxel, 1, 3>;
    using Persistence = PersistentHomology<Skeleton, Voxel, 1, 3>;

    InvertedRestrictedShape restrShape(_shape, lowerCorner, upperCorner);
    Skeleton skeleton(restrShape);
    Persistence pers(skeleton);
    std::vector<Voxel> chain;
    voxel_container candidate;

    pers.reduceMatrix();

    for (typename Persistence::persistent_pair pair : pers.getPersistentPairs())
    {
        if (pers.getTimestamp(pair.second) >= 2) {
            chain.clear();
            candidate.clear();
            pers.getCorrespondingVoxelChain(pair, chain);
            for (Voxel v : chain) {
                if (restrShape.template contains<EX_NEIGHBORHOOD>(
                            v[0], v[1], v[2]))
                {
                    candidate.insert(v);
                }
            }
            _candidates.push_back(candidate);
        }
    }
}

template <class AugmentedShape, class Voxel>
template <class ClosedInvertedRestrictedShape>
void SubstractiveCandidateGenerator<AugmentedShape,Voxel>::
_generatePeristenceCandidatesInBoxWithGuarantee(
        Point &lowerCorner, Point &upperCorner)
{
    using Skeleton = DualSkeleton<ClosedInvertedRestrictedShape, Voxel, 1, 3>;
    using Persistence = PersistentHomology<Skeleton, Voxel, 1, 3>;
    using AugShape = shape::AugmentedShape<ClosedInvertedRestrictedShape, Voxel>;
    using ShapeOverlay = ShapeOverlay<AugShape, Voxel>;

    ClosedInvertedRestrictedShape restrShape(_shape, lowerCorner, upperCorner);
    Skeleton skeleton(restrShape);
    Persistence pers(skeleton);
    std::vector<Voxel> chain;
    voxel_container candidate;

    AugShape augShape(restrShape);
    ShapeOverlay overlay(augShape);

    pers.reduceMatrix();

    for (typename Persistence::persistent_pair pair : pers.getPersistentPairs())
    {
        if (pers.getTimestamp(pair.second) >= 2) {
            chain.clear();
            candidate.clear();
            pers.getCorrespondingVoxelChain(pair, chain);
            for (Voxel v : chain) {
                if (restrShape.template contains<EX_NEIGHBORHOOD>(
                            v[0], v[1], v[2]))
                {
                    candidate.insert(v);
                }
            }

            unsigned int res = overlay.addVoxelsAndReturnH2Only(candidate);
            if (res <= overlay.getInitialNumberOfVoids()){
                overlay.saveAddedVoxelsWithH2Guarantee(candidate);
                _candidates.push_back(candidate);
            }
        }
    }
}

template <class AugmentedShape, class Voxel>
inline constexpr bool
SubstractiveCandidateGenerator<AugmentedShape,Voxel>::isAdditive(){
    return false;
}

