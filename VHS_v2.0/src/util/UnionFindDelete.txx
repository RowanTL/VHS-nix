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
 * @file UnionFindDelete.txx
 * Contains the implementation of the methods in the \ref UnionFindDelete class.
 **/

#include "UnionFindDelete.h"

using namespace shape_simplification::util;

template <class Element>
UnionFindDelete<Element>::UnionFindDelete() :
    _numberOfComponents(0), _maxIndex(-1) {}

template <class Element>
UnionFindDelete<Element>::UnionFindDelete(const UnionFindDelete& ufd)
    : _indices(ufd._indices), _nodes(ufd._nodes),
      _numberOfComponents(ufd._numberOfComponents), _maxIndex(ufd._maxIndex)
{
    for (auto iter : _nodes){
        Node& cur = _nodes.at(iter.first);
        if (cur.getParent() != nullptr)
            cur.setParent(&(
                    _elementToNode(cur.getParent()->getElement())));
        cur.setRightSibling(&(
                _elementToNode(cur.getRightSibling()->getElement())));
        cur.setLeftSibling(&(
                _elementToNode(cur.getLeftSibling()->getElement())));
        if (cur.getRightMostChild() != nullptr)
            cur.setRightMostChild(&(
                    _elementToNode(cur.getRightMostChild()->getElement())));
        cur.setPrevDFS(&(
                _elementToNode(cur.getPrevDFS()->getElement())));
        cur.setNextDFS(&(
                _elementToNode(cur.getNextDFS()->getElement())));
        std::vector<Node*> nodes;
        std::unordered_set<Node*>& ch = cur.getNonLeafChildren();
        for (Node* n : ch){
            nodes.push_back(n);
        }
        for (Node* n : nodes){
            ch.erase(n);
            ch.insert(&(_elementToNode(n->getElement())));
        }
    }
}

template <class Element>
void UnionFindDelete<Element>::initialize(const Element &e) {
    if (_indices.find(e) != _indices.end()) return;

    _nodes.emplace(++_maxIndex, Node(e));
    _indices.emplace(e, _maxIndex);
    _numberOfComponents++;

    Node& n = _nodes.at(_maxIndex);

    n.setNextDFS(&n);
    n.setPrevDFS(&n);
    n.setLeftSibling(&n);
    n.setRightSibling(&n);
}

template <class Element>
void UnionFindDelete<Element>::merge(const Element &e1, const Element &e2) {
    auto it2 = _indices.find(e2);

    if (it2 == _indices.end()) return;

    Node& e1Root = _find(_nodes.at(_indices.at(e1)));
    Node& e2Root = _find(_nodes.at(it2->second));

    if (e1Root == e2Root) return;

    if (e2Root.isInStrictSmallerThan(4))
        _mergeSmall(e1Root, e2Root);
    else if (e1Root.isInStrictSmallerThan(4))
        _mergeSmall(e2Root, e1Root);
    else if (e2Root.getRank() < e1Root.getRank())
        _mergeStandard(e1Root, e2Root);
    else
        _mergeStandard(e2Root, e1Root);

    _numberOfComponents--;
}

template <class Element>
Element UnionFindDelete<Element>::remove(const Element &e){
    Node& node = _elementToNode(e);

    if (node.isInStrictSmallerThan(5)) {
        return _removeInSmallTree(node);
    }
    if (node.isInReduced()) {
        return _removeInReducedTree(node);
    }

    Node& leaf = _findLeaf(node);
    _switchNodes(node, leaf);
    _removeLeafWithRebuild(leaf);
    return node.getElement();
}

template <class Element>
inline bool UnionFindDelete<Element>::contains(const Element &e){
    return _indices.find(e) != _indices.end();
}

template <class Element>
Element UnionFindDelete<Element>::find(const Element &e) {
    Node& node = _elementToNode(e);
    return _find(node).getElement();
}

template <class Element>
void UnionFindDelete<Element>::getUnion(
        const Element &e, std::vector<Element> &container)
{
    Node& start = _elementToNode(e);
    _getElementDFSTree(start, container);
}

template <class Element>
inline int UnionFindDelete<Element>::size() const {
    return _numberOfComponents;
}

