#include <cmath>
#include <iostream>
#include <iomanip>
#include <unordered_set>
#include <map>
#include <ctime>
#include <ANN/ANN.h>
#include <chrono>
#include <set>

#include "Shape.h"

#include <phat/compute_persistence_pairs.h>

#include <phat/representations/vector_vector.h>
#include <phat/representations/vector_heap.h>
#include <phat/representations/vector_set.h>
#include <phat/representations/vector_list.h>
#include <phat/representations/sparse_pivot_column.h>
#include <phat/representations/heap_pivot_column.h>
#include <phat/representations/full_pivot_column.h>
#include <phat/representations/bit_tree_pivot_column.h>

#include <phat/algorithms/twist_reduction.h>
#include <phat/algorithms/standard_reduction.h>
#include <phat/algorithms/row_reduction.h>
#include <phat/algorithms/chunk_reduction.h>
#include <phat/algorithms/spectral_sequence_reduction.h>

#include "Persistence.h"
#include "Shape.h"

#ifdef WITHGPERFTOOLS
#include <gperftools/profiler.h>
#endif

std::array<int, 9> Shape::stats() const {
	int comp = numComponents();
	int e = euler();
	int voids = numVoids();
	int loops = numLoops();
	int shape = _shape.size();
	int core = _core.size();
	int neighborhood = _neighborhood.size();
	int collar = _collar.size();
	int complexity = comp + voids + loops;
	
	return {complexity,e,comp,loops,voids,core,shape,neighborhood,collar};
}

void Shape::displayStats() const {
	auto s = stats();
	std::cout << "Complexity = " << s[0] << std::endl;
	std::cout << "Euler = " << s[1] << std::endl;
	std::cout << "Components = " << s[2] << std::endl;
	std::cout << "Loops = " << s[3] << std::endl;
	std::cout << "Voids = " << s[4] << std::endl;
	std::cout << "Core size = " << s[5] << std::endl;
	std::cout << "Shape size = " << s[6] << std::endl;
	std::cout << "Neigh size = " << s[7] << std::endl;
	std::cout << "Collar size = " << s[8] << std::endl;
}

