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
 * @file ConnectedComponents.h
 * Contains the declaration of the \ref ConnectedComponents class.
 **/

#include <vector>

#include "util/UnionFind.h"
#include "util/UnionFindDelete.h"

namespace shape_simplification{ namespace shape {

using util::UnionFind;
using util::UnionFindDelete;

template <class Voxel>
/**
 * class ConnectedComponents
 * @brief Keeps track of the connected components of a shape via a Union-Find
 * structure.
 * @tparam Voxel Voxel class.
 **/
class ConnectedComponents : public UnionFind<Voxel> {
public:
    /**
     * @brief Constructor.
     */
    ConnectedComponents();

    /**
     * @brief Clears the content of the structure.
     */
    void clear();

private:
};

template <class Voxel>
/**
 * class ConnectedComponentsWithRemoval
 * @brief Keeps track of the connected components of a shape
 * via a Union-Find-Delete structure. Careful: it can remove
 * a voxel from a component, but cannot split the component into two.
 * @tparam Voxel Voxel class.
 **/
class ConnectedComponentsWithRemoval : public UnionFindDelete<Voxel> {
public:
    /**
     * @brief Constructor.
     */
    ConnectedComponentsWithRemoval();
    /**
     * @brief Copy Constructor.
     */
    ConnectedComponentsWithRemoval(const ConnectedComponentsWithRemoval& cc);

    /**
     * @brief Returns the size of the connected component
     * containing the given voxel.
     * @param v Voxel contained in the connected component.
     * @return The size of the connected component
     * containing @p v.
     */
    unsigned long long getComponentSize(Voxel &v);
    /**
     * @brief Fills given container with the voxels
     * of the connected component containing the given voxel.
     * @param v Voxel contained in the connected component.
     * @param container Container for the connected component.
     * Does not get cleared before filling.
     */
    void getComponent(Voxel &v, std::vector<Voxel> &container);

    /**
     * @brief Clears the content of the structure.
     */
    void clear();

private:
};

#include "ConnectedComponents.txx"

}} // namespace shape_simplification::shape
