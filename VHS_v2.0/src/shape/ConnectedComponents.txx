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
 * @file ConnectedComponents.txx
 * Contains the implementation of the methods in the \ref ConnectedComponents
 * class.
 **/

#include "ConnectedComponents.h"

using namespace shape_simplification::shape;

template <class Voxel>
ConnectedComponents<Voxel>::ConnectedComponents() : UnionFind<Voxel>() {}

template <class Voxel>
void ConnectedComponents<Voxel>::clear(){
    UnionFind<Voxel>::clear();
}

template <class Voxel>
ConnectedComponentsWithRemoval<Voxel>::ConnectedComponentsWithRemoval()
    : UnionFindDelete<Voxel>()
{}

template <class Voxel>
ConnectedComponentsWithRemoval<Voxel>::ConnectedComponentsWithRemoval(
        const ConnectedComponentsWithRemoval& cc)
    : UnionFindDelete<Voxel>(cc)
{}

template <class Voxel>
unsigned long long int ConnectedComponentsWithRemoval<Voxel>::getComponentSize(
        Voxel &v)
{
    return UnionFindDelete<Voxel>::getUnionSize(v);
}

template <class Voxel>
void ConnectedComponentsWithRemoval<Voxel>::getComponent(
        Voxel &v, std::vector<Voxel> &container)
{
    UnionFindDelete<Voxel>::getUnion(v, container);
}

template <class Voxel>
void ConnectedComponentsWithRemoval<Voxel>::clear(){
    UnionFindDelete<Voxel>::clear();
}
