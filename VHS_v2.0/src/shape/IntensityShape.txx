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
 * @file IntensityShape.txx
 * Contains the implementation of the methods in the \ref IntensityShape class.
 **/

#include "IntensityShape.h"

using namespace shape_simplification::shape;

constexpr bool IntensityShape::isAddable() {
    return false;
}

constexpr bool IntensityShape::isRemovable() {
    return false;
}

template <typename IntensityShape::Level maxLevel>
void IntensityShape::addVoxel(Voxel v) {
    static_assert(maxLevel == -1, "Addition of voxel not enabled.");
}

template <typename IntensityShape::Level maxLevel>
void IntensityShape::removeVoxel(Voxel v) {
    static_assert(maxLevel == -1, "Removal of voxel not enabled.");
}

template <typename IntensityShape::Level level>
typename IntensityShape::voxel_iterator<level> IntensityShape::begin() const
{
    static_assert(level != OUTSIDE,
            "It is not possible to iterate voxels not contained in Shape.");
    return LevelIterator<level>(*this);
}

template <typename IntensityShape::Level level>
typename IntensityShape::voxel_iterator<level> IntensityShape::end() const
{
    static_assert(level != OUTSIDE,
            "It is not possible to iterate voxels not contained in Shape.");
    return LevelIterator<level>(*this, true);
}

template <typename IntensityShape::Level level>
typename IntensityShape::voxel_reverse_iterator<level>
IntensityShape::rbegin() const
{
    static_assert(level != OUTSIDE,
            "It is not possible to iterate voxels not contained in Shape.");
    return ReverseLevelIterator<level>(*this);
}

template <typename IntensityShape::Level level>
typename IntensityShape::voxel_reverse_iterator<level>
IntensityShape::rend() const
{
    static_assert(level != OUTSIDE,
            "It is not possible to iterate voxels not contained in Shape.");
    return ReverseLevelIterator<level>(*this, true);
}

template <IntensityShape::Level level>
IntensityShape::LevelIterator<level>::LevelIterator(
        const IntensityShape &shape, bool end) :
    _shape(shape), _currentLevel(0), _currentIt(0), _endLevel(256)
{
    if constexpr (level == CORE) {
        _endLevel = _shape.getCoreThreshold() + 1;
    } else if constexpr (level == MAIN_SHAPE){
        _endLevel = _shape.getShapeThreshold() + 1;
    } else if constexpr (level == NEIGHBORHOOD){
        _endLevel = _shape.getNeighborhoodThreshold() + 1;
    } else if constexpr (level == EXTENDEDN){
        _endLevel = _shape.getExtendedNeighborhoodThreshold() + 1;
    } else if constexpr (level == EX_MAIN_SHAPE){
        _currentLevel = _shape.getCoreThreshold() + 1;
        _endLevel = _shape.getShapeThreshold() + 1;
    } else if constexpr (level == EX_NEIGHBORHOOD){
        if (_shape.getExtendedNeighborhoodThreshold() <
                _shape.getNeighborhoodThreshold()){
            _currentLevel = _shape.getExtendedNeighborhoodThreshold() + 1;
        } else {
            _currentLevel = _shape.getShapeThreshold() + 1;
        }
        _endLevel = _shape.getNeighborhoodThreshold() + 1;
    } else if constexpr (level == EX_EXTENDEDN){
        if (_shape.getExtendedNeighborhoodThreshold() <=
                _shape.getNeighborhoodThreshold()){
            _currentLevel = _shape.getShapeThreshold() + 1;
        } else {
            _currentLevel = _shape.getNeighborhoodThreshold() + 1;
        }
        _endLevel = _shape.getExtendedNeighborhoodThreshold() + 1;
    }

    if (end) _currentLevel = _endLevel;

    while (_currentLevel < _endLevel &&
           _shape.getVoxelsWithGrayscale(255 - _currentLevel).size() == 0)
    {
        _currentLevel++;
    }
}

template <IntensityShape::Level level>
IntensityShape::LevelIterator<level> &
IntensityShape::LevelIterator<level>::operator++()
{
    _currentIt++;
    if (_currentIt == _shape.getVoxelsWithGrayscale(255 - _currentLevel).size())
    {
        _currentIt = 0;
        do {
            _currentLevel++;
        } while (_currentLevel < _endLevel &&
                 _shape.getVoxelsWithGrayscale(255 - _currentLevel).size() == 0);
    }
    return *this;
}

template <IntensityShape::Level level>
IntensityShape::LevelIterator<level>
IntensityShape::LevelIterator<level>::operator++(int)
{
    _currentIt++;
    if (_currentIt == _shape.getVoxelsWithGrayscale(255 - _currentLevel).size())
    {
        _currentIt = 0;
        do {
            _currentLevel++;
        } while (_currentLevel < _endLevel &&
                 _shape.getVoxelsWithGrayscale(255 - _currentLevel).size() == 0);
    }
    return *this;
}

