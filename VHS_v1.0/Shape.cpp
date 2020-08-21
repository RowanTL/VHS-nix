#include <cmath>
#include <map>
#include <random>
#include <chrono>

#include "Shape.h"
#include "ImageStack.h"

Shape::Shape() : _edgeCount{0}, _faceCount{0}, _cellCount{0} {
	for (int i=0; i<3; i++)
		_dimensions[i] = 10;
}

void Shape::loadScalar(std::string baseFilename, int num, int lower, int upper, int downsampling, double radius, double noise) {
	ImageStack images;
	images.load(baseFilename, num);
	for (int i=0; i<3; i++)
		_dimensions[i] = images.dimensions()[i]/downsampling;
		
	std::cout << "Shape dimensions =";
	for (int i=0; i<3; i++)
		std::cout << " " << _dimensions[i];
	std::cout << std::endl;
	
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator (seed);
	std::normal_distribution<double> distribution (0.0,noise);
	
	Point maxValues{0,0,0};
	
	// Build tree
	for (int z=0; z<_dimensions[2]; z++) {
		for (int y=0; y<_dimensions[1]; y++) {
			for (int x=0; x<_dimensions[0]; x++) {
				int v = 0;
				for (int k=0; k<downsampling; k++)
					for (int j=0; j<downsampling; j++)
						for (int i=0; i<downsampling; i++)
							v += images.get(downsampling*x+i, downsampling*y+j, downsampling*z+k);
				v /= downsampling*downsampling*downsampling;
				v += distribution(generator);
				
				if (v >= upper) {
					addToCore({x,y,z}, radius);
					maxValues[0] = std::max(x,maxValues[0]);
					maxValues[1] = std::max(y,maxValues[1]);
					maxValues[2] = std::max(z,maxValues[2]);
				}
				else if (v >= lower) {
					addToNeighborhood({x,y,z}, radius);
				}
			}
		}
		if ((z+1) % 10 == 0) {
			std::cout << "Processing image " << (z+1) << "                                  \r";
			std::cout.flush();
		}
	}
	std::cout << "Max values = ";
	for (int x : maxValues) std::cout << x << " ";
	std::cout << std::endl;
	std::cout << "Image loaded and preprocessed" << std::endl;	
	images.clear();
}
void Shape::loadProcessed(std::string baseFilename, int num) {
	ImageStack images;
	images.load(baseFilename, num);
	for (int i=0; i<3; i++)
		_dimensions[i] = images.dimensions()[i];
		
	std::cout << "Shape dimensions =";
	for (int i=0; i<3; i++)
		std::cout << " " << _dimensions[i];
	std::cout << std::endl;
	
	// Build tree
	for (int z=0; z<_dimensions[2]; z++) {
		for (int y=0; y<_dimensions[1]; y++) {
			for (int x=0; x<_dimensions[0]; x++) {
				int v = images.get(x, y, z);
				if (v >= 200) {
					addToCore({x,y,z},0);
				}
				else if (v >= 150) {
					addToShape({x,y,z},0);
				}
				else if (v >= 100) {
					addToNeighborhood({x,y,z},0);
				}
			}
		}
		if ((z+1) % 10 == 0) {
			std::cout << "Processing image " << (z+1) << "                                  \r";
			std::cout.flush();
		}
	}
	std::cout << "Image loaded" << std::endl;
	images.clear();
}

void Shape::save(std::string baseFilename) const {
	int num = _dimensions[2];
	for (int i=0; i<num; i++) {
		if ((i+1) % 10 == 0) {
			std::cout << "Write " << (i+1) << " of " << num << "           \r";
			std::cout.flush();
		}
		 
		std::string filename = baseFilename;
		if (i < 10)
			filename += "000";
		else if (i < 100)
			filename += "00";
		else if (i < 1000)
			filename += "0";
		filename += std::to_string(i) + ".png";
		
		png::image<png::gray_pixel> image(_dimensions[0], _dimensions[1]);
		Point p = {0,0,i};
		for (p[1]=0; p[1]<_dimensions[1]; p[1]++)
			for (p[0]=0; p[0]<_dimensions[0]; p[0]++) {
				if (inCore({p[0],p[1],p[2]})) 
					image[p[1]][p[0]] = (png::gray_pixel) 200;
				else if (inShape({p[0],p[1],p[2]}))
					image[p[1]][p[0]] = (png::gray_pixel) 150;
				else if (inNeighborhood({p[0],p[1],p[2]}))
					image[p[1]][p[0]] = (png::gray_pixel) 100;
				else
					image[p[1]][p[0]] = (png::gray_pixel) 0;
			}
		
		image.write(filename);
	}
}

void Shape::addToCore(const Point& p, double r) {
	_core.insert(p);
	addToShape(p, r);
}

bool Shape::inCore(const Point& p) const {
	return _core.find(p) != _core.end();
}


