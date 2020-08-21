#pragma once

#include <array>
#include <vector>
#include <unordered_set>
#include <set>
#include <list>
#include <tuple>
#include <map>

#include "UnionFind.h"

typedef std::array<int,4> Cell;	// Last component is an orientation

struct CellHash {
	size_t operator() (const Cell &p) const {
		int long long h = 0ULL;
		for (int i=0; i<4; i++)
			h += ((int long long) p[i]) << (16*i);
		return std::hash<int long long>{}(h);
	}
};

class Shape {
	private:
		// Size of the region
		Point _dimensions;
		
		// Note: everything included in the level below
		std::unordered_set<Point,PointHash> _core, _shape, _neighborhood;
		
		// Components of the core/shape
		UnionFind _components;
		
		// Collar voxels are non-shape/core points voxel to a shape/core voxel
		//mutable std::unordered_set<Point,PointHash> _collar;
		mutable std::set<Point> _collar;
		
		//std::unordered_map<Cell,int,CellHash> _shapeFaces, _shapeEdges;
		//std::unordered_map<Point,int,PointHash> _shapeVertices;
		std::map<Cell,int> _shapeFaces, _shapeEdges;
		std::map<Point,int> _shapeVertices;
		int _faceCount, _edgeCount, _cellCount;

	public:
		/**
		 * Construct an empty shape.
		 */
		Shape();
	
		/**
		 * Load a stack of png image files with scalar values.
		 */
		void loadScalar(std::string baseFilename, int num, int lower, int upper, int downsampling, double radius, double noise=0.);
	
		/**
		 * Load a stack of png images with quantized values 0 - nothing, 100 - neighborhood, 150 - shape, 200 - core
		 */
		void loadProcessed(std::string baseFilename, int num);
		 
		/**
		 * Save to a stack of png images with quantized values 0 - nothing, 100 - neighborhood, 150 - shape, 200 - core
		 */
		void save(std::string baseFilename) const;
		 
		/**
		 * Set the dimension of the shape.
		 */
		void setDimensions(Point d);
	
	public:
		/**
		 * Add voxel to core and a radius of it to the neighborhood.
		 * 
		 * @pre Voxel is not in core, shape or neighborhood.
		 */
		void addToCore(const Point& p, double r);
		
		/**
		 * Check if voxel in core.
		 */
		bool inCore(const Point& p) const;
		
		/**
		 * Add voxel to shape and a radius of it to the neighborhood.
		 * 
		 * @pre Voxel is not in core, shape or neighborhood.
		 */
		void addToShape(const Point& p, double r);
		
		/**
		 * Check if voxel in shape.
		 */
		bool inShape(const Point& p) const;
		
		/**
		 * Add voxel and its neighborhood to neighborhood.
		 * 
		 * @pre Voxel is not in core, shape or neighborhood.
		 */
		void addToNeighborhood(const Point& p, double r);
		
		/**
		 * Check if voxel in core.
		 */
		bool inNeighborhood(const Point& p) const;
		
		/**
		 * Check if voxel is in the collar.
		 */
		bool inCollar(const Point& p) const;
		
		/**
		 * Add a neighborhood voxel to the shape.
		 */
		void moveToShape(const Point& p);
		
		/**
		 * Get the euler characteristic of the shape.
		 */
		int euler() const;
		
		/**
		 * Get the number of components of the shape.
		 */
		int numComponents() const;
		
		/**
		 * Get the number of voids of the shape.
		 */
		int numVoids() const;
		
		/**
		 * Get the number of loop in the shape.
		 */
		int numLoops() const;
		
		/**
		 * Clear the current collar and build one.
		 */
		
		/**
		 * Return the six neighboring voxels.
		 */		
		static std::vector<Point> voxelNeighbors(const Point& p);
		
		/**
		 * Return the 26 voxels intersecting the current one.
		 */		
		static std::vector<Point> voxel26Neighbors(const Point& p);
		
		/**
		 * Return all faces of a voxel
		 */
		static std::vector<Cell> voxelFaces(const Point& p);
		
		/**
		 * Return all edges of a voxel
		 */
		static std::vector<Cell> voxelEdges(const Point& p);
		
		/**
		 * Return all vertices of a voxel
		 */
		static std::vector<Point> voxelVertices(const Point& p);
		
		/**
		 * Return statistics:  complexity, euler, components, loops, voids, core, size, shape, collar
		 */
		std::array<int, 9> stats() const;
		
		/**
		 * Check if the additions would create new voids.
		 */
		int additionalVoids(const std::set<Point>& additions, std::array<int,3>& newComplexity) const;
		
		/**
		 * Test a candidate simplification.  Note:  it assumes voids have already been checked.
		 */
		bool validSimplification(const std::set<Point> additions, std::array<int,3>& newComplexity) const;
	
		/**
		 * Apply the simplification.
		 */
		void applySimplification(const std::set<Point> additions);
		
		/**
		 * Find all voids contained in the neighborhood.
		 */
		std::vector<std::set<Point>> findVoids() const;
		
		/**
		 * Find all single voxels that merge components.
		 */
		std::set<std::tuple<int,std::set<Point>>> candidateOneVoxelMergers() const;
		
		/**
		 * Find all single voxels that might close loops and fill voids.
		 */
		std::vector<std::set<Point>> candidateOneVoxelSimplfications() const;
		
		/**
		 * Candidate persistence based simplifications.
		 */
		std::set<std::tuple<int,std::set<Point>>> candidatePeristenceSimplfications(int windowSize) const;
		
		/**
		 * Find all candidates persistence based simplifications in a region.
		 */
		std::set<std::tuple<int,std::set<Point>>> findCandidateSimplficiationsInBox(Point lowerCorner, Point upperCorner) const;
		
		/**
		 * Find valid simplifications and apply them.
		 */
		bool applyValidSimplifications(std::set<std::tuple<int,std::set<Point>>> candidates, std::array<int,3>& newComplexity);
		
		/**
		 * Shrink the core, shape and neighborhood as much as possible without changing the topology.
		 */
		Shape shell() const;
		
		/**
		 * Shrink the shape without touching the interior.
		 */
		static void reduce(const std::unordered_set<Point,PointHash>& interior, std::unordered_set<Point,PointHash>& shape);
		
		/*
		 * Display the statistics
		 */
		void displayStats() const;
		
		/**
		 * Do all of the simplifications using the given window size.
		 */
		void homologicalSimplification(int width);
		
		/**
		 * Perform a morphological opening on the shape.
		 */
		Shape opening(double r) const;
		
		/**
		 * Perform a morphological closing on the shape.
		 */
		Shape closing(double r) const;
};
