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
 * @file ClosedInvertedRestrictedShapeCopy.txx
 * Contains the implementation of the methods in the \ref ClosedInvertedRestrictedShapeCopy
 * class.
 **/

#include "ClosedInvertedRestrictedShapeCopy.h"

using namespace shape_simplification::shape;

template <class Shape, class Voxel>
ClosedInvertedRestrictedShapeCopy<Shape, Voxel>::ClosedInvertedRestrictedShapeCopy(
        const Shape &shape, Point &lowerBound, Point &upperBound)
    : InvertedRestrictedShapeCopy<Shape,Voxel>(shape, lowerBound, upperBound) {
    std::array<voxel_container, 3> &voxels = BaseShape<Voxel>::_getVoxels();
    int max = Voxel::getMaxCoordinateValue();

    if (lowerBound[0] > 0 || upperBound[0] < max){
        for (int y = std::max(lowerBound[1] - 1, 0);
             y <= std::min(upperBound[1] + 1, max);
             y++)
        {
            for (int z = std::max(lowerBound[2] - 1, 0);
                 z <= std::min(upperBound[2] + 1, max);
                 z++)
            {
                if (lowerBound[0] > 0){
                    Voxel vl(lowerBound[0] - 1, y, z);
                    voxels.at(0).insert(vl);
                    voxels.at(1).insert(vl);
                    voxels.at(2).insert(vl);
                }
                if (upperBound[0] < max){
                    Voxel vu(upperBound[0] + 1, y, z);
                    voxels.at(0).insert(vu);
                    voxels.at(1).insert(vu);
                    voxels.at(2).insert(vu);
                }
            }
        }
    }

    if (lowerBound[1] > 0 || upperBound[1] < max){
        for (int x = std::max(lowerBound[0], 0);
             x <= std::min(upperBound[0], max);
             x++)
        {
            for (int z = std::max(lowerBound[2] - 1, 0);
                 z <= std::min(upperBound[2] + 1, max);
                 z++)
            {
                if (lowerBound[1] > 0){
                    Voxel vl(x, lowerBound[1] - 1, z);
                    voxels.at(0).insert(vl);
                    voxels.at(1).insert(vl);
                    voxels.at(2).insert(vl);
                }
                if (upperBound[1] < max){
                    Voxel vu(x, upperBound[1] + 1, z);
                    voxels.at(0).insert(vu);
                    voxels.at(1).insert(vu);
                    voxels.at(2).insert(vu);
                }
            }
        }
    }

    if (lowerBound[2] > 0 || upperBound[2] < max){
        for (int x = std::max(lowerBound[0], 0);
             x <= std::min(upperBound[0], max);
             x++)
        {
            for (int y = std::max(lowerBound[1], 0);
                 y <= std::min(upperBound[1], max);
                 y++)
            {
                if (lowerBound[2] > 0){
                    Voxel vl(x, y, lowerBound[2] - 1);
                    voxels.at(0).insert(vl);
                    voxels.at(1).insert(vl);
                    voxels.at(2).insert(vl);
                }
                if (upperBound[2] < max){
                    Voxel vu(x, y, upperBound[2] + 1);
                    voxels.at(0).insert(vu);
                    voxels.at(1).insert(vu);
                    voxels.at(2).insert(vu);
                }
            }
        }
    }
}

template <class Shape, class Voxel>
bool ClosedInvertedRestrictedShapeCopy<Shape, Voxel>::_isWithinBounds(Voxel &v) {
    Point lowerBound = InvertedRestrictedShapeCopy<Shape, Voxel>::getLowerBound();
    Point upperBound = InvertedRestrictedShapeCopy<Shape, Voxel>::getUpperBound();
    return (v[0] >= lowerBound[0] - 1 && v[1] >= lowerBound[1] - 1 &&
            v[2] >= lowerBound[2] - 1 && v[0] <= upperBound[0] + 1 &&
            v[1] <= upperBound[1] + 1 && v[2] <= upperBound[2] + 1);
}
