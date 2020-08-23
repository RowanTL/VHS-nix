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
     * @brief Create a voxel with the specified coordinates.
     * @param x, y, z  the coordinates of the voxel
     * @pre x, y and z are all in the range [0, 65536)
     **/
    Voxel(int x, int y, int z);

    /**
     * @brief Create a voxel from a generic Point instance.
     * @param coordinates  the coordinates of the voxel
     **/
    explicit Voxel(const Point &coordinates);

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

    const static unsigned int _coordShift = 17U;  ///< Shift used for the Point coordinates.
    const static int _maxCoordSize = 65536;       ///< Maximal value for one coordinate.
};

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
    std::size_t operator()(const unordered_set<shape_simplification::shape::Voxel> &s) const {
        int long long h = 0ULL;
        for (auto it = s.begin(); it != s.end(); ++it) {
            shape_simplification::shape::Voxel v = *it;
            h += shape_simplification::shape::Voxel::VoxelHasher()(v);
        }
        return std::hash<int long long>{}(h);
    }
};

}

