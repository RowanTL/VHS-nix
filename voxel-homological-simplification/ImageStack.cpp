#include "ImageStack.h"

#include <iostream>

ImageStack::ImageStack() : _dim{0,0,0} {
}

void ImageStack::load(std::string baseFilename, int num) {
	std::cout << "Loading image stack " << num << std::endl;
	for (int i=0; i<num; i++) {
		if ((i+1) % 10 == 0) {
			std::cout << "Reading " << (i+1) << " of " << num << "           \r";
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
		
		png::image<png::gray_pixel> image(filename);
		_stack.push_back(image);					
	}
	std::cout << "Reading " << num << " of " << num << std::endl;
	_dim[0] = _stack[0].get_width();
	_dim[1] = _stack[1].get_height();
	_dim[2] = num;
}

const unsigned char ImageStack::get(int x, int y, int z) const {
	return _stack[z][y][x];
}

const std::array<int,3>& ImageStack::dimensions() const {
	return _dim;
}

void ImageStack::clear() {
	_stack.clear();
}
