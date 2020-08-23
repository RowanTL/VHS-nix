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
 * @file RemovableShape.txx
 * Contains the implementation of the methods in the \ref RemovableShape class.
 **/

#include "RemovableShape.h"

using namespace shape_simplification::shape;

template <class Voxel>
RemovableShape<Voxel>::RemovableShape() : BaseShape<Voxel>() {}

template <class Voxel>
RemovableShape<Voxel>::RemovableShape(std::vector<Voxel> &voxels,
                                      std::vector<Level> &minLevels)
    : BaseShape<Voxel>(voxels, minLevels) {}

template <class Voxel>
constexpr bool RemovableShape<Voxel>::isRemovable() {
    return true;
}

template <class Voxel>
template <typename RemovableShape<Voxel>::Level maxLevel>
void RemovableShape<Voxel>::removeVoxel(Voxel v) {
    std::array<voxel_container, 3> &voxels = BaseShape<Voxel>::_getVoxels();
    if constexpr (maxLevel == CORE) {
        voxels.at(0).erase(v);
    } else if constexpr (maxLevel == MAIN_SHAPE) {
        voxels.at(0).erase(v);
        voxels.at(1).erase(v);
    } else if constexpr (maxLevel == NEIGHBORHOOD) {
        voxels.at(0).erase(v);
        voxels.at(1).erase(v);
        voxels.at(2).erase(v);
    }
}
