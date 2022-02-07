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
 * @file RemovableShape.txx
 * Contains the implementation of the methods in the \ref RemovableShape class.
 **/

#include "RemovableShape.h"

using namespace shape_simplification::shape;

template <class Voxel>
RemovableShape<Voxel>::RemovableShape() : BaseShape<Voxel>() {}

template <class Voxel>
RemovableShape<Voxel>::RemovableShape(std::vector<Voxel> &voxels)
    : BaseShape<Voxel>(voxels) {}

template <class Voxel>
constexpr bool RemovableShape<Voxel>::isRemovable() {
    return true;
}

template <class Voxel>
template <Level maxLevel>
void RemovableShape<Voxel>::removeVoxel(Voxel v) {
    std::array<voxel_container, 3> &voxels = BaseShape<Voxel>::_getVoxels();

    if (voxels.at(2).find(v) == voxels.at(2).end()) return;

    if constexpr (maxLevel == CORE) {
        auto it = voxels.at(1).find(v);
        if (it != voxels.at(1).end()) {
            v.setLevel(MAIN_SHAPE);
            voxels.at(1).erase(v);
            voxels.at(1).insert(v);
            voxels.at(2).erase(v);
            voxels.at(2).insert(v);
        } else {
            v.setLevel(NEIGHBORHOOD);
            voxels.at(2).erase(v);
            voxels.at(2).insert(v);
        }
        voxels.at(0).erase(v);
    } else if constexpr (maxLevel == MAIN_SHAPE) {
        v.setLevel(NEIGHBORHOOD);
        voxels.at(2).erase(v);
        voxels.at(2).insert(v);

        voxels.at(0).erase(v);
        voxels.at(1).erase(v);
    } else if constexpr (maxLevel == NEIGHBORHOOD) {
        voxels.at(0).erase(v);
        voxels.at(1).erase(v);
        voxels.at(2).erase(v);
    }
}
