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

#pragma once

/**
 * @file VoxelSkeletonIterators.h
 * Contains the declaration of the classes and structures:
 *  - \ref VertexIterator,
 *  - \ref EdgeIterator,
 *  - \ref PositiveEdgeIterator,
 *  - \ref SquareIterator,
 *  - \ref PositiveSquareIterator,
 *  - \ref CubeCoordinates,
 *  - \ref DualEdgeIterator,
 *  - \ref DualSquareIterator,
 *  - \ref DualCubeIterator.
 **/

#include <vector>

#include "util/Point.h"

namespace shape_simplification{ namespace shape {

using util::Point;

template <class Voxel>
/**
 * class VertexIterator
 * @brief Simplified iterator class to iterate over the vertices of a given
 * voxel.
 * @tparam Voxel Voxel class to be used.
 **/
class VertexIterator {
public:
    using coordinate_type = std::vector<Point>;

    /**
     * @brief Constructor.
     * @param voxel Voxel to iterate over.
     */
    VertexIterator(const Voxel &voxel);

    /**
     * @brief Tests if the iterator arrived at the end.
     * @return True if it is the end, false otherwise.
     */
    bool isEnd();
    /**
     * @brief Let the iterator go to the next step.
     */
    void next();
    /**
     * @brief Returns the value of the current position of the iterator.
     * @return The value of the current position of the iterator.
     */
    coordinate_type& getCurrentValue();

private:
    static constexpr unsigned int _nberOfCells = 8; ///< Number of cells to iterate over.
    static constexpr unsigned int _tupleLength = 1; ///< Number of vertices in the cell.
    static constexpr char _shift[_nberOfCells][_tupleLength * 3] =
    {                                               ///< Shifts on the coordinates to be applied
        {0, 0, 0},                                  ///< when the iterator goes to the next step.
        {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0},
        {1, 0, 1}, {1, 1, 0}, {1, 1, 1}
    };
    const Point _base;                              ///< Starting coordinates of the iterator.
    int _currentPosition;                           ///< Current number of steps of the iterator.
    coordinate_type _currentCoordinates;            ///< Latest computed coordinates.
};

template <class Voxel>
/**
 * class EdgeIterator
 * @brief Simplified iterator class to iterate over the edges of a given voxel.
 * @tparam Voxel Voxel class to be used.
 **/
class EdgeIterator {
public:
    using coordinate_type = std::vector<Point>;

    /**
     * @brief Constructor.
     * @param voxel Voxel to iterate over.
     */
    EdgeIterator(const Voxel &voxel);

    /**
     * @brief Tests if the iterator arrived at the end.
     * @return True if it is the end, false otherwise.
     */
    bool isEnd();
    /**
     * @brief Let the iterator go to the next step.
     */
    void next();
    /**
     * @brief Returns the value of the current position of the iterator.
     * @return The value of the current position of the iterator.
     */
    coordinate_type& getCurrentValue();

private:
    static constexpr unsigned int _nberOfCells = 12;    ///< Number of cells to iterate over.
    static constexpr unsigned int _tupleLength = 2;     ///< Number of vertices in the cell.
    static constexpr char _shift[_nberOfCells][_tupleLength * 3] =
    {                                                   ///< Shifts on the coordinates to be applied
        {0, 0, 0, 0, 0, 1},                             ///< when the iterator goes to the next step.
        {0, 0, 0, 0, 1, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 1, 0, 1, 1},
        {0, 0, 1, 1, 0, 1}, {0, 1, 0, 0, 1, 1}, {0, 1, 0, 1, 1, 0},
        {0, 1, 1, 1, 1, 1}, {1, 0, 0, 1, 0, 1}, {1, 0, 0, 1, 1, 0},
        {1, 0, 1, 1, 1, 1}, {1, 1, 0, 1, 1, 1}
    };
    const Point _base;                                  ///< Starting coordinates of the iterator.
    int _currentPosition;                               ///< Current number of steps of the iterator.
    coordinate_type _currentCoordinates;                ///< Latest computed coordinates.
};

template <class Voxel>
/**
 * class PositiveEdgeIterator
 * @brief Simplified iterator class to iterate over
 * the positive edges of a given voxel.
 * @tparam Voxel Voxel class to be used.
 **/
class PositiveEdgeIterator {
public:
    using coordinate_type = std::vector<Point>;

