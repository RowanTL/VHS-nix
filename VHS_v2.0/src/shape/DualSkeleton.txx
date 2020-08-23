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
 * @file DualSkeleton.txx
 * Contains the implementation of the methods in the \ref DualSkeleton class.
 **/

#include "DualSkeleton.h"

using namespace shape_simplification::shape;

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
DualSkeleton<Shape, Voxel, minDim, maxDim>::DualSkeleton(Shape &shape)
    : BSkeleton(shape)
{
    _constructDualSkeleton();
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
DualSkeleton<Shape, Voxel, minDim, maxDim>::DualSkeleton()
    : BSkeleton() {}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
DualSkeleton<Shape, Voxel, minDim, maxDim>::~DualSkeleton(){}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline constexpr bool
DualSkeleton<Shape, Voxel, minDim, maxDim>::isPrimal(){
    return false;
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
DualSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        vertex &vertex,
        std::array<Voxel, 1>& container)
{
    container[0] = Voxel(
                vertex::vertexToCoordinate(vertex[0], 0),
                vertex::vertexToCoordinate(vertex[0], 1),
                vertex::vertexToCoordinate(vertex[0], 2)
            );
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
DualSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        vertex &vertex,
        std::vector<Voxel>& container)
{
    container.resize(1);
    container[0] = Voxel(
                vertex::vertexToCoordinate(vertex[0], 0),
                vertex::vertexToCoordinate(vertex[0], 1),
                vertex::vertexToCoordinate(vertex[0], 2)
            );
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
DualSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        edge &edge,
        std::array<Voxel, 2>& container)
{
    container[0] = Voxel(
                vertex::vertexToCoordinate(edge[0], 0),
                vertex::vertexToCoordinate(edge[0], 1),
                vertex::vertexToCoordinate(edge[0], 2)
            );
    container[1] = Voxel(
                vertex::vertexToCoordinate(edge[1], 0),
                vertex::vertexToCoordinate(edge[1], 1),
                vertex::vertexToCoordinate(edge[1], 2)
            );
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
DualSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        edge &edge,
        std::vector<Voxel>& container)
{
    container.resize(2);
    container[0] = Voxel(
                vertex::vertexToCoordinate(edge[0], 0),
                vertex::vertexToCoordinate(edge[0], 1),
                vertex::vertexToCoordinate(edge[0], 2)
            );
    container[1] = Voxel(
                vertex::vertexToCoordinate(edge[1], 0),
                vertex::vertexToCoordinate(edge[1], 1),
                vertex::vertexToCoordinate(edge[1], 2)
            );
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
DualSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        square &square,
        std::array<Voxel, 4>& container)
{
    container[0] = Voxel(
                vertex::vertexToCoordinate(square[0], 0),
                vertex::vertexToCoordinate(square[0], 1),
                vertex::vertexToCoordinate(square[0], 2)
            );
    container[1] = Voxel(
                vertex::vertexToCoordinate(square[1], 0),
                vertex::vertexToCoordinate(square[1], 1),
                vertex::vertexToCoordinate(square[1], 2)
            );
    container[2] = Voxel(
                vertex::vertexToCoordinate(square[2], 0),
                vertex::vertexToCoordinate(square[2], 1),
                vertex::vertexToCoordinate(square[2], 2)
            );
    container[3] = Voxel(
                vertex::vertexToCoordinate(square[3], 0),
                vertex::vertexToCoordinate(square[3], 1),
                vertex::vertexToCoordinate(square[3], 2)
            );
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
DualSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        square &square,
        std::vector<Voxel>& container)
{
    container.resize(4);
    container[0] = Voxel(
                vertex::vertexToCoordinate(square[0], 0),
                vertex::vertexToCoordinate(square[0], 1),
                vertex::vertexToCoordinate(square[0], 2)
            );
    container[1] = Voxel(
                vertex::vertexToCoordinate(square[1], 0),
                vertex::vertexToCoordinate(square[1], 1),
                vertex::vertexToCoordinate(square[1], 2)
            );
    container[2] = Voxel(
                vertex::vertexToCoordinate(square[2], 0),
                vertex::vertexToCoordinate(square[2], 1),
                vertex::vertexToCoordinate(square[2], 2)
            );
    container[3] = Voxel(
                vertex::vertexToCoordinate(square[3], 0),
                vertex::vertexToCoordinate(square[3], 1),
                vertex::vertexToCoordinate(square[3], 2)
            );
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
DualSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        cube &cube,
        std::array<Voxel, 8>& container)
{
    int bx = vertex::vertexToCoordinate(cube[0], 0);
    int by = vertex::vertexToCoordinate(cube[0], 1);
    int bz = vertex::vertexToCoordinate(cube[0], 2);

    container[0] = Voxel(bx, by, bz);
    container[1] = Voxel(bx + 1, by, bz);
    container[2] = Voxel(bx, by + 1, bz);
    container[3] = Voxel(bx, by, bz + 1);
    container[4] = Voxel(bx + 1, by + 1, bz);
    container[5] = Voxel(bx + 1, by, bz + 1);
    container[6] = Voxel(bx, by + 1, bz + 1);
    container[7] = Voxel(bx + 1, by + 1, bz + 1);
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline void
DualSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxels(
        cube &cube,
        std::vector<Voxel>& container)
{
    int bx = vertex::vertexToCoordinate(cube[0], 0);
    int by = vertex::vertexToCoordinate(cube[0], 1);
    int bz = vertex::vertexToCoordinate(cube[0], 2);
    container.resize(8);

    container[0] = Voxel(bx, by, bz);
    container[1] = Voxel(bx + 1, by, bz);
    container[2] = Voxel(bx, by + 1, bz);
    container[3] = Voxel(bx, by, bz + 1);
    container[4] = Voxel(bx + 1, by + 1, bz);
    container[5] = Voxel(bx + 1, by, bz + 1);
    container[6] = Voxel(bx, by + 1, bz + 1);
    container[7] = Voxel(bx + 1, by + 1, bz + 1);
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
template <unsigned int dim,
          typename DualSkeleton<Shape, Voxel, minDim, maxDim>::Level level>
void
DualSkeleton<Shape, Voxel, minDim, maxDim>::getCorrespondingVoxelsIn(
        Cell<dim>& cell,
        std::vector<Voxel>& container)
{
    constexpr unsigned int size = 1 << dim;
    Shape& shape = BSkeleton::_getShape();
    std::array<Voxel, size> allVoxels;
    getCorrespondingVoxels(cell, allVoxels);

    for (unsigned int i = 0; i < size; ++i) {
        if (shape.template contains<level>(allVoxels[i]))
            container.push_back(allVoxels[i]);
    }
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
void
DualSkeleton<Shape, Voxel, minDim, maxDim>::_constructDualSkeleton()
{
    Shape &shape = BSkeleton::_getShape();

    for (typename Shape::voxel_iterator it =
         shape.template begin<Shape::NEIGHBORHOOD>();
         it != shape.template end<Shape::NEIGHBORHOOD>(); ++it)
    {
        if constexpr (minDim == 0)
                _insertDualVertex(*it);

        if constexpr (minDim <= 1 && maxDim >= 1)
                _findSquaresAndInsertDualEdges(*it);

        if constexpr (minDim <= 2 && maxDim >= 2)
                _findEdgesAndInsertDualSquares(*it);

        if constexpr (maxDim == 3)
                _findVerticesAndInsertDualCubes(*it);
    }
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
void
DualSkeleton<Shape, Voxel, minDim, maxDim>::_insertDualVertex(const Voxel &v)
{
    std::vector<Point> coordinates(1, {v[0], v[1], v[2]});
    vertex vertex(coordinates, BSkeleton::_getShape().getMinLevel(v));

    BSkeleton::_insertCell(vertex);
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
void
DualSkeleton<Shape, Voxel, minDim, maxDim>::_findSquaresAndInsertDualEdges(
        const Voxel &v)
{
    for (PositiveSquareIterator<Voxel> it(v); !it.isEnd(); it.next()) {
        std::vector<Point>& vertices = it.getCurrentValue();
        square square(vertices, 0);
        _insertDualCellIfExisting(square);
    }
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
void
DualSkeleton<Shape, Voxel, minDim, maxDim>::_findEdgesAndInsertDualSquares(
        const Voxel &v)
{
    for (PositiveEdgeIterator<Voxel> it(v); !it.isEnd(); it.next()) {
        std::vector<Point>& vertices = it.getCurrentValue();
        edge edge(vertices, 0);
        _insertDualCellIfExisting(edge);
    }
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
void
DualSkeleton<Shape, Voxel, minDim, maxDim>::_findVerticesAndInsertDualCubes(
        const Voxel &v)
{
    std::vector<Point> coordinates(1, {v[0] + 1, v[1] + 1, v[2] + 1});
    vertex vertex(coordinates, 0);

    _insertDualCellIfExisting(vertex);
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
template<unsigned int dim>
void
DualSkeleton<Shape, Voxel, minDim, maxDim>::_insertDualCellIfExisting(
        Cell<dim>& cell)
{
    constexpr unsigned int dualDim = 3 - dim;
    constexpr unsigned int size = 1 << dualDim;
    int timestamp = 0;
    std::vector<Point> coordinates(size);
    std::array<Voxel, size> adj;
    PSkeleton::getCorrespondingVoxels(cell, adj);
    unsigned int adjExisting = 0;
    Shape &shape = BSkeleton::_getShape();

    for (unsigned int i = 0; i < size; ++i) {
        if (shape.template contains<Shape::NEIGHBORHOOD>(adj[i]))
            ++adjExisting;
    }

    if (adjExisting != size)
        return;

    for (unsigned int i = 0; i < size; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            coordinates.at(i)[j] = adj[i][j];
        }
        int tmp = shape.getMinLevel(adj[i]);
        if (timestamp < tmp)
            timestamp = tmp;
    }

    Cell<dualDim> dualCell(coordinates, timestamp);
    BSkeleton::_insertCell(dualCell);
}