void Shape::addToShape(const Point& p, double r) {
	_shape.insert(p);
	addToNeighborhood(p,r);
	
	_collar.erase(p);
	_components.initialize(p);
	for (Point p2 : voxelNeighbors(p)) {
		if (inShape(p2)) {
			_components.merge(p, p2);
		}
	}
	for (Point p2 : voxel26Neighbors(p)) {
		if (!inShape(p2))
			_collar.insert(p2);
	}
	
	for (Cell f : voxelFaces(p)) {
		auto iter = _shapeFaces.find(f);
		if (iter == _shapeFaces.end()) {
			_shapeFaces[f] = 1;
		}
		else {
			iter->second += 1;
			if (iter->second == 2) {
				_edgeCount += 1;
			}
		}
	}
	for (Cell e : voxelEdges(p)) {
		auto iter = _shapeEdges.find(e);
		if (iter == _shapeEdges.end()) {
			_shapeEdges[e] = 1;
		}
		else {
			iter->second += 1;
			if (iter->second == 4) {
				_faceCount += 1;
			}
		}
	}
	for (Point v : voxelVertices(p)) {
		auto iter = _shapeVertices.find(v);
		if (iter == _shapeVertices.end()) {
			_shapeVertices[v] = 1;
		}
		else {
			iter->second += 1;
			if (iter->second == 8) {
				_cellCount += 1;
			}
		}
	}
}


bool Shape::inShape(const Point& p) const {
	return _shape.find(p) != _shape.end();
}

void Shape::addToNeighborhood(const Point& p, double r) {
	_neighborhood.insert(p);
	
	int ir = r;
	for (int x=p[0]-ir; x<=p[0]+ir; x++)
		for (int y=p[1]-ir; y<=p[1]+ir; y++)
			for (int z=p[2]-ir; z<=p[2]+ir; z++) {
				Point p2{x,y,z};
				if (pow(x-p[0],2) + pow(y-p[1],2) + pow(z-p[2],2) <= r*r && !inShape(p2))
					_neighborhood.insert(p2);
			}
}

bool Shape::inNeighborhood(const Point& p) const {
	return _neighborhood.find(p) != _neighborhood.end();
}

bool Shape::inCollar(const Point& p) const {
	return _collar.find(p) != _collar.end();
}

void Shape::moveToShape(const Point& p) {
	//_collar.erase(p);
	addToShape(p,0.);
}

int Shape::euler() const {
	return (int) _shape.size() - _edgeCount + _faceCount - _cellCount;
}

int Shape::numComponents() const {
	return _components.size();
}


int Shape::numVoids() const {
	UnionFind collarComponents;
	std::map<Point,Point> componentRep;
	
	// Find the components of the collar
	for (Point p : _collar) {
		collarComponents.initialize(p);
		for (Point p2 : Shape::voxel26Neighbors(p))
			if (inCollar(p2)) {
				collarComponents.initialize(p2);
				collarComponents.merge(p, p2);
			}
	}
	
	// Find a representative for each component (max lexicographically)
	for (Point p : _collar) {
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
				if (!inCollar(p) && !inShape(p)) {
					_collar.insert(p);
					collarComponents.initialize(p);
					for (Point p2 : Shape::voxel26Neighbors(p))
						if (inCollar(p2))
							collarComponents.merge(p, p2);
				}
				else {
					done = true;
					break;
				}
			}
		}
	}
	return collarComponents.size() - 1;
}

int Shape::numLoops() const {
	return numComponents() + numVoids() - euler();
}

std::vector<Point> Shape::voxelNeighbors(const Point& p) {
	return { {p[0]-1,p[1],p[2]}, {p[0]+1,p[1],p[2]}, {p[0],p[1]-1,p[2]}, {p[0],p[1]+1,p[2]}, {p[0],p[1],p[2]-1}, {p[0],p[1],p[2]+1} };
}

std::vector<Point> Shape::voxel26Neighbors(const Point& p) {
	std::vector<Point> n;
	Point p2;
	for (p2[0]=p[0]-1; p2[0]<=p[0]+1; p2[0]++)
		for (p2[1]=p[1]-1; p2[1]<=p[1]+1; p2[1]++)
			for (p2[2]=p[2]-1; p2[2]<=p[2]+1; p2[2]++)
				if (p != p2)
					n.push_back(p2);
	return n;
}

std::vector<Cell> Shape::voxelFaces(const Point& p) {
	return { {p[0],p[1],p[2],0}, {p[0]+1,p[1],p[2],0}, {p[0],p[1],p[2],1}, {p[0],p[1]+1,p[2],1}, {p[0],p[1],p[2],2}, {p[0],p[1],p[2]+1,2} };
}

std::vector<Cell> Shape::voxelEdges(const Point& p) {
	return { {p[0],p[1],p[2],0}, {p[0],p[1]+1,p[2],0}, {p[0],p[1],p[2]+1,0}, {p[0],p[1]+1,p[2]+1,0},  
		{p[0],p[1],p[2],1}, {p[0]+1,p[1],p[2],1}, {p[0],p[1],p[2]+1,1},  {p[0]+1,p[1],p[2]+1,1}, 
		{p[0],p[1],p[2],2}, {p[0]+1,p[1],p[2],2}, {p[0],p[1]+1,p[2],2},  {p[0]+1,p[1]+1,p[2],2} };	
}

std::vector<Point> Shape::voxelVertices(const Point& p) {
	return { {p[0],p[1],p[2]}, {p[0],p[1],p[2]+1}, {p[0],p[1]+1,p[2]}, {p[0],p[1]+1,p[2]+1},
		{p[0]+1,p[1],p[2]}, {p[0]+1,p[1],p[2]+1}, {p[0]+1,p[1]+1,p[2]}, {p[0]+1,p[1]+1,p[2]+1} } ;
}

void Shape::setDimensions(Point d) {
	_dimensions = d;
}