    /**
     * @brief Constructor.
     * @param voxel Voxel to iterate over.
     */
    PositiveEdgeIterator(const Voxel &voxel);

    /**
     * @brief Tests if the iterator arrived at the end.
     * @return True if it is the end, false otherwise.
     */
    bool isEnd();
    /**
     * @brief Let the iterator go to the next step.
     */
    void next();
    /**
     * @brief Returns the value of the current position of the iterator.
     * @return The value of the current position of the iterator.
     */
    coordinate_type& getCurrentValue();

private:
    static constexpr unsigned int _nberOfCells = 3; ///< Number of cells to iterate over.
    static constexpr unsigned int _tupleLength = 2; ///< Number of vertices in the cell.
    static constexpr char _shift[_nberOfCells][_tupleLength * 3] =
    {                                               ///< Shifts on the coordinates to be applied
        {0, 1, 1, 1, 1, 1},                         ///< when the iterator goes to the next step.
        {1, 0, 1, 1, 1, 1},
        {1, 1, 0, 1, 1, 1}
    };
    const Point _base;                              ///< Starting coordinates of the iterator.
    int _currentPosition;                           ///< Current number of steps of the iterator.
    coordinate_type _currentCoordinates;            ///< Latest computed coordinates.
};

template <class Voxel>
/**
 * class SquareIterator
 * @brief Simplified iterator class to iterate over the squares of a given
 * voxel.
 * @tparam Voxel Voxel class to be used.
 **/
class SquareIterator {
public:
    using coordinate_type = std::vector<Point>;

    /**
     * @brief Constructor.
     * @param voxel Voxel to iterate over.
     */
    SquareIterator(const Voxel &voxel);

    /**
     * @brief Tests if the iterator arrived at the end.
     * @return True if it is the end, false otherwise.
     */
    bool isEnd();
    /**
     * @brief Let the iterator go to the next step.
     */
    void next();
    /**
     * @brief Returns the value of the current position of the iterator.
     * @return The value of the current position of the iterator.
     */
    coordinate_type& getCurrentValue();

private:
    static constexpr unsigned int _nberOfCells = 6; ///< Number of cells to iterate over.
    static constexpr unsigned int _tupleLength = 4; ///< Number of vertices in the cell.
    static constexpr char _shift[_nberOfCells][_tupleLength * 3] =
    {                                               ///< Shifts on the coordinates to be applied
        {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1},       ///< when the iterator goes to
        {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1},       ///< the next step.
        {0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0},
        {0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1},
        {0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1},
        {1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1}
    };
    const Point _base;                              ///< Starting coordinates of the iterator.
    int _currentPosition;                           ///< Current number of steps of the iterator.
    coordinate_type _currentCoordinates;            ///< Latest computed coordinates.
};

template <class Voxel>
/**
 * class PositiveSquareIterator
 * @brief Simplified iterator class to iterate over
 * the positive squares of a given voxel.
 * @tparam Voxel Voxel class to be used.
 **/
class PositiveSquareIterator {
public:
    using coordinate_type = std::vector<Point>;

    /**
     * @brief Constructor.
     * @param voxel Voxel to iterate over.
     */
    PositiveSquareIterator(const Voxel &voxel);

