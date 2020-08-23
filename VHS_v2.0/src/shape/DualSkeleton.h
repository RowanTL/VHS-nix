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
 * @file DualSkeleton.h
 * Contains the declaration of the \ref DualSkeleton class.
 **/

#include <array>
#include <vector>

#include "shape/BaseSkeleton.h"
#include "shape/PrimalSkeleton.h"
#include "shape/Cell.h"
#include "shape/VoxelSkeletonIterators.h"

namespace shape_simplification{ namespace shape {

template <class Shape, class Voxel,
          unsigned int minDim, unsigned int maxDim>
/**
 * class DualSkeleton
 * @brief The dual skeleton class.
 * @tparam Shape Shape class used to construct the skeleton.
 * @tparam Voxel Voxel class of the shape.
 * @tparam minDim Minimal dimension of the skeleton.
 * @tparam maxDim Maximal dimension of the skeleton.
 **/
class DualSkeleton : public BaseSkeleton<Shape, Voxel, minDim, maxDim> {
public:
    using BSkeleton = BaseSkeleton<Shape, Voxel, minDim, maxDim>;
    using PSkeleton = PrimalSkeleton<Shape, Voxel, minDim, maxDim>;
    using typename BSkeleton::cube;
    using typename BSkeleton::cube_container;
    using typename BSkeleton::cube_iterator;
    using typename BSkeleton::edge;
    using typename BSkeleton::edge_container;
    using typename BSkeleton::edge_iterator;
    using typename BSkeleton::square;
    using typename BSkeleton::square_container;
    using typename BSkeleton::square_iterator;
    using typename BSkeleton::vertex;
    using typename BSkeleton::vertex_container;
    using typename BSkeleton::vertex_iterator;
    template <int dim>
    using cell_container = typename BSkeleton::template cell_container<dim>;
    template <int dim>
    using cell_iterator = typename BSkeleton::template cell_iterator<dim>;
    using typename BSkeleton::Level;

    /**
     * @brief Constructor.
     * @param shape Shape associated to the skeleton.
     */
    DualSkeleton(Shape &shape);
    /**
     * @brief Empty Skeleton Constructor.
     */
    DualSkeleton();
    /**
     * @brief Destructor.
     */
    ~DualSkeleton();

    /**
     * @brief Tests if the class has constructed a primal (true)
     * or a dual (false) skeleton.
     * @return False.
     */
    static constexpr bool isPrimal();

    /**
     * @brief Returns the voxel corresponding to the dual cube of
     * the given vertex, independently if the voxel is in the shape or not.
     * @param vertex Given vertex.
     * @param container Container in which the voxel is stored.
     * The container will lose its previous content.
     */
    static void getCorrespondingVoxels(vertex &vertex, std::array<Voxel, 1>& container);
    /**
     * @brief Returns the voxel corresponding to the dual cube of
     * the given vertex, independently if the voxel is in the shape or not.
     * @param vertex Given vertex.
     * @param container Container in which the voxel is stored.
     * The container will lose its previous content.
     */
    static void getCorrespondingVoxels(vertex &vertex, std::vector<Voxel>& container);

    /**
     * @brief Returns the voxels containing the dual square of
     * the given edge, independently if the voxels are in the shape or not.
     * @param edge Given edge.
     * @param container Container in which the voxels are stored.
     * The container will lose its previous content.
     */
    static void getCorrespondingVoxels(edge &edge, std::array<Voxel, 2>& container);
    /**
     * @brief Returns the voxels containing the dual square of
     * the given edge, independently if the voxels are in the shape or not.
     * @param edge Given edge.
     * @param container Container in which the voxels are stored.
     * The container will lose its previous content.
     */
    static void getCorrespondingVoxels(edge &edge, std::vector<Voxel>& container);

    /**
     * @brief Returns the voxels containing the dual edge of
     * the given square, independently if the voxels are in the shape or not.
     * @param square Given square.
     * @param container Container in which the voxels are stored.
     * The container will lose its previous content.
     */
    static void getCorrespondingVoxels(square &square, std::array<Voxel, 4>& container);
    /**
     * @brief Returns the voxels containing the dual edge of
     * the given square, independently if the voxels are in the shape or not.
     * @param square Given square.
     * @param container Container in which the voxels are stored.
     * The container will lose its previous content.
     */
    static void getCorrespondingVoxels(square &square, std::vector<Voxel>& container);

    /**
     * @brief Returns the voxels containing the dual vertex of
     * the given cube, independently if the voxels are in the shape or not.
     * @param cube Given cube.
     * @param container Container in which the voxels are stored.
     * The container will lose its previous content.
     */
    static void getCorrespondingVoxels(cube &cube, std::array<Voxel, 8>& container);
    /**
     * @brief Returns the voxels containing the dual vertex of
     * the given cube, independently if the voxels are in the shape or not.
     * @param cube Given cube.
     * @param container Container in which the voxels are stored.
     * The container will lose its previous content.
     */
    static void getCorrespondingVoxels(cube &cube, std::vector<Voxel>& container);

    template <unsigned int dim, Level level>
    /**
     * @brief Returns the voxels containing the dual of
     * the given cell which are contained at level @p level
     * in the underlying shape.
     * @tparam dim Dimension of the given cell.
     * @tparam level Level in which the voxels should exists.
     * @param cell Given cell.
     * @param container Container in which the voxel is stored.
     * The container is *not* emptied before filling.
     */
    void getCorrespondingVoxelsIn(Cell<dim>& cell,
                                  std::vector<Voxel>& container);

private:
    /**
     * @brief Constructs the dual skeleton from the shape.
     */
    void _constructDualSkeleton();

    /**
     * @brief Inserts the vertex corresponding to the dual of the given voxel.
     * @param v Voxel whose dual vertex has to be inserted.
     * @param timestamp Timestamp corresponding to the voxel.
     */
    void _insertDualVertex(const Voxel &v);

    /**
     * @brief Inserts the edges corresponding to the dual of the given voxel
     * and which exists in the dual of the shape.
     * @param v Voxel whose dual edges have to be inserted.
     * @param timestamp Timestamp corresponding to the voxel.
     */
    void _findSquaresAndInsertDualEdges(const Voxel &v);

    /**
     * @brief Inserts the squares corresponding to the dual of the given voxel
     * and which exists in the dual of the shape.
     * @param v Voxel whose dual squares have to be inserted.
     * @param timestamp Timestamp corresponding to the voxel.
     */
    void _findEdgesAndInsertDualSquares(const Voxel &v);

    /**
     * @brief Inserts the cubes corresponding to the dual of the given voxel
     * and which exists in the dual of the shape.
     * @param v Voxel whose dual cubes have to be inserted.
     * @param timestamp Timestamp corresponding to the voxel.
     */
    void _findVerticesAndInsertDualCubes(const Voxel &v);

    template<unsigned int dim>
    /**
     * @brief Inserts the dual of the given primal cell
     * if it exists in the dual of the shape.
     * @param cell Cell whose dual has to be inserted.
     */
    void _insertDualCellIfExisting(Cell<dim>& cell);
};

#include "DualSkeleton.txx"

}} // namespace shape_simplification::shape
