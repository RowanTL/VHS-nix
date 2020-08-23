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
 * @file VoxelSkeletonIterators.txx
 * Contains the implementation of the methods in the classes:
 *  - \ref VertexIterator,
 *  - \ref EdgeIterator,
 *  - \ref SquareIterator,
 *  - \ref DualEdgeIterator,
 *  - \ref DualSquareIterator,
 *  - \ref DualCubeIterator,
 *  - \ref NeighborsIterator,
 *  - \ref NeighborsInIterator,
 *  - \ref ExtendedNeighborsIterator,
 *  - \ref ExtendedNeighborsInIterator.
 **/

#include "shape/VoxelSkeletonIterators.h"

using namespace shape_simplification::shape;

template <class Voxel>
VertexIterator<Voxel>::VertexIterator(const Voxel &voxel)
    : _base({voxel[0], voxel[1], voxel[2]}),
      _currentPosition(0),
      _currentCoordinates(_tupleLength, _base) {}

template <class Voxel> bool VertexIterator<Voxel>::isEnd() {
    return _currentPosition >= _nberOfCells;
}

template <class Voxel> void VertexIterator<Voxel>::next() {
    ++_currentPosition;
}

template <class Voxel>
typename VertexIterator<Voxel>::coordinate_type&
VertexIterator<Voxel>::getCurrentValue() {
    for (unsigned int i = 0; i < _tupleLength; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            _currentCoordinates.at(i)[j] = _base[j] + _shift[_currentPosition][3 * i + j];
        }
    }

    return _currentCoordinates;
}

template <class Voxel>
EdgeIterator<Voxel>::EdgeIterator(const Voxel &voxel)
    : _base({voxel[0], voxel[1], voxel[2]}),
      _currentPosition(0),
      _currentCoordinates(_tupleLength, _base) {}

template <class Voxel> bool EdgeIterator<Voxel>::isEnd() {
    return _currentPosition >= _nberOfCells;
}

template <class Voxel> void EdgeIterator<Voxel>::next() {
    ++_currentPosition;
}

template <class Voxel>
typename EdgeIterator<Voxel>::coordinate_type&
EdgeIterator<Voxel>::getCurrentValue() {
    for (unsigned int i = 0; i < _tupleLength; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            _currentCoordinates.at(i)[j] = _base[j] + _shift[_currentPosition][3 * i + j];
        }
    }

    return _currentCoordinates;
}

template <class Voxel>
PositiveEdgeIterator<Voxel>::PositiveEdgeIterator(const Voxel &voxel)
    : _base({voxel[0], voxel[1], voxel[2]}),
      _currentPosition(0),
      _currentCoordinates(_tupleLength, _base) {}

template <class Voxel> bool PositiveEdgeIterator<Voxel>::isEnd() {
    return _currentPosition >= _nberOfCells;
}

template <class Voxel> void PositiveEdgeIterator<Voxel>::next() {
    ++_currentPosition;
}

template <class Voxel>
typename PositiveEdgeIterator<Voxel>::coordinate_type&
PositiveEdgeIterator<Voxel>::getCurrentValue() {
    for (unsigned int i = 0; i < _tupleLength; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            _currentCoordinates.at(i)[j] = _base[j] + _shift[_currentPosition][3 * i + j];
        }
    }

    return _currentCoordinates;
}

template <class Voxel>
SquareIterator<Voxel>::SquareIterator(const Voxel &voxel)
    : _base({voxel[0], voxel[1], voxel[2]}),
      _currentPosition(0),
      _currentCoordinates(_tupleLength, _base) {}

template <class Voxel> bool SquareIterator<Voxel>::isEnd() {
    return _currentPosition >= _nberOfCells;
}

template <class Voxel> void SquareIterator<Voxel>::next() {
    ++_currentPosition;
}

template <class Voxel>
typename SquareIterator<Voxel>::coordinate_type&
SquareIterator<Voxel>::getCurrentValue() {
    for (unsigned int i = 0; i < _tupleLength; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            _currentCoordinates.at(i)[j] = _base[j] + _shift[_currentPosition][3 * i + j];
        }
    }

    return _currentCoordinates;
}

template <class Voxel>
PositiveSquareIterator<Voxel>::PositiveSquareIterator(const Voxel &voxel)
    : _base({voxel[0], voxel[1], voxel[2]}),
      _currentPosition(0),
      _currentCoordinates(_tupleLength, _base) {}

template <class Voxel> bool PositiveSquareIterator<Voxel>::isEnd() {
    return _currentPosition >= _nberOfCells;
}

template <class Voxel> void PositiveSquareIterator<Voxel>::next() {
    ++_currentPosition;
}

template <class Voxel>
typename PositiveSquareIterator<Voxel>::coordinate_type&
PositiveSquareIterator<Voxel>::getCurrentValue() {
    for (unsigned int i = 0; i < _tupleLength; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            _currentCoordinates.at(i)[j] = _base[j] + _shift[_currentPosition][3 * i + j];
        }
    }

    return _currentCoordinates;
}