    /**
     * @brief Tests if the iterator arrived at the end.
     * @return True if it is the end, false otherwise.
     */
    bool isEnd();
    /**
     * @brief Let the iterator go to the next step.
     */
    void next();
    /**
     * @brief Returns the value of the current position of the iterator.
     * @return The value of the current position of the iterator.
     */
    coordinate_type& getCurrentValue();

private:
    static constexpr unsigned int _nberOfCells = 3; ///< Number of cells to iterate over.
    static constexpr unsigned int _tupleLength = 4; ///< Number of vertices in the cell.
    static constexpr char _shift[_nberOfCells][_tupleLength * 3] =
    {                                               ///< Shifts on the coordinates to be applied
        {0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1},       ///< when the iterator goes to
        {0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1},       ///< the next step.
        {1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1}
    };
    const Point _base;                              ///< Starting coordinates of the iterator.
    int _currentPosition;                           ///< Current number of steps of the iterator.
    coordinate_type _currentCoordinates;            ///< Latest computed coordinates.
};

template <class Voxel>
/**
 * struct CubeCoordinates
 * @brief Structure which return the coordinates of the
 * cube corresponding to a given voxel.
 **/
struct CubeCoordinates {
public:
    using coordinate_type = std::vector<Point>;

    /**
     * @brief Returns the vertex coordinates of the
     * cube corresponding to the given voxel.
     * @param voxel Voxel
     * @return The vertex coordinates of the
     * cube corresponding to the given voxel.
     */
    coordinate_type operator()(const Voxel &voxel) {
        Point base({voxel[0], voxel[1], voxel[2]});
        coordinate_type coordinates(8, base);
        for (unsigned int i = 0; i < 8; ++i) {
            for (unsigned int j = 0; j < 3; ++j) {
                coordinates.at(i)[j] += _shift[i][j];
            }
        }
        return coordinates;
    }

private:
    static constexpr char _shift[8][3] =
    {   ///< Shifts to be applied on the voxel coordinates.
        {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
        {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}
    };
};

template <class Voxel>
/**
 * class DualEdgeIterator
 * @brief Simplified iterator class to iterate over the dual edges of a given
 * voxel.
 * @tparam Voxel Voxel class to be used.
 **/
class DualEdgeIterator {
public:
    using coordinate_type = std::vector<Point>;

    /**
     * @brief Constructor.
     * @param voxel Voxel to iterate over.
     */
    DualEdgeIterator(const Voxel &voxel);

    /**
     * @brief Tests if the iterator arrived at the end.
     * @return True if it is the end, false otherwise.
     */
    bool isEnd();
    /**
     * @brief Let the iterator go to the next step.
     */
    void next();
    /**
     * @brief Returns the value of the current position of the iterator.
     * @return The value of the current position of the iterator.
     */
    coordinate_type& getCurrentValue();

private:
    static constexpr unsigned int _nberOfCells = 6; ///< Number of cells to iterate over.
    static constexpr unsigned int _tupleLength = 2; ///< Number of vertices in the cell.
    static constexpr char _shift[_nberOfCells][_tupleLength * 3] =
    {                                               ///< Shifts on the coordinates to be applied
        {0, 0, 0, 0, 0, 1},                         ///< when the iterator goes to the next step.
        {0, 0, 0, 0, 1, 0},  {0, 0, 0, 1, 0, 0}, {0, 0, -1, 0, 0, 0},
        {0, -1, 0, 0, 0, 0}, {-1, 0, 0, 0, 0, 0}
    };
    const Point _base;                              ///< Starting coordinates of the iterator.
    int _currentPosition;                           ///< Current number of steps of the iterator.
    coordinate_type _currentCoordinates;            ///< Latest computed coordinates.
};

template <class Voxel>
/**
 * class DualSquareIterator
 * @brief Simplified iterator class to iterate over the dual squares of a given
 * voxel.
 * @tparam Voxel Voxel class to be used.
 **/
class DualSquareIterator {
public:
    using coordinate_type = std::vector<Point>;

    /**
     * @brief Constructor.
     * @param voxel Voxel to iterate over.
     */
    DualSquareIterator(const Voxel &voxel);