void Shape::homologicalSimplification(int width) {
	#ifdef WITHGPERFTOOLS
		ProfilerStart("gprof.txt");
	#endif
	
	std::cout << "Window size = " << width << std::endl;

	// Setup three clocks:  total, candidate generation, testing/applying candidates
	auto startTime {std::chrono::high_resolution_clock::now()};
	auto timeOnGenerating {std::chrono::duration<double>::zero()};
	auto timeOnProcessing {std::chrono::duration<double>::zero()};
	auto totalTime {std::chrono::duration<double>::zero()};
	auto currentTotalTime {std::chrono::duration<double>::zero()};
	
	auto start {std::chrono::high_resolution_clock::now()};
	std::array<int,3> newComplexity {numComponents(), numLoops(), numVoids()};
	std::vector<std::set<Point>> candidates;	
	int count;
	
	// Fill voids
	start = std::chrono::high_resolution_clock::now();
	candidates = findVoids();
	timeOnGenerating += std::chrono::high_resolution_clock::now() - start;
	
	std::cout << "There are " << candidates.size() << " voids" << std::endl;
	
	count=0;
	start = std::chrono::high_resolution_clock::now();
	for (auto a : candidates) {
			applySimplification(a);

	}
	timeOnProcessing += std::chrono::high_resolution_clock::now() - start;
	std::cout << std::endl;
	std::cout << std::endl;
	displayStats();
	newComplexity = {numComponents(), numLoops(), numVoids()};
	
	currentTotalTime = std::chrono::high_resolution_clock::now() - startTime;
	std::cout << "Total processing time = " << currentTotalTime.count() << std::endl;
	std::cout << "Generation time = " << timeOnGenerating.count() << std::endl;
	std::cout << "Processing time = " << timeOnProcessing.count() << std::endl;
	std::cout << std::endl;

	// Perform one vertex additions that merge components
	std::cout << std::endl;
	bool changed {true};
	while (changed) {
		changed = false;
		
		start = std::chrono::high_resolution_clock::now();
		auto allCandidates = candidateOneVoxelMergers();
		timeOnGenerating += std::chrono::high_resolution_clock::now() - start;
		
		std::cout << "There are " << allCandidates.size() << " candidate one voxel connectors" << std::endl;
		
		start = std::chrono::high_resolution_clock::now();
		changed = applyValidSimplifications(allCandidates, newComplexity);
		timeOnProcessing += std::chrono::high_resolution_clock::now() - start;
		
		std::cout << std::endl;
		std::cout << std::endl;
		displayStats();
		auto currentTotalTime {std::chrono::duration<double>::zero()};
		currentTotalTime = std::chrono::high_resolution_clock::now() - startTime;
		std::cout << "Total processing time = " << currentTotalTime.count() << std::endl;
		std::cout << "Generation time = " << timeOnGenerating.count() << std::endl;
		std::cout << "Processing time = " << timeOnProcessing.count() << std::endl;
		std::cout << std::endl;
	}
	
	// Persistence based simplficiations
	changed = true;
	while (changed) {
		changed = false;
		start = std::chrono::high_resolution_clock::now();
		Shape shapeCopy {*this};
		auto allCandidates = shapeCopy.candidatePeristenceSimplfications(width);
		timeOnGenerating += std::chrono::high_resolution_clock::now() - start;
		
		std::cout << "There are " << allCandidates.size() << " candidate persistence based simplfications" << std::endl;
		start = std::chrono::high_resolution_clock::now();	
		
		changed = applyValidSimplifications(allCandidates, newComplexity);
			
		/*	
		count=0;
		for (auto a : candidates) {
			count++;
			if (validSimplification(a,newComplexity)) {
				applySimplification(a);
				changed = true;
			}
			std::cout << count << " of " << candidates.size() << ": ";
			std::cout << newComplexity[0] << " components, "  << newComplexity[1];
			std::cout << " loops, " << newComplexity[2] << " voids.  ";
			std::cout << (newComplexity[0]+newComplexity[1]+newComplexity[2]) << " total complexity.            \r";
			std::cout.flush();
		}
		*/
		
		timeOnProcessing += std::chrono::high_resolution_clock::now() - start;
		std::cout << std::endl;
		std::cout << std::endl;
		displayStats();
		currentTotalTime = std::chrono::high_resolution_clock::now() - startTime;
		std::cout << "Total processing time = " << currentTotalTime.count() << std::endl;
		std::cout << "Generation time = " << timeOnGenerating.count() << std::endl;
		std::cout << "Processing time = " << timeOnProcessing.count() << std::endl;
		std::cout << std::endl;	
	}
	
	#ifdef WITHGPERFTOOLS
		ProfilerFlush();
		ProfilerStop();
	#endif
}

int Shape::additionalVoids(const std::set<Point>& additions, std::array<int,3>& newComplexity) const {
	int currentVoids = newComplexity[2];
	
	std::set<Point> newCollar {_collar};
	for (Point p : additions) {
		auto iter = newCollar.find(p);
		if (iter != newCollar.end())
			newCollar.erase(iter);
		for (Point p2 : voxel26Neighbors(p))
			if (!inShape(p2) && additions.find(p2) == additions.end())
				newCollar.insert(p2);
	}
	// Find the components of the collar
	UnionFind collarComponents;
	for (Point p : newCollar) {
		collarComponents.initialize(p);
		for (Point p2 : Shape::voxel26Neighbors(p))
			if (newCollar.find(p2) != newCollar.end()) {
				collarComponents.initialize(p2);
				collarComponents.merge(p, p2);
			}
	}	
	// Find a representative for each component (max lexicographically)
	std::map<Point,Point> componentRep;
	for (Point p : newCollar) {
		Point component = collarComponents.find(p);
		if (componentRep.find(component) == componentRep.end())
			componentRep[component] = p;
		else
			componentRep[component] = std::max(p, componentRep[component]);
	}	
	// Add a path to the regions corner from each rep if it in not present
	for (auto iter : componentRep) {
		Point p = iter.second;
		
		bool done=false;
		for (unsigned i=0; (i<3) && !done; i++) {
			while (p[i] < _dimensions[i]) {
				p[i]++;
				bool inCol = newCollar.find(p) != newCollar.end();
				bool inSha = inShape(p) || additions.find(p) != additions.end();
				if (!inCol && !inSha) {
					newCollar.insert(p);
					collarComponents.initialize(p);
					for (Point p2 : Shape::voxel26Neighbors(p))
						if (newCollar.find(p) != newCollar.end())
							collarComponents.merge(p, p2);
				}
				else {
					done = true;
					break;
				}
			}
		}
	}
	int newVoids = collarComponents.size() - 1;
	
	return newVoids - currentVoids;
}

