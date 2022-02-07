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
 * @file InflationShape.txx
 * Contains the implementation of the methods in the \ref Shape class.
 **/

#include "InflationShape.h"

using namespace shape_simplification::shape;


constexpr bool InflationShape::isAddable() {
    return false;
}

constexpr bool InflationShape::isRemovable() {
    return false;
}

template <Level maxLevel>
void InflationShape::addVoxel(Voxel v) {
    static_assert(maxLevel == -1, "Addition of voxel not enabled.");
}

template <Level maxLevel>
void InflationShape::removeVoxel(Voxel v) {
    static_assert(maxLevel == -1, "Removal of voxel not enabled.");
}

template <Level level>
typename InflationShape::voxel_iterator<level> InflationShape::begin() const
{
    static_assert(level != OUTSIDE,
            "It is not possible to iterate voxels not contained in Shape.");
    return LevelIterator<level>(*this);
}

template <Level level>
typename InflationShape::voxel_iterator<level> InflationShape::end() const
{
    static_assert(level != OUTSIDE,
            "It is not possible to iterate voxels not contained in Shape.");
    return LevelIterator<level>(*this, true);
}

template <Level level>
typename InflationShape::voxel_reverse_iterator<level>
InflationShape::rbegin() const
{
    static_assert(level != OUTSIDE,
            "It is not possible to iterate voxels not contained in Shape.");
    return ReverseLevelIterator<level>(*this);
}

template <Level level>
typename InflationShape::voxel_reverse_iterator<level>
InflationShape::rend() const
{
    static_assert(level != OUTSIDE,
            "It is not possible to iterate voxels not contained in Shape.");
    return ReverseLevelIterator<level>(*this, true);
}

template <Level level>
InflationShape::LevelIterator<level>::LevelIterator(
        const InflationShape &shape, bool end) :
    _shape(shape), _currentLevel(0), _currentIt(0), _endLevel(256)
{
    if constexpr (level == MAIN_SHAPE){
        _endLevel = 1;
    } else if constexpr (level == NEIGHBORHOOD){
        _endLevel = shape.getInflationValue() + 1;
    } else if constexpr (level == EX_NEIGHBORHOOD){
        _currentLevel = 1;
        _endLevel = shape.getInflationValue() + 1;
    }

    if (end) _currentLevel = _endLevel;
}

template <Level level>
InflationShape::LevelIterator<level> &
InflationShape::LevelIterator<level>::operator++()
{
    _currentIt++;
    if (_currentIt == _shape.getVoxelsAtDistance(_currentLevel).size()){
        _currentIt = 0;
        _currentLevel++;
    }
    return *this;
}

template <Level level>
InflationShape::LevelIterator<level>
InflationShape::LevelIterator<level>::operator++(int)
{
    _currentIt++;
    if (_currentIt == _shape.getVoxelsAtDistance(_currentLevel).size()){
        _currentIt = 0;
        _currentLevel++;
    }
    return *this;
}

template <Level level>
const Voxel &InflationShape::LevelIterator<level>::operator*() const
{
    return _shape.getVoxelsAtDistance(_currentLevel).at(_currentIt);
}

template <Level level>
bool InflationShape::LevelIterator<level>::operator==(
        const InflationShape::LevelIterator<level> &other) const
{
    return _currentIt == other._currentIt &&
            _currentLevel == other._currentLevel &&
            _endLevel == other._endLevel;
}

template <Level level>
bool InflationShape::LevelIterator<level>::operator!=(
        const InflationShape::LevelIterator<level> &other) const
{
    return _currentIt != other._currentIt ||
            _currentLevel != other._currentLevel ||
            _endLevel != other._endLevel;
}

template <Level level>
InflationShape::ReverseLevelIterator<level>::ReverseLevelIterator(
        const InflationShape &shape, bool end) :
    _shape(shape), _currentLevel(255), _currentIt(0), _endLevel(-1)
{
    if constexpr (level == MAIN_SHAPE){
        _currentLevel = 0;
    } else if constexpr (level == NEIGHBORHOOD){
        _currentLevel = shape.getInflationValue();
    } else if constexpr (level == EX_NEIGHBORHOOD){
        _endLevel = 0;
        _currentLevel = shape.getInflationValue();
    }

    if (end) _currentLevel = _endLevel;
}

template <Level level>
InflationShape::ReverseLevelIterator<level> &
InflationShape::ReverseLevelIterator<level>::operator++()
{
    _currentIt++;
    if (_currentIt == _shape.getVoxelsAtDistance(_currentLevel).size()){
        _currentIt = 0;
        _currentLevel--;
    }
    return *this;
}

template <Level level>
InflationShape::ReverseLevelIterator<level>
InflationShape::ReverseLevelIterator<level>::operator++(int)
{
    _currentIt++;
    if (_currentIt == _shape.getVoxelsAtDistance(_currentLevel).size()){
        _currentIt = 0;
        _currentLevel--;
    }
    return *this;
}

template <Level level>
const Voxel &InflationShape::ReverseLevelIterator<level>::operator*() const
{
    return _shape.getVoxelsAtDistance(_currentLevel).at(_currentIt);
}

template <Level level>
bool InflationShape::ReverseLevelIterator<level>::operator==(
        const InflationShape::ReverseLevelIterator<level> &other) const
{
    return _currentIt == other._currentIt &&
            _currentLevel == other._currentLevel &&
            _endLevel == other._endLevel;
}

template <Level level>
bool InflationShape::ReverseLevelIterator<level>::operator!=(
        const InflationShape::ReverseLevelIterator<level> &other) const
{
    return _currentIt != other._currentIt ||
            _currentLevel != other._currentLevel ||
            _endLevel != other._endLevel;
}


