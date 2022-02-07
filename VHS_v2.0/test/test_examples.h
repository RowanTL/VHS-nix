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

#pragma once

/**
 * @file test_examples.h
 * Contains example builders for the unit tests.
 **/

#include "png.hpp"
#include <vector>
#include <unordered_set>
#include <random>

#include "shape/ShapeUtilities.h"
#include "shape/Cell.h"

using shape_simplification::shape::Level;
using shape_simplification::shape::Cell;

// Euler Characteristic: 1
// Number of components: 2
// Number of loops: 1
// Number of voids: 0
template<class Voxel>
static void buildExample1(std::vector<Voxel>& voxels)
{
    Voxel v0(0, 0, 1, Level::NEIGHBORHOOD);  // N
    Voxel v1(0, 0, 2, Level::MAIN_SHAPE);  // S
    Voxel v2(0, 0, 3, Level::MAIN_SHAPE);  // S
    Voxel v3(0, 1, 2, Level::CORE);  // C
    Voxel v4(0, 1, 3, Level::MAIN_SHAPE);  // S
    Voxel v5(1, 0, 2, Level::MAIN_SHAPE);  // S
    Voxel v6(1, 0, 3, Level::NEIGHBORHOOD);  // N
    Voxel v7(1, 1, 2, Level::MAIN_SHAPE);  // S
    Voxel v8(1, 1, 3, Level::CORE);  // C
    Voxel v9(0, 2, 1, Level::MAIN_SHAPE);  // S
    Voxel v10(0, 2, 2, Level::CORE); // C
    Voxel v11(0, 3, 1, Level::NEIGHBORHOOD); // N
    Voxel v12(0, 3, 2, Level::MAIN_SHAPE); // S
    Voxel v13(1, 4, 3, Level::CORE); // C
    Voxel v14(0, 4, 2, Level::MAIN_SHAPE); // S
    Voxel v15(0, 4, 1, Level::MAIN_SHAPE); // S
    Voxel v16(0, 2, 0, Level::MAIN_SHAPE); // S
    Voxel v17(0, 3, 0, Level::CORE); // C
    Voxel v18(0, 4, 0, Level::MAIN_SHAPE); // S
    Voxel v19(1, 4, 4, Level::NEIGHBORHOOD); // N
    Voxel v20(0, 4, 3, Level::NEIGHBORHOOD); // N

    voxels.push_back(v0);
    voxels.push_back(v1);
    voxels.push_back(v2);
    voxels.push_back(v3);
    voxels.push_back(v4);
    voxels.push_back(v5);
    voxels.push_back(v6);
    voxels.push_back(v7);
    voxels.push_back(v8);
    voxels.push_back(v9);
    voxels.push_back(v10);
    voxels.push_back(v11);
    voxels.push_back(v12);
    voxels.push_back(v13);
    voxels.push_back(v14);
    voxels.push_back(v15);
    voxels.push_back(v16);
    voxels.push_back(v17);
    voxels.push_back(v18);
    voxels.push_back(v19);
    voxels.push_back(v20);
}

// Euler Characteristic: 2
// Number of components: 2
// Number of loops: 0
// Number of voids: 0
template<class Voxel>
static void buildExample1Small(std::vector<Voxel>& voxels)
{
    Voxel v0(0, 0, 1, Level::NEIGHBORHOOD);  // N
    Voxel v1(0, 0, 2, Level::MAIN_SHAPE);  // S
    Voxel v2(0, 0, 3, Level::MAIN_SHAPE);  // S
    Voxel v3(0, 1, 2, Level::CORE);  // C
    Voxel v4(0, 1, 3, Level::MAIN_SHAPE);  // S
    Voxel v5(1, 0, 2, Level::MAIN_SHAPE);  // S
    Voxel v6(1, 0, 3, Level::NEIGHBORHOOD);  // N
    Voxel v7(1, 1, 2, Level::MAIN_SHAPE);  // S
    Voxel v8(1, 1, 3, Level::CORE);  // C
    Voxel v9(0, 2, 1, Level::MAIN_SHAPE);  // S
    Voxel v10(0, 2, 2, Level::CORE); // C
    Voxel v11(0, 3, 1, Level::NEIGHBORHOOD); // N
    Voxel v12(0, 3, 2, Level::MAIN_SHAPE); // S
    Voxel v13(1, 4, 3, Level::CORE); // C

    voxels.push_back(v0);
    voxels.push_back(v1);
    voxels.push_back(v2);
    voxels.push_back(v3);
    voxels.push_back(v4);
    voxels.push_back(v5);
    voxels.push_back(v6);
    voxels.push_back(v7);
    voxels.push_back(v8);
    voxels.push_back(v9);
    voxels.push_back(v10);
    voxels.push_back(v11);
    voxels.push_back(v12);
    voxels.push_back(v13);
}

// Euler Characteristic: 2
// Number of components: 2
// Number of loops: 0
// Number of voids: 0
template<class Voxel>
static void buildExample1SmallTranslated(std::vector<Voxel>& voxels)
{
    Voxel v0(0, 0, 0, Level::NEIGHBORHOOD);  // N
    Voxel v1(0, 0, 1, Level::MAIN_SHAPE);  // S
    Voxel v2(0, 0, 2, Level::MAIN_SHAPE);  // S
    Voxel v3(0, 1, 1, Level::CORE);  // C
    Voxel v4(0, 1, 2, Level::MAIN_SHAPE);  // S
    Voxel v5(1, 0, 1, Level::MAIN_SHAPE);  // S
    Voxel v6(1, 0, 2, Level::NEIGHBORHOOD);  // N
    Voxel v7(1, 1, 1, Level::MAIN_SHAPE);  // S
    Voxel v8(1, 1, 2, Level::CORE);  // C
    Voxel v9(0, 2, 0, Level::MAIN_SHAPE);  // S
    Voxel v10(0, 2, 1, Level::CORE); // C
    Voxel v11(0, 3, 0, Level::NEIGHBORHOOD); // N
    Voxel v12(0, 3, 1, Level::MAIN_SHAPE); // S
    Voxel v13(1, 4, 2, Level::CORE); // C

    voxels.push_back(v0);
    voxels.push_back(v1);
    voxels.push_back(v2);
    voxels.push_back(v3);
    voxels.push_back(v4);
    voxels.push_back(v5);
    voxels.push_back(v6);
    voxels.push_back(v7);
    voxels.push_back(v8);
    voxels.push_back(v9);
    voxels.push_back(v10);
    voxels.push_back(v11);
    voxels.push_back(v12);
    voxels.push_back(v13);
}

