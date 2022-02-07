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
 * @file PackedData.h
 * Contains the declaration of the \ref PackedData class.
 **/

#include <cstdint>
#include <functional>
#include <vector>

namespace shape_simplification{ namespace util {

/**
 * class PackedData
 * @brief Stores coordinates of voxels and cells in a bit array.
 * The array has a capacity of 64 bits.
 **/
class PackedData {
public:
    using data_type = uint64_t;

    /**
     * @brief Create an instance with all bits zero.
     **/
    PackedData() = default;

    /**
     * @brief Create a new packed bit array of data with a specified value.
     **/
    explicit PackedData(data_type packedData);

    /**
     * @brief Access the underlying data.
     **/
    data_type rawData() const;

    /**
     * @brief Set underlying data to given value.
     **/
    void set(data_type packedData);

    /**
     * @brief Check is the current data prior to another.
     **/
    bool operator<(const PackedData &other) const;

    /**
     * @brief Check is the current data equal to another.
     **/
    bool operator==(const PackedData &other) const;

    /**
     * @brief Check is the current data not equal to another.
     **/
    bool operator!=(const PackedData &other) const;

private:
    data_type _packedData{0}; ///< Data.
};

/**
 * @brief Return a bit mask including the specified bits.
 * In includes all bits in the range (inclusive).
 **/
constexpr uint64_t bitMask(int first, int last = 0) {
    if (last > 0)
        return ((static_cast<uint64_t>(1)
                 << static_cast<unsigned>(last - first + 1)) -
                1)
                << first;
    else
        return static_cast<uint64_t>(1) << static_cast<unsigned>(first);
}

}} // namespace shape_simplification::util

namespace std {

/**
 * @brief Provides a std::hash function for PackedData.
 **/
template <> struct hash<shape_simplification::util::PackedData> {
    std::size_t
    operator()(const shape_simplification::util::PackedData &d) const {
        return hash<uint64_t>()(static_cast<uint64_t>(d.rawData()));
    }
};

} // namespace std
