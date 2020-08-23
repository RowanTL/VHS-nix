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
 * @file RawImageStack.h
 * Contains the declaration of the \ref RawImageStack class.
 **/

#include "png.hpp"
#include <array>
#include <chrono>
#include <random>
#include <string>
#include <vector>

#include "util/BaseImageStack.h"

namespace shape_simplification{ namespace util {

/**
 * @brief The ImageStack class which processes raw images.
 */
class RawImageStack : public BaseImageStack {
public:
    /**
     * @brief Constructor
     * @param downsampling Divide the dimensions of the stack by this value.
     * @param noise Noise value.
     */
    RawImageStack(unsigned int downsampling, unsigned int noise);

    /**
     * @brief Loads the given set of PNG images.
     * @pre The images have to be named with consecutive integers starting at 0
     * and using 4 digits (i.e., the first file is named 0000.png etc.).
     * @param baseFilename Path to directory with the images to load.
     * @param numberOfImages The number of images to be loaded.
     */
    void load(std::string baseFilename, unsigned int numberOfImages);

    /**
     * @brief Returns the level value of the given voxel in the downsampled
     * images.
     * @param x X coordinate of the voxel.
     * @param y Y coordinate of the voxel.
     * @param z Z coordinate of the voxel (i.e., layer number in stack).
     * @return The level value of the given voxel.
     */
    unsigned char getLevel(unsigned int x, unsigned int y, unsigned int z) const;

private:
    unsigned int _downsampling;   ///< Downsampling ratio.
    unsigned int _noise;          ///< Added noise value on level.
};

}} // namespace shape_simplification::util