// Euler Characteristic: 1
// Number of components: 2
// Number of loops: 1
// Number of voids: 0
template<class Voxel>
static void buildExample1Translated(std::vector<Voxel>& voxels)
{
    Voxel v0(1, 1, 2, Level::NEIGHBORHOOD);  // N
    Voxel v1(1, 1, 3, Level::MAIN_SHAPE);  // S
    Voxel v2(1, 1, 4, Level::MAIN_SHAPE);  // S
    Voxel v3(1, 2, 3, Level::CORE);  // C
    Voxel v4(1, 2, 4, Level::MAIN_SHAPE);  // S
    Voxel v5(2, 1, 3, Level::MAIN_SHAPE);  // S
    Voxel v6(2, 1, 4, Level::NEIGHBORHOOD);  // N
    Voxel v7(2, 2, 3, Level::MAIN_SHAPE);  // S
    Voxel v8(2, 2, 4, Level::CORE);  // C
    Voxel v9(1, 3, 2, Level::MAIN_SHAPE);  // S
    Voxel v10(1, 3, 3, Level::CORE); // C
    Voxel v11(1, 4, 2, Level::NEIGHBORHOOD); // N
    Voxel v12(1, 4, 3, Level::MAIN_SHAPE); // S
    Voxel v13(2, 5, 4, Level::CORE); // C
    Voxel v14(1, 5, 3, Level::MAIN_SHAPE); // S
    Voxel v15(1, 5, 2, Level::MAIN_SHAPE); // S
    Voxel v16(1, 3, 1, Level::MAIN_SHAPE); // S
    Voxel v17(1, 4, 1, Level::CORE); // C
    Voxel v18(1, 5, 1, Level::MAIN_SHAPE); // S
    Voxel v19(2, 5, 5, Level::NEIGHBORHOOD); // N
    Voxel v20(1, 5, 4, Level::NEIGHBORHOOD); // N

    voxels.push_back(v0);
    voxels.push_back(v1);
    voxels.push_back(v2);
    voxels.push_back(v3);
    voxels.push_back(v4);
    voxels.push_back(v5);
    voxels.push_back(v6);
    voxels.push_back(v7);
    voxels.push_back(v8);
    voxels.push_back(v9);
    voxels.push_back(v10);
    voxels.push_back(v11);
    voxels.push_back(v12);
    voxels.push_back(v13);
    voxels.push_back(v14);
    voxels.push_back(v15);
    voxels.push_back(v16);
    voxels.push_back(v17);
    voxels.push_back(v18);
    voxels.push_back(v19);
    voxels.push_back(v20);
}

// Euler Characteristic: 3
// Number of components: 3
// Number of loops: 1
// Number of voids: 1
template<class Voxel>
static void buildExample1Extended(std::vector<Voxel>& voxels)
{
    Voxel v0(1, 1, 2, Level::NEIGHBORHOOD);     // N
    Voxel v1(1, 1, 3, Level::MAIN_SHAPE);       // S
    Voxel v2(1, 1, 4, Level::MAIN_SHAPE);       // S
    Voxel v3(1, 2, 3, Level::CORE);             // C
    Voxel v4(1, 2, 4, Level::MAIN_SHAPE);       // S
    Voxel v5(2, 1, 3, Level::MAIN_SHAPE);       // S
    Voxel v6(2, 1, 4, Level::NEIGHBORHOOD);     // N
    Voxel v7(2, 2, 3, Level::MAIN_SHAPE);       // S
    Voxel v8(2, 2, 4, Level::CORE);             // C
    Voxel v9(1, 3, 2, Level::MAIN_SHAPE);       // S
    Voxel v10(1, 3, 3, Level::CORE);            // C
    Voxel v11(1, 4, 2, Level::NEIGHBORHOOD);    // N
    Voxel v12(1, 4, 3, Level::MAIN_SHAPE);      // S
    Voxel v13(2, 5, 4, Level::CORE);            // C
    Voxel v14(1, 5, 3, Level::MAIN_SHAPE);      // S
    Voxel v15(1, 5, 2, Level::MAIN_SHAPE);      // S
    Voxel v16(1, 3, 1, Level::MAIN_SHAPE);      // S
    Voxel v17(1, 4, 1, Level::MAIN_SHAPE);      // S
    Voxel v18(1, 5, 1, Level::MAIN_SHAPE);      // S
    Voxel v19(2, 5, 5, Level::NEIGHBORHOOD);    // N
    Voxel v20(1, 5, 4, Level::NEIGHBORHOOD);    // N

    voxels.push_back(v0);
    voxels.push_back(v1);
    voxels.push_back(v2);
    voxels.push_back(v3);
    voxels.push_back(v4);
    voxels.push_back(v5);
    voxels.push_back(v6);
    voxels.push_back(v7);
    voxels.push_back(v8);
    voxels.push_back(v9);
    voxels.push_back(v10);
    voxels.push_back(v11);
    voxels.push_back(v12);
    voxels.push_back(v13);
    voxels.push_back(v14);
    voxels.push_back(v15);
    voxels.push_back(v16);
    voxels.push_back(v17);
    voxels.push_back(v18);
    voxels.push_back(v19);
    voxels.push_back(v20);

    for (int x = 10; x < 14; x++) {
        for (int y = 10; y < 14; y++) {
            for (int z = 10; z < 14; z++) {
                voxels.push_back(Voxel(x, y, z));
                if (x > 10 && x < 13 && y > 10 && y < 13 && z > 10 && z < 13)
                    voxels.back().setLevel(Level::NEIGHBORHOOD);
                else
                    voxels.back().setLevel(Level::MAIN_SHAPE);
            }
        }
    }
}

