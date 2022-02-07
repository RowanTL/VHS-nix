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
 * @file Cell_test.cpp
 * Test file for the Cell classes.
 **/

#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <unordered_set>

#include "util/Point.h"
#include "shape/Cell.h"
#include "shape/CellAdjacencies.h"
#include "test_examples.h"

using shape_simplification::shape::Cell;
using shape_simplification::shape::CellContainer;
using shape_simplification::shape::CellAdjacencies;
using shape_simplification::util::PointStrictLessThanComparator;

std::string printVertex(PackedData v){
    int x = static_cast<int>(v.rawData() >> (17U * 0U)
                             & static_cast<uint64_t>(65536 - 1));
    int y = static_cast<int>(v.rawData() >> (17U * 1U)
                             & static_cast<uint64_t>(65536 - 1));
    int z = static_cast<int>(v.rawData() >> (17U * 2U)
                             & static_cast<uint64_t>(65536 - 1));

    return "(" + std::to_string(x) + ", "
               + std::to_string(y) + ", "
               + std::to_string(z) + ")";
}

template <unsigned int dim>
void verifyCellContent(std::mt19937 &randomGenerator, std::string name)
{
    using vertex = typename Cell<dim>::vertex;

    std::uniform_int_distribution<int> uniform(0, 65535);
    const unsigned int size = 1 << dim;

    std::vector<Point> verticesVP(size);
    std::vector<vertex> verticesVV(size);
    std::array<Point, size> verticesAP;
    std::array<vertex, size> verticesAV;

    for (unsigned int coord = 0; coord < 3; coord++) {
        for (unsigned int i = 0; i < size; ++i) {
            verticesAP[i][coord] = uniform(randomGenerator);
        }
    }
    std::sort(verticesAP.begin(), verticesAP.end(),
              PointStrictLessThanComparator());

    for (unsigned int i = 0; i < size; ++i) {
        verticesAV[i] = Cell<dim>::pointToVertex(verticesAP[i]);
    }

    for (unsigned int i = 0; i < size; ++i) {
        verticesVP.at(i) = verticesAP[i];
        verticesVV.at(i) = verticesAV[i];
    }

    Cell<dim> cellVP(verticesVP, 0);
    Cell<dim> cellVV(verticesVV, 1);
    Cell<dim> cellAP(verticesAP, 2);
    Cell<dim> cellAV(verticesAV, 0);

    ASSERT_EQ(dim, cellVP.getDim()) << name;
    ASSERT_EQ(0, cellVP.getTimestamp()) << name;
    ASSERT_EQ(dim, cellVV.getDim()) << name;
    ASSERT_EQ(1, cellVV.getTimestamp()) << name;
    ASSERT_EQ(dim, cellAP.getDim()) << name;
    ASSERT_EQ(2, cellAP.getTimestamp()) << name;
    ASSERT_EQ(dim, cellAV.getDim()) << name;
    ASSERT_EQ(0, cellAV.getTimestamp()) << name;

    for (unsigned int i = 0; i < size; ++i) {
        ASSERT_TRUE(verticesAV[i] == cellVP[i])
                << printVertex(verticesAV[i]) << ", "
                << printVertex(cellVP[i]) << " - " << name;
        ASSERT_TRUE(verticesAV[i] == cellVV[i])
                << printVertex(verticesAV[i]) << ", "
                << printVertex(cellVV[i]) << " - " << name;
        ASSERT_TRUE(verticesAV[i] == cellAP[i])
                << printVertex(verticesAV[i]) << ", "
                << printVertex(cellAP[i]) << " - " << name;
        ASSERT_TRUE(verticesAV[i] == cellAV[i])
                << printVertex(verticesAV[i]) << ", "
                << printVertex(cellAV[i]) << " - " << name;
    }
}

TEST(CellTest, Content) {
    std::mt19937 randomGenerator(0); // Repeatable seed

    for (int repetition = 0; repetition < 1000; repetition++) {
      verifyCellContent<0>(randomGenerator, "dim 0");
      verifyCellContent<1>(randomGenerator, "dim 1");
      verifyCellContent<2>(randomGenerator, "dim 2");
      verifyCellContent<3>(randomGenerator, "dim 3");
    }
}

