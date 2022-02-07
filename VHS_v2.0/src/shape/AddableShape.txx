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
 * @file AddableShape.txx
 * Contains the implementation of the methods in the \ref AddableShape class.
 **/

#include "AddableShape.h"

using namespace shape_simplification::shape;

template <class Voxel>
AddableShape<Voxel>::AddableShape() : BaseShape<Voxel>() {}

template <class Voxel>
AddableShape<Voxel>::AddableShape(std::vector<Voxel> &voxels)
    : BaseShape<Voxel>(voxels) {}

template <class Voxel>
constexpr bool AddableShape<Voxel>::isAddable()
{
    return true;
}

template <class Voxel>
template <Level minLevel>
void AddableShape<Voxel>::addVoxel(Voxel v)
{
    std::array<voxel_container, 3> &voxels = BaseShape<Voxel>::_getVoxels();
    typename voxel_container::iterator it = voxels.at(0).find(v);
    typename voxel_container::iterator it2 = voxels.at(1).find(v);
    typename voxel_container::iterator it3 = voxels.at(2).find(v);

    v.setLevel(minLevel);

    if constexpr (minLevel == CORE) {
        if (it != voxels.at(0).end()) {
            voxels.at(0).erase(it);
            voxels.at(1).erase(it2);
            voxels.at(2).erase(it3);
        } else if (it2 != voxels.at(1).end()) {
            voxels.at(1).erase(it2);
            voxels.at(2).erase(it3);
        } else if (it3 != voxels.at(2).end()) {
            voxels.at(2).erase(it3);
        }

        voxels.at(0).insert(v);
        voxels.at(1).insert(v);
        voxels.at(2).insert(v);
    } else if constexpr (minLevel == MAIN_SHAPE) {
        if (it == voxels.at(0).end()){
            if (it2 != voxels.at(1).end()) {
                voxels.at(1).erase(it2);
                voxels.at(2).erase(it3);
            } else if (it3 != voxels.at(2).end()) {
                voxels.at(2).erase(it3);
            }

            voxels.at(1).insert(v);
            voxels.at(2).insert(v);
        }
    } else if constexpr (minLevel == NEIGHBORHOOD) {
        if (it == voxels.at(0).end() && it2 == voxels.at(1).end()){
            if (it3 != voxels.at(2).end()) {
                voxels.at(2).erase(it3);
            }

            voxels.at(2).insert(v);
        }
    }

    std::array<unsigned int, 3> &dimensions = BaseShape<Voxel>::_getShapeDimensions();
    if (v[0] > dimensions[0])
        dimensions[0] = v[0];
    if (v[1] > dimensions[1])
        dimensions[1] = v[1];
    if (v[2] > dimensions[2])
        dimensions[2] = v[2];
}
