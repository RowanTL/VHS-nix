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
 * @file PackedData.cpp
 * Contains the implementation of the methods in the \ref PackedData class.
 **/

#include "PackedData.h"

namespace shape_simplification::util {

PackedData::PackedData(data_type packedData) : _packedData(packedData)
{}

PackedData::data_type PackedData::rawData() const {
    return _packedData;
}

void PackedData::set(data_type packedData) {
    _packedData = packedData;
}

bool PackedData::operator<(const PackedData &other) const {
    return _packedData < other._packedData;
}

bool PackedData::operator==(const PackedData &other) const {
    return _packedData == other._packedData;
}

bool PackedData::operator!=(const PackedData &other) const {
    return _packedData != other._packedData;
}

} // namespace shape_simplification::util
