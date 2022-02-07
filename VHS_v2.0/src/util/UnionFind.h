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
 * @file UnionFind.h
 * Contains the declaration of the \ref UnionFind class.
 **/

#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace shape_simplification{ namespace util {

template <class Element>
/**
 * class UnionFind
 * @brief A basic Union-Find structure.
 * @tparam Element Type of the elements stored in the structure.
 * Has to provide a corresponding std::hash function.
 */
class UnionFind {
public:
    using index = unsigned int;
    /**
     * @brief Constructor.
     */
    UnionFind();

    /**
     * @brief Adds an element to the structure in its own component,
     * when not already present.
     * @param e Element to add.
     */
    void initialize(const Element &e);

    /**
     * @brief Merges two components together
     * if the first given element is contained.
     * @param e1 An element of the first component.
     * @param e2 An element of the second component.
     * @pre e1 is contained in the structure.
     * @return True if the merge happens (both given elements were
     * contained and in different components), false otherwise.
     */
    bool merge(const Element &e1, const Element &e2);

    /**
     * @brief Tests if an element is already initialized.
     * @param e Element to test.
     * @return True if the structure contains the element @p e,
     * false otherwise.
     */
    bool contains(const Element &e) const;

    /**
     * @brief Retrieves the roots of all connected components of
     * given minimum size.
     * @param roots Empty container for the roots.
     * @param minSize Minimum size of the component.
     */
    void getRoots(std::unordered_set<Element>& roots, int minSize);
    /**
     * @brief Retrieves the roots of all connected components.
     * @param roots Empty container for the roots.
     */
    void getRoots(std::unordered_set<Element>& roots);

    /**
     * @brief Retrieves all connected components as vectors.
     * @param sets Empty container.
     */
    void getSets(std::vector<std::vector<Element> >& sets);

    /**
     * @brief Returns the representative of the component
     * the given element is contained in.
     * @pre @p e has to be contained in the structure.
     * @param e Element to find.
     * @return The representative of the component @p e is contained in.
     */
    Element find(const Element &e);

    /**
     * @brief Returns the number of components.
     * @return The number of components.
     */
    int size() const;

    /**
     * @brief Clears the structure.
     */
    void clear();

private:
    /**
     * @brief Node structure.
     */
    struct Node{
        /**
         * @brief Constructor.
         * @param element
         * @param parent
         * @param rank
         */
        Node(const Element& element, index parent, int rank)
            : _element(element), _parent(parent), _rank(rank){}

        Element _element;   ///< Element represented by the node.
        index _parent;      ///< Position of the parent.
        int _rank;          ///< Rank in the parent hierachy.
    };

    std::unordered_map<Element,index> _indices; ///< Maps element to its position.
    std::vector<Node> _nodes;                   ///< Node container.
    int _numberOfComponents;                    ///< Current number of components.
    index _maxIndex;                            ///< Number of nodes.

    /**
     * @brief Returns the representative of the component
     * the element at given position is contained in.
     * @param id Position of the element.
     * @return The representative of the component.
     */
    index _find(index id);
};

#include "UnionFind.txx"

}} // namespace shape_simplification::util