    /**
     * @brief Tests if the iterator arrived at the end.
     * @return True if it is the end, false otherwise.
     */
    bool isEnd();
    /**
     * @brief Let the iterator go to the next step.
     */
    void next();
    /**
     * @brief Returns the value of the current position of the iterator.
     * @return The value of the current position of the iterator.
     */
    coordinate_type& getCurrentValue();

private:
    static constexpr unsigned int _nberOfCells = 12;    ///< Number of cells to iterate over.
    static constexpr unsigned int _tupleLength = 4;     ///< Number of vertices in the cell.
    static constexpr char _shift[_nberOfCells][_tupleLength * 3] =
    {                                                   ///< Shifts on the coordinates to be applied
        {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1},           ///< when the iterator goes to the next step.
        {0, 0, 0, 0, 0, 1, 0, -1, 0, 0, -1, 1},
        {0, 0, 0, 0, 0, -1, 0, 1, 0, 0, 1, -1},
        {0, 0, 0, 0, 0, -1, 0, -1, 0, 0, -1, -1},
        {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 1, -1, 0, 0, -1, 0, 1},
        {0, 0, 0, 0, 0, -1, 1, 0, 0, 1, 0, -1},
        {0, 0, 0, 0, 0, -1, -1, 0, 0, -1, 0, -1},
        {0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0},
        {0, 0, 0, 1, 0, 0, 0, -1, 0, 1, -1, 0},
        {0, 0, 0, -1, 0, 0, 0, 1, 0, -1, 1, 0},
        {0, 0, 0, -1, 0, 0, 0, -1, 0, -1, -1, 0}
    };
    const Point _base;                                  ///< Starting coordinates of the iterator.
    int _currentPosition;                               ///< Current number of steps of the iterator.
    coordinate_type _currentCoordinates;                ///< Latest computed coordinates.
};

template <class Voxel>
/**
 * class DualCubeIterator
 * @brief Simplified iterator class to iterate over the dual cubes of a given
 * voxel.
 * @tparam Voxel Voxel class to be used.
 **/
class DualCubeIterator {
public:
    using coordinate_type = std::vector<Point>;

    /**
     * @brief Constructor.
     * @param voxel Voxel to iterate over.
     */
    DualCubeIterator(const Voxel &voxel);

    /**
     * @brief Tests if the iterator arrived at the end.
     * @return True if it is the end, false otherwise.
     */
    bool isEnd();
    /**
     * @brief Let the iterator go to the next step.
     */
    void next();
    /**
     * @brief Returns the value of the current position of the iterator.
     * @return The value of the current position of the iterator.
     */
    coordinate_type& getCurrentValue();

private:
    static constexpr unsigned int _nberOfCells = 8; ///< Number of cells to iterate over.
    static constexpr unsigned int _tupleLength = 8; ///< Number of vertices in the cell.
    static constexpr char _shift[_nberOfCells][_tupleLength * 3] =
    {                                               ///< Shifts on the coordinates to be applied
        {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1,     ///< when the iterator goes to the next
         1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},          ///< step.
        {-1, 0, 0, 0, 0, 0, -1, 1, 0, -1, 0, 1,
         0,  1, 0, 0, 0, 1, -1, 1, 1, 0,  1, 1},
        {0, -1, 0, 1, -1, 0, 0, 0, 0, 0, -1, 1,
         1, 0,  0, 1, -1, 1, 0, 0, 1, 1, 0,  1},
        {0, 0, -1, 1, 0, -1, 0, 1, -1, 0, 0, 0,
         1, 1, -1, 1, 0, 0,  0, 1, 0,  1, 1, 0},
        {-1, -1, 0, 0, -1, 0, -1, 0, 0, -1, -1, 1,
         0,  0,  0, 0, -1, 1, -1, 0, 1, 0,  0,  1},
        {-1, 0, -1, 0, 0, -1, -1, 1, -1, -1, 0, 0,
         0,  1, -1, 0, 0, 0,  -1, 1, 0,  0,  1, 0},
        {0, -1, -1, 1, -1, -1, 0, 0, -1, 0, -1, 0,
         1, 0,  -1, 1, -1, 0,  0, 0, 0,  1, 0,  0},
        {-1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0,
         0,  0,  -1, 0, -1, 0,  -1, 0, 0,  0,  0,  0},
    };
    const Point _base;                              ///< Starting coordinates of the iterator.
    int _currentPosition;                           ///< Current number of steps of the iterator.
    coordinate_type _currentCoordinates;            ///< Latest computed coordinates.
};

#include "VoxelSkeletonIterators.txx"

}} // namespace shape_simplification::shape
