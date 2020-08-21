Voxelized Homological Simplification (VHS) Software

**This is the first version of the software which was presented in _Some Heuristics for the Homological Simplification Problem_ for CCCG18: <http://www.cs.umanitoba.ca/\%7Ecccg2018/papers/session7B-p1.pdf>.**


Simplify:
	Performs homological simplification.  Input images have to be preprocessed first.
	Core and Neighborhood are unchanged.  Shape is the core with voxels added to acheive
	the simplification.

	simplify
	  -h [ --help ]             Help screen
	  -i [ --input ] arg        Input file prefix
	  -o [ --output ] arg       Output file prefix
	  -s [ --slices ] arg       Number of slices to use
	  -w [ --window ] arg (=32) Persistence window size range

Preprocess:
	Take a stack of images and calculate the core and neighborhood and
	output a quantized image stack.

	preprocess [-h] [-d DOWNSAMPLING] [-r RADIUS] [-l LOWER] [-u UPPER]
                  input_file_prefix output_file_prefix

	optional arguments:
	  -h, --help            show this help message and exit
	  -d DOWNSAMPLING, --downsampling DOWNSAMPLING
                        downsampling amount (power of 2)
	  -r RADIUS, --radius RADIUS
                        radius of neighborhood in downsampled shape(float)
	  -l LOWER, --lower LOWER
                        lower threshold for shape
	  -u UPPER, --upper UPPER
                        upper threshold for shape
	
