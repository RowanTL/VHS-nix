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
 * @file Point.h
 * Contains the definition of a \ref Point.
 **/

#include <array>
#include <functional>

namespace shape_simplification{ namespace util {
/**
 * @brief An unpacked representative for a point.
 **/
using Point = std::array<int, 3>;

/**
 * @brief Hash function for a point.
 */
struct PointHasher {
    size_t operator()(const Point &p) const {
        int long long h = 0ULL;
        for (unsigned int i = 0; i < 3; i++)
            h += static_cast<int long long>(p[i]) << (16 * i);
        return std::hash<int long long>{}(h);
    }
};

}} // namespace shape_simplification::util

namespace std {

/**
 * @brief Provides a std::hash function for points.
 **/
template<> struct hash<shape_simplification::util::Point> {
    std::size_t operator()(const shape_simplification::util::Point &p) const {
        return shape_simplification::util::PointHasher()(p);
    }
};

} // namespace std