template <IntensityShape::Level level>
const Voxel &IntensityShape::LevelIterator<level>::operator*() const
{
    return _shape.getVoxelsWithGrayscale(255 - _currentLevel).at(_currentIt);
}

template <IntensityShape::Level level>
bool IntensityShape::LevelIterator<level>::operator==(
        const IntensityShape::LevelIterator<level> &other) const
{
    return _currentIt == other._currentIt &&
            _currentLevel == other._currentLevel &&
            _endLevel == other._endLevel;
}

template <IntensityShape::Level level>
bool IntensityShape::LevelIterator<level>::operator!=(
        const IntensityShape::LevelIterator<level> &other) const
{
    return _currentIt != other._currentIt ||
            _currentLevel != other._currentLevel ||
            _endLevel != other._endLevel;
}

template <IntensityShape::Level level>
IntensityShape::ReverseLevelIterator<level>::ReverseLevelIterator(
        const IntensityShape &shape, bool end) :
    _shape(shape), _currentLevel(255), _currentIt(0), _endLevel(-1)
{
    if constexpr (level == CORE) {
        _currentLevel = _shape.getCoreThreshold();
    } else if constexpr (level == MAIN_SHAPE){
        _currentLevel = _shape.getShapeThreshold();
    } else if constexpr (level == NEIGHBORHOOD){
        _currentLevel = _shape.getNeighborhoodThreshold();
    } else if constexpr (level == EXTENDEDN){
        _currentLevel = _shape.getExtendedNeighborhoodThreshold();
    } else if constexpr (level == EX_MAIN_SHAPE){
        _currentLevel = _shape.getShapeThreshold();
        _endLevel = _shape.getCoreThreshold();
    } else if constexpr (level == EX_NEIGHBORHOOD){
        if (_shape.getExtendedNeighborhoodThreshold() <
                _shape.getNeighborhoodThreshold()){
            _endLevel = _shape.getExtendedNeighborhoodThreshold();
        } else {
            _endLevel = _shape.getShapeThreshold();
        }
        _currentLevel = _shape.getNeighborhoodThreshold();
    } else if constexpr (level == EX_EXTENDEDN){
        if (_shape.getExtendedNeighborhoodThreshold() <=
                _shape.getNeighborhoodThreshold()){
            _endLevel = _shape.getShapeThreshold();
        } else {
            _endLevel = _shape.getNeighborhoodThreshold();
        }
        _currentLevel = _shape.getExtendedNeighborhoodThreshold();
    }

    if (end) _currentLevel = _endLevel;

    while (_currentLevel > _endLevel &&
           _shape.getVoxelsWithGrayscale(255 - _currentLevel).size() == 0)
    {
        _currentLevel--;
    }
}

template <IntensityShape::Level level>
IntensityShape::ReverseLevelIterator<level> &
IntensityShape::ReverseLevelIterator<level>::operator++()
{
    _currentIt++;
    if (_currentIt == _shape.getVoxelsWithGrayscale(255 - _currentLevel).size())
    {
        _currentIt = 0;
        do {
            _currentLevel--;
        } while (_currentLevel > _endLevel &&
                 _shape.getVoxelsWithGrayscale(255 - _currentLevel).size() == 0);
    }
    return *this;
}

template <IntensityShape::Level level>
IntensityShape::ReverseLevelIterator<level>
IntensityShape::ReverseLevelIterator<level>::operator++(int)
{
    _currentIt++;
    if (_currentIt == _shape.getVoxelsWithGrayscale(255 - _currentLevel).size())
    {
        _currentIt = 0;
        do {
            _currentLevel--;
        } while (_currentLevel > _endLevel &&
                 _shape.getVoxelsWithGrayscale(255 - _currentLevel).size() == 0);
    }
    return *this;
}

template <IntensityShape::Level level>
const Voxel &IntensityShape::ReverseLevelIterator<level>::operator*() const
{
    return _shape.getVoxelsWithGrayscale(255 - _currentLevel).at(_currentIt);
}

template <IntensityShape::Level level>
bool IntensityShape::ReverseLevelIterator<level>::operator==(
        const IntensityShape::ReverseLevelIterator<level> &other) const
{
    return _currentIt == other._currentIt &&
            _currentLevel == other._currentLevel &&
            _endLevel == other._endLevel;
}

template <IntensityShape::Level level>
bool IntensityShape::ReverseLevelIterator<level>::operator!=(
        const IntensityShape::ReverseLevelIterator<level> &other) const
{
    return _currentIt != other._currentIt ||
            _currentLevel != other._currentLevel ||
            _endLevel != other._endLevel;
}


