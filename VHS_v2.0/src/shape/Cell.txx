/*
 * Copyright 2020-22 St. Louis University
 * Contributed by: Hannah Schreiber
 *
 * This file is part of VHS.
 *
 * VHS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * VHS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with VHS. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file Cell.txx
 * Contains the implementation of the methods in the \ref Cell class.
 **/

#include "Cell.h"
#include "CellAdjacencies.h"

using namespace shape_simplification::shape;

template <unsigned int dim>
Cell<dim>::Cell() : _timestamp(0) {}

template <>
inline Cell<0>::Cell(std::vector<Point> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    _vertices[0].set(_pointToVertexData(vertices.at(0)));
}

template <>
inline Cell<1>::Cell(std::vector<Point> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    Cell<1>::vertex::data_type d1 = _pointToVertexData(vertices.at(0));
    Cell<1>::vertex::data_type d2 = _pointToVertexData(vertices.at(1));

    if (d1 < d2){
        _vertices[0].set(d1);
        _vertices[1].set(d2);
    } else {
        _vertices[0].set(d2);
        _vertices[1].set(d1);
    }
}

template <>
inline Cell<2>::Cell(std::vector<Point> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    _vertices[0].set(_pointToVertexData(vertices.at(0)));
    _vertices[1].set(_pointToVertexData(vertices.at(1)));
    _vertices[2].set(_pointToVertexData(vertices.at(2)));
    _vertices[3].set(_pointToVertexData(vertices.at(3)));

    std::sort(_vertices.begin(), _vertices.end());
}

template <>
inline Cell<3>::Cell(std::vector<Point> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    _vertices[0].set(_pointToVertexData(vertices.at(0)));
    _vertices[1].set(_pointToVertexData(vertices.at(1)));
    _vertices[2].set(_pointToVertexData(vertices.at(2)));
    _vertices[3].set(_pointToVertexData(vertices.at(3)));
    _vertices[4].set(_pointToVertexData(vertices.at(4)));
    _vertices[5].set(_pointToVertexData(vertices.at(5)));
    _vertices[6].set(_pointToVertexData(vertices.at(6)));
    _vertices[7].set(_pointToVertexData(vertices.at(7)));

    std::sort(_vertices.begin(), _vertices.end());
}

template <>
inline Cell<0>::Cell(std::vector<vertex> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    _vertices[0].set(vertices.at(0).rawData());
}

template <>
inline Cell<1>::Cell(std::vector<vertex> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    Cell<1>::vertex::data_type d1 = vertices.at(0).rawData();
    Cell<1>::vertex::data_type d2 = vertices.at(1).rawData();

    if (d1 < d2){
        _vertices[0].set(d1);
        _vertices[1].set(d2);
    } else {
        _vertices[0].set(d2);
        _vertices[1].set(d1);
    }
}

template <>
inline Cell<2>::Cell(std::vector<vertex> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    _vertices[0].set(vertices.at(0).rawData());
    _vertices[1].set(vertices.at(1).rawData());
    _vertices[2].set(vertices.at(2).rawData());
    _vertices[3].set(vertices.at(3).rawData());

    std::sort(_vertices.begin(), _vertices.end());
}

template <>
inline Cell<3>::Cell(std::vector<vertex> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    _vertices[0].set(vertices.at(0).rawData());
    _vertices[1].set(vertices.at(1).rawData());
    _vertices[2].set(vertices.at(2).rawData());
    _vertices[3].set(vertices.at(3).rawData());
    _vertices[4].set(vertices.at(4).rawData());
    _vertices[5].set(vertices.at(5).rawData());
    _vertices[6].set(vertices.at(6).rawData());
    _vertices[7].set(vertices.at(7).rawData());

    std::sort(_vertices.begin(), _vertices.end());
}

template <>
inline Cell<0>::Cell(std::array<Point, 1> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    _vertices[0].set(_pointToVertexData(vertices.at(0)));
}

template <>
inline Cell<1>::Cell(std::array<Point, 2> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    Cell<1>::vertex::data_type d1 = _pointToVertexData(vertices.at(0));
    Cell<1>::vertex::data_type d2 = _pointToVertexData(vertices.at(1));

    if (d1 < d2){
        _vertices[0].set(d1);
        _vertices[1].set(d2);
    } else {
        _vertices[0].set(d2);
        _vertices[1].set(d1);
    }
}

