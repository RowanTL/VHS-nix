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
 * @file AdditiveCandidateGenerator.txx
 * Contains the implementation of the methods in the
 * \ref AdditiveCandidateGenerator class.
 **/

#include "AdditiveCandidateGenerator.h"

using namespace shape_simplification::simplifier;

template <class AugmentedShape, class Voxel>
AdditiveCandidateGenerator<AugmentedShape, Voxel>::
AdditiveCandidateGenerator(AugmentedShape &shape) : _shape(shape)
{}

template <class AugmentedShape, class Voxel>
AdditiveCandidateGenerator<AugmentedShape, Voxel>::
~AdditiveCandidateGenerator()
{}

template <class AugmentedShape, class Voxel>
const typename AdditiveCandidateGenerator<AugmentedShape, Voxel>::
candidate_container &
AdditiveCandidateGenerator<AugmentedShape, Voxel>::
generateH0OneVoxelCandidates()
{
    _candidates.clear();
    int x, y, z;

    for (Voxel v : _shape.getCollar()) {
        if (v.getLevel() == NEIGHBORHOOD) {
            voxel_container rep;
            for (neighbor_iterator<MAIN_SHAPE> it(v, _shape); !it.isEnd();
                 it.next()) {
                it.getCurrentValue(x, y, z);
                rep.insert(_shape.getComponentRepresentative(Voxel(x, y, z)));
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
const typename AdditiveCandidateGenerator<AugmentedShape, Voxel>::
candidate_container &
AdditiveCandidateGenerator<AugmentedShape, Voxel>::
generateH2OneVoxelCandidates()
{
    _candidates.clear();
    int x, y, z;

    for (Voxel v : _shape.getCollar()) {
        if (v.getLevel() == NEIGHBORHOOD) {
            int valid = 1;
            neighbor_iterator<MAIN_SHAPE> it(v, _shape);
            it.getCurrentValue(x, y, z);
            Voxel rep = _shape.getComponentRepresentative(Voxel(x, y, z));
            it.next();
            it.getCurrentValue(x, y, z);
            while (!it.isEnd() && valid > 0) {
                if (_shape.getComponentRepresentative(Voxel(x, y, z)) == rep)
                    valid++;
                else
                    valid = 0;
                it.next();
                it.getCurrentValue(x, y, z);
            }
            if (valid == 6) {
                voxel_container candidate;
                candidate.insert(v);
                _candidates.push_back(candidate);
            }
        }
    }

    return _candidates;
}

template <class AugmentedShape, class Voxel>
const typename AdditiveCandidateGenerator<AugmentedShape, Voxel>::
candidate_container &
AdditiveCandidateGenerator<AugmentedShape, Voxel>::
generateVoidFillings()
{
    _candidates.clear();
    int x, y, z;

    std::list<Voxel> seedPoints;
    CC collarComponents;
    Voxel maxCollar(0, 0, 0);

    for (Voxel v : _shape.getCollar()) {
        collarComponents.initialize(v);
        for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
            it.getCurrentValue(x, y, z);
            collarComponents.merge(v, Voxel(x, y, z));
        }
        maxCollar = std::max(maxCollar, v);
    }

    for (Voxel v : _shape.getCollar()) {
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
            for (extended_neighbor_iterator<NEIGHBORHOOD> it(cur, _shape);
                 !it.isEnd(); it.next()) {
                nberOfNeighborsNotOutside++;
                it.getCurrentValue(x, y, z);
                Voxel n(x, y, z);
                if (!(_shape.template contains<MAIN_SHAPE>(x, y, z))
                        && filling.find(n) == filling.end()
                        && found.find(n) == found.end()) {
                    queue.push(n);
                    found.insert(n);
                }
            }
        }
        if (nberOfNeighborsNotOutside == 26)
            _candidates.push_back(filling);
    }

    return _candidates;
}

template <class AugmentedShape, class Voxel>
template <class RestrictedShape>
const typename AdditiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
AdditiveCandidateGenerator<AugmentedShape, Voxel>::
generatePeristenceCandidates(int width, bool silent)
{
    return _generatePeristenceCandidates(
                width,
                [this](
                    Point& lower,
                    Point& upper
                ){
        _generatePeristenceCandidatesInBox<RestrictedShape>(lower, upper); },
                silent
    );
}

template <class AugmentedShape, class Voxel>
template <class RestrictedShape>
const typename AdditiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
AdditiveCandidateGenerator<AugmentedShape,Voxel>::
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
        _generatePeristenceCandidatesInBox<RestrictedShape>(
                    lower, upper); },
                silent
    );
}

