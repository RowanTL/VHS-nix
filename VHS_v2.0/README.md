# VHS (Voxelized Homological Simplification)

DESCRIPTION
--------------------------------

VHS is a C++ package whose purpose is to denoise voxelized data and output a
topologically accurate simplified shape. Its approach is based on heuristic
methods which compute approximate solutions for the _homological simplification problem_,
which is NP-hard in general [1].

Consider a pair of spaces, _C_ and _N_, where _C_ is included in _N_.
The homological simplification problem asks the following: can the persistent homology
of the inclusion map from _C_ to _N_ be realized as the homology of some intermediate
complex _X_, where _C_ ⊆ _X_ ⊆ _N_?

In this package's context, _C_, also called the _core_, represents the set of voxels which
has to be included in the simplified shape and _N_, denoted as the _neighborhood_, all the
voxels which can potentially be included in the simplified shape. VHS starts with an
intermediate shape _S_ constructed from all voxels with a grayscale value higher than a given
threshold _t_. Then _C_ and _N_ are build the same way from two additional thresholds _tc_
and _tn_ such that _tn_ < _t_ < _tc_. VHS will then remove voxels from _S_ which are not
in the core or add voxels to _S_ which are in the neighborhood, such that the topological
features of the resulting shape corresponds to the features common to _C_ and _N_.
For more details we refer to [2].

VHS is published under the GNU Lesser General Public License.

**Authors:** Hannah Schreiber (hannah.schreiber@slu.edu)<br>
**Contributors:** David Letscher

#### References
[1] D. Attali, U. Bauer, O. Devillers, M. Glisse, and A. Lieutier.
Homological Reconstruction and Simplification in R3. _Computational Geometry_,
48(8):606 – 621, 2015.<br>
[2] E. W. Chambers, T. Ju, D. Letscher, H. Schreiber, D. Zeng.
VHS: a package for homological simplification of voxelized data in R3.
**link-to-come**, 2020.

INSTALLATION
--------------------------------

