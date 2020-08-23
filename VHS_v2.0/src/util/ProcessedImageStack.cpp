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

/**
 * @file ProcessedImageStack.cpp
 * Contains the implementation of the methods in the \ref ProcessedImageStack
 * class.
 **/

#include "util/ProcessedImageStack.h"

namespace shape_simplification::util {

ProcessedImageStack::ProcessedImageStack() : BaseImageStack()
{}

unsigned char ProcessedImageStack::getLevel(unsigned int x,
                                            unsigned int y,
                                            unsigned int z) const
{
    return BaseImageStack::getValue(x, y, z);
}

} // namespace shape_simplification::util