template<unsigned int dim>
void verifyCellFaceRelations(
        Cell<dim> &m,
        Cell<0> (&vertices)[12],
        Cell<1> (&edges)[20],
        Cell<2> (&squares)[11],
        Cell<3> (&cubes)[2],
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt,
        std::string name)
{
    for (unsigned int i = 0; i < 12; i++){
        if (verticesTrueAt.find(i) == verticesTrueAt.end())
            ASSERT_FALSE(m.isFace(vertices[i]))
                    << printVertex(vertices[i][0]) << " - " << name;
        else
            ASSERT_TRUE(m.isFace(vertices[i]))
                    << printVertex(vertices[i][0]) << " - " << name;
    }
    for (unsigned int i = 0; i < 20; i++){
        if (edgesTrueAt.find(i) == edgesTrueAt.end())
            ASSERT_FALSE(m.isFace(edges[i]))
                    << printVertex(edges[i][0]) << ", "
                    << printVertex(edges[i][1]) << " - " << name;
        else
            ASSERT_TRUE(m.isFace(edges[i]))
                    << printVertex(edges[i][0]) << ", "
                    << printVertex(edges[i][1]) << " - " << name;
    }
    for (unsigned int i = 0; i < 11; i++){
        if (squaresTrueAt.find(i) == squaresTrueAt.end())
            ASSERT_FALSE(m.isFace(squares[i]))
                    << printVertex(squares[i][0]) << ", "
                    << printVertex(squares[i][1]) << ", "
                    << printVertex(squares[i][2]) << ", "
                    << printVertex(squares[i][3]) << " - " << name;
        else
            ASSERT_TRUE(m.isFace(squares[i]))
                    << printVertex(squares[i][0]) << ", "
                    << printVertex(squares[i][1]) << ", "
                    << printVertex(squares[i][2]) << ", "
                    << printVertex(squares[i][3]) << " - " << name;
    }
    for (unsigned int i = 0; i < 2; i++){
        if (cubesTrueAt.find(i) == cubesTrueAt.end())
            ASSERT_FALSE(m.isFace(cubes[i]))
                    << printVertex(cubes[i][0]) << ", "
                    << printVertex(cubes[i][1]) << ", "
                    << printVertex(cubes[i][2]) << ", "
                    << printVertex(cubes[i][3]) << ", "
                    << printVertex(cubes[i][4]) << ", "
                    << printVertex(cubes[i][5]) << ", "
                    << printVertex(cubes[i][6]) << ", "
                    << printVertex(cubes[i][7]) << " - " << name;
        else
            ASSERT_TRUE(m.isFace(cubes[i]))
                    << printVertex(cubes[i][0]) << ", "
                    << printVertex(cubes[i][1]) << ", "
                    << printVertex(cubes[i][2]) << ", "
                    << printVertex(cubes[i][3]) << ", "
                    << printVertex(cubes[i][4]) << ", "
                    << printVertex(cubes[i][5]) << ", "
                    << printVertex(cubes[i][6]) << ", "
                    << printVertex(cubes[i][7]) << " - " << name;
    }
}

