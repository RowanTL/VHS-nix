#pragma once

#include <vector>
#include <array>
#include <string>
#include <png++/png.hpp>

class ImageStack {
	private:
		std::vector<png::image<png::gray_pixel>> _stack;
		std::array<int,3> _dim;
		
	public:
		ImageStack();
	
		void load(std::string baseFilename, int num);
		
		void save(std::string baseFilename, int num);
	
		const unsigned char get(int x, int y, int z) const;
		
		const std::array<int,3>& dimensions() const;
		
		void clear();
};
