#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void ImageStackPython(py::module& m);
void ShapePython(py::module& m);
void UnionFindPython(py::module& m);

PYBIND11_PLUGIN(Shape) {
	py::module m("Shape", "Python library for shape analysis of stack of grayscale images");
	
	ImageStackPython(m);
	ShapePython(m);
	UnionFindPython(m);
	
	return m.ptr();
}	
