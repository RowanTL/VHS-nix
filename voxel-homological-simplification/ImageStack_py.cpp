#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "ImageStack.h"

namespace py = pybind11;

void ImageStackPython(py::module& m) {
	py::class_<ImageStack>(m, "ImageStack")
		.def(py::init<>())
		.def("load", &ImageStack::load)
		.def("get", &ImageStack::get)
		.def("dimensions", &ImageStack::dimensions)
		.def("clear", &ImageStack::clear);
}