// Euler Characteristic: 1
// Number of components: 2
// Number of loops: 1
// Number of voids: 0
// Inverted:
// Euler Characteristic: 2
// Number of components: 1
// Number of loops: 1
// Number of voids: 2
// Partially Inverted:
// Euler Characteristic: 3
// Number of components: 2
// Number of loops: 1
// Number of voids: 2
template<class Voxel>
static void buildExample2(std::vector<Voxel>& voxels)
{
    for (int x = 0; x < 2; x++){
        for (int y = 3; y < 7; y++){
            for (int z = 8; z < 12; z++){
                Voxel v(x, y, z);
                voxels.push_back(v);
                if (x == 1 && (y == 4 || y == 5) && (z == 9 || z == 10))
                    voxels.back().setLevel(Level::MAIN_SHAPE);
                else
                    voxels.back().setLevel(Level::NEIGHBORHOOD);
            }
        }
    }
    for (int y = 3; y < 7; y++){
        for (int z = 8; z < 11; z++){
            Voxel v(2, y, z);
            voxels.push_back(v);
            if ((y == 4 || y == 5) && (z == 9 || z == 10))
                voxels.back().setLevel(Level::MAIN_SHAPE);
            else
                voxels.back().setLevel(Level::NEIGHBORHOOD);
        }
    }
    for (int x = 6; x < 15; x++){
        for (int y = 0; y < 5; y++){
            for (int z = 0; z < 12; z++){
                Voxel v(x, y, z);
                voxels.push_back(v);
                if (x > 6 && x < 14 && y > 0 && y < 4 && z > 0 && z < 11
                        && (x != 10 || z < 4 || z > 7)){
                    if (y == 2 && (((x == 8 || x == 12) && z > 1 && z < 10) ||
                                   ((z == 2 || z == 9) && x > 8 && x < 12)) &&
                            (x != 8 || (z != 6 && z != 4 && z != 3))){
                        voxels.back().setLevel(Level::CORE);
                    } else {
                        voxels.back().setLevel(Level::MAIN_SHAPE);
                    }
                } else {
                    voxels.back().setLevel(Level::NEIGHBORHOOD);
                }
            }
        }
    }
}

template<class Voxel>
static void buildMiniExample1(std::vector<Voxel>& voxels)
{
    Voxel v0(0, 0, 0, Level::CORE);
    Voxel v1(0, 1, 0, Level::MAIN_SHAPE);
    Voxel v2(1, 1, 1, Level::CORE);
    Voxel v3(1, 0, 2, Level::NEIGHBORHOOD);

    voxels.push_back(v0);
    voxels.push_back(v1);
    voxels.push_back(v2);
    voxels.push_back(v3);
}

template<class Voxel>
static void buildMiniExample2(std::vector<Voxel>& voxels)
{
    Voxel v0(0, 0, 0, Level::CORE);
    Voxel v1(0, 1, 0, Level::NEIGHBORHOOD);
    Voxel v2(2, 0, 3, Level::MAIN_SHAPE);
    Voxel v3(0, 2, 1, Level::CORE);

    voxels.push_back(v0);
    voxels.push_back(v1);
    voxels.push_back(v2);
    voxels.push_back(v3);
}

template<class Voxel>
static void buildRandomExample(
        unsigned int nberOfVoxels,
        std::vector<Voxel> &voxels)
{
    std::unordered_set<Voxel> voxelSet;
    std::mt19937 randomGenerator(0); // Repeatable seed
    std::uniform_int_distribution<int> unicoord(0, 100);
    std::uniform_int_distribution<int> unilevel(0, 3);

    for (unsigned int i = 0; i < nberOfVoxels; ++i) {
        voxelSet.insert(Voxel(unicoord(randomGenerator),
                            unicoord(randomGenerator),
                            unicoord(randomGenerator),
                            static_cast<Level>(unilevel(randomGenerator))));
    }

    for (Voxel v : voxelSet) voxels.push_back(v);
}

// Euler Characteristic: 2
// Number of components: 2
// Number of loops: 0
// Number of voids: 0
template<class Voxel>
static void buildExample1SmallAsStack(
        std::vector<png::image<png::gray_pixel> >& stack)
{
    Voxel v0(0, 0, 1, Level::NEIGHBORHOOD);  // N
    Voxel v1(0, 0, 2, Level::MAIN_SHAPE);  // S
    Voxel v2(0, 0, 3, Level::MAIN_SHAPE);  // S
    Voxel v3(0, 1, 2, Level::CORE);  // C
    Voxel v4(0, 1, 3, Level::MAIN_SHAPE);  // S
    Voxel v5(1, 0, 2, Level::MAIN_SHAPE);  // S
    Voxel v6(1, 0, 3, Level::NEIGHBORHOOD);  // N
    Voxel v7(1, 1, 2, Level::MAIN_SHAPE);  // S
    Voxel v8(1, 1, 3, Level::CORE);  // C
    Voxel v9(0, 2, 1, Level::MAIN_SHAPE);  // S
    Voxel v10(0, 2, 2, Level::CORE); // C
    Voxel v11(0, 3, 1, Level::NEIGHBORHOOD); // N
    Voxel v12(0, 3, 2, Level::MAIN_SHAPE); // S
    Voxel v13(1, 4, 3, Level::CORE); // C

    std::vector<Voxel> voxels;
    voxels.push_back(v0);
    voxels.push_back(v1);
    voxels.push_back(v2);
    voxels.push_back(v3);
    voxels.push_back(v4);
    voxels.push_back(v5);
    voxels.push_back(v6);
    voxels.push_back(v7);
    voxels.push_back(v8);
    voxels.push_back(v9);
    voxels.push_back(v10);
    voxels.push_back(v11);
    voxels.push_back(v12);
    voxels.push_back(v13);


    for (unsigned int z = 0; z < 5; z++){
        png::image<png::gray_pixel> image(5, 5);

        for (unsigned int y = 0; y < 5; y++)
            for (unsigned int x = 0; x < 5; x++) {
                image[y][x] = static_cast<png::gray_pixel>(0);
            }

        stack.push_back(image);
    }

    for (Voxel v : voxels){
        if (v.getLevel() == Level::CORE)
            stack.at(v[2])[v[1]][v[0]] = static_cast<png::gray_pixel>(200);
        else if (v.getLevel() == Level::MAIN_SHAPE)
            stack.at(v[2])[v[1]][v[0]] = static_cast<png::gray_pixel>(150);
        else if (v.getLevel() == Level::NEIGHBORHOOD)
            stack.at(v[2])[v[1]][v[0]] = static_cast<png::gray_pixel>(100);
    }
}

