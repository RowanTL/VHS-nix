#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "UnionFind.h"

namespace py = pybind11;

void UnionFindPython(py::module& m) {
	py::class_<UnionFind>(m, "UnionFind")
		.def(py::init<>())
		.def("initialize", &UnionFind::initialize)
		.def("merge", &UnionFind::merge)
		.def("find", &UnionFind::find)
		.def("size", &UnionFind::size);
}
