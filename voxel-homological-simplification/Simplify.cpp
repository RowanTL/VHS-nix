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
	
	#ifndef _GLIBCXX_DEBUG
	
	// Options that need to be set	
	std::string inputPrefix, outputPrefix;
	int numSlices;
	int persWindow;
	
	try {
		po::options_description desc("Options");
		desc.add_options()
			("help,h", "Help screen")
			("input,i", po::value<std::string>(&inputPrefix)->required(), "Input file prefix")
			("output,o", po::value<std::string>(&outputPrefix)->required(), "Output file prefix")
			("slices,s", po::value<int>(&numSlices)->required(), "Number of slices to use")
			("window,w", po::value<int>(&persWindow)->default_value(32), "Persistence window size range (power of 2)")
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
	s.homologicalSimplification(persWindow);
	s.displayStats();
	s.save(outputPrefix);
	
	#else
	
	Shape s;
	s.loadProcessed("b/", 52);
	s.displayStats();
	s.homologicalSimplification(16);
	s.displayStats();
	s.save("c/");

	#endif

	#ifdef WITHGPERFTOOLS
	ProfilerStop();
	#endif
		
}