template<class Voxel>
static void buildPseudoVoidExampleAsStack(
        std::vector<png::image<png::gray_pixel> >& stack)
{
    for (unsigned int z = 0; z < 56; z++){
        png::image<png::gray_pixel> image(36, 36);

        for (unsigned int y = 0; y < 36; y++)
            for (unsigned int x = 0; x < 36; x++) {
                image[y][x] = static_cast<png::gray_pixel>(0);
            }

        stack.push_back(image);
    }

    for (int x = 4; x < 32; x++){
        for (int y = 4; y < 32; y++){
            for (int z = 4; z < 52; z++){
                stack.at(z)[y][x] = static_cast<png::gray_pixel>(100);
            }
        }
    }

    for (int x = 16; x < 20; x++){
        for (int y = 16; y < 20; y++){
            for (int z = 16; z < 40; z++){
                stack.at(z)[y][x] = static_cast<png::gray_pixel>(0);
            }
        }
    }

    for (int x = 8; x < 28; x++){
        for (int y = 8; y < 28; y++){
            for (int z = 8; z < 48; z++){
                if (!(x >= 12 && x < 24 && y < 12 && z >= 24 && z < 32) &&
                        !(x >= 12 && x < 24 && y >= 12
                          && y < 24 && z >= 12 && z < 44))
                    stack.at(z)[y][x] = static_cast<png::gray_pixel>(150);
            }
        }
    }
}

inline void buildExampleForCells(Cell<0> (&vertices)[12],
                          Cell<1> (&edges)[20],
                          Cell<2> (&squares)[11],
                          Cell<3> (&cubes)[2])
{
    std::vector<Point> points;
    std::vector<Cell<0>::vertex> coord;

    points.push_back({0,0,0});
    vertices[0] = Cell<0>(points, 0);   // (0,0,0)
    points.at(0)[0] = 1;
    vertices[1] = Cell<0>(points, 0);   // (1,0,0)
    points.at(0)[0] = 2;
    vertices[2] = Cell<0>(points, 0);   // (2,0,0)
    points.at(0)[1] = 1;
    vertices[3] = Cell<0>(points, 0);   // (2,1,0)
    points.at(0)[0] = 1;
    vertices[4] = Cell<0>(points, 0);   // (1,1,0)
    points.at(0)[0] = 0;
    vertices[5] = Cell<0>(points, 0);   // (0,1,0)
    points.at(0)[2] = 1;
    vertices[6] = Cell<0>(points, 0);   // (0,1,1)
    points.at(0)[0] = 1;
    vertices[7] = Cell<0>(points, 0);   // (1,1,1)
    points.at(0)[0] = 2;
    vertices[8] = Cell<0>(points, 0);   // (2,1,1)
    points.at(0)[1] = 0;
    vertices[9] = Cell<0>(points, 0);   // (2,0,1)
    points.at(0)[0] = 1;
    vertices[10] = Cell<0>(points, 0);  // (1,0,1)
    points.at(0)[0] = 0;
    vertices[11] = Cell<0>(points, 0);  // (0,0,1)

    coord.push_back(vertices[0][0]);
    coord.push_back(vertices[1][0]);
    edges[0] = Cell<1>(coord, 1);   // (v1,v2)
    coord.at(1) = vertices[5][0];
    edges[1] = Cell<1>(coord, 1);   // (v1,v6)
    coord.at(1) = vertices[11][0];
    edges[2] = Cell<1>(coord, 1);   // (v1,v12)
    coord.at(0) = vertices[10][0];
    edges[3] = Cell<1>(coord, 1);   // (v11,v12)
    coord.at(0) = vertices[6][0];
    edges[4] = Cell<1>(coord, 1);   // (v7,v12)
    coord.at(1) = vertices[5][0];
    edges[5] = Cell<1>(coord, 1);   // (v6,v7)
    coord.at(1) = vertices[7][0];
    edges[6] = Cell<1>(coord, 1);   // (v7,v8)
    coord.at(0) = vertices[10][0];
    edges[7] = Cell<1>(coord, 1);   // (v8,v11)
    coord.at(0) = vertices[4][0];
    edges[8] = Cell<1>(coord, 1);   // (v5,v8)
    coord.at(0) = vertices[8][0];
    edges[9] = Cell<1>(coord, 1);   // (v8,v9)
    coord.at(1) = vertices[9][0];
    edges[10] = Cell<1>(coord, 1);  // (v9,v10)
    coord.at(1) = vertices[3][0];
    edges[11] = Cell<1>(coord, 1);  // (v4,v9)
    coord.at(0) = vertices[4][0];
    edges[12] = Cell<1>(coord, 1);  // (v4,v5)
    coord.at(0) = vertices[2][0];
    edges[13] = Cell<1>(coord, 1);  // (v3,v4)
    coord.at(1) = vertices[9][0];
    edges[14] = Cell<1>(coord, 1);  // (v3,v10)
    coord.at(1) = vertices[1][0];
    edges[15] = Cell<1>(coord, 1);  // (v2,v3)
    coord.at(0) = vertices[10][0];
    edges[16] = Cell<1>(coord, 1);  // (v2,v11)
    coord.at(0) = vertices[4][0];
    edges[17] = Cell<1>(coord, 1);  // (v2,v5)
    coord.at(1) = vertices[5][0];
    edges[18] = Cell<1>(coord, 1);  // (v5,v6)
    coord.at(0) = vertices[9][0];
    coord.at(1) = vertices[10][0];
    edges[19] = Cell<1>(coord, 1);  // (v10,v11)

    coord.push_back(vertices[2][0]);
    coord.push_back(vertices[1][0]);
    squares[0] = Cell<2>(coord, 2); // (v2,v3,v10,v11)
    coord.at(0) = vertices[3][0];
    coord.at(1) = vertices[4][0];
    squares[1] = Cell<2>(coord, 2); // (v2,v3,v4,v5)
    coord.at(2) = vertices[7][0];
    coord.at(3) = vertices[8][0];
    squares[2] = Cell<2>(coord, 2); // (v4,v5,v8,v9)
    coord.at(0) = vertices[9][0];
    coord.at(1) = vertices[10][0];
    squares[3] = Cell<2>(coord, 2); // (v8,v9,v10,v11)
    coord.at(1) = vertices[2][0];
    coord.at(2) = vertices[3][0];
    squares[4] = Cell<2>(coord, 2); // (v3,v4,v9,v10)
    coord.at(0) = vertices[1][0];
    coord.at(1) = vertices[4][0];
    coord.at(2) = vertices[7][0];
    coord.at(3) = vertices[10][0];
    squares[5] = Cell<2>(coord, 2); // (v2,v5,v8,v11)
    coord.at(0) = vertices[6][0];
    coord.at(1) = vertices[11][0];
    squares[6] = Cell<2>(coord, 2); // (v7,v8,v11,v12)
    coord.at(1) = vertices[4][0];
    coord.at(3) = vertices[5][0];
    squares[7] = Cell<2>(coord, 2); // (v5,v6,v7,v8)
    coord.at(0) = vertices[0][0];
    coord.at(2) = vertices[1][0];
    squares[8] = Cell<2>(coord, 2); // (v1,v2,v5,v6)
    coord.at(1) = vertices[10][0];
    coord.at(3) = vertices[11][0];
    squares[9] = Cell<2>(coord, 2); // (v1,v2,v11,v12)
    coord.at(1) = vertices[5][0];
    coord.at(2) = vertices[6][0];
    squares[10] = Cell<2>(coord, 2);// (v1,v6,v7,v12)

    coord.push_back(vertices[1][0]);
    coord.push_back(vertices[4][0]);
    coord.push_back(vertices[7][0]);
    coord.push_back(vertices[10][0]);
    cubes[0] = Cell<3>(coord, 3);    // (s6,s7,s8,s9,s10,s11)
    coord.at(0) = vertices[2][0];
    coord.at(1) = vertices[3][0];
    coord.at(2) = vertices[8][0];
    coord.at(3) = vertices[9][0];
    cubes[1] = Cell<3>(coord, 3);    // (s1,s2,s3,s4,s5,s6)
}

