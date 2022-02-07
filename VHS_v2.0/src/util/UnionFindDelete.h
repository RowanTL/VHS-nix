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
 * @file UnionFindDelete.h
 * Contains the declaration of the \ref UnionFindDelete class.
 **/

#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace shape_simplification{ namespace util {

template <class Element>
/**
 * class UnionFindDelete
 * @brief Union-Find Structure with constant time deletion.
 * The structure is based on \cite union-find-delete.
 * @tparam Element Type of the elements stored in the structure.
 * Has to provide a corresponding std::hash function
 * and should be comparable (== operation).
 */
class UnionFindDelete {
private:
    /**
     * @brief Node class for the union trees.
     */
    class Node{
    public:
        /**
         * @brief Constructor
         * @param e Element.
         */
        Node(const Element& e);

        //Member setters and getters:

        const Element& getElement() const;
        void setElement(const Element& e);

        int getRank() const;
        void setRank(int newRank);

        Node* getParent();
        void setParent(Node* newParent);
        Node* getRightSibling();
        void setRightSibling(Node* newSibling);
        Node* getLeftSibling();
        void setLeftSibling(Node* newSibling);

        Node* getRightMostChild();
        void setRightMostChild(Node* newChild);

        Node* getPrevDFS();
        void setPrevDFS(Node* node);
        Node* getNextDFS();
        void setNextDFS(Node* node);

        std::unordered_set<Node*>& getNonLeafChildren();

        /**
         * @brief Tests if the node is a root.
         * @return True if it is a root, false otherwise.
         */
        bool isRoot();
        /**
         * @brief Tests if the node is a leaf.
         * @return True if it is a leaf, false otherwise.
         */
        bool isLeaf();
        /**
         * @brief Tests if the tree containing this node is reduced.
         * @return True if the tree is reduced, false otherwise.
         */
        bool isInReduced();
        /**
         * @brief Tests if the tree containing this node has size
         * stricter smaller than the given size.
         * @param size Given size.
         * @return True if the tree is strictly smaller than @p size,
         * false otherwise.
         */
        bool isInStrictSmallerThan(int size);

        /**
         * @brief operator ==
         * @param other
         * @return True if @p other represents the same element
         * than this node, false otherwise.
         */
        bool operator==(const Node &other) const;
        /**
         * @brief operator !=
         * @param other
         * @return True if @p other represents a different element
         * than this node, false otherwise.
         */
        bool operator!=(const Node &other) const;

    private:
        Element _element;       ///< Corresponding element.
        int _rank;              ///< Rank.
        Node* _parent;          ///< Parent node. Nullptr if none.
        Node* _rightSibling;    ///< Right sibling node. Cyclic.
        Node* _leftSibling;     ///< Left sibling node. Cyclic.
        Node* _rightMostChild;  ///< First most right child node. Nullptr if none.

        Node* _prevDFS;         ///< Previous node in DFS preorder.
        Node* _nextDFS;         ///< Next node in DFS preorder.

        std::unordered_set<Node*> _nonLeafChildren; ///< Empty if non-root.
    };

    /**
     * @brief Iterator class of a tree in the structure.
     */
    class ComponentIterator {
    public:
        /**
         * @brief Constructor
         * @param start Node in the tree from where to start the iteration.
         */
        ComponentIterator(Node* start);

        ComponentIterator &operator++();
        ComponentIterator operator++(int);

        const Element &operator*() const;;
        bool operator==(const ComponentIterator &other) const;
        bool operator!=(const ComponentIterator &other) const;

    private:
        Node* _current; ///< Current node in the iteration.
    };

public:
    using component_iterator = ComponentIterator;
    /**
     * @brief Constructor.
     */
    UnionFindDelete();
    /**
     * @brief Copy Constructor.
     */
    UnionFindDelete(const UnionFindDelete& ufd);

    /**
     * @brief Adds an element to the structure in its own component,
     * when not already present.
     * @param e Element to add.
     */
    void initialize(const Element &e);

    /**
     * @brief Merges two components together
     * if the first element is contained.
     * @param e1 An element of the first component.
     * @param e2 An element of the second component.
     * @pre e1 is contained in the structure.
     */
    void merge(const Element &e1, const Element &e2);

    /**
     * @brief Removes an element from its component.
     * Does not split the containing component.
     * @param e Element to remove.
     */
    Element remove(const Element &e);

    /**
     * @brief Tests if an element is already initialized.
     * @param e Element to test.
     * @return True if the structure contains the element @p e,
     * false otherwise.
     */
    bool contains(const Element &e);

    /**
     * @brief Returns the representative of the component
     * the given element is contained in.
     * @pre @p e has to be contained in the structure.
     * @param e Element to find.
     * @return The representative of the component
     * @p e is contained in.
     */
    Element find(const Element &e);

