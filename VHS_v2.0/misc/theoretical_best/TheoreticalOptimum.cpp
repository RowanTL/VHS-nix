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

#include "TheoreticalOptimum.h"

namespace shape_simplification::optimum {

TheoreticalOptimum::TheoreticalOptimum()
    : _minBettyNumbers{0, 0, 0},
      _nberOfCoreCells(0)
{}

TheoreticalOptimum::~TheoreticalOptimum()
{}

void TheoreticalOptimum::loadPreprocessedImage(
        std::string inputFileNameBase,
        unsigned int numberOfImages,
        unsigned char coreThreshold,
        unsigned char shapeThreshold,
        unsigned char neighborhoodThreshold)
{
    ProcessedImageStack proc;
    _shape.clear();

    _printMessage("--Load image.");

    proc.load(inputFileNameBase, numberOfImages);
    proc.setCoreThreshold(coreThreshold);
    proc.setShapeThreshold(shapeThreshold);
    proc.setNeighborhoodThreshold(neighborhoodThreshold);

    _printMessage("--Load shape.");

    proc >> _shape;
}

void TheoreticalOptimum::printTheoreticalOptimum(){
    _computeOptimum();
    _printTheoreticalOptimum();
}

void TheoreticalOptimum::_computeOptimum(){
    Skeleton skeleton(_shape);
    matrix_type matrix;
    std::vector<timestamp_type> indexToTimestamp;
    std::unordered_set<index> coreIndices;

    _constructMatrix(skeleton, matrix, indexToTimestamp);
    for (index i = 0; i < _nberOfCoreCells; i++) coreIndices.insert(i);

    phat::persistence_pairs pairs;
    phat::compute_persistence_pairs<phat::twist_reduction>(pairs, matrix);

    pairs.sort();
    for(index idx = 0; idx < pairs.get_num_pairs(); idx++) {
        index birth = pairs.get_pair(idx).first;
        index death = pairs.get_pair(idx).second;
        if (indexToTimestamp.at(birth) == 0) coreIndices.erase(birth);
        if (indexToTimestamp.at(death) == 0) coreIndices.erase(death);
    }

    for (index i : coreIndices){
        _minBettyNumbers[matrix.get_dim(i)]++;
    }
}

void TheoreticalOptimum::_constructMatrix(
        Skeleton &skeleton,
        matrix_type &matrix,
        std::vector<timestamp_type> &indexToTimestamp) {
    CellContainer cells;

    std::unordered_map<Cell<0>, index, Cell<0>::CellHasher> vertexToIndex;
    std::unordered_map<Cell<1>, index, Cell<1>::CellHasher> edgeToIndex;
    std::unordered_map<Cell<2>, index, Cell<2>::CellHasher> squareToIndex;
    std::unordered_map<Cell<3>, index, Cell<3>::CellHasher> cubeToIndex;

    _constructFiltration<0>(skeleton, cells);
    _constructFiltration<1>(skeleton, cells);
    _constructFiltration<2>(skeleton, cells);
    _constructFiltration<3>(skeleton, cells);

    index numberOfCells =
            cells.getSize<0>() +
            cells.getSize<1>() +
            cells.getSize<2>() +
            cells.getSize<3>();

    matrix.set_num_cols(numberOfCells);

    indexToTimestamp.resize(numberOfCells);

    cell_iterator<0> vertexIt = cells.begin<0>();
    cell_iterator<1> edgeIt = cells.begin<1>();
    cell_iterator<2> squareIt = cells.begin<2>();
    cell_iterator<3> cubeIt = cells.begin<3>();
    cell_iterator<0> vertexEnd = cells.end<0>();
    cell_iterator<1> edgeEnd = cells.end<1>();
    cell_iterator<2> squareEnd = cells.end<2>();
    cell_iterator<3> cubeEnd = cells.end<3>();

    index current = 0;
    for (unsigned int timestamp = 0; timestamp < 3; ++timestamp) {
        _addVertexColumns(vertexIt, vertexEnd, timestamp,
                          current, vertexToIndex,
                          matrix, indexToTimestamp);
        _addColumns<1>(edgeIt, edgeEnd, timestamp,
                       current, edgeToIndex, vertexToIndex,
                       matrix, indexToTimestamp);
        _addColumns<2>(squareIt, squareEnd, timestamp,
                       current, squareToIndex, edgeToIndex,
                       matrix, indexToTimestamp);
        _addColumns<3>(cubeIt, cubeEnd, timestamp,
                       current, cubeToIndex, squareToIndex,
                       matrix, indexToTimestamp);
        if (timestamp == 0) _nberOfCoreCells = current;
    }
}

void TheoreticalOptimum::_addVertexColumns(
        cell_iterator<0> &cellIt,
        cell_iterator<0> &endIt,
        unsigned int timestamp,
        index &current,
        std::unordered_map<Cell<0>, index, Cell<0>::CellHasher> &cellToIndex,
        matrix_type &matrix,
        std::vector<timestamp_type>& indexToTimestamp)
{
    if (cellIt == endIt) return;

    phat::column col;
    Cell<0> currentCell = *cellIt;
    while (cellIt != endIt && currentCell.getTimestamp() == timestamp) {
        matrix.set_dim(current, 0);
        matrix.set_col(current, col);
        indexToTimestamp.at(current) = currentCell.getTimestamp();
        cellToIndex.emplace(currentCell, current);
        ++current;
        ++cellIt;
        if (cellIt != endIt) currentCell = *cellIt;
    }
}

template <unsigned int dim>
void TheoreticalOptimum::_addColumns(
        cell_iterator<dim> &cellIt,
        cell_iterator<dim> &endIt,
        unsigned int timestamp,
        index &current,
        std::unordered_map<Cell<dim>,index,typename Cell<dim>::CellHasher>
        &cellToIndex,
        std::unordered_map<Cell<dim - 1>,index,typename Cell<dim-1>::CellHasher>
        &boundaryCellToIndex,
        matrix_type &matrix,
        std::vector<timestamp_type>& indexToTimestamp)
{
    if (cellIt == endIt) return;

    phat::column col;
    Cell<dim> currentCell = *cellIt;
    while (cellIt != endIt && currentCell.getTimestamp() == timestamp) {
        auto boundary = currentCell.getBoundary();
        for (Cell<dim - 1> cell : boundary) {
            col.push_back(boundaryCellToIndex.at(cell));
        }
        std::sort(col.begin(), col.end());
        matrix.set_dim(current, dim);
        matrix.set_col(current, col);
        indexToTimestamp.at(current) = currentCell.getTimestamp();
        cellToIndex.emplace(currentCell, current);
        col.clear();
        ++current;
        ++cellIt;
        if (cellIt != endIt) currentCell = *cellIt;
    }
}

template <unsigned int dim>
void TheoreticalOptimum::_constructFiltration(
        Skeleton &skeleton, CellContainer &cells)
{
    cells.resize<dim>(skeleton.template getNumberOfCells<dim>());

    unsigned int coreIndex = 0;
    unsigned int shapeIndex = skeleton.getNumberOfCellsInMainShape<dim>() - 1;
    unsigned int neighborhoodIndex = shapeIndex + 1;

    for (auto it = skeleton.template begin<dim>();
         it != skeleton.template end<dim>();
         ++it)
    {
        if ((*it).getTimestamp() == 0)
            cells.insertAt<dim>(*it, coreIndex++);
        else if ((*it).getTimestamp() == 1)
            cells.insertAt<dim>(*it, shapeIndex--);
        else
            cells.insertAt<dim>(*it, neighborhoodIndex++);
    }
}

void TheoreticalOptimum::_printTheoreticalOptimum(){
    std::cout << "\n";
    std::cout << "--- Theoretical Optimum ---\n";
    std::cout << "\n";
    std::cout << "Number of components: " << _minBettyNumbers[0] << "\n";
    std::cout << "Number of loops: " << _minBettyNumbers[1] << "\n";
    std::cout << "Number of voids: " << _minBettyNumbers[2] << "\n";
    std::cout << "\n";
    std::cout << "----------------------------\n";
    std::cout << "\n";
}

void TheoreticalOptimum::_printMessage(std::string message){
    std::cout << message << std::endl;
}

} // namespace shape_simplification::optimum