bool Shape::validSimplification(const std::set<Point> additions, std::array<int,3>& newComplexity) const {
	// Fail if it overlaps the shape
	for (Point p : additions)
		if (inShape(p))
			return false;
			
	int currentComponents = newComplexity[0];
	int currentLoops = newComplexity[1];
	int currentVoids = newComplexity[2];
	
	// Calculate updated number of components
	UnionFind updatedComponents;
	std::set<Point> reusedComponents;
	for (Point p : additions) {
		/*
		std::cout << "Addition ";
		for (auto x : p)
			std::cout << x << " ";
		std::cout << std::endl;
		*/
		updatedComponents.initialize(p);
	}
	for (Point p : additions)
		for (Point p2 : voxelNeighbors(p))
			if (inShape(p2)) {
				Point rep = _components.find(p2);
				reusedComponents.insert(rep);
				updatedComponents.initialize(rep);
				updatedComponents.merge(p, rep);
			}
			else if (additions.find(p2) != additions.end()) {
				updatedComponents.initialize(p2);
				updatedComponents.merge(p, p2);
			}
	int newComp = currentComponents - reusedComponents.size() + updatedComponents.size();
	
	if (newComp > currentComponents) {
		std::cout << std::endl << "ERROR!!!!!!  Increase in components" << std::endl << std::endl;
		return false;
	}
	
	// Calculate updated euler characteristic
	std::map<Point,int> additionalVertices;
	std::map<Cell,int> additionalEdges, additionalFaces;
	for (Point p : additions) {
		for (Cell f : voxelFaces(p)) {
			auto iter = additionalFaces.find(f);
			if (iter == additionalFaces.end()) {
				additionalFaces[f] = 1;
			}
			else {
				iter->second += 1;
			}
		}
		for (Cell e : voxelEdges(p)) {
			auto iter = additionalEdges.find(e);
			if (iter == additionalEdges.end()) {
				additionalEdges[e] = 1;
			}
			else {
				iter->second += 1;
			}
		}
		for (Point v : voxelVertices(p)) {
			auto iter = additionalVertices.find(v);
			if (iter == additionalVertices.end()) {
				additionalVertices[v] = 1;
			}
			else {
				iter->second += 1;
			}
		}		
	}
	
	int newEuler = euler() + additions.size();
	for (auto iter : additionalFaces) {
		int total = iter.second;
		auto iter2 = _shapeFaces.find(iter.first);
		if (iter2 != _shapeFaces.end())
			total += iter2->second;
		if (total == 2)
			newEuler--;
	}
	for (auto iter : additionalEdges) {
		int total = iter.second;
		auto iter2 = _shapeEdges.find(iter.first);
		if (iter2 != _shapeEdges.end())
			total += iter2->second;
		if (total == 4)
			newEuler++;
	}
	for (auto iter : additionalVertices) {
		int total = iter.second;
		auto iter2 = _shapeVertices.find(iter.first);
		if (iter2 != _shapeVertices.end())
			total += iter2->second;
		if (total == 8)
			newEuler--;
	}
	

	int newVoids = currentVoids;
		
	int newLoops = newComp + newVoids - newEuler;
	
	if (newLoops > currentLoops)
		return false;
		
	if (newComp + newLoops + newVoids >= currentComponents + currentLoops + currentVoids)
		return false;
	
	// Otherwise keep it
	newComplexity[0] = newComp;
	newComplexity[1] = newLoops;
	newComplexity[2] = newVoids;
	
	return true;
}

