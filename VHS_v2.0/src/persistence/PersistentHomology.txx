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
 * @file PersistentHomology.txx
 * Contains the implementation of the methods in the \ref PersistentHomology
 * class.
 **/

#include "PersistentHomology.h"

using namespace shape_simplification::persistence;

template <class Skeleton, class Voxel,
          unsigned int minDim, unsigned int maxDim>
PersistentHomology<Skeleton, Voxel, minDim, maxDim>::
PersistentHomology(Skeleton &skeleton)
    : _isReduced(false), _neighborhoodStart(-1)
{
    _constructMatrix(skeleton);
}

template <class Skeleton, class Voxel,
          unsigned int minDim, unsigned int maxDim>
PersistentHomology<Skeleton, Voxel, minDim, maxDim>::~PersistentHomology()
{}

template <class Skeleton, class Voxel,
          unsigned int minDim, unsigned int maxDim>
void PersistentHomology<Skeleton, Voxel, minDim, maxDim>::reduceMatrix()
{
    if (_isReduced) return;

    const index nberOfColumns = _matrix.getNumberOfColumns();
    std::vector<index> currentColumnWithPivot(nberOfColumns, -1);

    for (index curColIndex = 0; curColIndex < nberOfColumns; curColIndex++) {
        index pivot = _matrix.getPivot(curColIndex);
        while (pivot != -1 && currentColumnWithPivot[pivot] != -1) {
            _matrix.addColTo(currentColumnWithPivot[pivot], curColIndex);
            pivot = _matrix.getPivot(curColIndex);
        }
        if (pivot != -1) {
            currentColumnWithPivot[pivot] = curColIndex;
            if (_indexToTimestamp.at(pivot) != _indexToTimestamp.at(curColIndex)){
                _pairs.push_back(std::make_pair(pivot, curColIndex));
            }
        }
        _matrix.finalize(curColIndex);
    }

    _isReduced = true;
}

template <class Skeleton, class Voxel,
          unsigned int minDim, unsigned int maxDim>
const std::vector<
    typename PersistentHomology<Skeleton, Voxel, minDim, maxDim>::persistent_pair
>&
PersistentHomology<Skeleton, Voxel, minDim, maxDim>::getPersistentPairs() const
{
    return _pairs;
}

template <class Skeleton, class Voxel,
          unsigned int minDim, unsigned int maxDim>
void PersistentHomology<Skeleton, Voxel, minDim, maxDim>::
getCorrespondingVoxelChain(persistent_pair &pair, std::vector<Voxel> &chain)
{
    std::vector<index> col;
    _matrix.getInverseColumn(pair.second, col);

    for (index i : col) {
        for (Voxel v : _indexToVoxels.at(i))
            chain.push_back(v);
    }
}

template <class Skeleton, class Voxel,
          unsigned int minDim, unsigned int maxDim>
typename PersistentHomology<Skeleton, Voxel, minDim, maxDim>::
timestamp_type
PersistentHomology<Skeleton, Voxel, minDim, maxDim>::getTimestamp(
        index colIndex) const
{
    return _indexToTimestamp.at(colIndex);
}

template <class Skeleton, class Voxel,
          unsigned int minDim, unsigned int maxDim>
typename PersistentHomology<Skeleton, Voxel, minDim, maxDim>::
dim_type
PersistentHomology<Skeleton, Voxel, minDim, maxDim>::
getDimension(index colIndex)
{
    return _matrix.getDimension(colIndex);
}

template <class Skeleton, class Voxel,
          unsigned int minDim, unsigned int maxDim>