template <>
inline Cell<2>::Cell(std::array<Point, 4> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    _vertices[0].set(_pointToVertexData(vertices.at(0)));
    _vertices[1].set(_pointToVertexData(vertices.at(1)));
    _vertices[2].set(_pointToVertexData(vertices.at(2)));
    _vertices[3].set(_pointToVertexData(vertices.at(3)));

    std::sort(_vertices.begin(), _vertices.end());
}

template <>
inline Cell<3>::Cell(std::array<Point, 8> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    _vertices[0].set(_pointToVertexData(vertices.at(0)));
    _vertices[1].set(_pointToVertexData(vertices.at(1)));
    _vertices[2].set(_pointToVertexData(vertices.at(2)));
    _vertices[3].set(_pointToVertexData(vertices.at(3)));
    _vertices[4].set(_pointToVertexData(vertices.at(4)));
    _vertices[5].set(_pointToVertexData(vertices.at(5)));
    _vertices[6].set(_pointToVertexData(vertices.at(6)));
    _vertices[7].set(_pointToVertexData(vertices.at(7)));

    std::sort(_vertices.begin(), _vertices.end());
}

template <>
inline Cell<0>::Cell(std::array<vertex, 1> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    _vertices[0].set(vertices.at(0).rawData());
}

template <>
inline Cell<1>::Cell(std::array<vertex, 2> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    Cell<1>::vertex::data_type d1 = vertices.at(0).rawData();
    Cell<1>::vertex::data_type d2 = vertices.at(1).rawData();

    if (d1 < d2){
        _vertices[0].set(d1);
        _vertices[1].set(d2);
    } else {
        _vertices[0].set(d2);
        _vertices[1].set(d1);
    }
}

template <>
inline Cell<2>::Cell(std::array<vertex, 4> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    _vertices[0].set(vertices.at(0).rawData());
    _vertices[1].set(vertices.at(1).rawData());
    _vertices[2].set(vertices.at(2).rawData());
    _vertices[3].set(vertices.at(3).rawData());

    std::sort(_vertices.begin(), _vertices.end());
}

template <>
inline Cell<3>::Cell(std::array<vertex, 8> &vertices, timestamp_type timestamp)
    : _timestamp(timestamp)
{
    _vertices[0].set(vertices.at(0).rawData());
    _vertices[1].set(vertices.at(1).rawData());
    _vertices[2].set(vertices.at(2).rawData());
    _vertices[3].set(vertices.at(3).rawData());
    _vertices[4].set(vertices.at(4).rawData());
    _vertices[5].set(vertices.at(5).rawData());
    _vertices[6].set(vertices.at(6).rawData());
    _vertices[7].set(vertices.at(7).rawData());

    std::sort(_vertices.begin(), _vertices.end());
}

template <unsigned int dim>
inline constexpr unsigned int Cell<dim>::getDim() {
    return dim;
}

template <unsigned int dim>
inline constexpr
typename Cell<dim>::timestamp_type Cell<dim>::getTimestamp() const
{
    return _timestamp;
}

template <unsigned int dim>
template <unsigned int cellDim>
bool Cell<dim>::isFace(Cell<cellDim> &cell) {
    return CellAdjacencies::isFaceOf(*this, cell);
}

template <unsigned int dim>
template <unsigned int cellDim>
bool Cell<dim>::isPossibleCoface(Cell<cellDim> &cell) {
    return CellAdjacencies::isPossibleCofaceOf(*this, cell);
}

template <> inline typename Cell<1>::boundary_container Cell<1>::getBoundary() {
    std::array<vertex, 1> vertices{_vertices[0]};
    Cell<0> face1(vertices, -1);
    vertices[0] = _vertices[1];
    Cell<0> face2(vertices, -1);
    return boundary_container{face1, face2};
}

template <> inline typename Cell<2>::boundary_container Cell<2>::getBoundary() {
    std::array<vertex, 2> vertices{_vertices[0], _vertices[1]};
    Cell<1> face1(vertices, -1);
    vertices[1] = _vertices[2];
    Cell<1> face2(vertices, -1);
    vertices[0] = _vertices[1];
    vertices[1] = _vertices[3];
    Cell<1> face3(vertices, -1);
    vertices[0] = _vertices[2];
    Cell<1> face4(vertices, -1);
    return boundary_container{face1, face2, face3, face4};
}