template<unsigned int dim>
void verifyCellCofaceRelations(
        Cell<dim> &m,
        Cell<0> (&vertices)[12],
        Cell<1> (&edges)[20],
        Cell<2> (&squares)[11],
        Cell<3> (&cubes)[2],
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt,
        std::string name)
{
    for (unsigned int i = 0; i < 12; i++){
        if (verticesTrueAt.find(i) == verticesTrueAt.end())
            ASSERT_FALSE(m.isPossibleCoface(vertices[i]))
                    << printVertex(vertices[i][0]) << " - " << name;
        else
            ASSERT_TRUE(m.isPossibleCoface(vertices[i]))
                    << printVertex(vertices[i][0]) << " - " << name;
    }
    for (unsigned int i = 0; i < 20; i++){
        if (edgesTrueAt.find(i) == edgesTrueAt.end())
            ASSERT_FALSE(m.isPossibleCoface(edges[i]))
                    << printVertex(edges[i][0]) << ", "
                    << printVertex(edges[i][1]) << " - " << name;
        else
            ASSERT_TRUE(m.isPossibleCoface(edges[i]))
                    << printVertex(edges[i][0]) << ", "
                    << printVertex(edges[i][1]) << " - " << name;
    }
    for (unsigned int i = 0; i < 11; i++){
        if (squaresTrueAt.find(i) == squaresTrueAt.end())
            ASSERT_FALSE(m.isPossibleCoface(squares[i]))
                    << printVertex(squares[i][0]) << ", "
                    << printVertex(squares[i][1]) << ", "
                    << printVertex(squares[i][2]) << ", "
                    << printVertex(squares[i][3]) << " - " << name;
        else
            ASSERT_TRUE(m.isPossibleCoface(squares[i]))
                    << printVertex(squares[i][0]) << ", "
                    << printVertex(squares[i][1]) << ", "
                    << printVertex(squares[i][2]) << ", "
                    << printVertex(squares[i][3]) << " - " << name;
    }
    for (unsigned int i = 0; i < 2; i++){
        if (cubesTrueAt.find(i) == cubesTrueAt.end())
            ASSERT_FALSE(m.isPossibleCoface(cubes[i]))
                    << printVertex(cubes[i][0]) << ", "
                    << printVertex(cubes[i][1]) << ", "
                    << printVertex(cubes[i][2]) << ", "
                    << printVertex(cubes[i][3]) << ", "
                    << printVertex(cubes[i][4]) << ", "
                    << printVertex(cubes[i][5]) << ", "
                    << printVertex(cubes[i][6]) << ", "
                    << printVertex(cubes[i][7]) << " - " << name;
        else
            ASSERT_TRUE(m.isPossibleCoface(cubes[i]))
                    << printVertex(cubes[i][0]) << ", "
                    << printVertex(cubes[i][1]) << ", "
                    << printVertex(cubes[i][2]) << ", "
                    << printVertex(cubes[i][3]) << ", "
                    << printVertex(cubes[i][4]) << ", "
                    << printVertex(cubes[i][5]) << ", "
                    << printVertex(cubes[i][6]) << ", "
                    << printVertex(cubes[i][7]) << " - " << name;
    }
}

template<unsigned int dim, unsigned int index>
void verificationLoopForCellFaceRelation(
        Cell<0> (&vertices)[12],
        Cell<1> (&edges)[20],
        Cell<2> (&squares)[11],
        Cell<3> (&cubes)[2])
{
    std::unordered_set<unsigned int> verticesTrueAt;
    std::unordered_set<unsigned int> edgesTrueAt;
    std::unordered_set<unsigned int> squaresTrueAt;
    std::unordered_set<unsigned int> cubesTrueAt;

    buildFaceRelationForCellExample<dim,index>(
                verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt);
    if constexpr (dim == 0){
        verifyCellFaceRelations(vertices[index],
                vertices, edges, squares, cubes,
                verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt,
                "v" + std::to_string(index) + " face relation");

        verificationLoopForCellFaceRelation<dim, index-1>(
                    vertices, edges, squares, cubes);
    } else if constexpr (dim == 1) {
        verifyCellFaceRelations(edges[index],
                vertices, edges, squares, cubes,
                verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt,
                "e" + std::to_string(index) + " face relation");
        if constexpr (index == 0)
            verificationLoopForCellFaceRelation<dim-1, 11>(
                    vertices, edges, squares, cubes);
        else
            verificationLoopForCellFaceRelation<dim, index-1>(
                        vertices, edges, squares, cubes);
    } else if constexpr (dim == 2) {
        verifyCellFaceRelations(squares[index],
                vertices, edges, squares, cubes,
                verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt,
                "s" + std::to_string(index) + " face relation");
        if constexpr (index == 0)
            verificationLoopForCellFaceRelation<dim-1, 19>(
                    vertices, edges, squares, cubes);
        else
            verificationLoopForCellFaceRelation<dim, index-1>(
                        vertices, edges, squares, cubes);
    } else {
        verifyCellFaceRelations(cubes[index],
                vertices, edges, squares, cubes,
                verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt,
                "c" + std::to_string(index) + " face relation");
        if constexpr (index == 0)
            verificationLoopForCellFaceRelation<dim-1, 10>(
                    vertices, edges, squares, cubes);
        else
            verificationLoopForCellFaceRelation<dim, index-1>(
                        vertices, edges, squares, cubes);
    }

}

