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
 * @file BaseSkeleton.txx
 * Contains the implementation of the methods in the \ref BaseSkeleton class.
 **/

#include "BaseSkeleton.h"

using namespace shape_simplification::shape;

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
BaseSkeleton<Shape, Voxel, minDim, maxDim>::BaseSkeleton(Shape &shape)
    : _shape(shape), _nberOfCellsInMainShape{0, 0, 0, 0} {}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
BaseSkeleton<Shape, Voxel, minDim, maxDim>::BaseSkeleton()
    : _nberOfCellsInMainShape{0, 0, 0, 0} {}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
BaseSkeleton<Shape, Voxel, minDim, maxDim>::~BaseSkeleton() {}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline constexpr bool
BaseSkeleton<Shape, Voxel, minDim, maxDim>::isAddable(){
    return false;
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline constexpr bool
BaseSkeleton<Shape, Voxel, minDim, maxDim>::isRemovable(){
    return false;
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline constexpr unsigned int
BaseSkeleton<Shape, Voxel, minDim, maxDim>::getMinDim() {
    return minDim;
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline constexpr unsigned int
BaseSkeleton<Shape, Voxel, minDim, maxDim>::getMaxDim() {
    return maxDim;
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
template<unsigned int dim>
inline CellContainer::cell_iterator<dim>
BaseSkeleton<Shape, Voxel, minDim, maxDim>::begin(){
    return _cells.begin<dim>();
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
template<unsigned int dim>
inline CellContainer::cell_iterator<dim>
BaseSkeleton<Shape, Voxel, minDim, maxDim>::end(){
    return _cells.end<dim>();
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
template<unsigned int dim>
inline unsigned int
BaseSkeleton<Shape, Voxel, minDim, maxDim>::getNumberOfCells() const{
    return _cells.getSize<dim>();
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
template<unsigned int dim>
inline unsigned int
BaseSkeleton<Shape, Voxel, minDim, maxDim>::getNumberOfCellsInMainShape() const
{
    return _nberOfCellsInMainShape[dim];
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline int
BaseSkeleton<Shape, Voxel, minDim, maxDim>::getEulerCharacteristic() const {
    return _nberOfCellsInMainShape[0] - _nberOfCellsInMainShape[1] +
            _nberOfCellsInMainShape[2] - _nberOfCellsInMainShape[3];
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline const CellContainer&
BaseSkeleton<Shape, Voxel, minDim, maxDim>::getCellContainer() const
{
    return _cells;
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
template<unsigned int dim>
inline void
BaseSkeleton<Shape, Voxel, minDim, maxDim>::_insertCell(Cell<dim>& cell){
    _cells.insert<dim>(cell);
    if (cell.getTimestamp() < 2) ++_nberOfCellsInMainShape[dim];
}

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
inline Shape&
BaseSkeleton<Shape, Voxel, minDim, maxDim>::_getShape(){
    return _shape;
}
