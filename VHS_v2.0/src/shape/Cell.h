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
 * @file Cell.h
 * Contains the declaration of the \ref Cell class.
 **/

#include <array>
#include <vector>
#include <algorithm>

#include "util/PackedData.h"
#include "util/Point.h"

namespace shape_simplification{ namespace shape {

using util::PackedData;
using util::Point;

template <unsigned int dim>
/**
 * class Cell
 * @brief Class for the cells in the skeleton.
 * @tparam dim Dimension of the cell.
 * @pre @p dim has to be between 0 and 3 included.
 **/
class Cell {
public:
    using vertex = PackedData;
    using vertex_container = std::array<PackedData, (1 << dim)>;
    using boundary_container = std::array<Cell<(dim > 0 ? dim - 1 : 0)>, 2 * dim>;
    using timestamp_type = char;

    /**
     * @brief Empty Constructor.
     */
    Cell();

    /**
     * @brief Constructor.
     * @param vertices Vector of coordinates of the vertices of the cell.
     * @param timestamp Timestamp associated to the cell.
     * @pre @p vertices must have size \f$2^dim\f$.
     */
    Cell(std::vector<Point> &vertices, timestamp_type timestamp);

    /**
     * @brief Constructor.
     * @param vertices Vector of vertices of the cell.
     * @param timestamp Timestamp associated to the cell.
     * @pre @p vertices must have size \f$2^dim\f$.
     */
    Cell(std::vector<vertex> &vertices, timestamp_type timestamp);

    /**
     * @brief Constructor.
     * @param vertices Array of coordinates of the vertices of the cell.
     * @param timestamp Timestamp associated to the cell.
     */
    Cell(std::array<Point, (1 << dim)> &vertices, timestamp_type timestamp);

    /**
     * @brief Constructor.
     * @param vertices Array of vertices of the cell.
     * @param timestamp Timestamp associated to the cell.
     */
    Cell(std::array<vertex, (1 << dim)> &vertices, timestamp_type timestamp);

    /**
     * @brief Returns dimension of the cell.
     * @return Dimension of the cell.
     */
    static constexpr unsigned int getDim();

    /**
     * @brief Returns timestamp of the cell.
     * @return Timestamp of the cell.
     */
    constexpr timestamp_type getTimestamp() const;

    /**
     * @brief Tests if given cell is a face of this cell.
     * @tparam cellDim Dimension of the given cell.
     * @param cell Cell to be tested.
     * @return True if @p cell is a face, false otherwise.
     */
    template <unsigned int cellDim> bool isFace(Cell<cellDim> &cell);

    /**
     * @brief Tests if given cell is a coface of this cell,
     * independent of the fact that the cell is included in the skeleton or not.
     * @tparam cellDim Dimension of the given cell.
     * @param cell Cell to be tested.
     * @return True if @p cell is a coface, false otherwise.
     */
    template <unsigned int cellDim> bool isPossibleCoface(Cell<cellDim> &cell);

    /**
     * @brief Returns the boundary of the cell.
     * @return The boundary of the cell.
     */
    boundary_container getBoundary();

    /**
     * @brief Converts a vertex to a point corresponding to its coordinates.
     * @param v Vertex to be converted.
     * @return A point corresponding to the coordinates of @p v.
     */
    static Point vertexToPoint(vertex v);
    /**
     * @brief Returns the coordinate of the given voxel at given index.
     * @param v Given voxel.
     * @param i Given index.
     * @return Coordinate @p i of @p v.
     * @pre Index has to be between 0 and 2 inclusive.
     */
    static int vertexToCoordinate(vertex v, unsigned int i);

    /**
     * @brief Converts a coordinate point to a vertex with those coordinates.
     * @param p Point to be converted.
     * @return Vertex with the coordinates of @p p.
     */
    static vertex pointToVertex(Point &p);

    /**
     * @brief Tests if the given cell is equal to this cell.
     * @param other Cell to be tested.
     * @return True if both cells are equal, false otherwise.
     */
    bool operator==(const Cell<dim> &other) const;

    /**
     * @brief Returns the vertex at index @p i.
     * The vertices are ordered in lexicographical order on their coordinates.
     * @param i Index of the vertex to be returned.
     * @pre @i has to be between 0 and \f$2^dim - 1\f$ inclusive.
     * @return Vertex at index @p i.
     */
    vertex operator[](unsigned int i) const;

