#include "UnionFind.h"

#include <iostream>

UnionFind::UnionFind() : _numComponents(0) {
	//std::cout << "\t\tNew union find " << this << std::endl;
}

void UnionFind::initialize(const Point& p) {
	/*std::cout << "\t\tInitializing " << this << std::endl;
	std::cout << "\t\t\tParents " << &_parent << std::endl;
	for (auto i : _parent) {
		std::cout << "\t\t\t\t";
		for (auto x : i.first) std::cout << x << " ";
		std::cout << " - ";
		for (auto x : i.second) std::cout << x << " ";
		std::cout << std::endl;
	}
	std::cout << "\t\t";
	for (auto x : p) std::cout << x << " ";
	*/
	if (_parent.find(p) == _parent.end()) {
		//std::cout << "new ";
		_parent[p] = p;
		_rank[p] = 1;
		_numComponents++;
	}
	//std::cout << _numComponents << std::endl;
}
		
void UnionFind::merge(const Point& p1, const Point& p2) {
	/*
	std::cout << "\t\tMerge " << this << " ";
	for (auto x : p1) std::cout << x << " ";
	std::cout << "and ";
	for (auto x : p2) std::cout << x << " ";
	*/
	
	Point p1Parent = find(p1);
	Point p2Parent = find(p2);
	
	if (p1Parent == p2Parent) {
		//std::cout << " same " << _numComponents << std::endl;
		return;
	}
		
	//std::cout << "different ";
	
	_numComponents--;
	
	if (_rank[p1Parent] < _rank[p2Parent])
		_parent[p1Parent] = p2Parent;
	else if (_rank[p1Parent] > _rank[p2Parent])
		_parent[p2Parent] = p1Parent;
	else {
		_parent[p2Parent] = p1Parent;
		_rank[p1Parent]++;
	}
	//std::cout << _numComponents << std::endl;
}
		
Point UnionFind::find(const Point& p) const {
	if (_parent[p] != p)
		_parent[p] = find(_parent[p]);
	return _parent[p];
}

int UnionFind::size() const {	
	return _numComponents;
}