void Shape::applySimplification(const std::set<Point> additions) {
	for (Point p : additions)
		moveToShape(p);
	//std::cout << std::endl << "Adding " << additions.size() << " " << _shape.size() << std::endl;
}

std::set<std::tuple<int,std::set<Point>>> Shape::candidateOneVoxelMergers() const {
	std::unordered_map<Point,std::set<Point>,PointHash> adjacentComponents;
	for (Point p : _shape) 
		if (inNeighborhood(p)) {
			Point rep = _components.find(p);
			for (Point p2 : voxelNeighbors(p))
				if (inCollar(p2)) {
					auto iter = adjacentComponents.find(p2);
					if (iter == adjacentComponents.end())
						adjacentComponents[p2] = {rep};
					else
						adjacentComponents[p2].insert(rep);
				}
		}
	
	std::set<std::tuple<int,std::set<Point>>> candidates;
	for (auto iter : adjacentComponents)
		if (iter.second.size() > 1) {
			std::tuple<int,std::set<Point>> candidate(iter.first.size(), {iter.first});
			candidates.insert(candidate);
	}
		
	return candidates;
}

std::vector<std::set<Point>> Shape::candidateOneVoxelSimplfications() const {
	std::vector<std::set<Point>> candidates;
	for (Point p : _collar) 
		if (inNeighborhood(p)) {
			int count {0};
			for (Point p2 : voxelNeighbors(p)) {
				if (inShape(p2))
					count++;
			}
			if (count >= 3)
				candidates.push_back({p});
		}
	
	return candidates;
}

std::set<std::tuple<int,std::set<Point>>> Shape::candidatePeristenceSimplfications(int windowSize) const {
	std::set<std::tuple<int,std::set<Point>>> candidates;
	Point lower;
	for (lower[0]=0; lower[0]<_dimensions[0]; lower[0] += windowSize/2)
		for (lower[1]=0; lower[1]<_dimensions[1]; lower[1] += windowSize/2)
			for (lower[2]=0; lower[2]<_dimensions[2]; lower[2] += windowSize/2) {
				std::cout << "Looking for simplifications is box " << lower[0] << " " << lower[1] << " " << lower[2];
				std::cout << ", " <<candidates.size() << " so far         \r";
				Point upper{lower[0]+windowSize/2, lower[1]+windowSize/2, lower[2]+windowSize/2};
				std::set<std::tuple<int,std::set<Point>>> newCandidates = findCandidateSimplficiationsInBox(lower, upper);
				for (auto s : newCandidates)
					candidates.insert(s);
			}
	std::cout << std::endl;
	return candidates;
}

