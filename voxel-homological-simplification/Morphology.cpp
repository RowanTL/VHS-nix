#include "Shape.h"

#include <ANN/ANN.h>

Shape Shape::opening(double r) const {
	Shape s;
	
	std::cout << std::endl << "Prior" << std::endl;
	displayStats();
	
	// Setup ANN with points for distance to collar
	ANNpointArray dataPts = annAllocPts(_collar.size(), 3);
	std::vector<Point> points;
	int count = 0;
	for (Point p : _collar) {
		points.push_back(p);
		for (int i=0; i<3; i++)
			dataPts[count][i] = p[i];
		count++;
	}
	ANNkd_tree kdTree(dataPts, count, 3);
	
	// Add all points of the shape of distance >= r to collar
	ANNpoint queryPoint = annAllocPt(3);
	ANNidxArray nnIdx = new ANNidx[1];
	ANNdistArray dists = new ANNdist[1];
	for (Point p : _shape) {
		for (int i=0; i<3; i++)
			queryPoint[i] = p[i];
			
		kdTree.annkSearch(queryPoint, 1, nnIdx, dists, 0.);
		if (dists[0] >= r) {
			s.addToShape(points[nnIdx[0]],r);
		}
	}
	
	
	std::cout << std::endl << "After erosion" << std::endl;
	s.displayStats();
	std::cout << std::endl;
	
	// Create final shape
	Shape finalShape;
	for (Point p : s._neighborhood)
		finalShape.addToShape(p,0.);
	
	return finalShape;
}