template <class Element>
double UnionFindDelete<Element>::getUnionSize(const Element &e){
    Node& node = _elementToNode(e);
    Node* current = &node;
    double size = 0;
    do {
        size++;
        current = current->getNextDFS();
    } while (current != &node);
    return size;
}

template <class Element>
inline void UnionFindDelete<Element>::clear(){
    _indices.clear();
    _nodes.clear();
    _numberOfComponents = 0;
    _maxIndex = -1;
}

template <class Element>
inline typename UnionFindDelete<Element>::component_iterator
UnionFindDelete<Element>::begin(const Element& e){
    return ComponentIterator(&(_elementToNode(e)));
}

template <class Element>
inline typename UnionFindDelete<Element>::component_iterator
UnionFindDelete<Element>::end(const Element& e){
    return ComponentIterator(_elementToNode(e).getPrevDFS());
}

template <class Element>
void UnionFindDelete<Element>::_mergeSmall(Node &mainRoot, Node &mergeRoot){
    std::vector<Node*> elements;
    _getNodeDFSTree(mergeRoot, elements);

    for (Node* n : elements) {
        _addNewRootLeaf(mainRoot, *n);
    }

    if (mainRoot.getRank() == 0) mainRoot.setRank(1);
}

template <class Element>
void UnionFindDelete<Element>::_mergeStandard(Node &mainRoot, Node &mergeRoot)
{
    if (mainRoot.getRank() == mergeRoot.getRank())
        mainRoot.setRank(mainRoot.getRank() + 1);

    Node* leftMostChild;
    Node* rightMostChild;
    if (mainRoot.getRightMostChild() != nullptr){
        rightMostChild = mainRoot.getRightMostChild();
        leftMostChild = rightMostChild->getRightSibling();
        leftMostChild->setLeftSibling(&mergeRoot);
        rightMostChild->setRightSibling(&mergeRoot);
        mainRoot.setRightMostChild(&mergeRoot);
    } else {
        mainRoot.setRightMostChild(&mergeRoot);
        leftMostChild = &mergeRoot;
        rightMostChild = &mergeRoot;
    }

    mergeRoot.setParent(&mainRoot);
    mergeRoot.setRightSibling(leftMostChild);
    mergeRoot.setLeftSibling(rightMostChild);
    mergeRoot.getNonLeafChildren().clear();

    if (!mergeRoot.isLeaf()) mainRoot.getNonLeafChildren().insert(&mergeRoot);

    Node* tmp = mainRoot.getNextDFS();
    mainRoot.setNextDFS(&mergeRoot);
    tmp->setPrevDFS(mergeRoot.getPrevDFS());
    mergeRoot.getPrevDFS()->setNextDFS(tmp);
    mergeRoot.setPrevDFS(&mainRoot);
}

template <class Element>
Element UnionFindDelete<Element>::_removeInSmallTree(Node& node){
    std::vector<Node*> elements;
    _getNodeDFSTree(node, elements, &node);
    Element e = node.getElement();
    _nodes.erase(_indices.at(e));
    _indices.erase(e);

    if (elements.size() == 0){
        _numberOfComponents--;
        return e;
    }

    Node *newRoot = elements.at(0);
    newRoot->setRank(1);
    newRoot->setParent(nullptr);
    newRoot->setLeftSibling(newRoot);
    newRoot->setRightSibling(newRoot);
    newRoot->setPrevDFS(elements.back());
    if (elements.size() > 1) {
        newRoot->setNextDFS(elements.at(1));
        newRoot->setRightMostChild(elements.at(1));
    } else {
        newRoot->setNextDFS(newRoot);
        newRoot->setRightMostChild(nullptr);
    }
    newRoot->getNonLeafChildren().clear();

    for (unsigned i = 1; i < elements.size(); ++i) {
        Node *cur = elements.at(i);
        Node *left;
        if (i + 1 == elements.size()) left = elements.at(1);
        else left = elements.at(i + 1);
        Node *right = elements.at(i - 1);
        if (i == 1) right = elements.back();

        cur->setRank(0);
        cur->setParent(newRoot);
        cur->setLeftSibling(left);
        cur->setRightSibling(right);
        cur->setRightMostChild(nullptr);
        cur->getNonLeafChildren().clear();

        cur->setPrevDFS(right);
        cur->setNextDFS(left);
    }

    if (elements.size() > 1) {
        elements.at(1)->setPrevDFS(newRoot);
        elements.back()->setNextDFS(newRoot);
    }

    return newRoot->getElement();
}