template <> inline typename Cell<3>::boundary_container Cell<3>::getBoundary() {
    std::array<vertex, 4> vertices{_vertices[0], _vertices[1], _vertices[2],
                _vertices[3]};
    Cell<2> face1(vertices, -1);
    vertices[2] = _vertices[4];
    vertices[3] = _vertices[5];
    Cell<2> face2(vertices, -1);
    vertices[1] = _vertices[2];
    vertices[3] = _vertices[6];
    Cell<2> face3(vertices, -1);
    vertices[0] = _vertices[1];
    vertices[1] = _vertices[3];
    vertices[2] = _vertices[5];
    vertices[3] = _vertices[7];
    Cell<2> face4(vertices, -1);
    vertices[0] = _vertices[2];
    vertices[2] = _vertices[6];
    Cell<2> face5(vertices, -1);
    vertices[0] = _vertices[4];
    vertices[1] = _vertices[5];
    Cell<2> face6(vertices, -1);
    return boundary_container{face1, face2, face3, face4, face5, face6};
}

template <unsigned int dim>
bool Cell<dim>::operator==(const Cell<dim> &other) const {
    for (typename vertex_container::size_type i = 0; i < _vertices.size(); ++i)
    {
        if (_vertices.at(i) != other[i])
            return false;
    }
    return true;
}

template <unsigned int dim>
typename Cell<dim>::vertex Cell<dim>::operator[](unsigned int i) const
{
    return _vertices[i];
}

template <unsigned int dim>
typename Cell<dim>::vertex Cell<dim>::pointToVertex(Point &p) {
    return vertex(_pointToVertexData(p));
}

template <unsigned int dim>
Point Cell<dim>::vertexToPoint(vertex v) {
    Point p;

    for (unsigned int i = 0; i < 3; ++i) {
        p[i] = static_cast<int>(v.rawData() >> (_coordShift * i) &
                                static_cast<uint64_t>(_maxCoordSize - 1));
    }

    return p;
}

template <unsigned int dim>
int Cell<dim>::vertexToCoordinate(vertex v, unsigned int i){
    return static_cast<int>(v.rawData() >> (_coordShift * i) &
                            static_cast<uint64_t>(_maxCoordSize - 1));
}

template <unsigned int dim>
typename Cell<dim>::vertex::data_type Cell<dim>::_pointToVertexData(Point &p) {
    return (static_cast<uint64_t>(p[2]) << _coordShift * 2) +
            (static_cast<uint64_t>(p[1]) << _coordShift) +
            static_cast<uint64_t>(p[0]);
}

template <>
inline const Cell<0>& CellContainer::at(size_type<0> index) const{
    return _vertices.at(index);
}

template <>
inline const Cell<1>& CellContainer::at(size_type<1> index) const{
    return _edges.at(index);
}

template <>
inline const Cell<2>& CellContainer::at(size_type<2> index) const{
    return _squares.at(index);
}

template <>
inline const Cell<3>& CellContainer::at(size_type<3> index) const{
    return _cubes.at(index);
}

template <>
inline CellContainer::cell_iterator<0> CellContainer::begin<0>(){
    return _vertices.begin();
}

template <>
inline CellContainer::cell_iterator<1> CellContainer::begin<1>(){
    return _edges.begin();
}

template <>
inline CellContainer::cell_iterator<2> CellContainer::begin<2>(){
    return _squares.begin();
}

template <>
inline CellContainer::cell_iterator<3> CellContainer::begin<3>(){
    return _cubes.begin();
}

template <>
inline CellContainer::cell_iterator<0> CellContainer::end<0>(){
    return _vertices.end();
}

template <>
inline CellContainer::cell_iterator<1> CellContainer::end<1>(){
    return _edges.end();
}

template <>
inline CellContainer::cell_iterator<2> CellContainer::end<2>(){
    return _squares.end();
}

template <>
inline CellContainer::cell_iterator<3> CellContainer::end<3>(){
    return _cubes.end();
}

template <>
inline CellContainer::const_cell_iterator<0> CellContainer::begin<0>() const
{
    return _vertices.begin();
}

template <>
inline CellContainer::const_cell_iterator<1> CellContainer::begin<1>() const
{
    return _edges.begin();
}

template <>
inline CellContainer::const_cell_iterator<2> CellContainer::begin<2>() const
{
    return _squares.begin();
}

template <>
inline CellContainer::const_cell_iterator<3> CellContainer::begin<3>() const
{
    return _cubes.begin();
}