To compile VHS easily, we recommend to use CMake (<https://cmake.org/>).

VHS uses four external libraries:
- **libpng** (<http://www.libpng.org/pub/png/libpng.html>): standard library for manipulation of
	PNG images.
- **png++** (<https://www.nongnu.org/pngpp/>): C++ wrapper for the libpng library.
	Version 0.2.9 is available in the _external_ directory of the package and does
	not need to be reinstalled. If you want to use another version, please rectify the path in the
	CMakeLists file (line 139).
- **Googletest** (<https://github.com/google/googletest>): used here for the unitary tests.
- **PHAT** (<https://bitbucket.org/phat-code/phat>): tool box for computing persistent homology.
	Version 1.6 is available in the _external_ directory of the package and does
	not need to be reinstalled. If you want to use another version, please rectify the path in the
	CMakeLists file (line 138).

To compile, run in a terminal:
> $	cd /path-to-software/<br>
> $	mkdir build<br>
> $	cd build<br>
> $	cmake ..<br>
> $	make

To test if everything works fine:
> $ ./test

And then to use the software:
> $	./simplifier [args]

VHS comes with two additional executables:
- _imgproc_: Downsamples a given stack of grayscale png images and/or shifts the pixel
	values by a given value. Useful if your data is too big to be efficiently processed.
- _optsol_: Computes the optimal possible Betti numbers for the given input.
	Enables to judge the accuracy of the simplifier output. It is also necessary for some
	of the options of the simplifier.

USAGE
--------------------------------

All input images are assumed to have the PNG format and grayscale pixel values.
The voxelized shape is build from a stack of numbered images: the dimensions of the
images corresponds to the _x_-axis and _y_-axis and the numbers of the images to the _z_-axis.
So a pixel with coordinates (_i_,_j_) of image number _k_ will be projected to the voxel
of coordinates (_i_,_j_,_k_).

The name of an image has to have to following format:

	*xxxx.png

where * is an arbitrary name all images in the stack have to have in common and
xxxx the number of the image written with four digits.<br>
In the remaining of this section, we refer to * as _name\_base_ and
to the relative or absolute path of the directory containing the stack as _dir\_path_.

#### imgproc

	./imgproc down noise nberim in-dir_path/in-name_base out-dir_path/[out-name_base] [trans]
- down: value by which the image dimensions should be divided.
- noise: maximal noise value (unsigned integer value) added to the grayscale value of a pixel.
- nberim: number of images in the stack.
- in: input dir\_path and name\_base linked by the default directory separator for paths
	of your system. Note that name\_base can be omitted if the images are only named by they
	numbering, but **not** the separator, otherwise the last directory name will be use as
	base\_name.
- out: output dir\_path and name\_base linked by the default directory separator for paths
	of your system. If no name\_base is given, then all output images will simply be named xxxx.png.
	But do not forget the separator, otherwise the directory name will be use as base\_name.
- trans: optional, value by which all pixel values higher than 10 will be shifted.

The argument `--silent` can be added at an arbitrary position to avoid output in the terminal.

#### optsol

	./optsol nberim in-dir_path/in-name_base [tc ts tn]
- nberim: number of images in the stack.
- in: input dir\_path and name\_base linked by the default directory separator for paths
	of your system. Note that name\_base can be omitted if the images are only named by they
	numbering, but **not** the separator, otherwise the last directory name will be use as
	base\_name.
- tc: grayscale threshold value for the core. Default value: 200.
- ts: grayscale threshold value for the shape. Default value: 150.
- tn: grayscale threshold value for the neighborhood. Default value: 100.

Note that if one threshold value is given, all three threshold values have to be given.

#### simplifier

	./simplifier [args]
where _args_ can be one or the combination of the following:

Type of simplification (obligatory, choose only one):
- `-s nberim in-dir_path/in-name_base out-dir_path/[out-name_base]`:
	standard simplifier.
- `-f nberim in-dir_path/in-name_base out-dir_path/[out-name_base]`:
	standard simplifier, but removals are applied before additions
	(it is the other way around in the standard version).
- `-a nberim in-dir_path/in-name_base out-dir_path/[out-name_base]`:
	only additions are used.
- `-r nberim in-dir_path/in-name_base out-dir_path/[out-name_base]`:
	only removals are used.
	- nberim: number of images in the stack.
	- in: input dir\_path and name\_base linked by the default directory separator for paths
		of your system. Note that name\_base can be omitted if the images are only named by they
		numbering, but **not** the separator, otherwise the last directory name will be use as
		base\_name.
	- out: output dir\_path and name\_base linked by the default directory separator for paths
		of your system. If no name\_base is given, then all output images will simply be named
		xxxx.png.
		But do not forget the separator, otherwise the directory name will be use as base\_name.

Shape related options:
- `-tc value`: grayscale threshold value for the core. Default value: 200.
- `-ts value`: grayscale threshold value for the shape. Default value: 150.
- `-tn value`: grayscale threshold value for the neighborhood. Default value: 100.
- `-opt0 value`: lowest number of connected components possible. Default value: 1.
- `-opt1 value`: lowest number of loops possible. Default value: 0.
- `-opt2 value`: lowest number of voids possible. Default value: 0.

Simplification related options:
- `-w value`: width to be used for the boxes. Default value: 100.<br>
	The persistence computation is done for each box instead of the whole shape at once.
	The boxes can be overlapping (each voxel is covered eight times)
	or not (each voxel is covered only once).
- `--noover`: non-overlapping boxes.
- `-cc value`: maximal size of a component to be completely removed.
	Give a negative value if no limit is wanted. Default value: -1.
	This option is usually not needed.
- `-pc value`: maximum % of noise to remove (Value between 0 and 100). Default value: 100.<br>
	For a more accurate %, also specify the `-opt0/1/2` options whose value can be computed
	with `optsol`.

Additional options:
- `--silent`: avoids output in the terminal.
