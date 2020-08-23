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
 * @file PersistentHomology.h
 * Contains the declaration of the \ref PersistentHomology class.
 **/

#include <set>
#include <utility>
#include <vector>
#include <unordered_map>

#include "persistence/BoundaryMatrix.h"
#include "shape/Cell.h"
#include "util/Point.h"

namespace shape_simplification{ namespace persistence {

using shape::Cell;
using util::Point;

template <class Skeleton, class Voxel,
          unsigned int minDim, unsigned int maxDim>
/**
 * class PersistentHomology
 * @brief Computes persistence from the given Skeleton.
 * Keeps also track of the cycles changing the homology.
 * @tparam Skeleton Skeleton type.
 * @tparam Voxel Voxel type used by the skeleton.
 * @tparam minDim Minimal dimension to compute persistence from.
 * @tparam maxDim Maximal dimension to compute persistence from.
 **/
class PersistentHomology {
public:
    using index = BoundaryMatrix::index;
    using persistent_pair = std::pair<index, index>;
    using timestamp_type = Cell<0>::timestamp_type;
    using dim_type = BoundaryMatrix::dim_type;
    template<int dim>
    using cell_container = CellContainer::cell_container<dim>;
    template <int dim>
    using cell_iterator = CellContainer::cell_iterator<dim>;

    /**
     * @brief Constructor.
     * @param skeleton Skeleton from which to extract the filtration.
     */
    PersistentHomology(Skeleton &skeleton);
    /**
     * @brief Destructor.
     */
    ~PersistentHomology();

    /**
     * @brief Reduces the boundary matrix.
     */
    void reduceMatrix();
    /**
     * @brief If \ref reduceMatrix was called,
     * it returns the corresponding persistence pairs.
     * @return The persistence pairs from the reduced matrix.
     */
    const std::vector<persistent_pair> &getPersistentPairs() const;
    /**
     * @brief Returns Voxels in the cycle
     * which created the given persistence pair.
     * @param pair Persistence pair.
     * @param cycle Reference to an empty set of
     * Voxel where the result will be stored.
     */
    void getCorrespondingVoxelChain(persistent_pair &pair, std::vector<Voxel> &chain);
    /**
     * @brief Returns the timestamp of the given column.
     * @param colIndex Column index.
     * @return The timestamp of @p colIndex.
     */
    timestamp_type getTimestamp(index colIndex) const;
    /**
     * @brief Returns the dimension of the given column.
     * @param colIndex Column index.
     * @return The dimension of @p colIndex.
     */
    dim_type getDimension(index colIndex);

private:
    BoundaryMatrix _matrix;                             ///< Boundary matrix.
    std::vector<persistent_pair> _pairs;                ///< Persistence pairs, after matrix
                                                        ///< reduction.
    std::vector<timestamp_type> _indexToTimestamp;      ///< Map from column index to timestamp.
    std::vector<std::vector<Voxel> > _indexToVoxels;    ///< Map from column index to corresponding
                                                        ///< voxels in the shape.
    bool _isReduced;                                    ///< True if matrix is reduced.
    index _neighborhoodStart;                           ///< Index of the first neigborhood cell.

    /**
     * @brief Constructs the boundary matrix from the skeleton.
     * @param skeleton Given skeleton.
     */
    void _constructMatrix(Skeleton &skeleton);
    template <unsigned int dim>
    /**
     * @brief Constructs filtration for given dimension.
     * @tparam dim Dimension of the cells.
     * @param skeleton
     * @param cells
     */
    void _constructFiltration(Skeleton &skeleton, CellContainer &cells);
    /**
     * @brief Inserts 0-dimensional columns into the boundary matrix.
     * @param cellIt
     * @param endIt
     * @param timestamp
     * @param current
     * @param cellToIndex
     * @param skeleton
     */
    void _addVertexColumns(
            cell_iterator<0> &cellIt,
            cell_iterator<0> &endIt,
            int timestamp,
            index &current,
            std::unordered_map<Cell<0>,index,Cell<0>::CellHasher> &cellToIndex,
            Skeleton &skeleton);
    template <unsigned int dim>
    /**
     * @brief Inserts dim-dimensional columns into the boundary matrix.
     * @tparam dim Dimension of the columns.
     * @param cellIt
     * @param endIt
     * @param timestamp
     * @param current
     * @param cellToIndex
     * @param boundaryCellToIndex
     * @param skeleton
     */
    void
    _addColumns(cell_iterator<dim> &cellIt,
                cell_iterator<dim> &endIt,
                int timestamp,
                index &current,
                std::unordered_map<Cell<dim>,index,typename Cell<dim>::CellHasher>
                    &cellToIndex,
                std::unordered_map<Cell<dim - 1>,index,typename Cell<dim - 1>::CellHasher>
                    &boundaryCellToIndex,
                Skeleton &skeleton);
};

#include "PersistentHomology.txx"

}} // namespace shape_simplification::persistence
