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
 * @file Voxel.h
 * Contains the declaration of the \ref Voxel class.
 **/

#include <array>
#include <cstdint>
#include <functional>
#include <ostream>
#include <unordered_set>

#include "shape/ShapeUtilities.h"
#include "util/PackedData.h"
#include "util/Point.h"

namespace shape_simplification{ namespace shape {

using util::PackedData;
using util::Point;

/**
 * class Voxel
 * @brief Stores a single voxel.
 *
 * Internally, the voxel is stored as an unsigned 64-bit integer with 16 bits
 * devoted to each coordinate and 1 bit of padding between each (with x
 * stored the lowest order bits and z in the highest). So each coordinate is in
 * the range [0, 65536). Note the extra bit of spacing makes arithmatic
 * possible.
 **/
class Voxel : public PackedData {
public:
    /**
     * @brief Create a new voxel with unspecified coordinates.
     **/
    Voxel();

    /**
     * @brief Creates a voxel with the specified coordinates.
     * @param x The x-coordinates of the voxel.
     * @param y The y-coordinates of the voxel.
     * @param z The z-coordinates of the voxel.
     * @pre x, y and z are all in the range [0, 65536)
     **/
    Voxel(int x, int y, int z);
    /**
     * @brief Creates a voxel with the specified coordinates and level.
     * @param x The x-coordinates of the voxel.
     * @param y The y-coordinates of the voxel.
     * @param z The z-coordinates of the voxel.
     * @param level Minimum level of the voxel in the shape.
     * @pre x, y and z are all in the range [0, 65536)
     */
    Voxel(int x, int y, int z, Level level);

    /**
     * @brief Creates a voxel from a generic Point instance.
     * @param coordinates The coordinates of the voxel.
     **/
    explicit Voxel(const Point &coordinates);
    /**
     * @brief Creates a voxel from a generic Point instance.
     * @param coordinates The coordinates of the voxel.
     * @param level Minimum level of the voxel in the shape.
     */
    explicit Voxel(const Point &coordinates, Level level);

    /**
     * @brief Return a coordiate of the voxel.
     * @param i the coordinate of the voxel to access.
     * @return the value of that coordinate
     * @pre i is between 0 and 2 inclusive.
     **/
    int operator[](int i) const;

    /**
     * @brief Returns maximal possible value of a voxel coordinate.
     * @return Maximal possible value of a voxel coordinate.
     */
    static int getMaxCoordinateValue();

    /**
     * @brief Returns the stored level of the voxel.
     * @return The stored level of the voxel.
     */
    Level getLevel() const;
    /**
     * @brief Sets the level of the voxel.
     * @param level Level to store.
     */
    void setLevel(const Level &level);

    /**
     * @brief Hash function for a voxel.
     */
    struct VoxelHasher {
        std::size_t operator()(const Voxel &voxel) const {
            return std::hash<PackedData>()(static_cast<PackedData>(voxel));
        }
    };

private:
    /**
     * @brief Disabled Constructor
     * @param packedData unsigned long
     */
    explicit Voxel(uint64_t packedData);

    Level _level;                                   ///< Level of the voxel.

    const static unsigned int _coordShift = 17U;    ///< Shift used for the Point coordinates.
    const static int _maxCoordSize = 65536;         ///< Maximal value for one coordinate.
};

/**
 * @brief Streams the coordinates of the voxel as string in the given output.
 * @param outputStream Outputstream.
 * @param voxel Voxel to stream.
 * @return
 */
std::ostream &operator<<(std::ostream &outputStream, Voxel voxel);

}} // namespace shape_simplification::shape

namespace std {

/**
 * @brief Provides a std::hash function for voxels.
 **/
template <> struct hash<shape_simplification::shape::Voxel> {
    std::size_t operator()(const shape_simplification::shape::Voxel &v) const {
        return shape_simplification::shape::Voxel::VoxelHasher()(v);
    }
};

/**
 * @brief Provides a std::hash function for unordered sets of voxels.
 **/
template <> struct hash<unordered_set<shape_simplification::shape::Voxel> > {
    std::size_t operator()(
            const unordered_set<shape_simplification::shape::Voxel> &s) const
    {
        int long long h = 0ULL;
        for (auto it = s.begin(); it != s.end(); ++it) {
            shape_simplification::shape::Voxel v = *it;
            h += shape_simplification::shape::Voxel::VoxelHasher()(v);
        }
        return std::hash<int long long>{}(h);
    }
};

}

