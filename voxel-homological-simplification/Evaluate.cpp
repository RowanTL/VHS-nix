#include <iostream>
#include <cstdlib>
#include <string>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include "Shape.h"

#ifdef WITHGPERFTOOLS
#include <gperftools/profiler.h>
#endif

int main(int argc, char **argv) {
	// Options that need to be set	
	std::string inputPrefix, outputPrefix;
	int numSlices;
	int persWindow;
	
	try {
		po::options_description desc("Options");
		desc.add_options()
			("help,h", "Help screen")
			("input,i", po::value<std::string>(&inputPrefix)->required(), "Input file prefix")
			("slices,s", po::value<int>(&numSlices)->required(), "Number of slices to use")
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
	s.displayStats();
	Shape s2 {s.shell()};
	s2.displayStats();
	
	#ifdef WITHGPERFTOOLS
	ProfilerStop();
	#endif
		
}