std::set<std::tuple<int,std::set<Point>>> Shape::findCandidateSimplficiationsInBox(Point lowerCorner, Point upperCorner) const {
	std::set<std::tuple<int,std::set<Point>>> candidates; 
	
	// Setup a local filtration for the region
	int width = upperCorner[0] - lowerCorner[0];
	
	std::vector<std::vector<std::vector<double>>> localFiltration;
	for (int i=0; i<width; i++) {
		std::vector<std::vector<double>> tempM;
		for (int j=0; j<width; j++) {
			std::vector<double> tempV(width);
			tempM.push_back(tempV);
		}
		localFiltration.push_back(tempM);
	}	
	//double localFiltration[width][width][width];
	
	for (int i=0; i<width; i++)
		for (int j=0; j<width; j++)
			for (int k=0; k<width; k++)
				localFiltration[i][j][k] = 1e6;
	
	int localShapeCount {0}, localNeighCount {0};
	for (Point p : _neighborhood) {
		Point p2;
		bool valid=true;
		for (int i=0; i<3; i++) {
			p2[i] = p[i] - lowerCorner[i];
			if (p2[i] < 0 || p2[i] >= width)
				valid = false;
		}
		if (valid) {
			localNeighCount++;
			localFiltration[p2[0]][p2[1]][p2[2]] = 1.;
		}
	}
	for (Point p : _shape) {
		Point p2;
		bool valid=true;
		for (int i=0; i<3; i++) {
			p2[i] = p[i] - lowerCorner[i];
			if (p2[i] < 0 || p2[i] >= width)
				valid = false;
		}
		if (valid) {
			localShapeCount++;
			localFiltration[p2[0]][p2[1]][p2[2]] = 0.;
		}
	}
	
	//std::cout << std::endl;
	//std::cout << "Counts = " << localShapeCount << " " << localNeighCount << std::endl;
	
	// Setting up the persistence matrix
	typedef std::tuple<double, int, double, std::set<Point>> CubicalCell;
	std::set<CubicalCell> cells;
	for (int i=0; i<width; i++)
		for (int j=0; j<width; j++)
			for (int k=0; k<width; k++)
				if (localFiltration[i][j][k] < 1e6) {
					// Vertex for cell
					cells.insert(CubicalCell(localFiltration[i][j][k], 0, rand(), { Point({i,j,k}) }));
					
					// Edges in positive directions
					if (i < width-1 && localFiltration[i+1][j][k] < 1e6) {
						double t = std::max(localFiltration[i][j][k], localFiltration[i+1][j][k]);
						cells.insert(CubicalCell(t, 1, rand(), { Point({i,j,k}), Point({i+1,j,k}) }));
					}
					if (j < width-1 && localFiltration[i][j+1][k] < 1e6) {
						double t = std::max(localFiltration[i][j][k], localFiltration[i][j+1][k]);
						cells.insert(CubicalCell(t, 1, rand(), { Point({i,j,k}), Point({i,j+1,k}) }));
					}
					if (k < width-1 && localFiltration[i][j][k+1] < 1e6) {
						double t = std::max(localFiltration[i][j][k], localFiltration[i][j][k+1]);
						cells.insert(CubicalCell(t, 1, rand(), { Point({i,j,k}), Point({i,j,k+1}) }));
					}
						
					// Faces along positive planes
					if (i<width-1 && j<width-1)
						if (localFiltration[i+1][j][k] < 1e6 && localFiltration[i][j+1][k] < 1e6 && localFiltration[i+1][j+1][k] < 1e6) {
							double t = std::max(localFiltration[i][j][k], localFiltration[i+1][j][k]);
							t = std::max(t, localFiltration[i][j+1][k]);
							t = std::max(t, localFiltration[i+1][j+1][k]);
							cells.insert(CubicalCell(t,2, rand(), { Point({i,j,k}), Point({i+1,j,k}), Point({i,j+1,k}), Point({i+1,j+1,k}) }));
						}
					if (i<width-1 && k<width-1)
						if (localFiltration[i+1][j][k] < 1e6 && localFiltration[i][j][k+1] < 1e6 && localFiltration[i+1][j][k+1] < 1e6) {
							double t = std::max(localFiltration[i][j][k], localFiltration[i+1][j][k]);
							t = std::max(t, localFiltration[i][j][k+1]);
							t = std::max(t, localFiltration[i+1][j][k+1]);
							cells.insert(CubicalCell(t,2, rand(), { Point({i,j,k}), Point({i+1,j,k}), Point({i,j,k+1}), Point({i+1,j,k+1}) }));
						}
					if (j<width-1 && k<width-1)
						if (localFiltration[i][j+1][k] < 1e6 && localFiltration[i][j][k+1] < 1e6 && localFiltration[i][j+1][k+1] < 1e6) {
							double t = std::max(localFiltration[i][j][k], localFiltration[i][j+1][k]);
							t = std::max(t, localFiltration[i][j][k+1]);
							t = std::max(t, localFiltration[i][j+1][k+1]);
							cells.insert(CubicalCell(t,2, rand(), { Point({i,j,k}), Point({i,j+1,k}), Point({i,j,k+1}), Point({i,j+1,k+1}) }));
						}		
						
					// Cube
					if (i<width-1 && j<width-1 && k<width-1)
						if (localFiltration[i+1][j][k] < 1e6 && localFiltration[i][j+1][k] < 1e6 && localFiltration[i+1][j+1][k] < 1e6)
							if (localFiltration[i][j][k+1] < 1e6 && localFiltration[i+1][j][k+1] < 1e6)
								if (localFiltration[i][j+1][k+1] < 1e6 && localFiltration[i+1][j+1][k+1] < 1e6) {
									double t = std::max(localFiltration[i][j][k], localFiltration[i+1][j][k]);
									t = std::max(t, localFiltration[i][j+1][k]);
									t = std::max(t, localFiltration[i+1][j+1][k]);
									t = std::max(t, localFiltration[i][j+1][k+1]);
									t = std::max(t, localFiltration[i+1][j+1][k+1]);
									t = std::max(t, localFiltration[i+1][j][k+1]);
									t = std::max(t, localFiltration[i][j][k+1]);
									cells.insert(CubicalCell(t,3, rand(), { Point({i,j,k}), Point({i+1,j,k}), Point({i,j+1,k}), Point({i+1,j+1,k}), Point({i,j,k+1}), Point({i+1,j,k+1}), Point({i,j+1,k+1}), Point({i+1,j+1,k+1}) }));
								}
				}
					
	// Setup persistence matrix
	phat::boundary_matrix< phat::bit_tree_pivot_column > boundaryMatrix, inverseBoundaryMatrix;
	boundaryMatrix.set_num_cols(cells.size());
	inverseBoundaryMatrix.set_num_cols(cells.size());
	std::vector<phat::index> col;
	
	std::map<std::set<Point>, int> cellToCol;
	std::vector<std::set<Point>> colToVoxels;
	std::vector<double> times;
	int colCount = 0;
	
	for (auto& c : cells) {
		double t = std::get<0>(c);
		int d = std::get<1>(c);
		std::set<Point> points = std::get<3>(c);
		colToVoxels.push_back(points);
	
		boundaryMatrix.set_dim(colCount,d);
		switch (d) {
			case 0:
				break;
			case 1:
				for (auto& p : points) {
					col.push_back(cellToCol[{p}]);
				}
				std::sort(col.begin(), col.end());
				break;
			case 2:
				std::vector<Point> pts;
				for (auto& p : points)
					pts.push_back(p);
				col.push_back(cellToCol[{pts[0],pts[1]}]);
				col.push_back(cellToCol[{pts[0],pts[2]}]);
				col.push_back(cellToCol[{pts[1],pts[3]}]);
				col.push_back(cellToCol[{pts[2],pts[3]}]);
				std::sort(col.begin(), col.end());
				break;		
		}
		boundaryMatrix.set_col(colCount,col);
		col.clear();
		times.push_back(t);
		cellToCol[points] = colCount;
		colCount++;
	}
	
	for (int i=0; i<cells.size(); i++) {
		col.push_back(i);
		inverseBoundaryMatrix.set_col(i, col);
		col.clear();
	}
	
	standard_reduction_with_inverse reduce;
	reduce(boundaryMatrix, inverseBoundaryMatrix);
	
	// Find candidate simplifications
	for (phat::index colId=0; colId<boundaryMatrix.get_num_cols(); colId++) {
		std::vector<phat::index> col;
		boundaryMatrix.get_col(colId, col);
		if (!col.empty()) {							
			double birth = times[col[col.size()-1]];	
			double death = times[colId];
			int d = boundaryMatrix.get_dim(colId);
			
			bool valid = true;
			
			if (d==1) {
				Point c1 = *(colToVoxels[col[0]].begin()); 
				Point c2 = *(colToVoxels[col[1]].begin()); 
				
				for (int i=0; i<3; i++) {
					c1[i] += lowerCorner[i];
					c2[i] += lowerCorner[i];
				}
				
				if (_components.find(c1) == _components.find(c2))
					valid = false;
			}
			
			if (valid)															
				if (birth < .1 && birth < death && death < 1e6 ) {
					//std::cout << "Candidate simplification" << std::endl;
					std::set<Point> newPoints;
					inverseBoundaryMatrix.get_col(colId, col);
					for (auto i : col) {
						for (auto p : colToVoxels[i]) {
							Point p2 = Point({lowerCorner[0]+p[0], lowerCorner[1]+p[1], lowerCorner[2]+p[2]});
							if (!inShape(p2))
								newPoints.insert(p2);
						}
					}
						
					if (newPoints.size()>0) {
						candidates.insert(make_tuple(newPoints.size(),newPoints));
					}
				}
		}
	}
	
	return candidates;
}

