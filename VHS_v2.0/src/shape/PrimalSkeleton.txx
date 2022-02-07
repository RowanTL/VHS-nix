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
 * @file PrimalSkeleton.txx
 * Contains the implementation of the methods in the \ref PrimalSkeleton class.
 **/

#include "PrimalSkeleton.h"

using namespace shape_simplification::shape;

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::PrimalSkeleton(Shape &shape)
    : BSkeleton(shape)
{
    _constructPrimalSkeleton();
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::PrimalSkeleton()
    : BSkeleton() {}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::~PrimalSkeleton(){}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline constexpr bool
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::isPrimal(){
    return true;
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        vertex &vertex,
        std::array<Voxel, 8>& container)
{
    int bx = vertex::vertexToCoordinate(vertex[0], 0);
    int by = vertex::vertexToCoordinate(vertex[0], 1);
    int bz = vertex::vertexToCoordinate(vertex[0], 2);

    container[0] = Voxel(bx, by, bz);
    container[1] = Voxel(bx - 1, by, bz);
    container[2] = Voxel(bx, by - 1, bz);
    container[3] = Voxel(bx, by, bz - 1);
    container[4] = Voxel(bx - 1, by - 1, bz);
    container[5] = Voxel(bx - 1, by, bz - 1);
    container[6] = Voxel(bx, by - 1, bz - 1);
    container[7] = Voxel(bx - 1, by - 1, bz - 1);
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        vertex &vertex,
        std::vector<Voxel>& container)
{
    int bx = vertex::vertexToCoordinate(vertex[0], 0);
    int by = vertex::vertexToCoordinate(vertex[0], 1);
    int bz = vertex::vertexToCoordinate(vertex[0], 2);
    container.resize(8);

    container[0] = Voxel(bx, by, bz);
    container[1] = Voxel(bx - 1, by, bz);
    container[2] = Voxel(bx, by - 1, bz);
    container[3] = Voxel(bx, by, bz - 1);
    container[4] = Voxel(bx - 1, by - 1, bz);
    container[5] = Voxel(bx - 1, by, bz - 1);
    container[6] = Voxel(bx, by - 1, bz - 1);
    container[7] = Voxel(bx - 1, by - 1, bz - 1);
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        edge &edge,
        std::array<Voxel, 4>& container)
{
    int tailX = vertex::vertexToCoordinate(edge[0], 0);
    int tailY = vertex::vertexToCoordinate(edge[0], 1);
    int tailZ = vertex::vertexToCoordinate(edge[0], 2);
    int maskX = vertex::vertexToCoordinate(edge[1], 0) - tailX - 1;
    int maskY = vertex::vertexToCoordinate(edge[1], 1) - tailY - 1;
    int maskZ = vertex::vertexToCoordinate(edge[1], 2) - tailZ - 1;

    container[0] = Voxel(tailX, tailY, tailZ);
    container[1] = Voxel(tailX, tailY + maskY, tailZ + maskZ);
    container[2] = Voxel(tailX + maskX, tailY, tailZ + maskZ);
    container[3] = Voxel(tailX + maskX, tailY + maskY, tailZ);
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        edge &edge,
        std::vector<Voxel>& container)
{
    int tailX = vertex::vertexToCoordinate(edge[0], 0);
    int tailY = vertex::vertexToCoordinate(edge[0], 1);
    int tailZ = vertex::vertexToCoordinate(edge[0], 2);
    int maskX = vertex::vertexToCoordinate(edge[1], 0) - tailX - 1;
    int maskY = vertex::vertexToCoordinate(edge[1], 1) - tailY - 1;
    int maskZ = vertex::vertexToCoordinate(edge[1], 2) - tailZ - 1;
    container.resize(4);

    container[0] = Voxel(tailX, tailY, tailZ);
    container[1] = Voxel(tailX, tailY + maskY, tailZ + maskZ);
    container[2] = Voxel(tailX + maskX, tailY, tailZ + maskZ);
    container[3] = Voxel(tailX + maskX, tailY + maskY, tailZ);
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        square &square,
        std::array<Voxel, 2>& container)
{
    int tailX = vertex::vertexToCoordinate(square[0], 0);
    int tailY = vertex::vertexToCoordinate(square[0], 1);
    int tailZ = vertex::vertexToCoordinate(square[0], 2);
    int maskX = vertex::vertexToCoordinate(square[3], 0) - tailX - 1;
    int maskY = vertex::vertexToCoordinate(square[3], 1) - tailY - 1;
    int maskZ = vertex::vertexToCoordinate(square[3], 2) - tailZ - 1;

    container[0] = Voxel(tailX, tailY, tailZ);
    container[1] = Voxel(tailX + maskX, tailY + maskY, tailZ + maskZ);
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        square &square,
        std::vector<Voxel>& container)
{
    int tailX = vertex::vertexToCoordinate(square[0], 0);
    int tailY = vertex::vertexToCoordinate(square[0], 1);
    int tailZ = vertex::vertexToCoordinate(square[0], 2);
    int maskX = vertex::vertexToCoordinate(square[3], 0) - tailX - 1;
    int maskY = vertex::vertexToCoordinate(square[3], 1) - tailY - 1;
    int maskZ = vertex::vertexToCoordinate(square[3], 2) - tailZ - 1;
    container.resize(2);

    container[0] = Voxel(tailX, tailY, tailZ);
    container[1] = Voxel(tailX + maskX, tailY + maskY, tailZ + maskZ);
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        cube &cube,
        std::array<Voxel, 1>& container)
{
    container[0] = Voxel(
                vertex::vertexToCoordinate(cube[0], 0),
                vertex::vertexToCoordinate(cube[0], 1),
                vertex::vertexToCoordinate(cube[0], 2)
            );
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        cube &cube,
        std::vector<Voxel>& container)
{
    container.resize(1);
    container[0] = Voxel(
                vertex::vertexToCoordinate(cube[0], 0),
                vertex::vertexToCoordinate(cube[0], 1),
                vertex::vertexToCoordinate(cube[0], 2)
            );
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
template <unsigned int dim, Level level>
void
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxelsIn(
        Cell<dim>& cell,
        std::vector<Voxel>& container)
{
    constexpr unsigned int size = 1 << (3 - dim);
    Shape& shape = BSkeleton::_getShape();
    std::array<Voxel, size> allVoxels;
    container.clear();
    getCorrespondingVoxels(cell, allVoxels);

    for (unsigned int i = 0; i < size; ++i) {
        if (shape.template contains<level>(allVoxels[i][0],
                                           allVoxels[i][1],
                                           allVoxels[i][2]))
            container.push_back(allVoxels[i]);
    }
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
void
PrimalSkeleton<Shape, Voxel, minDim, maxDim>::_constructPrimalSkeleton()
{
    //To do: has to ensure that no cell is inserted twice.
    //Beside for the static methods, this class is not used,
    //so it is not priority.
}