template <class Element>
Element UnionFindDelete<Element>::_removeInReducedTree(Node& node){
    if (node.isRoot()) {
        _switchNodes(node, *(node.getRightMostChild()));
        _removeLeaf(*(node.getRightMostChild()));
        return node.getElement();
    }
    _removeLeaf(node);
    return node.getParent()->getElement();
}

template <class Element>
typename UnionFindDelete<Element>::Node& UnionFindDelete<Element>::_findLeaf(
        Node& node)
{
    // Assumes that tree is full.
    if (node.isLeaf()) return node;
    if (node.isRoot()) return *(node.getPrevDFS());
    if (node.getParent()->getRightMostChild()->getRightSibling() == &node)
        return *(node.getPrevDFS());
    return *(node.getLeftSibling()->getPrevDFS());
}

template <class Element>
void UnionFindDelete<Element>::_removeLeaf(Node& leaf){
    leaf.getPrevDFS()->setNextDFS(leaf.getNextDFS());
    leaf.getNextDFS()->setPrevDFS(leaf.getPrevDFS());

    leaf.getLeftSibling()->setRightSibling(leaf.getRightSibling());
    leaf.getRightSibling()->setLeftSibling(leaf.getLeftSibling());
    if (leaf.getParent() != nullptr &&
            leaf.getParent()->getRightMostChild() == &leaf){
        if (leaf.getLeftSibling() != &leaf)
            leaf.getParent()->setRightMostChild(leaf.getLeftSibling());
        else {
            Node *parent = leaf.getParent();
            parent->setRightMostChild(nullptr);
            if (parent->getParent() != nullptr && parent->getParent()->isRoot())
                parent->getParent()->getNonLeafChildren().erase(parent);
        }
    }
    if (leaf.getParent() == nullptr)
        _numberOfComponents--;
    else if (leaf.getParent()->isRoot() && leaf.getParent()->isLeaf())
        leaf.getParent()->setRank(0);

    Element e = leaf.getElement();
    _nodes.erase(_indices.at(e));
    _indices.erase(e);
}

template <class Element>
void UnionFindDelete<Element>::_removeLeafWithRebuild(Node& leaf){
    Node* parent = leaf.getParent();
    _removeLeaf(leaf);
    if (parent != nullptr && !parent->isInReduced()) _rebuildLocally(*parent);
}

template <class Element>
void UnionFindDelete<Element>::_rebuildLocally(Node &node){
    if (!node.isRoot()){
        Node* child1 = node.getRightMostChild()->getRightSibling();
        Node* child2 = child1->getRightSibling();
        _relink(*child1);
        _relink(*child2);
        return;
    }
    Node* child = *(node.getNonLeafChildren().begin());
    Node* gchild1 = child->getRightMostChild()->getRightSibling();
    Node* gchild2 = gchild1->getRightSibling();
    Node* gchild3 = gchild2->getRightSibling();
    _relink(*gchild1);
    _relink(*gchild2);
    _relink(*gchild3);
}

template <class Element>
typename UnionFindDelete<Element>::Node&
UnionFindDelete<Element>::_find(Node& node){
    if (node.isRoot()) return node;

    Node *current = &node;
    Node *parent = node.getParent();
    while (!parent->isRoot()){
        _relink(*current);
        current = parent;
        parent = current->getParent();
    }

    return *parent;
}

