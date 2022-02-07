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
 * @file ProcessedImageStack.h
 * Contains the declaration of the \ref ProcessedImageStack class.
 **/

#include "png.hpp"
#include <array>
#include <string>
#include <vector>

#include "util/BaseImageStack.h"

namespace shape_simplification{ namespace util {

/**
 * @brief The ImageStack class which loads already preprocessed images.
 */
class ProcessedImageStack : public BaseImageStack {
public:
    /**
     * @brief Constructor.
     */
    ProcessedImageStack();

    /**
     * @brief Returns the level value of the given voxel.
     * @param x X coordinate of the voxel.
     * @param y Y coordinate of the voxel.
     * @param z Z coordinate of the voxel (i.e., layer number in stack).
     * @return The level value of the given voxel.
     */
    unsigned char getLevel(
            unsigned int x, unsigned int y, unsigned int z) const;
};

}} // namespace shape_simplification::util