template<unsigned int dim, unsigned int index>
inline void buildCofaceRelationForCellExample(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
}

template<>
inline void buildCofaceRelationForCellExample<0,0>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{0});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{0, 1, 2});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{8, 9, 10});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<0,1>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{1});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{0, 15, 16, 17});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{0, 1, 5, 8, 9});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0, 1});
}

template<>
inline void buildCofaceRelationForCellExample<0,2>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{2});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{13, 14, 15});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{0, 1, 4});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<0,3>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{3});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{11, 12, 13});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{1, 2, 4});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<0,4>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{4});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{8, 12, 17, 18});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{1, 2, 5, 7, 8});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0, 1});
}

template<>
inline void buildCofaceRelationForCellExample<0,5>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{5});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{1, 5, 18});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{7, 8, 10});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<0,6>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{6});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{4, 5, 6});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{6, 7, 10});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<0,7>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{7});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{6, 7, 8, 9});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{2, 3, 5, 6, 7});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0, 1});
}

template<>
inline void buildCofaceRelationForCellExample<0,8>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{8});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{9, 10, 11});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{2, 3, 4});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<0,9>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{9});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{10, 14, 19});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{0, 3, 4});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<0,10>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{10});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{3, 7, 16, 19});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{0, 3, 5, 6, 9});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0, 1});
}

template<>
inline void buildCofaceRelationForCellExample<0,11>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{11});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{2, 3, 4});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{6, 9, 10});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<1,0>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{0});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{8, 9});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<1,1>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{1});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{8, 10});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<1,2>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{2});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{9, 10});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<1,3>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{3});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{6, 9});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<1,4>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{4});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{6, 10});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<1,5>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{5});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{7, 10});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<1,6>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{6});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{6, 7});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<1,7>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{7});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{3, 5, 6});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0, 1});
}

template<>
inline void buildCofaceRelationForCellExample<1,8>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{8});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{2, 5, 7});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0, 1});
}

template<>
inline void buildCofaceRelationForCellExample<1,9>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{9});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{2, 3});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<1,10>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{10});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{3, 4});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<1,11>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{11});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{2, 4});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<1,12>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{12});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{1, 2});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<1,13>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{13});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{1, 4});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<1,14>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{14});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{0, 4});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<1,15>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{15});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{0, 1});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<1,16>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{16});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{0, 5, 9});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0, 1});
}

template<>
inline void buildCofaceRelationForCellExample<1,17>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{17});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{1, 5, 8});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0, 1});
}

template<>
inline void buildCofaceRelationForCellExample<1,18>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{18});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{7, 8});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<1,19>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    edgesTrueAt.insert(std::initializer_list<unsigned int>{19});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{0, 3});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<2,0>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    squaresTrueAt.insert(std::initializer_list<unsigned int>{0});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<2,1>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    squaresTrueAt.insert(std::initializer_list<unsigned int>{1});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<2,2>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    squaresTrueAt.insert(std::initializer_list<unsigned int>{2});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<2,3>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    squaresTrueAt.insert(std::initializer_list<unsigned int>{3});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<2,4>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    squaresTrueAt.insert(std::initializer_list<unsigned int>{4});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildCofaceRelationForCellExample<2,5>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    squaresTrueAt.insert(std::initializer_list<unsigned int>{5});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0, 1});
}

template<>
inline void buildCofaceRelationForCellExample<2,6>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    squaresTrueAt.insert(std::initializer_list<unsigned int>{6});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<2,7>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    squaresTrueAt.insert(std::initializer_list<unsigned int>{7});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<2,8>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    squaresTrueAt.insert(std::initializer_list<unsigned int>{8});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<2,9>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    squaresTrueAt.insert(std::initializer_list<unsigned int>{9});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<2,10>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    squaresTrueAt.insert(std::initializer_list<unsigned int>{10});
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<3,0>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    cubesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildCofaceRelationForCellExample<3,1>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    cubesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<unsigned int dim, unsigned int index>
inline void buildFaceRelationForCellExample(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
}

template<>
inline void buildFaceRelationForCellExample<0,0>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildFaceRelationForCellExample<0,1>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildFaceRelationForCellExample<0,2>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{2});
}

template<>
inline void buildFaceRelationForCellExample<0,3>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{3});
}

template<>
inline void buildFaceRelationForCellExample<0,4>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{4});
}

template<>
inline void buildFaceRelationForCellExample<0,5>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{5});
}

template<>
inline void buildFaceRelationForCellExample<0,6>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{6});
}

template<>
inline void buildFaceRelationForCellExample<0,7>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{7});
}

template<>
inline void buildFaceRelationForCellExample<0,8>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{8});
}

template<>
inline void buildFaceRelationForCellExample<0,9>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{9});
}

template<>
inline void buildFaceRelationForCellExample<0,10>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{10});
}

template<>
inline void buildFaceRelationForCellExample<0,11>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{11});
}

template<>
inline void buildFaceRelationForCellExample<1,0>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{0, 1});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildFaceRelationForCellExample<1,1>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{0, 5});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildFaceRelationForCellExample<1,2>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{0, 11});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{2});
}

template<>
inline void buildFaceRelationForCellExample<1,3>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{10, 11});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{3});
}

template<>
inline void buildFaceRelationForCellExample<1,4>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{6, 11});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{4});
}