template<>
void verificationLoopForCellFaceRelation<0,0>(
        Cell<0> (&vertices)[12],
        Cell<1> (&edges)[20],
        Cell<2> (&squares)[11],
        Cell<3> (&cubes)[2])
{
    std::unordered_set<unsigned int> verticesTrueAt;
    std::unordered_set<unsigned int> edgesTrueAt;
    std::unordered_set<unsigned int> squaresTrueAt;
    std::unordered_set<unsigned int> cubesTrueAt;

    buildFaceRelationForCellExample<0,0>(
                verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt);
    verifyCellFaceRelations(vertices[0],
            vertices, edges, squares, cubes,
            verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt,
            "v0 face relation");
}

template<unsigned int dim, unsigned int index>
void verificationLoopForCellCofaceRelation(
        Cell<0> (&vertices)[12],
        Cell<1> (&edges)[20],
        Cell<2> (&squares)[11],
        Cell<3> (&cubes)[2])
{
    std::unordered_set<unsigned int> verticesTrueAt;
    std::unordered_set<unsigned int> edgesTrueAt;
    std::unordered_set<unsigned int> squaresTrueAt;
    std::unordered_set<unsigned int> cubesTrueAt;

    buildCofaceRelationForCellExample<dim,index>(
                verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt);
    if constexpr (dim == 0){
        verifyCellCofaceRelations(vertices[index],
                vertices, edges, squares, cubes,
                verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt,
                "v" + std::to_string(index) + " coface relation");

        verificationLoopForCellCofaceRelation<dim, index-1>(
                    vertices, edges, squares, cubes);
    } else if constexpr (dim == 1) {
        verifyCellCofaceRelations(edges[index],
                vertices, edges, squares, cubes,
                verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt,
                "e" + std::to_string(index) + " coface relation");
        if constexpr (index == 0)
            verificationLoopForCellCofaceRelation<dim-1, 11>(
                    vertices, edges, squares, cubes);
        else
            verificationLoopForCellCofaceRelation<dim, index-1>(
                        vertices, edges, squares, cubes);
    } else if constexpr (dim == 2) {
        verifyCellCofaceRelations(squares[index],
                vertices, edges, squares, cubes,
                verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt,
                "s" + std::to_string(index) + " coface relation");
        if constexpr (index == 0)
            verificationLoopForCellCofaceRelation<dim-1, 19>(
                    vertices, edges, squares, cubes);
        else
            verificationLoopForCellCofaceRelation<dim, index-1>(
                        vertices, edges, squares, cubes);
    } else {
        verifyCellCofaceRelations(cubes[index],
                vertices, edges, squares, cubes,
                verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt,
                "c" + std::to_string(index) + " coface relation");
        if constexpr (index == 0)
            verificationLoopForCellCofaceRelation<dim-1, 10>(
                    vertices, edges, squares, cubes);
        else
            verificationLoopForCellCofaceRelation<dim, index-1>(
                        vertices, edges, squares, cubes);
    }

}

template<>
void verificationLoopForCellCofaceRelation<0,0>(
        Cell<0> (&vertices)[12],
        Cell<1> (&edges)[20],
        Cell<2> (&squares)[11],
        Cell<3> (&cubes)[2])
{
    std::unordered_set<unsigned int> verticesTrueAt;
    std::unordered_set<unsigned int> edgesTrueAt;
    std::unordered_set<unsigned int> squaresTrueAt;
    std::unordered_set<unsigned int> cubesTrueAt;

    buildCofaceRelationForCellExample<0,0>(
                verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt);
    verifyCellCofaceRelations(vertices[0],
            vertices, edges, squares, cubes,
            verticesTrueAt, edgesTrueAt, squaresTrueAt, cubesTrueAt,
            "v0 coface relation");
}

