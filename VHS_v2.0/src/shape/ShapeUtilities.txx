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
 * @file ShapeUtilities.txx
 * Contains the implementation of the methods in the classes:
 *  - \ref NeighborsIterator,
 *  - \ref NeighborsInIterator,
 *  - \ref ExtendedNeighborsIterator,
 *  - \ref ExtendedNeighborsInIterator.
 **/

#include "shape/ShapeUtilities.h"

using namespace shape_simplification::shape;

template <class Voxel>
NeighborsIterator<Voxel>::NeighborsIterator(const Voxel &voxel)
    : _base({voxel[0], voxel[1], voxel[2]}), _currentPosition(0) {
    while (!isEnd() && !_isValidPosition())
        ++_currentPosition;
}

template <class Voxel>
bool NeighborsIterator<Voxel>::isEnd() {
    return _currentPosition >= 6;
}

template <class Voxel>
void NeighborsIterator<Voxel>::next() {
    do {
        ++_currentPosition;
    } while (!isEnd() && !_isValidPosition());
}

template <class Voxel>
void NeighborsIterator<Voxel>::getCurrentValue(int &x, int &y, int &z)
{
    x = _base[0] + _shift[_currentPosition][0];
    y = _base[1] + _shift[_currentPosition][1];
    z = _base[2] + _shift[_currentPosition][2];
}

template <class Voxel> bool NeighborsIterator<Voxel>::_isValidPosition()
{
    for (unsigned int i = 0; i < 3; ++i) {
        if (_base[i] + _shift[_currentPosition][i] < 0) return false;
    }
    return true;
}

template <class Voxel>
ExtendedNeighborsIterator<Voxel>::ExtendedNeighborsIterator(const Voxel &voxel)
    : _base({voxel[0], voxel[1], voxel[2]}), _currentPosition(0)
{
    while (!isEnd() && !_isValidPosition())
        ++_currentPosition;
}

template <class Voxel>
bool ExtendedNeighborsIterator<Voxel>::isEnd()
{
    return _currentPosition >= 26;
}

template <class Voxel>
void ExtendedNeighborsIterator<Voxel>::next()
{
    do {
        ++_currentPosition;
    } while (!isEnd() && !_isValidPosition());
}

template <class Voxel>
void ExtendedNeighborsIterator<Voxel>::getCurrentValue(int &x, int &y, int &z)
{
    x = _base[0] + _shift[_currentPosition][0];
    y = _base[1] + _shift[_currentPosition][1];
    z = _base[2] + _shift[_currentPosition][2];
}

template <class Voxel>
bool ExtendedNeighborsIterator<Voxel>::_isValidPosition()
{
    for (unsigned int i = 0; i < 3; ++i) {
        if (_base[i] + _shift[_currentPosition][i] < 0) return false;
    }
    return true;
}

template <class Shape, class Voxel, Level level>
NeighborsInIterator<Shape, Voxel, level>::NeighborsInIterator(
        const Voxel &voxel, const Shape &shape)
    : NeighborsIterator<Voxel>(voxel), _shape(shape)
{
    while (!(NeighborsIterator<Voxel>::isEnd()) && !_isValidPosition()) {
        NeighborsIterator<Voxel>::next();
    }
}

template <class Shape, class Voxel, Level level>
void NeighborsInIterator<Shape, Voxel, level>::next()
{
    NeighborsIterator<Voxel>::next();

    while (!(NeighborsIterator<Voxel>::isEnd()) && !_isValidPosition()) {
        NeighborsIterator<Voxel>::next();
    }
}

template <class Shape, class Voxel, Level level>
bool NeighborsInIterator<Shape, Voxel, level>::_isValidPosition()
{
    int x, y, z;
    NeighborsIterator<Voxel>::getCurrentValue(x, y, z);
    return _shape.template contains<level>(x, y, z);
}

template <class Shape, class Voxel, Level level>
ExtendedNeighborsInIterator<Shape, Voxel, level>::ExtendedNeighborsInIterator(
        const Voxel &voxel, const Shape &shape)
    : ExtendedNeighborsIterator<Voxel>(voxel), _shape(shape)
{
    while (!(ExtendedNeighborsIterator<Voxel>::isEnd()) && !_isValidPosition())
    {
        ExtendedNeighborsIterator<Voxel>::next();
    }
}

template <class Shape, class Voxel, Level level>
void ExtendedNeighborsInIterator<Shape, Voxel, level>::next()
{
    ExtendedNeighborsIterator<Voxel>::next();
    while (!(ExtendedNeighborsIterator<Voxel>::isEnd()) && !_isValidPosition())
    {
        ExtendedNeighborsIterator<Voxel>::next();
    }
}

template <class Shape, class Voxel, Level level>
bool ExtendedNeighborsInIterator<Shape, Voxel, level>::_isValidPosition()
{
    int x, y, z;
    ExtendedNeighborsIterator<Voxel>::getCurrentValue(x, y, z);
    return _shape.template contains<level>(x, y, z);
}