    /**
     * @brief Hash function for the cell.
     */
    struct CellHasher {
        std::size_t operator()(const Cell<dim> &cell) const {
            int size = (1 << dim);
            std::size_t seed = static_cast<std::size_t>(size);

            for (int i = 0; i < size; i++) {
                seed ^= static_cast<std::size_t>(cell[i].rawData()) + 0x9e3779b9 +
                        (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

    /**
     * @brief Lexicographical order for cells.
     */
    struct CellOrder {
        bool operator()(const Cell<dim> &c1, const Cell<dim> &c2) const {
            if (c1.getTimestamp() != c2.getTimestamp())
                return c1.getTimestamp() < c2.getTimestamp();
            int size = (1 << dim);
            for (int i = 0; i < size; ++i) {
                if (c1[i] != c2[i])
                    return c1[i] < c2[i];
            }
            return false;
        }
    };

private:
    timestamp_type _timestamp;  ///< Timestamp associated to the cell.
    vertex_container _vertices; ///< Vertices of the cell. Ordered in
                                ///< lexicographical order on their coordinates.
    const static unsigned int _coordShift = 17U;  ///< Shift used for the Point coordinates.
    const static int _maxCoordSize = 65536;       ///< Maximal value for one coordinate.

    /**
     * @brief Converts a coordinate point to a vertex
     * underlying data corresponding to those coordinates.
     * @param p Point to be converted.
     * @return Vertex data with the coordinates of @p p.
     */
    static vertex::data_type _pointToVertexData(Point &p);
};

struct CellContainer{
    using vertex = Cell<0>;
    using vertex_container = std::vector<vertex>;
    using edge = Cell<1>;
    using edge_container = std::vector<edge>;
    using square = Cell<2>;
    using square_container = std::vector<square>;
    using cube = Cell<3>;
    using cube_container = std::vector<cube>;
    template <int dim>
    using cell_container = typename std::vector<Cell<dim> >;
    template <int dim>
    using cell_iterator = typename std::vector<Cell<dim> >::iterator;
    template <int dim>
    using size_type = typename cell_container<dim>::size_type;

    template <unsigned int dim>
    /**
     * @brief Returns cell at given index.
     * @tparam dim Dimension of the cell.
     * @param index
     * @return Reference to cell at given index.
     */
    const Cell<dim>& at(size_type<dim> index) const;

    template <unsigned int dim>
    /**
     * @brief Returns usual begin iterator.
     * @tparam dim Dimension of the cell.
     * @return Begin iterator.
     */
    cell_iterator<dim> begin();
    template <unsigned int dim>
    /**
     * @brief Returns usual end iterator.
     * @tparam dim Dimension of the cell.
     * @return End iterator.
     */
    cell_iterator<dim> end();

    template <unsigned int dim>
    /**
     * @brief Returns number of cells in given dimension.
     * @tparam dim Dimension of the cells.
     * @return Number of cells.
     */
    size_type<dim> getSize() const;
    template <unsigned int dim>
    /**
     * @brief Resize the container.
     * @tparam dim Dimension of the cells.
     * @param size New size of the container.
     */
    void resize(size_type<dim> size);

    template <unsigned int dim>
    /**
     * @brief Inserts the given cell.
     * @tparam dim Dimension of the cell.
     * @param cell Cell to insert.
     */
    void insert(Cell<dim>& cell);
    template <unsigned int dim>
    /**
     * @brief Inserts the given cell at given index.
     * @tparam dim Dimension of the cell.
     * @param cell Cell to insert.
     * @param index
     * @pre Index has to be strictly smaller than the size of
     * the container.
     */
    void insertAt(Cell<dim>& cell, size_type<dim> index);

    template <unsigned int dim>
    /**
     * @brief Sorts the container by lexicographical order.
     * @tparam dim Dimension of the cell.
     */
    void sort();

    vertex_container _vertices;     ///< Vertex container.
    edge_container _edges;          ///< Edge container.
    square_container _squares;      ///< Square container.
    cube_container _cubes;          ///< Cube container.
};

}} // namespace shape_simplification::shape

namespace std {

/**
 * @brief Provides a std::hash function for cells.
 **/
template<int dim> struct hash<shape_simplification::shape::Cell<dim> > {
    std::size_t operator()(const shape_simplification::shape::Cell<dim> &c) const {
        return shape_simplification::shape::Cell<dim>::CellHasher()(c);
    }
};

} // namespace std

#include "Cell.txx"