template<>
inline void buildFaceRelationForCellExample<1,5>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{5, 6});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{5});
}

template<>
inline void buildFaceRelationForCellExample<1,6>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{6, 7});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{6});
}

template<>
inline void buildFaceRelationForCellExample<1,7>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{7, 10});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{7});
}

template<>
inline void buildFaceRelationForCellExample<1,8>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{4, 7});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{8});
}

template<>
inline void buildFaceRelationForCellExample<1,9>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{7, 8});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{9});
}

template<>
inline void buildFaceRelationForCellExample<1,10>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{8, 9});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{10});
}

template<>
inline void buildFaceRelationForCellExample<1,11>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{3, 8});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{11});
}

template<>
inline void buildFaceRelationForCellExample<1,12>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{3, 4});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{12});
}

template<>
inline void buildFaceRelationForCellExample<1,13>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{2, 3});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{13});
}

template<>
inline void buildFaceRelationForCellExample<1,14>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{2, 9});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{14});
}

template<>
inline void buildFaceRelationForCellExample<1,15>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{1, 2});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{15});
}

template<>
inline void buildFaceRelationForCellExample<1,16>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{1, 10});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{16});
}

template<>
inline void buildFaceRelationForCellExample<1,17>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{1, 4});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{17});
}

template<>
inline void buildFaceRelationForCellExample<1,18>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{4, 5});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{18});
}

template<>
inline void buildFaceRelationForCellExample<1,19>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{9, 10});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{19});
}

template<>
inline void buildFaceRelationForCellExample<2,0>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{1, 2, 9, 10});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{14, 15, 16, 19});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{0});
}

template<>
inline void buildFaceRelationForCellExample<2,1>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{1, 2, 3, 4});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{12, 13, 15, 17});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{1});
}

template<>
inline void buildFaceRelationForCellExample<2,2>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{3, 4, 7, 8});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{8, 9, 11, 12});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{2});
}

template<>
inline void buildFaceRelationForCellExample<2,3>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{7, 8, 9, 10});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{7, 9, 10, 19});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{3});
}

template<>
inline void buildFaceRelationForCellExample<2,4>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{2, 3, 8, 9});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{10, 11, 13, 14});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{4});
}

template<>
inline void buildFaceRelationForCellExample<2,5>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{1, 4, 7, 10});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{7, 8, 16, 17});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{5});
}

template<>
inline void buildFaceRelationForCellExample<2,6>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{6, 7, 10, 11});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{3, 4, 6, 7});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{6});
}

template<>
inline void buildFaceRelationForCellExample<2,7>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{4, 5, 6, 7});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{5, 6, 8, 18});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{7});
}

template<>
inline void buildFaceRelationForCellExample<2,8>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{0, 1, 4, 5});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{0, 1, 17, 18});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{8});
}

template<>
inline void buildFaceRelationForCellExample<2,9>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{0, 1, 10, 11});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{0, 2, 3, 16});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{9});
}

template<>
inline void buildFaceRelationForCellExample<2,10>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(std::initializer_list<unsigned int>{0, 5, 6, 11});
    edgesTrueAt.insert(std::initializer_list<unsigned int>{1, 2, 4, 5});
    squaresTrueAt.insert(std::initializer_list<unsigned int>{10});
}

template<>
inline void buildFaceRelationForCellExample<3,0>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(
                std::initializer_list<unsigned int>{
                    0, 1, 4, 5, 6, 7, 10, 11});
    edgesTrueAt.insert(
                std::initializer_list<unsigned int>{
                    0, 1, 2, 3, 4, 5, 6, 7, 8, 16, 17, 18});
    squaresTrueAt.insert(
                std::initializer_list<unsigned int>{
                    5, 6, 7, 8, 9, 10});
    cubesTrueAt.insert(
                std::initializer_list<unsigned int>{0});
}

template<>
inline void buildFaceRelationForCellExample<3,1>(
        std::unordered_set<unsigned int> &verticesTrueAt,
        std::unordered_set<unsigned int> &edgesTrueAt,
        std::unordered_set<unsigned int> &squaresTrueAt,
        std::unordered_set<unsigned int> &cubesTrueAt)
{
    verticesTrueAt.clear();
    edgesTrueAt.clear();
    squaresTrueAt.clear();
    cubesTrueAt.clear();
    verticesTrueAt.insert(
                std::initializer_list<unsigned int>{
                    1, 2, 3, 4, 7, 8, 9, 10});
    edgesTrueAt.insert(
                std::initializer_list<unsigned int>{
                    7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 19});
    squaresTrueAt.insert(
                std::initializer_list<unsigned int>{
                    0, 1, 2, 3, 4, 5});
    cubesTrueAt.insert(
                std::initializer_list<unsigned int>{1});
}

