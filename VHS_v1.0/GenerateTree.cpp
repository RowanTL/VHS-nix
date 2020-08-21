#include <iostream>
#include <cstdlib>
#include <string>
#include <boost/program_options.hpp>
#include <png++/png.hpp>
#include <random>
#include <chrono>
#include <ANN/ANN.h>

namespace po = boost::program_options;

#include "Shape.h"

int main(int argc, char **argv) {	
	// Options that need to be set	
	std::string outputPrefix;
	int size;
	int maxDepth;
	
	try {
		po::options_description desc("Options");
		desc.add_options()
			("help,h", "Help screen")
			("output,o", po::value<std::string>(&outputPrefix)->required(), "Output file prefix")
			("size,s", po::value<int>(&size)->required(), "Size of the output image")
			("depth,d", po::value<int>(&maxDepth)->default_value(5), "Recursive depth of tree")
			;
			
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		
		if (vm.count("help")) {
			std::cout << desc << std::endl;
			return 1;
		}
		
		po::notify(vm);
		
	} 
	catch (const std::exception& ex) {
		std::cout << ex.what() << std::endl;
		return 0;
	}
	
	// construct a trivial random generator engine from a time-based seed:
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator (seed);
	std::normal_distribution<double> distribution (0.0,1.0);
	
	// Generate a series of line segments that form the tree (each has two endpoints and a radius)
	//	Line segments live in a unit cube.
	//	Intial segment length .25, radius .05
	//	Child edges:
	//		horizontal displacement rotates on axis direction and .707 the previous horizontal (from above we have an H-tree)
	//		vertical displacement .75 of previous vertical
	//		radius .707 of previous
	typedef std::array<double,3> RealPoint;
	typedef std::tuple<RealPoint,RealPoint,double> LineSegment;
	std::list<LineSegment> segments { {{.5,.5,0},{.5,.5,.125},.025} };
	std::list<std::tuple<int,int,RealPoint>> queue;	// Depth, direction (0,1), top
	queue.push_back( {1,0,{.5,.5,.125}} );
	
	while (!queue.empty()) {
		auto next = queue.front();
		queue.pop_front();
		int depth = std::get<0>(next);
		int direction = std::get<1>(next);
		RealPoint top = std::get<2>(next); 
		double radius = .025 * pow(.707,depth);
		double horiz = .075 * pow(.707,depth);
		double verticle = .125 * pow(.9,depth);
		
		for (int offset=-1; offset<=1; offset+=2) {
			RealPoint bottom {top};
			bottom[direction] += horiz*offset;
			bottom[2] += verticle;
			
			// Add noise to the bottom
			for (int i=0; i<3; i++)
				bottom[i] += .5 * verticle * distribution(generator);
			
			segments.push_back( {top,bottom,radius} );
			std::cout << "Segment: ";
			for (double x : top) std::cout << x << " ";
			std::cout << "-> ";
			for (double x : bottom) std::cout << x << " ";
			std::cout << "r = " << radius << std::endl;
			if (depth < maxDepth)
				queue.push_front( {depth+1,1-direction,bottom} );
		}
	}
	
	// Setup an ANN struture
	std::map<RealPoint, std::set<LineSegment>> pointLookup;
	for (LineSegment segment : segments) {
		RealPoint ends[2] = { std::get<0>(segment), std::get<1>(segment) };
		for (RealPoint p : ends) {
			if (pointLookup.find(p) == pointLookup.end())
				pointLookup[p] = std::set<LineSegment>({ segment });
			else
				pointLookup[p].insert(segment);
		}
	}
	ANNpointArray dataPts = annAllocPts(pointLookup.size(), 3);
	std::vector<RealPoint> pointById;
	int count = 0;
	for (auto iter : pointLookup) {
		pointById.push_back(iter.first);
		for (int i=0; i<3; i++)
			dataPts[count][i] = iter.first[i];
		count++;
	}
	ANNkd_tree kdTree(dataPts, count, 3);
	ANNpoint queryPoint = annAllocPt(3);
	ANNidxArray nnIdx = new ANNidx[10];
	ANNdistArray dists = new ANNdist[10];	
	
	Point p;
	for (p[2]=0; p[2]<size; p[2]++) {
		std::cout << "Writing image " << (p[2]+1) << " of " << size << "    \r";
		std::cout.flush();
		
		std::string filename = outputPrefix;
		if (p[2] < 10)
			filename += "000";
		else if (p[2] < 100)
			filename += "00";
		else if (p[2] < 1000)
			filename += "0";
		filename += std::to_string(p[2]) + ".png";
		
		png::image<png::gray_pixel> image(size,size);
		
		for (p[1]=0; p[1]<size; p[1]++)
			for (p[0]=0; p[0]<size; p[0]++) {
				
				for (int i=0; i<3; i++)
					queryPoint[i] = (double) p[i]/size;
					
				kdTree.annkSearch(queryPoint, 10, nnIdx, dists, 0.);
										
				// Find intensity:  150 at center of edge, 50 at edge of neighborhood, 0 outside
				int intensity = 0;
				for (int i=0; i<10; i++)
					for (auto segment : pointLookup[pointById[nnIdx[i]]]) {
						RealPoint start = std::get<0>(segment);
						RealPoint end = std::get<1>(segment);
						double r = std::get<2>(segment);
						// Calculate distance from x to the line segment
						double dist{0.}, dot{0.}, l2{0.};
						RealPoint v, w;
						for (int i=0; i<3; i++) {
							v[i] = end[i] - start[i];
							w[i] = queryPoint[i] - start[i];
							dot += v[i]*w[i];
							l2 += v[i]*v[i];
						}
						
						if (dot <= 0.)	{
							for (int i=0; i<3; i++)
								dist += pow(queryPoint[i]-start[i],2);
							dist = sqrt(dist);
						}
						else if (l2 <= dot) {
							for (int i=0; i<3; i++)
								dist += pow(queryPoint[i]-end[i],2);
							dist = sqrt(dist);							
						}
						else {
							double t = dot/l2;
							for (int i=0; i<3; i++)
								dist += pow(queryPoint[i]-(start[i]+t*v[i]),2);
							dist = sqrt(dist);							
						}
						
						intensity = std::max(intensity, (int) (50. + 100.*(r-dist)/r));
					}
				image[p[1]][p[0]] = (png::gray_pixel) intensity;			
			}
		image.write(filename);	
	}
	delete []dataPts;
	delete []nnIdx;
	delete []dists;
			
	annClose();	
	std::cout << std::endl;
}