template <class Element>
void UnionFindDelete<Element>::_relink(Node& node)
{
    Node *parent = node.getParent();
    if (parent == nullptr) return;
    Node *grandparent = parent->getParent();
    if (grandparent == nullptr) return;

    Node* oldMostLeft = parent->getRightMostChild()->getRightSibling();

    if (node.getRightSibling() != &node){
        node.getLeftSibling()->setRightSibling(node.getRightSibling());
        node.getRightSibling()->setLeftSibling(node.getLeftSibling());
        if (parent->getRightMostChild() == &node)
            parent->setRightMostChild(node.getLeftSibling());
    } else {
        parent->setRightMostChild(nullptr);
        parent->setRank(0);
        if (grandparent->isRoot())
            grandparent->getNonLeafChildren().erase(parent);
    }

    if (&node != oldMostLeft){
        Node *tmp = node.getLeftSibling()->getPrevDFS();
        node.getPrevDFS()->setNextDFS(node.getLeftSibling());
        node.getLeftSibling()->setPrevDFS(node.getPrevDFS());
        node.setPrevDFS(parent->getPrevDFS());
        tmp->setNextDFS(parent);
        parent->getPrevDFS()->setNextDFS(&node);
        parent->setPrevDFS(tmp);

        node.setRightSibling(parent->getRightSibling());
        node.setLeftSibling(parent);
        parent->getRightSibling()->setLeftSibling(&node);
        parent->setRightSibling(&node);
        if (grandparent->getRightMostChild() == parent)
            grandparent->setRightMostChild(&node);
    } else {
        node.setLeftSibling(parent->getLeftSibling());
        node.setRightSibling(parent);
        parent->getLeftSibling()->setRightSibling(&node);
        parent->setLeftSibling(&node);
    }
    if (grandparent->isRoot() && !node.isLeaf())
        grandparent->getNonLeafChildren().insert(&node);
    if (grandparent->isRoot() && grandparent->getNonLeafChildren().empty())
        grandparent->setRank(1);

    node.setParent(grandparent);

    Node* child1 = parent->getRightMostChild();
    if (child1 != nullptr &&
            child1->getRightSibling() != child1 &&
            child1->getRightSibling() == child1->getLeftSibling()){
        Node* child2 = child1->getRightSibling();
        _relink(*child1);
        _relink(*child2);
    }
}

template <class Element>
void UnionFindDelete<Element>::_addNewRootLeaf(Node& root, Node& leaf){
    Node* leftMostChild;
    if (root.getRightMostChild() != nullptr){
        leftMostChild = root.getRightMostChild()->getRightSibling();
        leftMostChild->setLeftSibling(&leaf);
        root.getRightMostChild()->setRightSibling(&leaf);
    } else {
        root.setRightMostChild(&leaf);
        leftMostChild = &leaf;
    }

    leaf.setParent(&root);
    leaf.setRightSibling(leftMostChild);
    leaf.setLeftSibling(root.getRightMostChild());
    leaf.setRightMostChild(nullptr);

    leaf.setPrevDFS(root.getPrevDFS());
    leaf.setNextDFS(&root);
    root.getPrevDFS()->setNextDFS(&leaf);
    root.setPrevDFS(&leaf);

    leaf.setRank(0);
    leaf.getNonLeafChildren().clear();
}

template <class Element>
void UnionFindDelete<Element>::_getElementDFSTree(
        Node& start, std::vector<Element> &container)
{
    Node* current = &start;
    do {
        container.push_back(current->getElement());
        current = current->getNextDFS();
    } while (current != &start);
}

template <class Element>
void UnionFindDelete<Element>::_getNodeDFSTree(
        Node& start,
        std::vector<Node*> &container,
        Node *toIgnore)
{
    Node* current = &start;
    do {
        if (current != toIgnore) container.push_back(current);
        current = current->getNextDFS();
    } while (current != &start);
}

template <class Element>
void UnionFindDelete<Element>::_switchNodes(Node& node1, Node& node2){
    Element tmpE = node1.getElement();

    auto it1 = _indices.find(tmpE);
    auto it2 = _indices.find(node2.getElement());
    double tmpIdx = it1->second;
    it1->second = it2->second;
    it2->second = tmpIdx;

    node1.setElement(node2.getElement());
    node2.setElement(tmpE);
}

template <class Element>
typename UnionFindDelete<Element>::Node&
UnionFindDelete<Element>::_elementToNode(const Element &e){
    return _nodes.at(_indices.at(e));
}

template <class Element>
UnionFindDelete<Element>::Node::Node(const Element& e) :
    _element(e), _rank(0), _parent(nullptr), _rightSibling(this),
    _leftSibling(this), _rightMostChild(nullptr),
    _prevDFS(this), _nextDFS(this)
{}

template <class Element>
inline const Element& UnionFindDelete<Element>::Node::getElement() const{
    return _element;
}

template <class Element>
inline void UnionFindDelete<Element>::Node::setElement(const Element& e){
    _element = e;
}

template <class Element>
inline int UnionFindDelete<Element>::Node::getRank() const{
    return _rank;
}