inline void buildExampleForCellContainer(CellContainer &cc)
{
    Cell<0> vertices[5];
    Cell<1> edges[5];
    Cell<2> squares[5];
    Cell<3> cubes[5];

    std::vector<Point> points;

    points.push_back({24,4,78});
    vertices[0] = Cell<0>(points, 0);
    points.at(0) = {14,57,89};
    vertices[3] = Cell<0>(points, 1);
    points.at(0) = {14,78,9};
    vertices[2] = Cell<0>(points, 1);
    points.at(0) = {58,7,8};
    vertices[1] = Cell<0>(points, 1);
    points.at(0) = {0,1,0};
    vertices[4] = Cell<0>(points, 2);

    points.at(0) = {14,57,89};
    points.push_back({24,4,78});
    edges[2] = Cell<1>(points, 0);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    edges[4] = Cell<1>(points, 0);
    points.at(0) = {0,1,0};
    points.at(1) = {24,4,78};
    edges[0] = Cell<1>(points, 0);
    points.at(0) = {14,57,89};
    points.at(1) = {58,7,8};
    edges[3] = Cell<1>(points, 1);
    points.at(0) = {58,7,8};
    points.at(1) = {14,52,89};
    edges[1] = Cell<1>(points, 2);

    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.push_back({24,4,78});
    points.push_back({75,87,4});
    squares[2] = Cell<2>(points, 0);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {24,6,78};
    points.at(3) = {75,87,4};
    squares[1] = Cell<2>(points, 1);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,2};
    points.at(2) = {24,4,78};
    points.at(3) = {75,87,4};
    squares[0] = Cell<2>(points, 2);
    points.at(0) = {14,78,60};
    points.at(1) = {0,1,0};
    points.at(2) = {86,7,78};
    points.at(3) = {40,87,4};
    squares[3] = Cell<2>(points, 2);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,78};
    points.at(3) = {75,87,4};
    squares[4] = Cell<2>(points, 2);

    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,78};
    points.at(3) = {75,87,4};
    points.push_back({24,4,78});
    points.push_back({24,14,4});
    points.push_back({96,35,5});
    points.push_back({12,4,33});
    cubes[1] = Cell<3>(points, 0);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {8,4,78};
    points.at(3) = {75,87,4};
    points.at(4) = {24,4,78};
    points.at(5) = {24,3,4};
    points.at(6) = {96,35,5};
    points.at(7) = {12,4,33};
    cubes[3] = Cell<3>(points, 1);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,7};
    points.at(3) = {75,87,4};
    points.at(4) = {24,4,78};
    points.at(5) = {11,14,4};
    points.at(6) = {96,35,5};
    points.at(7) = {12,4,33};
    cubes[0] = Cell<3>(points, 1);
    points.at(0) = {14,78,11};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,78};
    points.at(3) = {75,87,4};
    points.at(4) = {24,4,44};
    points.at(5) = {24,14,4};
    points.at(6) = {78,35,5};
    points.at(7) = {12,4,33};
    cubes[2] = Cell<3>(points, 2);
    points.at(0) = {14,78,9};
    points.at(1) = {0,3,0};
    points.at(2) = {77,4,78};
    points.at(3) = {84,87,4};
    points.at(4) = {24,4,78};
    points.at(5) = {24,14,3};
    points.at(6) = {96,11,5};
    points.at(7) = {14,4,33};
    cubes[4] = Cell<3>(points, 2);

    for (Cell<0> c : vertices) cc.insert(c);
    for (Cell<1> c : edges) cc.insert(c);
    for (Cell<2> c : squares) cc.insert(c);
    for (Cell<3> c : cubes) cc.insert(c);
}

inline void buildSortedLexicographicalyForCellContainerExample(
        Cell<0> (&verticesSorted)[5], Cell<1> (&edgesSorted)[5],
        Cell<2> (&squaresSorted)[5], Cell<3> (&cubesSorted)[5])
{
    std::vector<Point> points;

    points.push_back({0,1,0});
    verticesSorted[0] = Cell<0>(points, 0);
    points.at(0) = {58,7,8};
    verticesSorted[1] = Cell<0>(points, 1);
    points.at(0) = {14,78,9};
    verticesSorted[2] = Cell<0>(points, 1);
    points.at(0) = {24,4,78};
    verticesSorted[3] = Cell<0>(points, 1);
    points.at(0) = {14,57,89};
    verticesSorted[4] = Cell<0>(points, 2);

    points.at(0) = {14,78,9};
    points.push_back({0,1,0});
    edgesSorted[0] = Cell<1>(points, 0);
    points.at(0) = {0,1,0};
    points.at(1) = {24,4,78};
    edgesSorted[1] = Cell<1>(points, 0);
    points.at(0) = {58,7,8};
    points.at(1) = {14,52,89};
    edgesSorted[2] = Cell<1>(points, 0);
    points.at(0) = {14,57,89};
    points.at(1) = {58,7,8};
    edgesSorted[3] = Cell<1>(points, 1);
    points.at(0) = {14,57,89};
    points.at(1) = {24,4,78};
    edgesSorted[4] = Cell<1>(points, 2);

    points.at(0) = {14,78,60};
    points.at(1) = {0,1,0};
    points.push_back({86,7,78});
    points.push_back({40,87,4});
    squaresSorted[0] = Cell<2>(points, 0);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {24,4,78};
    points.at(3) = {75,87,4};
    squaresSorted[1] = Cell<2>(points, 1);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,78};
    points.at(3) = {75,87,4};
    squaresSorted[2] = Cell<2>(points, 2);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {24,6,78};
    points.at(3) = {75,87,4};
    squaresSorted[3] = Cell<2>(points, 2);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,2};
    points.at(2) = {24,4,78};
    points.at(3) = {75,87,4};
    squaresSorted[4] = Cell<2>(points, 2);

    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {8,4,78};
    points.at(3) = {75,87,4};
    points.push_back({24,4,78});
    points.push_back({24,3,4});
    points.push_back({96,35,5});
    points.push_back({12,4,33});
    cubesSorted[0] = Cell<3>(points, 0);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,7};
    points.at(3) = {75,87,4};
    points.at(4) = {24,4,78};
    points.at(5) = {11,14,4};
    points.at(6) = {96,35,5};
    points.at(7) = {12,4,33};
    cubesSorted[1] = Cell<3>(points, 1);
    points.at(0) = {14,78,11};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,78};
    points.at(3) = {75,87,4};
    points.at(4) = {24,4,44};
    points.at(5) = {24,14,4};
    points.at(6) = {78,35,5};
    points.at(7) = {12,4,33};
    cubesSorted[2] = Cell<3>(points, 1);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,78};
    points.at(3) = {75,87,4};
    points.at(4) = {24,4,78};
    points.at(5) = {24,14,4};
    points.at(6) = {96,35,5};
    points.at(7) = {12,4,33};
    cubesSorted[3] = Cell<3>(points, 2);
    points.at(0) = {14,78,9};
    points.at(1) = {0,3,0};
    points.at(2) = {77,4,78};
    points.at(3) = {84,87,4};
    points.at(4) = {24,4,78};
    points.at(5) = {24,14,3};
    points.at(6) = {96,11,5};
    points.at(7) = {14,4,33};
    cubesSorted[4] = Cell<3>(points, 2);
}