TEST(CellTest, Relations) {
    Cell<0> vertices[12];
    Cell<1> edges[20];
    Cell<2> squares[11];
    Cell<3> cubes[2];

    buildExampleForCells(vertices, edges, squares, cubes);

    verificationLoopForCellFaceRelation<3,1>(vertices, edges, squares, cubes);
    verificationLoopForCellCofaceRelation<3,1>(vertices, edges, squares, cubes);
}

TEST(CellTest, Converters) {
    using vertex = typename Cell<0>::vertex;

    std::vector<Point> points;
    points.push_back({31,6,248});
    Cell<0> cell(points, 0);

    Point resPoint = Cell<0>::vertexToPoint(cell[0]);

    ASSERT_EQ(31, resPoint[0]) << "vertexToPoint X";
    ASSERT_EQ(6, resPoint[1]) << "vertexToPoint Y";
    ASSERT_EQ(248, resPoint[2]) << "vertexToPoint Z";

    int resX = Cell<0>::vertexToCoordinate(cell[0], 0);
    int resY = Cell<0>::vertexToCoordinate(cell[0], 1);
    int resZ = Cell<0>::vertexToCoordinate(cell[0], 2);

    ASSERT_EQ(31, resX) << "vertexToCoordinate X";
    ASSERT_EQ(6, resY) << "vertexToCoordinate Y";
    ASSERT_EQ(248, resZ) << "vertexToCoordinate Z";

    vertex resV = Cell<0>::pointToVertex(points.at(0));

    ASSERT_TRUE(cell[0] == resV) << "pointToVertex";
}

TEST(CellContainerTest, Content) {
    CellContainer cc;
    Cell<0> vertices[12];
    Cell<1> edges[20];
    Cell<2> squares[11];
    Cell<3> cubes[2];

    buildExampleForCells(vertices, edges, squares, cubes);

    for (Cell<0> c : vertices) cc.insert(c);
    for (Cell<1> c : edges) cc.insert(c);
    for (Cell<2> c : squares) cc.insert(c);
    for (Cell<3> c : cubes) cc.insert(c);

    ASSERT_EQ(12, cc.getSize<0>());
    ASSERT_EQ(20, cc.getSize<1>());
    ASSERT_EQ(11, cc.getSize<2>());
    ASSERT_EQ(2, cc.getSize<3>());

    CellContainer::const_cell_iterator<0> it0 = cc.begin<0>();
    for (unsigned int i = 0; i < 12; i++){
        ASSERT_TRUE(vertices[i] == cc.at<0>(i)) << "dim 0 at " << i;
        ASSERT_TRUE(vertices[i] == *it0) << "dim 0 iterator " << i;
        it0++;
    }
    ASSERT_TRUE(it0 == cc.end<0>());

    CellContainer::const_cell_iterator<1> it1 = cc.begin<1>();
    for (unsigned int i = 0; i < 20; i++){
        ASSERT_TRUE(edges[i] == cc.at<1>(i)) << "dim 1 at " << i;
        ASSERT_TRUE(edges[i] == *it1) << "dim 1 iterator " << i;
        it1++;
    }
    ASSERT_TRUE(it1 == cc.end<1>());

    CellContainer::const_cell_iterator<2> it2 = cc.begin<2>();
    for (unsigned int i = 0; i < 11; i++){
        ASSERT_TRUE(squares[i] == cc.at<2>(i)) << "dim 2 at " << i;
        ASSERT_TRUE(squares[i] == *it2) << "dim 2 iterator " << i;
        it2++;
    }
    ASSERT_TRUE(it2 == cc.end<2>());

    CellContainer::const_cell_iterator<3> it3 = cc.begin<3>();
    for (unsigned int i = 0; i < 2; i++){
        ASSERT_TRUE(cubes[i] == cc.at<3>(i)) << "dim 3 at " << i;
        ASSERT_TRUE(cubes[i] == *it3) << "dim 3 iterator " << i;
        it3++;
    }
    ASSERT_TRUE(it3 == cc.end<3>());
}