Shape Shape::shell() const {
	Shape s {*this};
	Shape::reduce(std::unordered_set<Point,PointHash>(), s._core);
	Shape::reduce(s._core, s._shape);
	Shape::reduce(s._shape, s._neighborhood);
	
	return s;
}

void Shape::reduce(const std::unordered_set<Point,PointHash>& interior, std::unordered_set<Point,PointHash>& shape) {
	std::list<Point> queue;
	
	// Find the boundary
	for (Point p : shape) {
		for (Point p2 : voxel26Neighbors(p)) {
			if (shape.find(p2) == shape.end()) {
				queue.push_back(p);
				break;
			}
		}
	}
	
	// Shell away
	while (!queue.empty()) {
		std::cout << "Queue size = " << queue.size() << ", shape size = " << shape.size() << "                \r";
		Point p = queue.front();
		queue.pop_front();
		
		if (interior.find(p) == interior.end()) {
			bool present[3][3][3];
			bool island = true;
			for (int i=0; i<3; i++)
				for (int j=0; j<3; j++)
					for (int k=0; k<3; k++)
						if (shape.find(Point{p[0]-1+i,p[1]-1+j,p[2]-1+k}) == shape.end())
							present[i][j][k] = false;
						else {
							present[i][j][k] = true;
							island = false;
						}
			
			bool valid = not island;
			
			// If two adjacent faces have neighbors present it must have the one with common edge
			for (int axis1 = 0; axis1 <= 1 && valid; axis1++)
				for (int direction1 = -1; direction1 <= 1 && valid; direction1+=2) {
					Point n1 {p};
					n1[axis1] += direction1;
					
					if (shape.find(n1) != shape.end()) {
						for (int axis2 = axis1=1; axis2 <= 2 && valid; axis2++)
							for (int direction2 = -1; direction2 <=2 && valid; direction2++) {
								Point n2 {p};
								n2[axis2] += direction2;
								
								if (shape.find(n2) != shape.end()) {
									Point n3 {p};
									n3[axis1] += direction1;
									n3[axis2] += direction2;
									
									if (shape.find(n3) == shape.end())
										valid = false;
								}
							}
					}
				}
			
			// If three adjacent around corner must have the fourth
			int direction[3];
			for (direction[0]=-1; direction[0]<=1 && valid; direction[0]+=2)
				for (direction[1]=-1; direction[1]<=1 && valid; direction[1]+=2)
					for (direction[2]=-1; direction[2]<=1 && valid; direction[2]+=2) {
						bool candidate = true;
						for (int i=0; i<3 && candidate; i++) {
							Point p2 {p};
							p2[i] += direction[i];
							if (shape.find(p2) == shape.end())
								candidate = false;
						}
						if (candidate) {
							Point p2 {p};
							for (int i=0; i<3; i++)
								p2[i] += direction[i];
							if (shape.find(p2) == shape.end())
								valid = false;
						}
					}
			
			// Remove it, add neighbors
			if (valid) {
				for (Point p2 : voxelNeighbors(p))
					if (shape.find(p2) != shape.end())
						shape.insert(p);
				shape.erase(p);
			}
		}
	}		
	std::cout << std::endl;
}