inline void buildSortedByTimestampForCellContainerExample(
        Cell<0> (&verticesSorted)[5], Cell<1> (&edgesSorted)[5],
        Cell<2> (&squaresSorted)[5], Cell<3> (&cubesSorted)[5])
{
    std::vector<Point> points;

    points.push_back({24,4,78});
    verticesSorted[0] = Cell<0>(points, 0);
    points.at(0) = {14,57,89};
    verticesSorted[3] = Cell<0>(points, 1);
    points.at(0) = {14,78,9};
    verticesSorted[2] = Cell<0>(points, 1);
    points.at(0) = {58,7,8};
    verticesSorted[1] = Cell<0>(points, 1);
    points.at(0) = {0,1,0};
    verticesSorted[4] = Cell<0>(points, 2);

    points.at(0) = {14,57,89};
    points.push_back({24,4,78});
    edgesSorted[1] = Cell<1>(points, 0);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    edgesSorted[2] = Cell<1>(points, 0);
    points.at(0) = {0,1,0};
    points.at(1) = {24,4,78};
    edgesSorted[0] = Cell<1>(points, 0);
    points.at(0) = {14,57,89};
    points.at(1) = {58,7,8};
    edgesSorted[3] = Cell<1>(points, 1);
    points.at(0) = {58,7,8};
    points.at(1) = {14,52,89};
    edgesSorted[4] = Cell<1>(points, 2);

    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.push_back({24,4,78});
    points.push_back({75,87,4});
    squaresSorted[0] = Cell<2>(points, 0);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {24,6,78};
    points.at(3) = {75,87,4};
    squaresSorted[1] = Cell<2>(points, 1);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,2};
    points.at(2) = {24,4,78};
    points.at(3) = {75,87,4};
    squaresSorted[2] = Cell<2>(points, 2);
    points.at(0) = {14,78,60};
    points.at(1) = {0,1,0};
    points.at(2) = {86,7,78};
    points.at(3) = {40,87,4};
    squaresSorted[3] = Cell<2>(points, 2);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,78};
    points.at(3) = {75,87,4};
    squaresSorted[4] = Cell<2>(points, 2);

    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,78};
    points.at(3) = {75,87,4};
    points.push_back({24,4,78});
    points.push_back({24,14,4});
    points.push_back({96,35,5});
    points.push_back({12,4,33});
    cubesSorted[0] = Cell<3>(points, 0);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {8,4,78};
    points.at(3) = {75,87,4};
    points.at(4) = {24,4,78};
    points.at(5) = {24,3,4};
    points.at(6) = {96,35,5};
    points.at(7) = {12,4,33};
    cubesSorted[2] = Cell<3>(points, 1);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,7};
    points.at(3) = {75,87,4};
    points.at(4) = {24,4,78};
    points.at(5) = {11,14,4};
    points.at(6) = {96,35,5};
    points.at(7) = {12,4,33};
    cubesSorted[1] = Cell<3>(points, 1);
    points.at(0) = {14,78,11};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,78};
    points.at(3) = {75,87,4};
    points.at(4) = {24,4,44};
    points.at(5) = {24,14,4};
    points.at(6) = {78,35,5};
    points.at(7) = {12,4,33};
    cubesSorted[3] = Cell<3>(points, 2);
    points.at(0) = {14,78,9};
    points.at(1) = {0,3,0};
    points.at(2) = {77,4,78};
    points.at(3) = {84,87,4};
    points.at(4) = {24,4,78};
    points.at(5) = {24,14,3};
    points.at(6) = {96,11,5};
    points.at(7) = {14,4,33};
    cubesSorted[4] = Cell<3>(points, 2);
}

inline void buildSortedForFiltrationForCellContainerExample(
        Cell<0> (&verticesSorted)[5], Cell<1> (&edgesSorted)[5],
        Cell<2> (&squaresSorted)[5], Cell<3> (&cubesSorted)[5])
{
    std::vector<Point> points;

    points.push_back({24,4,78});
    verticesSorted[0] = Cell<0>(points, 0);
    points.at(0) = {14,57,89};
    verticesSorted[3] = Cell<0>(points, 1);
    points.at(0) = {14,78,9};
    verticesSorted[2] = Cell<0>(points, 1);
    points.at(0) = {58,7,8};
    verticesSorted[1] = Cell<0>(points, 1);
    points.at(0) = {0,1,0};
    verticesSorted[4] = Cell<0>(points, 2);

    points.at(0) = {14,57,89};
    points.push_back({24,4,78});
    edgesSorted[2] = Cell<1>(points, 0);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    edgesSorted[0] = Cell<1>(points, 0);
    points.at(0) = {0,1,0};
    points.at(1) = {24,4,78};
    edgesSorted[1] = Cell<1>(points, 0);
    points.at(0) = {14,57,89};
    points.at(1) = {58,7,8};
    edgesSorted[3] = Cell<1>(points, 1);
    points.at(0) = {58,7,8};
    points.at(1) = {14,52,89};
    edgesSorted[4] = Cell<1>(points, 2);

    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.push_back({24,4,78});
    points.push_back({75,87,4});
    squaresSorted[0] = Cell<2>(points, 0);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {24,6,78};
    points.at(3) = {75,87,4};
    squaresSorted[1] = Cell<2>(points, 1);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,2};
    points.at(2) = {24,4,78};
    points.at(3) = {75,87,4};
    squaresSorted[4] = Cell<2>(points, 2);
    points.at(0) = {14,78,60};
    points.at(1) = {0,1,0};
    points.at(2) = {86,7,78};
    points.at(3) = {40,87,4};
    squaresSorted[2] = Cell<2>(points, 2);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,78};
    points.at(3) = {75,87,4};
    squaresSorted[3] = Cell<2>(points, 2);

    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,78};
    points.at(3) = {75,87,4};
    points.push_back({24,4,78});
    points.push_back({24,14,4});
    points.push_back({96,35,5});
    points.push_back({12,4,33});
    cubesSorted[0] = Cell<3>(points, 0);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {8,4,78};
    points.at(3) = {75,87,4};
    points.at(4) = {24,4,78};
    points.at(5) = {24,3,4};
    points.at(6) = {96,35,5};
    points.at(7) = {12,4,33};
    cubesSorted[1] = Cell<3>(points, 1);
    points.at(0) = {14,78,9};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,7};
    points.at(3) = {75,87,4};
    points.at(4) = {24,4,78};
    points.at(5) = {11,14,4};
    points.at(6) = {96,35,5};
    points.at(7) = {12,4,33};
    cubesSorted[2] = Cell<3>(points, 1);
    points.at(0) = {14,78,11};
    points.at(1) = {0,1,0};
    points.at(2) = {77,4,78};
    points.at(3) = {75,87,4};
    points.at(4) = {24,4,44};
    points.at(5) = {24,14,4};
    points.at(6) = {78,35,5};
    points.at(7) = {12,4,33};
    cubesSorted[3] = Cell<3>(points, 2);
    points.at(0) = {14,78,9};
    points.at(1) = {0,3,0};
    points.at(2) = {77,4,78};
    points.at(3) = {84,87,4};
    points.at(4) = {24,4,78};
    points.at(5) = {24,14,3};
    points.at(6) = {96,11,5};
    points.at(7) = {14,4,33};
    cubesSorted[4] = Cell<3>(points, 2);
}