    /**
     * @brief Fills given container with the elements
     * of the union containing the given element.
     * @param v Voxel contained in the union.
     * @param container Container for the union.
     * Does not get cleared before filling.
     */
    void getUnion(const Element &e, std::vector<Element> &container);

    /**
     * @brief Returns the number of components.
     * @return The number of components.
     */
    int size() const;

    /**
     * @brief Returns the size of the union containing given element.
     * @param e Element contained in the union.
     * @return Size of the union containing @p e.
     */
    double getUnionSize(const Element &e);

    /**
     * @brief Clears the structure.
     */
    void clear();

    /**
     * @brief Returns an iterator pointing to the given element.
     * The iterator will than iterate over the component containing the
     * element.
     * @param e Starting element.
     * @return Iterator pointing to @p e.
     */
    component_iterator begin(const Element& e);
    /**
     * @brief Returns an iterator pointing to the elememt
     * previous to the given element. The elements have a cyclic connection, so
     * the end iterator can also be used as iterator. Be careful in loops,
     * because this end iterator points therefore to the last element and
     * not to a ``real'' end as usual.
     * @param e Starting element of the iteration to end.
     * @return Iterator pointing to @p prev(e).
     */
    component_iterator end(const Element& e);

private:
    std::unordered_map<Element, double> _indices;   ///< Map from element to index.
    std::unordered_map<double, Node> _nodes;        ///< Map from index to Node.
    int _numberOfComponents;                        ///< Current number of components.
    double _maxIndex;                               ///< Maximum index of an element.

    /**
     * @brief Merges the second root to the first root if the tree
     * of the second root is considered small (Here called when samller than 4).
     * @param mainRoot Root to persist.
     * @param mergeRoot Root getting merged. Tree has to be small.
     */
    void _mergeSmall(Node &mainRoot, Node &mergeRoot);
    /**
     * @brief Merges the second root to the first root.
     * @param mainRoot Root to persist.
     * @param mergeRoot Root getting merged.
     */
    void _mergeStandard(Node &mainRoot, Node &mergeRoot);

    /**
     * @brief Removes node from a small tree (Here called when samller than 5).
     * @param node Node to be removed.
     */
    Element _removeInSmallTree(Node& node);
    /**
     * @brief Removes node from a reduced tree.
     * @param node Node to be removed.
     */
    Element _removeInReducedTree(Node& node);
    /**
     * @brief Returns a leaf in the same tree as given node.
     * @param node Node of the tree.
     * @return Reference to a leaf in the same tree as @p node.
     */
    Node& _findLeaf(Node& node);
    /**
     * @brief Removes a leaf.
     * @param leaf Leaf to be removed.
     */
    void _removeLeaf(Node& leaf);
    /**
     * @brief Removes a leaf and rebuild locally if the resulting tree is
     * not reduced.
     * @param leaf Leaf to be removed.
     */
    void _removeLeafWithRebuild(Node& leaf);
    /**
     * @brief Rebuilds locally around the given node to ensure a balanced tree.
     * @param node Node to rebuild around.
     */
    void _rebuildLocally(Node& node);

    /**
     * @brief Returns the root of the given node.
     * @param node Node of the tree we want the root of.
     * @return Reference to the root of the tree containing @p node.
     */
    Node& _find(Node& node);

    /**
     * @brief Detachs given node from its parent and relinks it to its
     * grandparent.
     * @param node Node to relink.
     */
    void _relink(Node& node);
    /**
     * @brief Attaches given leaf to the given root.
     * Assumes that leaf is already independent from its old position.
     * @param root Root node.
     * @param leaf Leaf node to attach.
     */
    void _addNewRootLeaf(Node& root, Node& leaf);
    /**
     * @brief Fills container with all elements in the given tree.
     * @param start Node of the tree.
     * @param container Container. Is not cleard before filling.
     */
    void _getElementDFSTree(Node& start, std::vector<Element> &container);
    /**
     * @brief Fills container with all nodes in the given tree.
     * @param start Node of the tree.
     * @param container Container. Is not cleard before filling.
     * @param toIgnore Optional node to not add in the container.
     */
    void _getNodeDFSTree(Node& start,
                         std::vector<Node*> &container,
                         Node* toIgnore = nullptr);
    /**
     * @brief Switches the position of the two given nodes in their tree.
     * @param node1 First node.
     * @param node2 Second node.
     */
    void _switchNodes(Node& node1, Node& node2);
    /**
     * @brief Returns corresponding node of the given element.
     * @param e Element.
     * @return Reference to the corresponding node of @p e.
     */
    Node& _elementToNode(const Element &e);
};

#include "UnionFindDelete.txx"

}} // namespace shape_simplification::util
