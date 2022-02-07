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
 * @file BaseImageStack.cpp
 * Contains the implementation of the methods in the \ref BaseImageStack class.
 **/

#include "util/BaseImageStack.h"

namespace shape_simplification::util {

BaseImageStack::BaseImageStack() :
    _dim({0, 0, 0}),
    _coreThreshold(200),
    _shapeThreshold(150),
    _neighborhoodThreshold(100)
{}

BaseImageStack::~BaseImageStack() {}

void BaseImageStack::load(std::string baseFilename,
                          unsigned int numberOfImages)
{
    for (int i = numberOfImages - 1; i >= 0; i--) {
        std::string filename = baseFilename;
        if (i < 10)
            filename += "000";
        else if (i < 100)
            filename += "00";
        else if (i < 1000)
            filename += "0";
        filename += std::to_string(i) + ".png";

        png::image<png::gray_pixel> image(filename);
        _stack.push_back(image);
    }

    _dim[0] = _stack[0].get_width();
    _dim[1] = _stack[1].get_height();
    _dim[2] = numberOfImages;
}

void BaseImageStack::load(std::vector<png::image<png::gray_pixel> > &images)
{
    _stack = images;

    _dim[0] = _stack[0].get_width();
    _dim[1] = _stack[1].get_height();
    _dim[2] = images.size();
}

void BaseImageStack::save(std::string baseFilename)
{
    for (int i = _dim[2] - 1; i >= 0; i--) {
        std::string filename = baseFilename;
        if (i < 10)
            filename += "000";
        else if (i < 100)
            filename += "00";
        else if (i < 1000)
            filename += "0";
        filename += std::to_string(i) + ".png";

        _stack.at(i).write(filename);
    }
}

void BaseImageStack::addEmptyLayer()
{
    png::image<png::gray_pixel> image(_dim[0], _dim[1]);
    _stack.push_back(image);
    _dim[2]++;
}

void BaseImageStack::removeTopLayer()
{
    _stack.pop_back();
    _dim[2]--;
}


unsigned char BaseImageStack::getValue(unsigned int x,
                                       unsigned int y,
                                       unsigned int z) const
{
    return _stack[z][y][x];
}

void BaseImageStack::setValue(unsigned char value,
                              unsigned int x,
                              unsigned int y,
                              unsigned int z)
{
    _stack[z][y][x] = value;
}

const std::array<unsigned int, 3> &BaseImageStack::getDimensions() const
{
    return _dim;
}

void BaseImageStack::setDimensions(unsigned int x,
                                   unsigned int y,
                                   unsigned int z)
{
    _dim[0] = x;
    _dim[1] = y;
    _dim[2] = z;
}

unsigned char BaseImageStack::getNeighborhoodThreshold() const
{
    return _neighborhoodThreshold;
}

void BaseImageStack::setNeighborhoodThreshold(unsigned char value)
{
    _neighborhoodThreshold = value;
}

unsigned char BaseImageStack::getShapeThreshold() const
{
    return _shapeThreshold;
}

void BaseImageStack::setShapeThreshold(unsigned char value)
{
    _shapeThreshold = value;
}

unsigned char BaseImageStack::getCoreThreshold() const
{
    return _coreThreshold;
}

void BaseImageStack::setCoreThreshold(unsigned char value)
{
    _coreThreshold = value;
}

void BaseImageStack::clear()
{
    _stack.clear();
}

bool BaseImageStack::isWithinBounds(unsigned int x,
                                    unsigned int y,
                                    unsigned int z){
    return x >= 0 && x < _dim[0] &&
            y >= 0 && y < _dim[1] &&
            z >= 0 && z < _dim[2];
}

} // namespace shape_simplification::util
