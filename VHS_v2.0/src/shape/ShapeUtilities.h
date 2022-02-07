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
 * @file ShapeUtilities.h
 * Contains the declaration of the classes and structures:
 *  - \ref Level,
 *  - \ref NeighborsIterator,
 *  - \ref NeighborsInIterator,
 *  - \ref ExtendedNeighborsIterator,
 *  - \ref ExtendedNeighborsInIterator.
 **/

#include <vector>

#include "util/Point.h"

namespace shape_simplification{ namespace shape {

using util::Point;

/// Enum class for the different levels in the shape.
enum Level {
    CORE,               ///< Core of the shape.
    MAIN_SHAPE,         ///< Shape to be simplified.
    NEIGHBORHOOD,       ///< Neighborhood of the shape.
    OUTSIDE,            ///< Outside of the shape.
    EX_MAIN_SHAPE,      ///< Shape - Core.
    EX_NEIGHBORHOOD,    ///< Neighborhood - Shape.
    NOT_SPECIFIED
};

template <class Voxel>
/**
 * class NeighborsIterator
 * @brief Simplified iterator class to iterate over the 6 direct neighbors
 * of a given voxel.
 * @tparam Voxel Voxel class to be used.
 **/
class NeighborsIterator {
public:
    /**
     * @brief Constructor.
     * @param voxel Voxel to iterate over.
     */
    NeighborsIterator(const Voxel &voxel);

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
    void getCurrentValue(int& x, int& y, int& z);

private:
    static constexpr char _shift[6][3] =
    {                       ///< Shifts on the coordinates to be applied
        {0, 0, 1},          ///< when the iterator goes to the next step.
        {0, 1, 0}, {1, 0, 0}, {0, 0, -1}, {0, -1, 0}, {-1, 0, 0}
    };
    const Point _base;      ///< Starting coordinates of the iterator.
    int _currentPosition;   ///< Current number of steps of the iterator.

    /**
     * @brief Tests if the current position has valid coordinates.
     * @return True if valid, false otherwise.
     */
    bool _isValidPosition();
};

template <class Voxel>
/**
 * class ExtendedNeighborsIterator
 * @brief Simplified iterator class to iterate over the 26 neighbors of a
 * given voxel.
 * @tparam Voxel Voxel class to be used.
 **/
class ExtendedNeighborsIterator {
public:
    /**
     * @brief Constructor.
     * @param voxel Voxel to iterate over.
     */
    ExtendedNeighborsIterator(const Voxel &voxel);

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
    void getCurrentValue(int& x, int& y, int& z);

private:
    static constexpr char _shift[26][3] =
    {                       ///< Shifts on the coordinates to be applied
        {0, 0, 1},          ///< when the iterator goes to the next step.
        {0, 0, -1},  {0, 1, 0},   {0, 1, 1},   {0, 1, -1},  {0, -1, 0},
        {0, -1, 1},  {0, -1, -1}, {1, 0, 0},   {1, 0, 1},   {1, 0, -1},
        {1, 1, 0},   {1, 1, 1},   {1, 1, -1},  {1, -1, 0},  {1, -1, 1},
        {1, -1, -1}, {-1, 0, 0},  {-1, 0, 1},  {-1, 0, -1}, {-1, 1, 0},
        {-1, 1, 1},  {-1, 1, -1}, {-1, -1, 0}, {-1, -1, 1}, {-1, -1, -1}
    };
    const Point _base;      ///< Starting coordinates of the iterator.
    int _currentPosition;   ///< Current number of steps of the iterator.

    /**
     * @brief Tests if the current position has valid coordinates.
     * @return True if valid, false otherwise.
     */
    bool _isValidPosition();
};

template <class Shape, class Voxel, Level level>
/**
 * class NeighborsInIterator
 * @brief Simplified iterator class to iterate over the direct neighbors
 * (among 6) of a given voxel existing in the given shape.
 * @tparam Voxel Voxel class to be used.
 **/
class NeighborsInIterator : public NeighborsIterator<Voxel> {
public:
    /**
     * @brief Constructor.
     * @param voxel Voxel to iterate over.
     * @param shape Shape the neighbors have to be contained in.
     */
    NeighborsInIterator(const Voxel &voxel, const Shape &shape);

    /**
     * @brief Let the iterator go to the next step.
     */
    void next();

private:
    const Shape &_shape; ///< Shape the neighbors have to be contained in.

    /**
     * @brief Tests if the current position has valid coordinates.
     * @return True if valid, false otherwise.
     */
    bool _isValidPosition();
};

template <class Shape, class Voxel, Level level>
/**
 * class ExtendedNeighborsInIterator
 * @brief Simplified iterator class to iterate over the neighbors (among 26)
 * of a given voxel existing in the given shape.
 * @tparam Voxel Voxel class to be used.
 **/
class ExtendedNeighborsInIterator : public ExtendedNeighborsIterator<Voxel> {
public:
    /**
     * @brief Constructor.
     * @param voxel Voxel to iterate over.
     * @param shape Shape the neighbors have to be contained in.
     */
    ExtendedNeighborsInIterator(const Voxel &voxel, const Shape &shape);

    /**
     * @brief Let the iterator go to the next step.
     */
    void next();

private:
    const Shape &_shape; ///< Shape the neighbors have to be contained in.

    /**
     * @brief Tests if the current position has valid coordinates.
     * @return True if valid, false otherwise.
     */
    bool _isValidPosition();
};

#include "ShapeUtilities.txx"

}   //namespace shape_simplification
}   //namespace shape