template <>
inline CellContainer::const_cell_iterator<0> CellContainer::end<0>() const
{
    return _vertices.end();
}

template <>
inline CellContainer::const_cell_iterator<1> CellContainer::end<1>() const
{
    return _edges.end();
}

template <>
inline CellContainer::const_cell_iterator<2> CellContainer::end<2>() const
{
    return _squares.end();
}

template <>
inline CellContainer::const_cell_iterator<3> CellContainer::end<3>() const
{
    return _cubes.end();
}

template <>
inline CellContainer::size_type<0> CellContainer::getSize<0>() const{
    return _vertices.size();
}

template <>
inline CellContainer::size_type<1> CellContainer::getSize<1>() const{
    return _edges.size();
}

template <>
inline CellContainer::size_type<2> CellContainer::getSize<2>() const{
    return _squares.size();
}

template <>
inline CellContainer::size_type<3> CellContainer::getSize<3>() const{
    return _cubes.size();
}

template <>
inline void CellContainer::resize<0>(size_type<0> size){
    _vertices.resize(size);
}

template <>
inline void CellContainer::resize<1>(size_type<1> size){
    _edges.resize(size);
}

template <>
inline void CellContainer::resize<2>(size_type<2> size){
    _squares.resize(size);
}

template <>
inline void CellContainer::resize<3>(size_type<3> size){
    _cubes.resize(size);
}

template <>
inline void CellContainer::insert(vertex& v){
    _vertices.push_back(v);
}

template <>
inline void CellContainer::insert(edge& e){
    _edges.push_back(e);
}

template <>
inline void CellContainer::insert(square& s){
    _squares.push_back(s);
}

template <>
inline void CellContainer::insert(cube& c){
    _cubes.push_back(c);
}

template <>
inline void CellContainer::insertAt(Cell<0>& cell, size_type<0> index){
    _vertices.at(index) = cell;
}

template <>
inline void CellContainer::insertAt(Cell<1>& cell, size_type<1> index){
    _edges.at(index) = cell;
}

template <>
inline void CellContainer::insertAt(Cell<2>& cell, size_type<2> index){
    _squares.at(index) = cell;
}

template <>
inline void CellContainer::insertAt(Cell<3>& cell, size_type<3> index){
    _cubes.at(index) = cell;
}

template <>
inline void CellContainer::sortLexicographicaly<0>(){
    std::sort(_vertices.begin(),
              _vertices.end(),
              Cell<0>::LexicographicalOrder());
}

template <>
inline void CellContainer::sortLexicographicaly<1>(){
    std::sort(_edges.begin(),
              _edges.end(),
              Cell<1>::LexicographicalOrder());
}

template <>
inline void CellContainer::sortLexicographicaly<2>(){
    std::sort(_squares.begin(),
              _squares.end(),
              Cell<2>::LexicographicalOrder());
}

template <>
inline void CellContainer::sortLexicographicaly<3>(){
    std::sort(_cubes.begin(),
              _cubes.end(),
              Cell<3>::LexicographicalOrder());
}

template <>
inline void CellContainer::sortByTimestamp<0>(){
    std::sort(_vertices.begin(),
              _vertices.end(),
              Cell<0>::TimestampOrder());
}

template <>
inline void CellContainer::sortByTimestamp<1>(){
    std::sort(_edges.begin(),
              _edges.end(),
              Cell<1>::TimestampOrder());
}

template <>
inline void CellContainer::sortByTimestamp<2>(){
    std::sort(_squares.begin(),
              _squares.end(),
              Cell<2>::TimestampOrder());
}

template <>
inline void CellContainer::sortByTimestamp<3>(){
    std::sort(_cubes.begin(),
              _cubes.end(),
              Cell<3>::TimestampOrder());
}

template <>
inline void CellContainer::sortForFiltration<0>(){
    std::sort(_vertices.begin(),
              _vertices.end(),
              Cell<0>::FiltrationOrder());
}

template <>
inline void CellContainer::sortForFiltration<1>(){
    std::sort(_edges.begin(),
              _edges.end(),
              Cell<1>::FiltrationOrder());
}

template <>
inline void CellContainer::sortForFiltration<2>(){
    std::sort(_squares.begin(),
              _squares.end(),
              Cell<2>::FiltrationOrder());
}

template <>
inline void CellContainer::sortForFiltration<3>(){
    std::sort(_cubes.begin(),
              _cubes.end(),
              Cell<3>::FiltrationOrder());
}




