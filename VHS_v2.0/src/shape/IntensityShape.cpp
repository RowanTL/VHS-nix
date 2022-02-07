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
 * @file IntensityShape.cpp
 * Contains the implementation of the methods in the \ref IntensityShape class.
 **/

#include "IntensityShape.h"

namespace shape_simplification::shape {

IntensityShape::IntensityShape() :
    _shapeDimensions{0, 0, 0},
    _coreThreshold(0),
    _shapeThreshold(0),
    _neighborhoodThreshold(0),
    _extendedNeighborhoodThreshold(0)
{}

IntensityShape::IntensityShape(
        ProcessedImageStack &stack, unsigned char extendedNThreshold) :
    _shapeDimensions{0, 0, 0},
    _coreThreshold(255 - stack.getCoreThreshold()),
    _shapeThreshold(255 - stack.getShapeThreshold()),
    _neighborhoodThreshold(255 - stack.getNeighborhoodThreshold()),
    _extendedNeighborhoodThreshold(255 - extendedNThreshold)
{
    const std::array<unsigned int, 3> &dimensions = stack.getDimensions();

    for (unsigned int z = 0; z < dimensions[2]; z++) {
        for (unsigned int y = 0; y < dimensions[1]; y++) {
            for (unsigned int x = 0; x < dimensions[0]; x++) {
                unsigned char level = stack.getLevel(x, y, z);
                if (level >= extendedNThreshold) {
                    Voxel v(x, y, z);

                    _voxels.at(255 - level).push_back(v);
                    _levels.emplace(v, 255 - level);

                    if (x > _shapeDimensions[0])
                        _shapeDimensions[0] = x;
                    if (y > _shapeDimensions[1])
                        _shapeDimensions[1] = y;
                    if (z > _shapeDimensions[2])
                        _shapeDimensions[2] = z;
                }
            }
        }
    }
}

template <>
bool IntensityShape::contains<IntensityShape::CORE>(Voxel v) const
{
    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _coreThreshold;
}

template <>
bool IntensityShape::contains<IntensityShape::MAIN_SHAPE>(Voxel v) const
{
    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _shapeThreshold;
}

template <>
bool IntensityShape::contains<IntensityShape::NEIGHBORHOOD>(Voxel v) const
{
    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _neighborhoodThreshold;
}

template <>
bool IntensityShape::contains<IntensityShape::EXTENDEDN>(Voxel v) const
{
    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _extendedNeighborhoodThreshold;
}

template <>
bool IntensityShape::contains<IntensityShape::EX_MAIN_SHAPE>(Voxel v) const
{
    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _shapeThreshold
            && it->second > _coreThreshold;
}

template <>
bool IntensityShape::contains<IntensityShape::EX_NEIGHBORHOOD>(Voxel v) const
{
    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _neighborhoodThreshold
            && it->second > _shapeThreshold;
}

template <>
bool IntensityShape::contains<IntensityShape::EX_EXTENDEDN>(Voxel v) const
{
    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _extendedNeighborhoodThreshold
            && it->second > _shapeThreshold;
}

template <>
bool IntensityShape::contains<IntensityShape::CORE>(
        int x, int y, int z) const
{
    Voxel v(x, y, z);

    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _coreThreshold;
}

template <>
bool IntensityShape::contains<IntensityShape::MAIN_SHAPE>(
        int x, int y, int z) const
{
    Voxel v(x, y, z);

    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _shapeThreshold;
}

template <>
bool IntensityShape::contains<IntensityShape::NEIGHBORHOOD>(
        int x, int y, int z) const
{
    Voxel v(x, y, z);

    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _neighborhoodThreshold;
}

template <>
bool IntensityShape::contains<IntensityShape::EXTENDEDN>(
        int x, int y, int z) const
{
    Voxel v(x, y, z);

    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _extendedNeighborhoodThreshold;
}

template <>
bool IntensityShape::contains<IntensityShape::EX_MAIN_SHAPE>(
        int x, int y, int z) const
{
    Voxel v(x, y, z);

    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _shapeThreshold
            && it->second > _coreThreshold;
}

template <>
bool IntensityShape::contains<IntensityShape::EX_NEIGHBORHOOD>(
        int x, int y, int z) const
{
    Voxel v(x, y, z);

    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _neighborhoodThreshold
            && it->second > _shapeThreshold;
}

template <>
bool IntensityShape::contains<IntensityShape::EX_EXTENDEDN>(
        int x, int y, int z) const
{
    Voxel v(x, y, z);

    auto it = _levels.find(v);

    if (it == _levels.end()) return false;

    return it->second <= _extendedNeighborhoodThreshold
            && it->second > _shapeThreshold;
}

unsigned int IntensityShape::getMinLevel(Voxel v) {
    auto it = _levels.find(v);

    if (it == _levels.end()) return _extendedNeighborhoodThreshold + 1;

    return it->second;
}

unsigned int IntensityShape::getMinLevel(int x, int y, int z) {
    Voxel v(x, y, z);

    auto it = _levels.find(v);

    if (it == _levels.end()) return _extendedNeighborhoodThreshold + 1;

    return it->second;
}

const std::array<unsigned int, 3>& IntensityShape::getShapeDimensions() const {
    return _shapeDimensions;
}

void IntensityShape::clear(){
    _levels.clear();
    for (voxel_container &c : _voxels) c.clear();
    _shapeDimensions[0] = 0;
    _shapeDimensions[1] = 0;
    _shapeDimensions[2] = 0;
}

unsigned int IntensityShape::getCoreThreshold() const
{
    return _coreThreshold;
}

void IntensityShape::setCoreThreshold(unsigned int coreThreshold)
{
    _coreThreshold = coreThreshold;
}

unsigned int IntensityShape::getShapeThreshold() const
{
    return _shapeThreshold;
}

void IntensityShape::setShapeThreshold(unsigned int shapeThreshold)
{
    _shapeThreshold = shapeThreshold;
}

unsigned int IntensityShape::getNeighborhoodThreshold() const
{
    return _neighborhoodThreshold;
}

void IntensityShape::setNeighborhoodThreshold(
        unsigned int neighborhoodThreshold)
{
    _neighborhoodThreshold = neighborhoodThreshold;
}

unsigned int IntensityShape::getExtendedNeighborhoodThreshold() const
{
    return _extendedNeighborhoodThreshold;
}

void IntensityShape::setExtendedNeighborhoodThreshold(
        unsigned int extendedNeighborhoodThreshold)
{
    _extendedNeighborhoodThreshold = extendedNeighborhoodThreshold;
}

unsigned int IntensityShape::getNeighborhoodSize(){
    unsigned int res = 0;

    for (int i = _neighborhoodThreshold; i >= 0; i--){
        res += _voxels.at(i).size();
    }

    return res;
}

unsigned int IntensityShape::getShapeSize(){
    unsigned int res = 0;

    for (int i = _shapeThreshold; i >= 0; i--){
        res += _voxels.at(i).size();
    }

    return res;
}

unsigned int IntensityShape::getCoreSize(){
    unsigned int res = 0;

    for (int i = _coreThreshold; i >= 0; i--){
        res += _voxels.at(i).size();
    }

    return res;
}

const IntensityShape::voxel_container& IntensityShape::getVoxelsWithGrayscale(
        unsigned char grayscaleValue) const
{
    return _voxels.at(255 - grayscaleValue);
}

} // namespace shape_simplification::shape

