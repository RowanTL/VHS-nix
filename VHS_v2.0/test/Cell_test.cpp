/*
 * Copyright 2020 St. Louis University
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

#include <algorithm>
#include <gtest/gtest.h>
#include <random>

#include "shape/Cell.h"
#include "util/Point.h"

using shape_simplification::shape::Cell;
using shape_simplification::shape::Point;

void verifyCellVertexAgreementForVertices(
        std::mt19937 &randomGenerator, std::vector<Point> &verticesPV,
        std::vector<Cell<0>::vertex> &verticesVV, std::array<Point, 1> &verticesPA,
        std::array<Cell<0>::vertex, 1> &verticesVA);
void verifyCellVertexAgreementForEdges(
        std::mt19937 &randomGenerator, std::vector<Point> &verticesPV,
        std::vector<Cell<1>::vertex> &verticesVV, std::array<Point, 2> &verticesPA,
        std::array<Cell<1>::vertex, 2> &verticesVA);
void verifyCellVertexAgreementForSquares(
        std::mt19937 &randomGenerator, std::vector<Point> &verticesPV,
        std::vector<Cell<2>::vertex> &verticesVV, std::array<Point, 4> &verticesPA,
        std::array<Cell<2>::vertex, 4> &verticesVA);
void verifyCellVertexAgreementForCubes(
        std::mt19937 &randomGenerator, std::vector<Point> &verticesPV,
        std::vector<Cell<3>::vertex> &verticesVV, std::array<Point, 8> &verticesPA,
        std::array<Cell<3>::vertex, 8> &verticesVA);
void verifyCellEqualityOperatorForVertices(std::mt19937 &randomGenerator,
                                           std::vector<Point> &vertices);
void verifyCellEqualityOperatorForEdges(std::mt19937 &randomGenerator,
                                        std::vector<Point> &vertices);
void verifyCellEqualityOperatorForSquares(std::mt19937 &randomGenerator,
                                          std::vector<Point> &vertices);
void verifyCellEqualityOperatorForCubes(std::mt19937 &randomGenerator,
                                        std::vector<Point> &vertices);
void verifyCellIsFaceCoFaceMethodsForVertices();
void verifyCellIsFaceCoFaceMethodsForEdges();
void verifyCellIsFaceCoFaceMethodsForSquares();
void verifyCellIsFaceCoFaceMethodsForCubes();
void verifyCellGetBoundaryMethodForEdges();
void verifyCellGetBoundaryMethodForSquares();
void verifyCellGetBoundaryMethodForCubes();

TEST(CellTest, VertexAgreement) {
    std::mt19937 randomGenerator(0);
    std::vector<Point> vertexVertexPV(1);
    std::vector<Cell<0>::vertex> vertexVertexVV(1);
    std::array<Point, 1> vertexVertexPA;
    std::array<Cell<0>::vertex, 1> vertexVertexVA;
    std::vector<Point> edgeVerticesPV(2);
    std::vector<Cell<1>::vertex> edgeVerticesVV(2);
    std::array<Point, 2> edgeVerticesPA;
    std::array<Cell<1>::vertex, 2> edgeVerticesVA;
    std::vector<Point> squareVerticesPV(4);
    std::vector<Cell<2>::vertex> squareVerticesVV(4);
    std::array<Point, 4> squareVerticesPA;
    std::array<Cell<2>::vertex, 4> squareVerticesVA;
    std::vector<Point> cubeVerticesPV(8);
    std::vector<Cell<3>::vertex> cubeVerticesVV(8);
    std::array<Point, 8> cubeVerticesPA;
    std::array<Cell<3>::vertex, 8> cubeVerticesVA;

    for (int repetition = 0; repetition < 100000; repetition++) {
        verifyCellVertexAgreementForVertices(randomGenerator, vertexVertexPV,
                                             vertexVertexVV, vertexVertexPA,
                                             vertexVertexVA);
        verifyCellVertexAgreementForEdges(randomGenerator, edgeVerticesPV,
                                          edgeVerticesVV, edgeVerticesPA,
                                          edgeVerticesVA);
        verifyCellVertexAgreementForSquares(randomGenerator, squareVerticesPV,
                                            squareVerticesVV, squareVerticesPA,
                                            squareVerticesVA);
        verifyCellVertexAgreementForCubes(randomGenerator, cubeVerticesPV,
                                          cubeVerticesVV, cubeVerticesPA,
                                          cubeVerticesVA);
    }
}

void verifyCellVertexAgreementForVertices(
        std::mt19937 &randomGenerator, std::vector<Point> &verticesPV,
        std::vector<Cell<0>::vertex> &verticesVV, std::array<Point, 1> &verticesPA,
        std::array<Cell<0>::vertex, 1> &verticesVA) {
    std::uniform_int_distribution<int> uniform(0, 65535);
    Point p;
    for (unsigned int coord = 0; coord < 3; coord++)
        p[coord] = uniform(randomGenerator);
    Cell<0>::vertex vertex = Cell<0>::pointToVertex(p);

    verticesPV.at(0) = p;
    verticesVV.at(0) = vertex;
    verticesPA[0] = p;
    verticesVA[0] = vertex;

    Cell<0> cellPV(verticesPV, 0);
    Cell<0> cellVV(verticesVV, 1);
    Cell<0> cellPA(verticesPA, 2);
    Cell<0> cellVA(verticesVA, 0);

    ASSERT_EQ(0, cellPV.getDim());
    ASSERT_EQ(0, cellPV.getTimestamp());
    ASSERT_EQ(0, cellVV.getDim());
    ASSERT_EQ(1, cellVV.getTimestamp());
    ASSERT_EQ(0, cellPA.getDim());
    ASSERT_EQ(2, cellPA.getTimestamp());
    ASSERT_EQ(0, cellVA.getDim());
    ASSERT_EQ(0, cellVA.getTimestamp());

    ASSERT_EQ(vertex, cellPV[0]);
    ASSERT_EQ(vertex, cellVV[0]);
    ASSERT_EQ(vertex, cellPA[0]);
    ASSERT_EQ(vertex, cellVA[0]);
}

void verifyCellVertexAgreementForEdges(
        std::mt19937 &randomGenerator, std::vector<Point> &verticesPV,
        std::vector<Cell<1>::vertex> &verticesVV, std::array<Point, 2> &verticesPA,
        std::array<Cell<1>::vertex, 2> &verticesVA) {
    std::uniform_int_distribution<int> uniform(0, 65535);
    const unsigned int size = 2;
    Point points[size];
    for (unsigned int coord = 0; coord < 3; coord++) {
        for (unsigned int i = 0; i < size; ++i) {
            points[i][coord] = uniform(randomGenerator);
        }
    }
    Cell<1>::vertex vertices[size];
    for (unsigned int i = 0; i < size; ++i) {
        vertices[i] = Cell<1>::pointToVertex(points[i]);
    }
    std::sort(vertices, vertices + size);

    for (unsigned int i = 0; i < size; ++i) {
        verticesPV.at(i) = points[i];
        verticesVV.at(i) = vertices[i];
        verticesPA[i] = points[i];
        verticesVA[i] = vertices[i];
    }

    Cell<1> cellPV(verticesPV, 0);
    Cell<1> cellVV(verticesVV, 1);
    Cell<1> cellPA(verticesPA, 2);
    Cell<1> cellVA(verticesVA, 0);

    ASSERT_EQ(1, cellPV.getDim());
    ASSERT_EQ(0, cellPV.getTimestamp());
    ASSERT_EQ(1, cellVV.getDim());
    ASSERT_EQ(1, cellVV.getTimestamp());
    ASSERT_EQ(1, cellPA.getDim());
    ASSERT_EQ(2, cellPA.getTimestamp());
    ASSERT_EQ(1, cellVA.getDim());
    ASSERT_EQ(0, cellVA.getTimestamp());

    for (unsigned int i = 0; i < size; ++i) {
        ASSERT_EQ(vertices[i], cellPV[i]);
        ASSERT_EQ(vertices[i], cellVV[i]);
        ASSERT_EQ(vertices[i], cellPA[i]);
        ASSERT_EQ(vertices[i], cellVA[i]);
    }
}

void verifyCellVertexAgreementForSquares(
        std::mt19937 &randomGenerator, std::vector<Point> &verticesPV,
        std::vector<Cell<2>::vertex> &verticesVV, std::array<Point, 4> &verticesPA,
        std::array<Cell<2>::vertex, 4> &verticesVA) {
    std::uniform_int_distribution<int> uniform(0, 65535);
    const unsigned int size = 4;
    Point points[size];
    for (unsigned int coord = 0; coord < 3; coord++) {
        for (unsigned int i = 0; i < size; ++i) {
            points[i][coord] = uniform(randomGenerator);
        }
    }
    Cell<2>::vertex vertices[size];
    for (unsigned int i = 0; i < size; ++i) {
        vertices[i] = Cell<2>::pointToVertex(points[i]);
    }
    std::sort(vertices, vertices + size);

    for (unsigned int i = 0; i < size; ++i) {
        verticesPV.at(i) = points[i];
        verticesVV.at(i) = vertices[i];
        verticesPA[i] = points[i];
        verticesVA[i] = vertices[i];
    }

    Cell<2> cellPV(verticesPV, 0);
    Cell<2> cellVV(verticesVV, 1);
    Cell<2> cellPA(verticesPA, 2);
    Cell<2> cellVA(verticesVA, 0);

    ASSERT_EQ(2, cellPV.getDim());
    ASSERT_EQ(0, cellPV.getTimestamp());
    ASSERT_EQ(2, cellVV.getDim());
    ASSERT_EQ(1, cellVV.getTimestamp());
    ASSERT_EQ(2, cellPA.getDim());
    ASSERT_EQ(2, cellPA.getTimestamp());
    ASSERT_EQ(2, cellVA.getDim());
    ASSERT_EQ(0, cellVA.getTimestamp());

    for (unsigned int i = 0; i < size; ++i) {
        ASSERT_EQ(vertices[i], cellPV[i]);
        ASSERT_EQ(vertices[i], cellVV[i]);
        ASSERT_EQ(vertices[i], cellPA[i]);
        ASSERT_EQ(vertices[i], cellVA[i]);
    }
}

void verifyCellVertexAgreementForCubes(
        std::mt19937 &randomGenerator, std::vector<Point> &verticesPV,
        std::vector<Cell<3>::vertex> &verticesVV, std::array<Point, 8> &verticesPA,
        std::array<Cell<3>::vertex, 8> &verticesVA) {
    std::uniform_int_distribution<int> uniform(0, 65535);
    const unsigned int size = 8;
    Point points[size];
    for (unsigned int coord = 0; coord < 3; coord++) {
        for (unsigned int i = 0; i < size; ++i) {
            points[i][coord] = uniform(randomGenerator);
        }
    }
    Cell<3>::vertex vertices[size];
    for (unsigned int i = 0; i < size; ++i) {
        vertices[i] = Cell<3>::pointToVertex(points[i]);
    }
    std::sort(vertices, vertices + size);

    for (unsigned int i = 0; i < size; ++i) {
        verticesPV.at(i) = points[i];
        verticesVV.at(i) = vertices[i];
        verticesPA[i] = points[i];
        verticesVA[i] = vertices[i];
    }

    Cell<3> cellPV(verticesPV, 0);
    Cell<3> cellVV(verticesVV, 1);
    Cell<3> cellPA(verticesPA, 2);
    Cell<3> cellVA(verticesVA, 0);

    ASSERT_EQ(3, cellPV.getDim());
    ASSERT_EQ(0, cellPV.getTimestamp());
    ASSERT_EQ(3, cellVV.getDim());
    ASSERT_EQ(1, cellVV.getTimestamp());
    ASSERT_EQ(3, cellPA.getDim());
    ASSERT_EQ(2, cellPA.getTimestamp());
    ASSERT_EQ(3, cellVA.getDim());
    ASSERT_EQ(0, cellVA.getTimestamp());

    for (unsigned int i = 0; i < size; ++i) {
        ASSERT_EQ(vertices[i], cellPV[i]);
        ASSERT_EQ(vertices[i], cellVV[i]);
        ASSERT_EQ(vertices[i], cellPA[i]);
        ASSERT_EQ(vertices[i], cellVA[i]);
    }
}

TEST(CellTest, VertexPointConvertion) {
    std::mt19937 randomGenerator(0);
    std::uniform_int_distribution<int> uniform(0, 65535);
    Point p;
    Cell<0>::vertex v;

    for (int repetition = 0; repetition < 100000; repetition++) {
        for (unsigned int coord = 0; coord < 3; coord++)
            p[coord] = uniform(randomGenerator);
        v = Cell<0>::pointToVertex(p);

        Point conv = Cell<0>::vertexToPoint(v);
        ASSERT_EQ(p[0], conv[0]);
        ASSERT_EQ(p[1], conv[1]);
        ASSERT_EQ(p[2], conv[2]);
    }
}

TEST(CellTest, EqualityOperator) {
    std::mt19937 randomGenerator(0);
    std::vector<Point> vertexVertex(1);
    std::vector<Point> edgeVertices(2);
    std::vector<Point> squareVertices(4);
    std::vector<Point> cubeVertices(8);

    for (int repetition = 0; repetition < 100000; repetition++) {
        verifyCellEqualityOperatorForVertices(randomGenerator, vertexVertex);
        verifyCellEqualityOperatorForEdges(randomGenerator, edgeVertices);
        verifyCellEqualityOperatorForSquares(randomGenerator, squareVertices);
        verifyCellEqualityOperatorForCubes(randomGenerator, cubeVertices);
    }
}

void verifyCellEqualityOperatorForVertices(std::mt19937 &randomGenerator,
                                           std::vector<Point> &vertices) {
    std::uniform_int_distribution<int> uniform(0, 65535);
    Point p1, p2;
    for (unsigned int coord = 0; coord < 3; coord++)
        p1[coord] = uniform(randomGenerator);
    vertices.at(0) = p1;

    Cell<0> cell1(vertices, 0);
    Cell<0> cell2(vertices, 2);

    while (p1[0] == p2[0] && p1[1] == p2[1] && p1[2] == p2[2])
        for (unsigned int coord = 0; coord < 3; coord++)
            p2[coord] = uniform(randomGenerator);
    vertices.at(0) = p2;
    Cell<0> cell3(vertices, 1);

    ASSERT_TRUE(cell1 == cell2);
    ASSERT_FALSE(cell1 == cell3);
}

void verifyCellEqualityOperatorForEdges(std::mt19937 &randomGenerator,
                                        std::vector<Point> &vertices) {
    std::uniform_int_distribution<int> uniform(0, 65535);
    const unsigned int size = 2;
    Point points[size];
    for (unsigned int coord = 0; coord < 3; coord++) {
        for (unsigned int i = 0; i < size; ++i) {
            points[i][coord] = uniform(randomGenerator);
        }
    }

    for (unsigned int i = 0; i < size; ++i) {
        vertices.at(i) = points[i];
    }
    Cell<1> cell1(vertices, 0);
    Cell<1> cell2(vertices, 2);

    Point p = points[0];
    while (p[0] == points[0][0] && p[1] == points[0][1] && p[2] == points[0][2])
        for (unsigned int coord = 0; coord < 3; coord++)
            p[coord] = uniform(randomGenerator);
    vertices.at(0) = p;
    Cell<1> cell3(vertices, 1);

    ASSERT_TRUE(cell1 == cell2);
    ASSERT_FALSE(cell1 == cell3);
}

void verifyCellEqualityOperatorForSquares(std::mt19937 &randomGenerator,
                                          std::vector<Point> &vertices) {
    std::uniform_int_distribution<int> uniform(0, 65535);
    const unsigned int size = 4;
    Point points[size];
    for (unsigned int coord = 0; coord < 3; coord++) {
        for (unsigned int i = 0; i < size; ++i) {
            points[i][coord] = uniform(randomGenerator);
        }
    }

    for (unsigned int i = 0; i < size; ++i) {
        vertices.at(i) = points[i];
    }
    Cell<2> cell1(vertices, 0);
    Cell<2> cell2(vertices, 2);

    Point p = points[0];
    while (p[0] == points[0][0] && p[1] == points[0][1] && p[2] == points[0][2])
        for (unsigned int coord = 0; coord < 3; coord++)
            p[coord] = uniform(randomGenerator);
    vertices.at(0) = p;
    Cell<2> cell3(vertices, 1);

    ASSERT_TRUE(cell1 == cell2);
    ASSERT_FALSE(cell1 == cell3);
}

void verifyCellEqualityOperatorForCubes(std::mt19937 &randomGenerator,
                                        std::vector<Point> &vertices) {
    std::uniform_int_distribution<int> uniform(0, 65535);
    const unsigned int size = 8;
    Point points[size];
    for (unsigned int coord = 0; coord < 3; coord++) {
        for (unsigned int i = 0; i < size; ++i) {
            points[i][coord] = uniform(randomGenerator);
        }
    }

    for (unsigned int i = 0; i < size; ++i) {
        vertices.at(i) = points[i];
    }
    Cell<3> cell1(vertices, 0);
    Cell<3> cell2(vertices, 2);

    Point p = points[0];
    while (p[0] == points[0][0] && p[1] == points[0][1] && p[2] == points[0][2])
        for (unsigned int coord = 0; coord < 3; coord++)
            p[coord] = uniform(randomGenerator);
    vertices.at(0) = p;
    Cell<3> cell3(vertices, 1);

    ASSERT_TRUE(cell1 == cell2);
    ASSERT_FALSE(cell1 == cell3);
}

TEST(CellTest, IsFaceCoFaceMethods) {
    verifyCellIsFaceCoFaceMethodsForVertices();
    verifyCellIsFaceCoFaceMethodsForEdges();
    verifyCellIsFaceCoFaceMethodsForSquares();
    verifyCellIsFaceCoFaceMethodsForCubes();
}

void verifyCellIsFaceCoFaceMethodsForVertices() {
    std::vector<Point> vertices;
    Point p = {0, 0, 0};

    vertices.push_back(p);
    Cell<0> vertex(vertices, 0);

    p[0] = 1;
    vertices.push_back(p);
    Cell<1> edge(vertices, 0);

    vertices.at(0)[2] = 1;
    vertices.at(1)[2] = 1;
    p[0] = 0;
    p[1] = 1;
    p[2] = 1;
    vertices.push_back(p);
    p[0] = 1;
    vertices.push_back(p);
    Cell<2> square(vertices, 0);

    p[2] = 0;
    vertices.push_back(p);
    p[1] = 0;
    vertices.push_back(p);
    p[0] = 0;
    vertices.push_back(p);
    p[1] = 1;
    vertices.push_back(p);
    Cell<3> cube(vertices, 0);

    ASSERT_TRUE(vertex.isFace(vertex));
    ASSERT_FALSE(vertex.isFace(edge));
    ASSERT_FALSE(vertex.isFace(square));
    ASSERT_FALSE(vertex.isFace(cube));
    ASSERT_TRUE(vertex.isPossibleCoface(vertex));
    ASSERT_TRUE(vertex.isPossibleCoface(edge));
    ASSERT_FALSE(vertex.isPossibleCoface(square));
    ASSERT_TRUE(vertex.isPossibleCoface(cube));
}

void verifyCellIsFaceCoFaceMethodsForEdges() {
    std::vector<Point> vertices;
    Point p = {0, 0, 0};

    vertices.push_back(p);
    Cell<0> vertex(vertices, 0);

    p[0] = 1;
    vertices.push_back(p);
    Cell<1> edge(vertices, 0);

    vertices.at(0)[2] = 1;
    vertices.at(1)[2] = 1;
    p[0] = 0;
    p[1] = 1;
    p[2] = 1;
    vertices.push_back(p);
    p[0] = 1;
    vertices.push_back(p);
    Cell<2> square(vertices, 0);

    p[2] = 0;
    vertices.push_back(p);
    p[1] = 0;
    vertices.push_back(p);
    p[0] = 0;
    vertices.push_back(p);
    p[1] = 1;
    vertices.push_back(p);
    Cell<3> cube(vertices, 0);

    ASSERT_TRUE(edge.isFace(vertex));
    ASSERT_TRUE(edge.isFace(edge));
    ASSERT_FALSE(edge.isFace(square));
    ASSERT_FALSE(edge.isFace(cube));
    ASSERT_FALSE(edge.isPossibleCoface(vertex));
    ASSERT_TRUE(edge.isPossibleCoface(edge));
    ASSERT_FALSE(edge.isPossibleCoface(square));
    ASSERT_TRUE(edge.isPossibleCoface(cube));
}

void verifyCellIsFaceCoFaceMethodsForSquares() {
    std::vector<Point> vertices;
    Point p = {0, 0, 0};

    vertices.push_back(p);
    Cell<0> vertex(vertices, 0);

    vertices.at(0)[2] = 1;
    p[0] = 1;
    p[2] = 1;
    vertices.push_back(p);
    Cell<1> edge(vertices, 0);

    p[1] = 1;
    vertices.push_back(p);
    p[0] = 0;
    vertices.push_back(p);
    Cell<2> square(vertices, 0);

    p[2] = 0;
    vertices.push_back(p);
    p[1] = 0;
    vertices.push_back(p);
    p[0] = 1;
    vertices.push_back(p);
    p[1] = 1;
    vertices.push_back(p);
    Cell<3> cube(vertices, 0);

    ASSERT_FALSE(square.isFace(vertex));
    ASSERT_TRUE(square.isFace(edge));
    ASSERT_TRUE(square.isFace(square));
    ASSERT_FALSE(square.isFace(cube));
    ASSERT_FALSE(square.isPossibleCoface(vertex));
    ASSERT_FALSE(square.isPossibleCoface(edge));
    ASSERT_TRUE(square.isPossibleCoface(square));
    ASSERT_TRUE(square.isPossibleCoface(cube));
}

void verifyCellIsFaceCoFaceMethodsForCubes() {
    std::vector<Point> vertices;
    Point p = {0, 0, 0};

    vertices.push_back(p);
    Cell<0> vertex(vertices, 0);

    vertices.at(0)[0] = 1;
    vertices.at(0)[2] = 2;
    p[0] = 1;
    p[2] = 1;
    vertices.push_back(p);
    Cell<1> edge(vertices, 0);

    vertices.at(0)[0] = 0;
    vertices.at(0)[2] = 1;
    p[1] = 1;
    vertices.push_back(p);
    p[0] = 0;
    vertices.push_back(p);
    Cell<2> square(vertices, 0);

    p[2] = 0;
    vertices.push_back(p);
    p[1] = 0;
    vertices.push_back(p);
    p[0] = 1;
    vertices.push_back(p);
    p[1] = 1;
    vertices.push_back(p);
    Cell<3> cube(vertices, 0);

    ASSERT_TRUE(cube.isFace(vertex));
    ASSERT_FALSE(cube.isFace(edge));
    ASSERT_TRUE(cube.isFace(square));
    ASSERT_TRUE(cube.isFace(cube));
    ASSERT_FALSE(cube.isPossibleCoface(vertex));
    ASSERT_FALSE(cube.isPossibleCoface(edge));
    ASSERT_FALSE(cube.isPossibleCoface(square));
    ASSERT_TRUE(cube.isPossibleCoface(cube));
}

TEST(CellTest, GetBoundaryMethod) {
    verifyCellGetBoundaryMethodForEdges();
    verifyCellGetBoundaryMethodForSquares();
    verifyCellGetBoundaryMethodForCubes();
}

void verifyCellGetBoundaryMethodForEdges() {
    std::vector<Point> vertices;
    Point p = {0, 0, 0};

    vertices.push_back(p);
    Cell<0> v1(vertices, 0);
    vertices.at(0)[0] = 1;
    Cell<0> v2(vertices, 0);

    p[0] = 0;
    vertices.push_back(p);
    Cell<1> edge(vertices, 0);

    Cell<1>::boundary_container boundary = edge.getBoundary();

    ASSERT_EQ(2, boundary.size());

    for (auto it = boundary.begin(); it != boundary.end(); ++it) {
        ASSERT_TRUE(*it == v1 || *it == v2);
        for (auto it2 = it + 1; it2 != boundary.end(); ++it2) {
            ASSERT_FALSE(*it == *it2);
        }
    }
}

void verifyCellGetBoundaryMethodForSquares() {
    std::vector<Point> vertices;
    Point p = {0, 0, 0};

    vertices.push_back(p);
    p[2] = 1;
    vertices.push_back(p);
    Cell<1> e1(vertices, 0);
    vertices.at(0)[2] = 1;
    vertices.at(1)[0] = 1;
    Cell<1> e2(vertices, 0);
    vertices.at(0)[0] = 1;
    vertices.at(1)[2] = 0;
    Cell<1> e3(vertices, 0);
    vertices.at(0)[2] = 0;
    vertices.at(1)[0] = 0;
    Cell<1> e4(vertices, 0);

    vertices.push_back(p);
    p[0] = 1;
    vertices.push_back(p);
    Cell<2> square(vertices, 0);

    Cell<2>::boundary_container boundary = square.getBoundary();

    ASSERT_EQ(4, boundary.size());

    for (auto it = boundary.begin(); it != boundary.end(); ++it) {
        ASSERT_TRUE(*it == e1 || *it == e2 || *it == e3 || *it == e4);
        for (auto it2 = it + 1; it2 != boundary.end(); ++it2) {
            ASSERT_FALSE(*it == *it2);
        }
    }
}

void verifyCellGetBoundaryMethodForCubes() {
    std::vector<Point> vertices;
    Point p1 = {0, 0, 0};
    Point p2 = {0, 0, 1};
    Point p3 = {0, 1, 0};
    Point p4 = {0, 1, 1};
    Point p5 = {1, 0, 0};
    Point p6 = {1, 0, 1};
    Point p7 = {1, 1, 0};
    Point p8 = {1, 1, 1};

    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
    vertices.push_back(p4);
    Cell<2> s1(vertices, 0);
    vertices.at(0) = p1;
    vertices.at(1) = p2;
    vertices.at(2) = p5;
    vertices.at(3) = p6;
    Cell<2> s2(vertices, 0);
    vertices.at(0) = p1;
    vertices.at(1) = p3;
    vertices.at(2) = p5;
    vertices.at(3) = p7;
    Cell<2> s3(vertices, 0);
    vertices.at(0) = p2;
    vertices.at(1) = p4;
    vertices.at(2) = p6;
    vertices.at(3) = p8;
    Cell<2> s4(vertices, 0);
    vertices.at(0) = p3;
    vertices.at(1) = p4;
    vertices.at(2) = p7;
    vertices.at(3) = p8;
    Cell<2> s5(vertices, 0);
    vertices.at(0) = p5;
    vertices.at(1) = p6;
    vertices.at(2) = p7;
    vertices.at(3) = p8;
    Cell<2> s6(vertices, 0);

    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
    vertices.push_back(p4);
    Cell<3> cube(vertices, 0);

    Cell<3>::boundary_container boundary = cube.getBoundary();

    ASSERT_EQ(6, boundary.size());

    for (auto it = boundary.begin(); it != boundary.end(); ++it) {
        ASSERT_TRUE(*it == s1 || *it == s2 || *it == s3 || *it == s4 || *it == s5 ||
                    *it == s6);
        for (auto it2 = it + 1; it2 != boundary.end(); ++it2) {
            ASSERT_FALSE(*it == *it2);
        }
    }
}
