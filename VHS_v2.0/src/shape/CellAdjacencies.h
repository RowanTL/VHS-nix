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

#pragma once

/**
 * @file CellAdjacencies.h
 * Contains the declaration of the \ref CellAdjacencies class.
 **/

#include "shape/Cell.h"
#include "util/PackedData.h"
#include "util/Point.h"

namespace shape_simplification{ namespace shape {

using util::PackedData;
using util::Point;

/**
 * class CellAdjacencies
 * @brief Class defining static methods testing the face and coface relation
 * between cells.
 **/
class CellAdjacencies {
public:
    using vertex = Cell<0>;
    using edge = Cell<1>;
    using square = Cell<2>;
    using cube = Cell<3>;

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static bool isFaceOf(const vertex &cell, const vertex &face) {
        return cell == face;
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static bool isFaceOf(const edge &cell, const vertex &face) {
        return cell[0] == face[0] || cell[1] == face[0];
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static bool isFaceOf(const edge &cell, const edge &face) {
        return cell == face;
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static bool isFaceOf(const square &cell, const vertex &face) {
        return cell[0] == face[0] || cell[1] == face[0] ||
                cell[2] == face[0] || cell[3] == face[0];
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static bool isFaceOf(const square &cell, const edge &face) {
        return (cell[0] == face[0] && cell[1] == face[1]) ||
                (cell[0] == face[0] && cell[2] == face[1]) ||
                (cell[1] == face[0] && cell[3] == face[1]) ||
                (cell[2] == face[0] && cell[3] == face[1]);
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static bool isFaceOf(const square &cell, const square &face) {
        return cell == face;
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static bool isFaceOf(const cube &cell, const vertex &face) {
        return cell[0] == face[0] || cell[1] == face[0] ||
                cell[2] == face[0] || cell[3] == face[0] ||
                cell[4] == face[0] || cell[5] == face[0] ||
                cell[6] == face[0] || cell[7] == face[0];
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static bool isFaceOf(const cube &cell, const edge &face) {
        return (cell[0] == face[0] && cell[1] == face[1]) ||
                (cell[0] == face[0] && cell[2] == face[1]) ||
                (cell[0] == face[0] && cell[4] == face[1]) ||
                (cell[1] == face[0] && cell[3] == face[1]) ||
                (cell[1] == face[0] && cell[5] == face[1]) ||
                (cell[2] == face[0] && cell[3] == face[1]) ||
                (cell[2] == face[0] && cell[6] == face[1]) ||
                (cell[3] == face[0] && cell[7] == face[1]) ||
                (cell[4] == face[0] && cell[5] == face[1]) ||
                (cell[4] == face[0] && cell[6] == face[1]) ||
                (cell[5] == face[0] && cell[7] == face[1]) ||
                (cell[6] == face[0] && cell[7] == face[1]);
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static bool isFaceOf(const cube &cell, const square &face) {
        return (cell[0] == face[0] && cell[1] == face[1]
                && cell[2] == face[2] && cell[3] == face[3]) ||
                (cell[0] == face[0] && cell[1] == face[1]
                && cell[4] == face[2] && cell[5] == face[3]) ||
                (cell[0] == face[0] && cell[2] == face[1]
                && cell[4] == face[2] && cell[6] == face[3]) ||
                (cell[1] == face[0] && cell[3] == face[1]
                && cell[5] == face[2] && cell[7] == face[3]) ||
                (cell[4] == face[0] && cell[5] == face[1]
                && cell[6] == face[2] && cell[7] == face[3]) ||
                (cell[2] == face[0] && cell[3] == face[1]
                && cell[6] == face[2] && cell[7] == face[3]);
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static bool isFaceOf(const cube &cell, const cube &face) {
        return cell == face;
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static constexpr bool isFaceOf(const vertex &cell, const edge &face) {
        return false;
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static constexpr bool isFaceOf(const vertex &cell, const square &face) {
        return false;
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static constexpr bool isFaceOf(const vertex &cell, const cube &face) {
        return false;
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static constexpr bool isFaceOf(const edge &cell, const square &face) {
        return false;
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static constexpr bool isFaceOf(const edge &cell, const cube &face) {
        return false;
    }

    /**
     * @brief Tests if @p face is a face of @p cell.
     * @return True if @p face is a face of @p cell, false otherwise.
     */
    static constexpr bool isFaceOf(const square &cell, const cube &face) {
        return false;
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static bool isPossibleCofaceOf(const vertex &cell, const vertex &coface) {
        return cell == coface;
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static bool isPossibleCofaceOf(const vertex &cell, const edge &coface) {
        return isFaceOf(coface, cell);
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static bool isPossibleCofaceOf(const vertex &cell, const square &coface) {
        return isFaceOf(coface, cell);
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static bool isPossibleCofaceOf(const vertex &cell, const cube &coface) {
        return isFaceOf(coface, cell);
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static bool isPossibleCofaceOf(const edge &cell, const edge &coface) {
        return cell == coface;
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static bool isPossibleCofaceOf(const edge &cell, const square &coface) {
        return isFaceOf(coface, cell);
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static bool isPossibleCofaceOf(const edge &cell, const cube &coface) {
        return isFaceOf(coface, cell);
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static bool isPossibleCofaceOf(const square &cell, const square &coface) {
        return cell == coface;
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static bool isPossibleCofaceOf(const square &cell, const cube &coface) {
        return isFaceOf(coface, cell);
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static bool isPossibleCofaceOf(const cube &cell, const cube &coface) {
        return cell == coface;
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static constexpr bool isPossibleCofaceOf(const edge &cell,
                                             const vertex &coface) {
        return false;
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static constexpr bool isPossibleCofaceOf(const square &cell,
                                             const vertex &coface) {
        return false;
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static constexpr bool isPossibleCofaceOf(const square &cell,
                                             const edge &coface) {
        return false;
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static constexpr bool isPossibleCofaceOf(const cube &cell,
                                             const vertex &coface) {
        return false;
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static constexpr bool isPossibleCofaceOf(const cube &cell,
                                             const edge &coface) {
        return false;
    }

    /**
     * @brief Tests if @p coface is a coface of @p cell, independently of any
     * skeleton (i.e., there is no garantee that @p coface is included in the
     * current skeleton).
     * @return True if @p coface is a coface of @p cell, false otherwise.
     */
    static constexpr bool isPossibleCofaceOf(const cube &cell,
                                             const square &coface) {
        return false;
    }

private:
    /**
     * @brief Constructor. Not meant to be instanciated.
     */
    CellAdjacencies() {}
};

}} // namespace shape_simplification::shape

