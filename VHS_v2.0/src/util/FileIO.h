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
 * @file FileIO.h
 * Contains methods related to file reading and writing.
 **/

#include <algorithm>
#include <iostream>
#include <istream>
#include <ostream>

#include "png.hpp"

#include "shape/Voxel.h"
#include "shape/ShapeUtilities.h"
#include "util/BaseImageStack.h"

namespace shape_simplification{ namespace util {

using shape_simplification::shape::Voxel;
using shape_simplification::shape::Level;
using shape_simplification::shape::CORE;
using shape_simplification::shape::MAIN_SHAPE;
using shape_simplification::shape::NEIGHBORHOOD;
using shape_simplification::shape::EX_MAIN_SHAPE;
using shape_simplification::shape::EX_NEIGHBORHOOD;

template <class Shape>
/**
 * @brief Loads the information of a stack of images into a shape class.
 * @tparam Shape Shape class
 * @param inputImages Image stack to transform into a shape.
 * @param shape Resulting shape.
 */
void operator>>(BaseImageStack &inputImages, Shape &shape) {
    const std::array<unsigned int, 3> &dimensions = inputImages.getDimensions();

    for (unsigned int z = 0; z < dimensions[2]; z++) {
        for (unsigned int y = 0; y < dimensions[1]; y++) {
            for (unsigned int x = 0; x < dimensions[0]; x++) {
                unsigned char level = inputImages.getLevel(x, y, z);
                if (level >= inputImages.getCoreThreshold()) {
                    shape.template addVoxel<CORE>(Voxel(x, y, z));
                } else if (level >= inputImages.getShapeThreshold()) {
                    shape.template addVoxel<MAIN_SHAPE>(Voxel(x, y, z));
                } else if (level >= inputImages.getNeighborhoodThreshold()) {
                    shape.template addVoxel<NEIGHBORHOOD>(Voxel(x, y, z));
                }
            }
        }
    }
}

template <class Shape>
/**
 * @brief Saves a shape as a set of PNG images.
 * The core, the main shape and the neighborhood are associated to
 * grayscale value 200, 150 and 100 respectively.
 * @param baseFilename Directory the images should be saved in.
 * @param shape Shape to save.
 */
void operator<<(std::string baseFilename, Shape &shape) {
    const std::array<unsigned int, 3> &dimensions = shape.getShapeDimensions();
    unsigned int numberOfImages = dimensions[2];

    for (unsigned int i = 0; i < numberOfImages; i++) {
        std::string filename = baseFilename;
        if (i < 10)
            filename += "000";
        else if (i < 100)
            filename += "00";
        else if (i < 1000)
            filename += "0";
        filename += std::to_string(i) + ".png";

        png::image<png::gray_pixel> image(dimensions[0], dimensions[1]);
        unsigned int z = i;
        for (unsigned int y = 0; y < dimensions[1]; y++)
            for (unsigned int x = 0; x < dimensions[0]; x++) {
                if (shape.template contains<CORE>(x, y, z))
                    image[y][x] = static_cast<png::gray_pixel>(200);
                else if (shape.template contains<EX_MAIN_SHAPE>(x, y, z))
                    image[y][x] = static_cast<png::gray_pixel>(150);
                else if (shape.template contains<EX_NEIGHBORHOOD>(x, y, z))
                    image[y][x] = static_cast<png::gray_pixel>(100);
                else
                    image[y][x] = static_cast<png::gray_pixel>(0);
            }

        image.write(filename);
    }
}

}} // namespace shape_simplification::util