template <class Element>
inline void UnionFindDelete<Element>::Node::setRank(int newRank){
    _rank = newRank;
}

template <class Element>
inline typename UnionFindDelete<Element>::Node*
UnionFindDelete<Element>::Node::getParent(){
    return _parent;
}

template <class Element>
inline void UnionFindDelete<Element>::Node::setParent(Node* newParent){
    _parent = newParent;
}

template <class Element>
inline typename UnionFindDelete<Element>::Node*
UnionFindDelete<Element>::Node::getRightSibling(){
    return _rightSibling;
}

template <class Element>
inline void UnionFindDelete<Element>::Node::setRightSibling(Node* newSibling){
    _rightSibling = newSibling;
}

template <class Element>
inline typename UnionFindDelete<Element>::Node*
UnionFindDelete<Element>::Node::getLeftSibling(){
    return _leftSibling;
}

template <class Element>
inline void UnionFindDelete<Element>::Node::setLeftSibling(Node* newSibling){
    _leftSibling = newSibling;
}

template <class Element>
inline typename UnionFindDelete<Element>::Node*
UnionFindDelete<Element>::Node::getRightMostChild(){
    return _rightMostChild;
}

template <class Element>
inline void UnionFindDelete<Element>::Node::setRightMostChild(Node* newChild){
    _rightMostChild = newChild;
}

template <class Element>
inline typename UnionFindDelete<Element>::Node*
UnionFindDelete<Element>::Node::getPrevDFS(){
    return _prevDFS;
}

template <class Element>
inline void UnionFindDelete<Element>::Node::setPrevDFS(Node* node){
    _prevDFS = node;
}

template <class Element>
inline typename UnionFindDelete<Element>::Node*
UnionFindDelete<Element>::Node::getNextDFS(){
    return _nextDFS;
}

template <class Element>
inline void UnionFindDelete<Element>::Node::setNextDFS(Node* node){
    _nextDFS = node;
}

template <class Element>
inline std::unordered_set<typename UnionFindDelete<Element>::Node*>&
UnionFindDelete<Element>::Node::getNonLeafChildren(){
    return _nonLeafChildren;
}

template <class Element>
inline bool UnionFindDelete<Element>::Node::isRoot(){
    return _parent == nullptr;
}

template <class Element>
inline bool UnionFindDelete<Element>::Node::isLeaf(){
    return _rightMostChild == nullptr;
}

template <class Element>
inline bool UnionFindDelete<Element>::Node::isInReduced(){
    return (isRoot() && _rank < 2) ||
            (_parent != nullptr
            && _parent->isRoot()
            && _parent->getRank() == 1);
}

template <class Element>
bool UnionFindDelete<Element>::Node::isInStrictSmallerThan(int size){
    Node* current = this;
    for (int i = 1; i < size; ++i){
        current = current->getNextDFS();
        if (current == this) {
            return true;
        }
    }
    return false;
}

template <class Element>
inline bool UnionFindDelete<Element>::Node::operator==(const Node &other) const
{
    return _element == other.getElement();
}

template <class Element>
inline bool UnionFindDelete<Element>::Node::operator!=(const Node &other) const
{
    return !(_element == other.getElement());
}

template <class Element>
UnionFindDelete<Element>::ComponentIterator::ComponentIterator(Node* start)
    : _current(start)
{}

template <class Element>
inline typename UnionFindDelete<Element>::ComponentIterator&
UnionFindDelete<Element>::ComponentIterator::operator++()
{
    _current = _current->getNextDFS();
    return *this;
}

template <class Element>
inline typename UnionFindDelete<Element>::ComponentIterator
UnionFindDelete<Element>::ComponentIterator::operator++(int)
{
    _current = _current->getNextDFS();
    return *this;
}

template <class Element>
inline const Element&
UnionFindDelete<Element>::ComponentIterator::operator*() const
{
    return _current->getElement();
}

template <class Element>
inline bool
UnionFindDelete<Element>::ComponentIterator::operator==(
        const ComponentIterator &other) const
{
    return *_current == *(other._current);
}

template <class Element>
inline bool
UnionFindDelete<Element>::ComponentIterator::operator!=(
        const ComponentIterator &other) const
{
    return *_current != *(other._current);
}



