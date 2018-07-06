#include <iostream>
#include <cstdlib>
#include <string>
#include <boost/program_options.hpp>
#include <ANN/ANN.h>

namespace po = boost::program_options;

#include "Shape.h"

#ifdef WITHGPERFTOOLS
#include <gperftools/profiler.h>
#endif

int main(int argc, char **argv) {
	// Options that need to be set	
	std::string inputPrefix, outputPrefix;
	int numSlices;
	double r;
	
	try {
		po::options_description desc("Options");
		desc.add_options()
			("help,h", "Help screen")
			("input,i", po::value<std::string>(&inputPrefix)->required(), "Input file prefix")
			("output,o", po::value<std::string>(&outputPrefix)->required(), "Output file prefix")
			("slices,s", po::value<int>(&numSlices)->required(), "Number of slices to use")
			("radius,r", po::value<double>(&r)->default_value(1), "Radius of erosion and dialation")
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

	Shape s;

	s.loadProcessed(inputPrefix, numSlices);
	Shape s2 = s.opening(r);
	s2.displayStats();
	s2.save(outputPrefix);
}