template <class Voxel>
DualEdgeIterator<Voxel>::DualEdgeIterator(const Voxel &voxel)
    : _base({voxel[0], voxel[1], voxel[2]}),
      _currentPosition(0),
      _currentCoordinates(_tupleLength, _base) {}

template <class Voxel> bool DualEdgeIterator<Voxel>::isEnd() {
    return _currentPosition >= _nberOfCells;
}

template <class Voxel> void DualEdgeIterator<Voxel>::next() {
    do {
        ++_currentPosition;
    } while (_currentPosition < _nberOfCells &&
             (_base[0] + _shift[_currentPosition][0] < 0 ||
              _base[1] + _shift[_currentPosition][1] < 0 ||
              _base[2] + _shift[_currentPosition][2] < 0));
}

template <class Voxel>
typename DualEdgeIterator<Voxel>::coordinate_type&
DualEdgeIterator<Voxel>::getCurrentValue() {
    for (unsigned int i = 0; i < _tupleLength; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            _currentCoordinates.at(i)[j] = _base[j] + _shift[_currentPosition][3 * i + j];
        }
    }

    return _currentCoordinates;
}

template <class Voxel>
DualSquareIterator<Voxel>::DualSquareIterator(const Voxel &voxel)
    : _base({voxel[0], voxel[1], voxel[2]}),
      _currentPosition(0),
      _currentCoordinates(_tupleLength, _base) {}

template <class Voxel> bool DualSquareIterator<Voxel>::isEnd() {
    return _currentPosition >= _nberOfCells;
}

template <class Voxel> void DualSquareIterator<Voxel>::next() {
    do {
        ++_currentPosition;
    } while (_currentPosition < _nberOfCells &&
             (_base[0] + _shift[_currentPosition][3] < 0 ||
              _base[0] + _shift[_currentPosition][6] < 0 ||
              _base[0] + _shift[_currentPosition][9] < 0 ||
              _base[1] + _shift[_currentPosition][7] < 0 ||
              _base[1] + _shift[_currentPosition][10] < 0 ||
              _base[2] + _shift[_currentPosition][5] < 0 ||
              _base[2] + _shift[_currentPosition][11] < 0));
}

template <class Voxel>
typename DualSquareIterator<Voxel>::coordinate_type&
DualSquareIterator<Voxel>::getCurrentValue() {
    for (unsigned int i = 0; i < _tupleLength; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            _currentCoordinates.at(i)[j] = _base[j] + _shift[_currentPosition][3 * i + j];
        }
    }

    return _currentCoordinates;
}

template <class Voxel>
DualCubeIterator<Voxel>::DualCubeIterator(const Voxel &voxel)
    : _base({voxel[0], voxel[1], voxel[2]}),
      _currentPosition(0),
      _currentCoordinates(_tupleLength, _base) {}

template <class Voxel> bool DualCubeIterator<Voxel>::isEnd() {
    return _currentPosition >= _nberOfCells;
}

template <class Voxel> void DualCubeIterator<Voxel>::next() {
    do {
        ++_currentPosition;
    } while (_currentPosition < _nberOfCells &&
             (_base[0] + _shift[_currentPosition][0] < 0 ||
              _base[0] + _shift[_currentPosition][3] < 0 ||
              _base[0] + _shift[_currentPosition][6] < 0 ||
              _base[0] + _shift[_currentPosition][9] < 0 ||
              _base[0] + _shift[_currentPosition][12] < 0 ||
              _base[0] + _shift[_currentPosition][15] < 0 ||
              _base[0] + _shift[_currentPosition][18] < 0 ||
              _base[0] + _shift[_currentPosition][21] < 0 ||
              _base[1] + _shift[_currentPosition][1] < 0 ||
              _base[1] + _shift[_currentPosition][4] < 0 ||
              _base[1] + _shift[_currentPosition][7] < 0 ||
              _base[1] + _shift[_currentPosition][10] < 0 ||
              _base[1] + _shift[_currentPosition][13] < 0 ||
              _base[1] + _shift[_currentPosition][16] < 0 ||
              _base[1] + _shift[_currentPosition][19] < 0 ||
              _base[1] + _shift[_currentPosition][22] < 0 ||
              _base[2] + _shift[_currentPosition][2] < 0 ||
              _base[2] + _shift[_currentPosition][5] < 0 ||
              _base[2] + _shift[_currentPosition][8] < 0 ||
              _base[2] + _shift[_currentPosition][11] < 0 ||
              _base[2] + _shift[_currentPosition][14] < 0 ||
              _base[2] + _shift[_currentPosition][17] < 0 ||
              _base[2] + _shift[_currentPosition][20] < 0 ||
              _base[2] + _shift[_currentPosition][23] < 0));
}