std::vector<std::set<Point>> Shape::findVoids() const {
	std::cout << std::endl << "Finding voids" << std::endl;
	UnionFind collarComponents;
	Point maxCollar {0,0,0};
	
	for (Point p : _collar) {
		collarComponents.initialize(p);
		maxCollar = std::max(maxCollar, p);
	}
	
	for (Point p : _collar) {
		for (Point p2 : voxel26Neighbors(p))
			if (inCollar(p2))
				collarComponents.merge(p,p2);
	}
				
	std::list<Point> seedPoints;
	for (Point p : _collar) {
		Point rep = collarComponents.find(p);
		if (p == rep && rep != collarComponents.find(maxCollar))
			seedPoints.push_back(p);
	}
		
	std::vector<std::set<Point>> candidates;	
	int count = 0;
	for (Point seed : seedPoints) {
		count++;
		std::cout << count << " of " << seedPoints.size() << " building region.    \r";
		std::set<Point> v;
		std::list<Point> queue{seed};
		std::set<Point> found;
		bool valid = true;
		while (!queue.empty()) {
			Point p {queue.front()};
			queue.pop_front();
			v.insert(p);
			for (Point p2 : voxel26Neighbors(p)) {
				if (!inShape(p2) && inNeighborhood(p2) && v.find(p2) == v.end() && found.find(p2) == found.end()) {
					queue.push_back(p2);
					found.insert(p2);
				}
				else if (!inNeighborhood(p2))
					valid = false;
			}
			if (!valid) break;
		}		
		if (valid)
			candidates.push_back(v);
	}
	std::cout << std::endl;
	
	return candidates;
}

