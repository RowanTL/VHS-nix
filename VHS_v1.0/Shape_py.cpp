#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Shape.h"

namespace py = pybind11;

void ShapePython(py::module& m) {
	py::class_<Shape>(m, "Shape")
		.def(py::init<>())
		.def("loadScalar", &Shape::loadScalar)
		.def("loadProcessed", &Shape::loadProcessed)
		.def("save", &Shape::save)
		.def("addToCore", &Shape::addToCore)
		.def("displayStats", &Shape::displayStats)
		.def("homologicalSimplfication", &Shape::homologicalSimplification)
		;
}