template <class Voxel>
typename DualCubeIterator<Voxel>::coordinate_type&
DualCubeIterator<Voxel>::getCurrentValue() {
    for (unsigned int i = 0; i < _tupleLength; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            _currentCoordinates.at(i)[j] = _base[j] + _shift[_currentPosition][3 * i + j];
        }
    }

    return _currentCoordinates;
}

template <class Voxel>
NeighborsIterator<Voxel>::NeighborsIterator(const Voxel &voxel)
    : _base({voxel[0], voxel[1], voxel[2]}), _currentPosition(0) {
    while (!isEnd() && !_isValidPosition())
        ++_currentPosition;
}

template <class Voxel> bool NeighborsIterator<Voxel>::isEnd() {
    return _currentPosition >= 6;
}

template <class Voxel> void NeighborsIterator<Voxel>::next() {
    do {
        ++_currentPosition;
    } while (!isEnd() && !_isValidPosition());
}

template <class Voxel> Voxel NeighborsIterator<Voxel>::getCurrentValue() {
    return Voxel(_base[0] + _shift[_currentPosition][0],
            _base[1] + _shift[_currentPosition][1],
            _base[2] + _shift[_currentPosition][2]);
}

template <class Voxel> bool NeighborsIterator<Voxel>::_isValidPosition() {
    for (unsigned int i = 0; i < 3; ++i) {
        if (_base[i] + _shift[_currentPosition][i] < 0) return false;
    }
    return true;
}

template <class Voxel>
ExtendedNeighborsIterator<Voxel>::ExtendedNeighborsIterator(const Voxel &voxel)
    : _base({voxel[0], voxel[1], voxel[2]}), _currentPosition(0) {
    while (!isEnd() && !_isValidPosition())
        ++_currentPosition;
}

template <class Voxel> bool ExtendedNeighborsIterator<Voxel>::isEnd() {
    return _currentPosition >= 26;
}

template <class Voxel> void ExtendedNeighborsIterator<Voxel>::next() {
    do {
        ++_currentPosition;
    } while (!isEnd() && !_isValidPosition());
}

template <class Voxel>
Voxel ExtendedNeighborsIterator<Voxel>::getCurrentValue() {
    return Voxel(_base[0] + _shift[_currentPosition][0],
            _base[1] + _shift[_currentPosition][1],
            _base[2] + _shift[_currentPosition][2]);
}

template <class Voxel>
bool ExtendedNeighborsIterator<Voxel>::_isValidPosition() {
    for (unsigned int i = 0; i < 3; ++i) {
        if (_base[i] + _shift[_currentPosition][i] < 0) return false;
    }
    return true;
}

template <class Shape, class Voxel, typename Shape::Level level>
NeighborsInIterator<Shape, Voxel, level>::NeighborsInIterator(
        const Voxel &voxel, const Shape &shape)
    : NeighborsIterator<Voxel>(voxel), _shape(shape) {
    while (!(NeighborsIterator<Voxel>::isEnd()) && !_isValidPosition()) {
        NeighborsIterator<Voxel>::next();
    }
}

template <class Shape, class Voxel, typename Shape::Level level>
void NeighborsInIterator<Shape, Voxel, level>::next() {
    NeighborsIterator<Voxel>::next();

    while (!(NeighborsIterator<Voxel>::isEnd()) && !_isValidPosition()) {
        NeighborsIterator<Voxel>::next();
    }
}

template <class Shape, class Voxel, typename Shape::Level level>
bool NeighborsInIterator<Shape, Voxel, level>::_isValidPosition() {
    Voxel v = NeighborsIterator<Voxel>::getCurrentValue();
    return _shape.template contains<level>(v);
}

template <class Shape, class Voxel, typename Shape::Level level>
ExtendedNeighborsInIterator<Shape, Voxel, level>::ExtendedNeighborsInIterator(
        const Voxel &voxel, const Shape &shape)
    : ExtendedNeighborsIterator<Voxel>(voxel), _shape(shape) {
    while (!(ExtendedNeighborsIterator<Voxel>::isEnd()) && !_isValidPosition()) {
        ExtendedNeighborsIterator<Voxel>::next();
    }
}

template <class Shape, class Voxel, typename Shape::Level level>
void ExtendedNeighborsInIterator<Shape, Voxel, level>::next() {
    ExtendedNeighborsIterator<Voxel>::next();
    while (!(ExtendedNeighborsIterator<Voxel>::isEnd()) && !_isValidPosition()) {
        ExtendedNeighborsIterator<Voxel>::next();
    }
}

template <class Shape, class Voxel, typename Shape::Level level>
bool ExtendedNeighborsInIterator<Shape, Voxel, level>::_isValidPosition() {
    Voxel v = ExtendedNeighborsIterator<Voxel>::getCurrentValue();
    return _shape.template contains<level>(v);
}
