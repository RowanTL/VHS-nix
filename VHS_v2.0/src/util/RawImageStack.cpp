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
 * @file RawImageStack.cpp
 * Contains the implementation of the methods in the \ref RawImageStack class.
 **/

#include "util/RawImageStack.h"

namespace shape_simplification::util {

RawImageStack::RawImageStack(unsigned int downsampling, unsigned int noise)
    : BaseImageStack(),
      _downsampling(downsampling),
      _noise(noise)
{}

void RawImageStack::load(
        std::string baseFilename,
        unsigned int numberOfImages)
{
    BaseImageStack::load(baseFilename, numberOfImages);
    const std::array<unsigned int, 3> &dimensions =
            BaseImageStack::getDimensions();
    BaseImageStack::setDimensions(
                dimensions[0] / _downsampling,
                dimensions[1] / _downsampling,
                dimensions[2] / _downsampling
            );
}

unsigned char RawImageStack::getLevel(unsigned int x,
                                      unsigned int y,
                                      unsigned int z) const
{
    unsigned int seed = static_cast<unsigned int>(
                std::chrono::system_clock::now().time_since_epoch().count()
                );
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(
                0.0,
                static_cast<double>(_noise)
            );

    int level = 0;
    for (unsigned int k = 0; k < _downsampling; k++)
        for (unsigned int j = 0; j < _downsampling; j++)
            for (unsigned int i = 0; i < _downsampling; i++)
                level += BaseImageStack::getValue(
                            _downsampling * x + i,
                            _downsampling * y + j,
                            _downsampling * z + k
                            );
    level /= _downsampling * _downsampling * _downsampling;
    level += static_cast<int>(distribution(generator));

    return static_cast<unsigned char>(level);
}

} // namespace shape_simplification::util