void PersistentHomology<Skeleton, Voxel, minDim, maxDim>::
_constructMatrix(Skeleton &skeleton)
{
    CellContainer cells;

    std::unordered_map<Cell<0>, index, Cell<0>::CellHasher> vertexToIndex;
    std::unordered_map<Cell<1>, index, Cell<1>::CellHasher> edgeToIndex;
    std::unordered_map<Cell<2>, index, Cell<2>::CellHasher> squareToIndex;
    std::unordered_map<Cell<3>, index, Cell<3>::CellHasher> cubeToIndex;

    if constexpr (minDim == 0)
            _constructFiltration<0>(skeleton, cells);
    if constexpr (minDim <= 1 && maxDim >= 1)
            _constructFiltration<1>(skeleton, cells);
    if constexpr (minDim <= 2 && maxDim >= 2)
            _constructFiltration<2>(skeleton, cells);
    if constexpr (maxDim == 3)
            _constructFiltration<3>(skeleton, cells);

    unsigned int numberOfCells =
            cells.getSize<0>() +
            cells.getSize<1>() +
            cells.getSize<2>() +
            cells.getSize<3>();

    _matrix.setNumberOfColumns(numberOfCells);

    _indexToTimestamp.resize(numberOfCells);
    _indexToVoxels.resize(numberOfCells);

    cell_iterator<0> vertexIt = cells.begin<0>();
    cell_iterator<1> edgeIt = cells.begin<1>();
    cell_iterator<2> squareIt = cells.begin<2>();
    cell_iterator<3> cubeIt = cells.begin<3>();
    cell_iterator<0> vertexEnd = cells.end<0>();
    cell_iterator<1> edgeEnd = cells.end<1>();
    cell_iterator<2> squareEnd = cells.end<2>();
    cell_iterator<3> cubeEnd = cells.end<3>();

    index current = 0;
    for (int timestamp = 0; timestamp < 3; ++timestamp) {
        if (timestamp == 2) _neighborhoodStart = current;
        if constexpr (minDim == 0)
                _addVertexColumns(vertexIt, vertexEnd, timestamp,
                                  current, vertexToIndex, skeleton);
        if constexpr (minDim <= 1 && maxDim >= 1)
                _addColumns<1>(edgeIt, edgeEnd, timestamp,
                               current, edgeToIndex, vertexToIndex, skeleton);
        if constexpr (minDim <= 2 && maxDim >= 2)
                _addColumns<2>(squareIt, squareEnd, timestamp,
                               current, squareToIndex, edgeToIndex, skeleton);
        if constexpr (maxDim == 3)
                _addColumns<3>(cubeIt, cubeEnd, timestamp,
                               current, cubeToIndex, squareToIndex, skeleton);
    }
}

template <class Skeleton, class Voxel,
          unsigned int minDim, unsigned int maxDim>
template <unsigned int dim>
void PersistentHomology<Skeleton, Voxel, minDim, maxDim>::
_constructFiltration(Skeleton &skeleton, CellContainer &cells)
{
    cells.resize<dim>(skeleton.template getNumberOfCells<dim>());

    unsigned int coreIndex = 0;
    unsigned int shapeIndex = skeleton.template getNumberOfCellsInMainShape<dim>() - 1;
    unsigned int neighborhoodIndex = shapeIndex + 1;

    for (auto it = skeleton.template begin<dim>();
         it != skeleton.template end<dim>();
         ++it)
    {
        if ((*it).getTimestamp() == 0) cells.insertAt<dim>(*it, coreIndex++);
        else if ((*it).getTimestamp() == 1) cells.insertAt<dim>(*it, shapeIndex--);
        else cells.insertAt<dim>(*it, neighborhoodIndex++);
    }
}

template <class Skeleton, class Voxel,
          unsigned int minDim, unsigned int maxDim>
void PersistentHomology<Skeleton, Voxel, minDim, maxDim>::
_addVertexColumns(
        cell_iterator<0> &cellIt,
        cell_iterator<0> &endIt,
        int timestamp, index &current,
        std::unordered_map<Cell<0>, index, Cell<0>::CellHasher> &cellToIndex,
        Skeleton &skeleton)
{
    if (cellIt == endIt) return;

    std::vector<index> col;
    Cell<0> currentCell = *cellIt;
    while (cellIt != endIt && currentCell.getTimestamp() == timestamp) {
        _matrix.addColumn(current, 0, col);
        skeleton.getCorrespondingVoxels(currentCell, _indexToVoxels.at(current));
        _indexToTimestamp.at(current) = currentCell.getTimestamp();
        cellToIndex.emplace(currentCell, current);
        ++current;
        ++cellIt;
        if (cellIt != endIt) currentCell = *cellIt;
    }
}

template <class Skeleton, class Voxel,
          unsigned int minDim, unsigned int maxDim>
template <unsigned int dim>
void PersistentHomology<Skeleton, Voxel, minDim, maxDim>::
_addColumns(
        cell_iterator<dim> &cellIt,
        cell_iterator<dim> &endIt,
        int timestamp, index &current,
        std::unordered_map<Cell<dim>, index, typename Cell<dim>::CellHasher>
        &cellToIndex,
        std::unordered_map<Cell<dim - 1>, index, typename Cell<dim - 1>::CellHasher>
        &boundaryCellToIndex,
        Skeleton &skeleton)
{
    if (cellIt == endIt) return;

    std::vector<index> col;
    Cell<dim> currentCell = *cellIt;
    while (cellIt != endIt && currentCell.getTimestamp() == timestamp) {
        auto boundary = currentCell.getBoundary();
        if (dim != minDim) {
            for (Cell<dim - 1> cell : boundary) {
                col.push_back(boundaryCellToIndex.at(cell));
            }
            std::sort(col.begin(), col.end());
        }
        _matrix.addColumn(current, dim, col);
        skeleton.getCorrespondingVoxels(currentCell, _indexToVoxels.at(current));
        _indexToTimestamp.at(current) = currentCell.getTimestamp();
        cellToIndex.emplace(currentCell, current);
        col.clear();
        ++current;
        ++cellIt;
        if (cellIt != endIt) currentCell = *cellIt;
    }
}


