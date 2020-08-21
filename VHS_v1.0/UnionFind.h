#pragma once

#include <unordered_map>
#include <array>

typedef std::array<int,3> Point;

struct PointHash {
	size_t operator() (const Point &p) const {
		int long long h = 0ULL;
		for (int i=0; i<3; i++)
			h += ((int long long) p[i]) << (16*i);
		return std::hash<int long long>{}(h);
	}
};

class UnionFind {
	private:
		mutable std::unordered_map<Point, Point, PointHash> _parent;
		std::unordered_map<Point, int, PointHash> _rank;
		int _numComponents;
		
	public:
		UnionFind();
		
		void initialize(const Point& p);
		
		void merge(const Point& p1, const Point& p2);
		
		Point find(const Point& p) const;
		
		int size() const;	
};