TEST(CellContainerTest, Modifier) {
    CellContainer cc;
    Cell<0> vertices[12];
    Cell<1> edges[20];
    Cell<2> squares[11];
    Cell<3> cubes[2];

    buildExampleForCells(vertices, edges, squares, cubes);

    cc.resize<0>(12);
    for (unsigned int i = 0; i < 12; i++){
        cc.insertAt(vertices[i], 11 - i);
    }
    cc.resize<1>(20);
    for (unsigned int i = 0; i < 20; i++){
        cc.insertAt(edges[i], 19 - i);
    }
    cc.resize<2>(11);
    for (unsigned int i = 0; i < 11; i++){
        cc.insertAt(squares[i], 10 - i);
    }
    cc.resize<3>(2);
    for (unsigned int i = 0; i < 2; i++){
        cc.insertAt(cubes[i], 1 - i);
    }

    for (unsigned int i = 0; i < 12; i++){
        ASSERT_TRUE(vertices[i] == cc.at<0>(11 - i)) << "dim 0 at " << i;
    }
    for (unsigned int i = 0; i < 20; i++){
        ASSERT_TRUE(edges[i] == cc.at<1>(19 - i)) << "dim 1 at " << i;
    }
    for (unsigned int i = 0; i < 11; i++){
        ASSERT_TRUE(squares[i] == cc.at<2>(10 - i)) << "dim 2 at " << i;
    }
    for (unsigned int i = 0; i < 2; i++){
        ASSERT_TRUE(cubes[i] == cc.at<3>(1 - i)) << "dim 3 at " << i;
    }
}

TEST(CellContainerTest, Sorts) {
    CellContainer cc;

    Cell<0> verticesSorted[5];
    Cell<1> edgesSorted[5];
    Cell<2> squaresSorted[5];
    Cell<3> cubesSorted[5];

    buildExampleForCellContainer(cc);

    buildSortedByTimestampForCellContainerExample(
                verticesSorted, edgesSorted,
                squaresSorted, cubesSorted);

    cc.sortByTimestamp<0>();
    cc.sortByTimestamp<1>();
    cc.sortByTimestamp<2>();
    cc.sortByTimestamp<3>();

    for (unsigned int i = 0; i < 5; i++){
        ASSERT_TRUE(verticesSorted[i] == cc.at<0>(i))
                << "dim 0 time order " << i;
        ASSERT_TRUE(edgesSorted[i] == cc.at<1>(i))
                << "dim 1 time order " << i;
        ASSERT_TRUE(squaresSorted[i] == cc.at<2>(i))
                << "dim 2 time order " << i;
        ASSERT_TRUE(cubesSorted[i] == cc.at<3>(i))
                << "dim 3 time order " << i;
    }

    buildSortedLexicographicalyForCellContainerExample(
            verticesSorted, edgesSorted,
            squaresSorted, cubesSorted);

    cc.sortLexicographicaly<0>();
    cc.sortLexicographicaly<1>();
    cc.sortLexicographicaly<2>();
    cc.sortLexicographicaly<3>();

    for (unsigned int i = 0; i < 5; i++){
        ASSERT_TRUE(verticesSorted[i] == cc.at<0>(i))
                << "dim 0 lex order " << i;
        ASSERT_TRUE(edgesSorted[i] == cc.at<1>(i))
                << "dim 1 lex order " << i;
        ASSERT_TRUE(squaresSorted[i] == cc.at<2>(i))
                << "dim 2 lex order " << i;
        ASSERT_TRUE(cubesSorted[i] == cc.at<3>(i))
                << "dim 3 lex order " << i;
    }

    buildSortedForFiltrationForCellContainerExample(
            verticesSorted, edgesSorted,
            squaresSorted, cubesSorted);

    cc.sortForFiltration<0>();
    cc.sortForFiltration<1>();
    cc.sortForFiltration<2>();
    cc.sortForFiltration<3>();

    for (unsigned int i = 0; i < 5; i++){
        ASSERT_TRUE(verticesSorted[i] == cc.at<0>(i))
                << "dim 0 filtration order " << i;
        ASSERT_TRUE(edgesSorted[i] == cc.at<1>(i))
                << "dim 1 filtration order " << i;
        ASSERT_TRUE(squaresSorted[i] == cc.at<2>(i))
                << "dim 2 filtration order " << i;
        ASSERT_TRUE(cubesSorted[i] == cc.at<3>(i))
                << "dim 3 filtration order " << i;
    }
}