bool Shape::applyValidSimplifications(std::set<std::tuple<int,std::set<Point>>> candidates, std::array<int,3>& newComplexity) {
	int initialCandidates = candidates.size();
	bool changed = false;
	
	int currentSize = 4096;
	int count=0;
	while (!candidates.empty()) {
		if (currentSize < 1) currentSize = 1;
		// Build a maximal disjoint set
		std::set<std::tuple<int,std::set<Point>>> currentGroup;
		std::set<Point> region, merged;
		std::set<std::tuple<int,std::set<Point>>> inValid;
		
		auto iter=candidates.begin();
		while (iter!=candidates.end() && currentGroup.size() < currentSize) {
			bool valid=true;
			for (Point p : std::get<1>(*iter))
				if (inShape(p)) {
					inValid.insert(*iter);
					valid = false;
					break;
				}
				else if (region.find(p) != region.end()) {
					valid = false;
					break;
				}
				
			if (valid) {
				currentGroup.insert(*iter);
				for (Point p : std::get<1>(*iter)) {
					merged.insert(p);
					for (Point p2 : voxel26Neighbors(p))
						region.insert(p2);
				}
			}
			iter++;
		}
		currentSize = currentGroup.size();
		
		int addedVoids = additionalVoids(merged, newComplexity);
		count++;
		if (addedVoids == 0) {
			// Loop through and check on the number of components and loops
			for (auto candidate : currentGroup) {
				candidates.erase(candidate);
				if (validSimplification(std::get<1>(candidate), newComplexity)) {
					applySimplification(std::get<1>(candidate));
					changed = true;
				}
				std::cout << (initialCandidates-candidates.size()) << " of " << initialCandidates;
				std::cout << " complexity = " << (newComplexity[0]+newComplexity[1]+newComplexity[2]) << " (";
				for (int x: newComplexity) std::cout << " " << x;
				std::cout << " ), current size = " << currentSize;
				std::cout << " passes = " << count << "        \r";
				std::cout.flush();
			}
			currentSize = 2*(currentSize+1);
		}
		else {
			if (currentSize > 1) { 
				currentSize = currentGroup.size()/(addedVoids+1);
				
				std::cout << (initialCandidates-candidates.size()) << " of " << initialCandidates;
				std::cout << " complexity = " << (newComplexity[0]+newComplexity[1]+newComplexity[2]) << " (";
				for (int x: newComplexity) std::cout << " " << x;
				std::cout << " ), current size = " << currentSize;
				std::cout << " passes = " << count << "        \r";
				std::cout.flush();
			}
			else {
				candidates.erase(candidates.begin());
				
				std::cout << (initialCandidates-candidates.size()) << " of " << initialCandidates;
				std::cout << " complexity = " << (newComplexity[0]+newComplexity[1]+newComplexity[2]) << " (";
				for (int x: newComplexity) std::cout << " " << x;
				std::cout << " ), current size = " << currentSize;
				std::cout << " passes = " << count << "        \r";
				std::cout.flush();
				currentSize = 2*(currentSize+1);
			}
		}
		
		for (auto c : inValid)
			candidates.erase(c);
	}
	return changed;
}
