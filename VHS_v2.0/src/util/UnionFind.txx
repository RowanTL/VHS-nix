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
 * @file UnionFind.txx
 * Contains the implementation of the methods in the \ref UnionFind class.
 **/

#include "UnionFind.h"

using namespace shape_simplification::util;

template <class Element>
UnionFind<Element>::UnionFind() : _numberOfComponents(0), _maxIndex(0) {}

template <class Element>
void UnionFind<Element>::initialize(const Element &e) {
    if (_indices.find(e) == _indices.end()) {
        _indices.emplace(e, _maxIndex);
        _nodes.push_back(Node(e, _maxIndex++, 1));
        _numberOfComponents++;
    }
}

template <class Element>
bool UnionFind<Element>::merge(const Element &e1, const Element &e2) {
    auto it2 = _indices.find(e2);

    if (it2 == _indices.end()) return false;

    Node& e1Parent = _nodes.at(_find(_indices.at(e1)));
    Node& e2Parent = _nodes.at(_find(it2->second));

    if (e1Parent._parent == e2Parent._parent) {
        return false;
    }

    _numberOfComponents--;

    if (e1Parent._rank < e2Parent._rank){
        e1Parent._parent = e2Parent._parent;
    } else if (e1Parent._rank > e2Parent._rank){
        e2Parent._parent = e1Parent._parent;
    } else {
        e2Parent._parent = e1Parent._parent;
        (e1Parent._rank)++;
    }

    return true;
}

template <class Element>
inline bool UnionFind<Element>::contains(const Element &e) const
{
    return _indices.find(e) != _indices.end();
}

template <class Element>
void UnionFind<Element>::getRoots(
        std::unordered_set<Element>& roots, int minSize)
{
    if (minSize == 1){
        getRoots(roots);
        return;
    }

    roots.clear();
    std::unordered_map<index, int> counts;

    for (Node& n : _nodes){
        index i = _find(n._parent);
        auto it = counts.find(i);
        if (it == counts.end()){
            counts.emplace(i, 1);
        } else {
            (it->second)++;
        }
    }

    for (auto it : counts){
        if (it.second >= minSize){
            roots.insert(_nodes.at(it.first)._element);
        }
    }
}

template <class Element>
void UnionFind<Element>::getRoots(std::unordered_set<Element>& roots)
{
    roots.clear();

    for (Node& n : _nodes){
        index i = _find(n._parent);
        roots.insert(_nodes.at(i)._element);
    }
}

template <class Element>
void UnionFind<Element>::getSets(std::vector<std::vector<Element> >& sets)
{
    sets.clear();
    sets.resize(_numberOfComponents);

    std::unordered_map<index, index> indexTranslation;
    index lastIndex = 0;

    for (const Node& n : _nodes){
        index r = _find(n._parent);
        if (indexTranslation.find(r) == indexTranslation.end()){
            indexTranslation.emplace(r, lastIndex);
            sets.at(lastIndex).push_back(n._element);
            lastIndex++;
        } else {
            sets.at(indexTranslation.at(r)).push_back(n._element);
        }
    }
}

template <class Element>
Element UnionFind<Element>::find(const Element &e) {
    Node &n = _nodes.at(_find(_indices.at(e)));
    return n._element;
}

template <class Element>
inline int UnionFind<Element>::size() const {
    return _numberOfComponents;
}

template <class Element>
void UnionFind<Element>::clear(){
    _indices.clear();
    _nodes.clear();
    _numberOfComponents = 0;
    _maxIndex = 0;
}

template <class Element>
typename UnionFind<Element>::index UnionFind<Element>::_find(index id){
    Node &n = _nodes.at(id);
    if (n._parent != id)
        n._parent = _find(n._parent);
    return n._parent;
}

