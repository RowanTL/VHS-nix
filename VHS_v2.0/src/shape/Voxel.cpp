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
 * @file Voxel.cpp
 * Contains the implementation of the methods in the \ref Voxel class.
 **/

#include "Voxel.h"

namespace shape_simplification::shape {

using shape_simplification::util::bitMask;

Voxel::Voxel() : PackedData(0) {}

Voxel::Voxel(int x, int y, int z)
    : PackedData((static_cast<uint64_t>(z) << _coordShift * 2) +
                 (static_cast<uint64_t>(y) << _coordShift) +
                 static_cast<uint64_t>(x)), _level(NOT_SPECIFIED)
{}

Voxel::Voxel(int x, int y, int z, Level level)
    : PackedData((static_cast<uint64_t>(z) << _coordShift * 2) +
                 (static_cast<uint64_t>(y) << _coordShift) +
                 static_cast<uint64_t>(x)), _level(level)
{}

Voxel::Voxel(const std::array<int, 3> &coordinates)
    : PackedData(
          (static_cast<uint64_t>(coordinates[2]) << _coordShift * 2) +
          (static_cast<uint64_t>(coordinates[1]) << _coordShift) +
          static_cast<uint64_t>(coordinates[0])
    ), _level(NOT_SPECIFIED)
{}

Voxel::Voxel(const std::array<int, 3> &coordinates, Level level)
    : PackedData(
          (static_cast<uint64_t>(coordinates[2]) << _coordShift * 2) +
          (static_cast<uint64_t>(coordinates[1]) << _coordShift) +
          static_cast<uint64_t>(coordinates[0])
    ), _level(level)
{}

int Voxel::operator[](int i) const {
    return static_cast<int>(rawData() >>
                            (_coordShift * static_cast<unsigned int>(i)) &
                            static_cast<uint64_t>(_maxCoordSize - 1));
}

int Voxel::getMaxCoordinateValue(){
    return _maxCoordSize;
}

Level Voxel::getLevel() const
{
    return _level;
}

void Voxel::setLevel(const Level &level)
{
    _level = level;
}

std::ostream &operator<<(std::ostream &outputStream, Voxel voxel) {
    outputStream << "(" << voxel[0] << ", "
                 << voxel[1] << ", "
                 << voxel[2] << ")";
    return outputStream;
}

} // namespace shape_simplification::shape