template <class AugmentedShape, class Voxel>
template <class RestrictedShape>
const typename AdditiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
AdditiveCandidateGenerator<AugmentedShape,Voxel>::
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
        _generatePeristenceCandidatesInBox<RestrictedShape>(
                    lower, upper); },
                silent
    );
}

template <class AugmentedShape, class Voxel>
template <class ClosedRestrictedShape>
const typename AdditiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
AdditiveCandidateGenerator<AugmentedShape, Voxel>::
generatePeristenceCandidatesWithGuarantee(int width, bool silent)
{
    return _generatePeristenceCandidates(
                width,
                [this](
                    Point& lower,
                    Point& upper
                ){
        _generatePeristenceCandidatesInBoxWithGuarantee<ClosedRestrictedShape>(
                    lower, upper); },
                silent
    );
}

template <class AugmentedShape, class Voxel>
template <class ClosedRestrictedShape>
const typename AdditiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
AdditiveCandidateGenerator<AugmentedShape,Voxel>::
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
        _generatePeristenceCandidatesInBoxWithGuarantee<ClosedRestrictedShape>(
                    lower, upper); },
                silent
    );
}

template <class AugmentedShape, class Voxel>
template <class ClosedRestrictedShape>
const typename AdditiveCandidateGenerator<AugmentedShape,Voxel>::
candidate_container&
AdditiveCandidateGenerator<AugmentedShape,Voxel>::
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
        _generatePeristenceCandidatesInBoxWithGuarantee<ClosedRestrictedShape>(
                    lower, upper); },
                silent
    );
}

template <class AugmentedShape, class Voxel>
const typename AdditiveCandidateGenerator<
    AugmentedShape,Voxel>::candidate_container&
AdditiveCandidateGenerator<AugmentedShape,Voxel>::_generatePeristenceCandidates(
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
const typename AdditiveCandidateGenerator<
    AugmentedShape,Voxel>::candidate_container&
AdditiveCandidateGenerator<AugmentedShape,Voxel>::_generatePeristenceCandidates(
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
const typename AdditiveCandidateGenerator<
    AugmentedShape,Voxel>::candidate_container&
AdditiveCandidateGenerator<AugmentedShape,Voxel>::_generatePeristenceCandidates(
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
template <class RestrictedShape>
void AdditiveCandidateGenerator<AugmentedShape,Voxel>::
_generatePeristenceCandidatesInBox(Point &lowerCorner, Point &upperCorner)
{
    using Skeleton = DualSkeleton<RestrictedShape, Voxel, 0, 2>;
    using Persistence = PersistentHomology<Skeleton, Voxel, 0, 2>;

    RestrictedShape restrShape(_shape, lowerCorner, upperCorner);
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
template <class ClosedRestrictedShape>
void AdditiveCandidateGenerator<AugmentedShape,Voxel>::
_generatePeristenceCandidatesInBoxWithGuarantee(
        Point &lowerCorner, Point &upperCorner)
{
    using Skeleton = DualSkeleton<ClosedRestrictedShape, Voxel, 0, 2>;
    using Persistence = PersistentHomology<Skeleton, Voxel, 0, 2>;
    using AugShape = shape::AugmentedShape<ClosedRestrictedShape, Voxel>;
    using ShapeOverlay = ShapeOverlay<AugShape, Voxel>;

    ClosedRestrictedShape restrShape(_shape, lowerCorner, upperCorner);
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
AdditiveCandidateGenerator<AugmentedShape,Voxel>::isAdditive(){
    return true;
}

