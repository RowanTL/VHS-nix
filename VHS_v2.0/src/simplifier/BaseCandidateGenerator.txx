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
 * @file BaseCandidateGenerator.txx
 * Contains the implementation of the methods in the
 * \ref BaseCandidateGenerator class.
 **/

#include "BaseCandidateGenerator.h"

using namespace shape_simplification::simplifier;

template <class AugmentedShape, class Voxel, int dim>
BaseCandidateGenerator<AugmentedShape,Voxel,dim>::BaseCandidateGenerator(
        AugmentedShape &shape)
    : _shape(shape) {}

template <class AugmentedShape, class Voxel, int dim>
BaseCandidateGenerator<AugmentedShape,Voxel,dim>::~BaseCandidateGenerator()
{}

template <class AugmentedShape, class Voxel, int dim>
template <class RestrictedShape>
const typename BaseCandidateGenerator<AugmentedShape,Voxel,dim>::candidate_container&
BaseCandidateGenerator<AugmentedShape,Voxel,dim>::generatePeristenceCandidates(
        int width, bool silent) {
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

template <class AugmentedShape, class Voxel, int dim>
template <class RestrictedShape>
const typename BaseCandidateGenerator<AugmentedShape,Voxel,dim>::candidate_container&
BaseCandidateGenerator<AugmentedShape,Voxel,dim>::generatePeristenceCandidates(
        int width, std::unordered_set<Point>& lowerPoints, bool silent) {
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

template <class AugmentedShape, class Voxel, int dim>
template <class RestrictedShape>
const typename BaseCandidateGenerator<AugmentedShape,Voxel,dim>::candidate_container&
BaseCandidateGenerator<AugmentedShape,Voxel,dim>::generatePeristenceCandidates(
        int width, Point lowerPoint, bool silent) {
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

template <class AugmentedShape, class Voxel, int dim>
template <class ClosedRestrictedShape>
const typename BaseCandidateGenerator<AugmentedShape,Voxel,dim>::candidate_container&
BaseCandidateGenerator<AugmentedShape,Voxel,dim>::
generatePeristenceCandidatesWithGuarantee(int width, bool silent) {
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

template <class AugmentedShape, class Voxel, int dim>
template <class ClosedRestrictedShape>
const typename BaseCandidateGenerator<AugmentedShape,Voxel,dim>::candidate_container&
BaseCandidateGenerator<AugmentedShape,Voxel,dim>::
generatePeristenceCandidatesWithGuarantee(int width,
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

template <class AugmentedShape, class Voxel, int dim>
template <class ClosedRestrictedShape>
const typename BaseCandidateGenerator<AugmentedShape,Voxel,dim>::candidate_container&
BaseCandidateGenerator<AugmentedShape,Voxel,dim>::
generatePeristenceCandidatesWithGuarantee(int width, Point lowerPoint, bool silent) {
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

template <class AugmentedShape, class Voxel, int dim>
inline constexpr bool
BaseCandidateGenerator<AugmentedShape,Voxel,dim>::isAdditive(){
    return false;
}

template <class AugmentedShape, class Voxel, int dim>
const typename BaseCandidateGenerator<AugmentedShape,Voxel,dim>::candidate_container&
BaseCandidateGenerator<AugmentedShape,Voxel,dim>::_generatePeristenceCandidates(
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
                              << "; " << _candidates.size() << " so far" << std::endl;
                }

                Point upper{lower[0] + width, lower[1] + width, lower[2] + width};

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

template <class AugmentedShape, class Voxel, int dim>
const typename BaseCandidateGenerator<AugmentedShape,Voxel,dim>::candidate_container&
BaseCandidateGenerator<AugmentedShape,Voxel,dim>::_generatePeristenceCandidates(
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

template <class AugmentedShape, class Voxel, int dim>
const typename BaseCandidateGenerator<AugmentedShape,Voxel,dim>::candidate_container&
BaseCandidateGenerator<AugmentedShape,Voxel,dim>::_generatePeristenceCandidates(
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
        std::cout << "Found " << _candidates.size() << " in this box." << std::endl;
    }

    return _candidates;
}

template <class AugmentedShape, class Voxel, int dim>
template <class RestrictedShape>
void BaseCandidateGenerator<AugmentedShape,Voxel,dim>::
_generatePeristenceCandidatesInBox(Point &lowerCorner, Point &upperCorner) {
    using Skeleton = DualSkeleton<RestrictedShape, Voxel, dim, dim+1>;
    using Persistence = PersistentHomology<Skeleton, Voxel, dim, dim+1>;

    RestrictedShape restrShape(_shape, lowerCorner, upperCorner);
    Skeleton skeleton(restrShape);
    Persistence pers(skeleton);
    std::vector<Voxel> chain;
    voxel_container candidate;

    pers.reduceMatrix();

    for (typename Persistence::persistent_pair pair : pers.getPersistentPairs()) {
        if (pers.getTimestamp(pair.second) >= 2) {
            chain.clear();
            candidate.clear();
            pers.getCorrespondingVoxelChain(pair, chain);
            for (Voxel v : chain) {
                if (restrShape.containsOnlyInNeighborhood(v)){
                    candidate.insert(v);
                }
            }
            _candidates.push_back(candidate);
        }
    }
}

template <class AugmentedShape, class Voxel, int dim>
template <class ClosedRestrictedShape>
void BaseCandidateGenerator<AugmentedShape,Voxel,dim>::
_generatePeristenceCandidatesInBoxWithGuarantee(Point &lowerCorner, Point &upperCorner) {
    using Skeleton = DualSkeleton<ClosedRestrictedShape, Voxel, dim, dim+1>;
    using Persistence = PersistentHomology<Skeleton, Voxel, dim, dim+1>;
    using AugShape = shape::AugmentedShape<ClosedRestrictedShape, Voxel>;
    using ShapeOverlay = ShapeOverlay<AugShape, Voxel>;

    ClosedRestrictedShape restrShape(_shape, lowerCorner, upperCorner);
    Skeleton skeleton(restrShape);
    Persistence pers(skeleton);
    std::vector<Voxel> chain;
    voxel_container candidate;
    voxel_container adjacentShapeVoxels;

    AugShape augShape(restrShape);
    ShapeOverlay overlay(augShape);

    pers.reduceMatrix();

    for (typename Persistence::persistent_pair pair : pers.getPersistentPairs()) {
        if (pers.getTimestamp(pair.second) >= 2) {
            chain.clear();
            candidate.clear();
            adjacentShapeVoxels.clear();
            pers.getCorrespondingVoxelChain(pair, chain);
            for (Voxel v : chain) {
                if (restrShape.containsOnlyInNeighborhood(v)){
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



