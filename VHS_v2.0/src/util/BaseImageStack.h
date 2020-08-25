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
 * @file BaseImageStack.h
 * Contains the declaration of the \ref BaseImageStack class.
 **/

#include "png.hpp"
#include <array>
#include <string>
#include <vector>

namespace shape_simplification{ namespace util {

/**
 * @brief Base for the ImageStack classes.
 */
class BaseImageStack {
public:
    /**
     * @brief Constructor
     */
    BaseImageStack();

    /**
     * @brief Destructor (virtual)
     */
    virtual ~BaseImageStack();

    /**
     * @brief Loads the given set of PNG images.
     * @pre The images have to be named with consecutive integers starting at 0
     * and using 4 digits (i.e., the first file is named 0000.png etc.).
     * @param baseFilename Path to directory with the images to load.
     * @param numberOfImages The number of images to be loaded.
     */
    void load(std::string baseFilename, unsigned int numberOfImages);

    /**
     * @brief Saves this stack of images as PNG images.
     * @param baseFilename Path to directory, where the files should be saved.
     * @param numberOfImages The number of images to be saved
     * (max. the number of stacks).
     */
    void save(std::string baseFilename, unsigned int numberOfImages);

    /**
     * @brief Removes all loaded images from the stack.
     */
    void clear();

    /**
     * @brief Returns the level value of the given voxel.
     * @param x X coordinate of the voxel.
     * @param y Y coordinate of the voxel.
     * @param z Z coordinate of the voxel (i.e., layer number in stack).
     * @return The level value of the given voxel.
     */
    virtual unsigned char getLevel(unsigned int x, unsigned int y,
                                   unsigned int z) const = 0;

    /**
     * @brief Returns the dimensions of the image stack.
     * @return The dimensions of the image stack.
     */
    const std::array<unsigned int, 3> &getDimensions() const;

    unsigned char getCoreThreshold() const;
    void setCoreThreshold(unsigned char value);

    unsigned char getShapeThreshold() const;
    void setShapeThreshold(unsigned char value);

    unsigned char getNeighborhoodThreshold() const;
    void setNeighborhoodThreshold(unsigned char value);

protected:
    /**
     * @brief Returns the grayscale value of the given pixel.
     * @param x X coordinate of the pixel.
     * @param y Y coordinate of the pixel.
     * @param z Z coordinate of the pixel (i.e., layer number in stack).
     * @return The grayscale value of the given pixel.
     */
    unsigned char getValue(unsigned int x, unsigned int y, unsigned int z) const;

    /**
     * @brief Sets the dimensions of the image stack.
     * @param x Maximal size of X coordinate
     * (needs to be smaller than original value).
     * @param y Maximal size of Y coordinate
     * (needs to be smaller than original value).
     * @param z Number of images in the stack to considerate.
     */
    void setDimensions(unsigned int x, unsigned int y, unsigned int z);

private:
    std::vector<png::image<png::gray_pixel>> _stack;    ///< Image stack.
    std::array<unsigned int, 3> _dim;                   ///< Dimensions of the stack: [x,y,z].
    unsigned char _coreThreshold;                        ///< Core Threshold.
    unsigned char _shapeThreshold;                       ///< Shape Threshold.
    unsigned char _neighborhoodThreshold;                ///< Neighborhood Threshold.
};

}} // namespace shape_simplification::util
