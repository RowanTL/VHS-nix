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

#pragma once

/**
 * @file test_examples.h
 * Contains example builders for the unit tests.
 **/

#include <vector>
#include <unordered_map>
#include <random>

// Euler Characteristic: 1
// Number of components: 2
// Number of loops: 1
// Number of voids: 0
template<class Shape, class Voxel>
static void buildExample1(
        std::vector<Voxel>& voxels, std::vector<typename Shape::Level>& minLevels) {
    Voxel v0(0, 0, 1);  // N
    Voxel v1(0, 0, 2);  // S
    Voxel v2(0, 0, 3);  // S
    Voxel v3(0, 1, 2);  // C
    Voxel v4(0, 1, 3);  // S
    Voxel v5(1, 0, 2);  // S
    Voxel v6(1, 0, 3);  // N
    Voxel v7(1, 1, 2);  // S
    Voxel v8(1, 1, 3);  // C
    Voxel v9(0, 2, 1);  // S
    Voxel v10(0, 2, 2); // C
    Voxel v11(0, 3, 1); // N
    Voxel v12(0, 3, 2); // S
    Voxel v13(1, 4, 3); // C
    Voxel v14(0, 4, 2); // S
    Voxel v15(0, 4, 1); // S
    Voxel v16(0, 2, 0); // S
    Voxel v17(0, 3, 0); // C
    Voxel v18(0, 4, 0); // S
    Voxel v19(1, 4, 4); // N
    Voxel v20(0, 4, 3); // N

    voxels.push_back(v0);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v1);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v2);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v3);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v4);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v5);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v6);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v7);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v8);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v9);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v10);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v11);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v12);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v13);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v14);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v15);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v16);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v17);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v18);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v19);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v20);
    minLevels.push_back(Shape::NEIGHBORHOOD);
}

// Euler Characteristic: 2
template<class Shape, class Voxel>
static void buildExample1Small(
        std::vector<Voxel>& voxels, std::vector<typename Shape::Level>& minLevels) {
    Voxel v0(0, 0, 1);  // N
    Voxel v1(0, 0, 2);  // S
    Voxel v2(0, 0, 3);  // S
    Voxel v3(0, 1, 2);  // C
    Voxel v4(0, 1, 3);  // S
    Voxel v5(1, 0, 2);  // S
    Voxel v6(1, 0, 3);  // N
    Voxel v7(1, 1, 2);  // S
    Voxel v8(1, 1, 3);  // C
    Voxel v9(0, 2, 1);  // S
    Voxel v10(0, 2, 2); // C
    Voxel v11(0, 3, 1); // N
    Voxel v12(0, 3, 2); // S
    Voxel v13(1, 4, 3); // C

    voxels.push_back(v0);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v1);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v2);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v3);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v4);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v5);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v6);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v7);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v8);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v9);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v10);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v11);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v12);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v13);
    minLevels.push_back(Shape::CORE);
}

template<class Shape, class Voxel>
static void buildExample1SmallTranslated(
        std::vector<Voxel>& voxels, std::vector<typename Shape::Level>& minLevels) {
    Voxel v0(0, 0, 0);  // N
    Voxel v1(0, 0, 1);  // S
    Voxel v2(0, 0, 2);  // S
    Voxel v3(0, 1, 1);  // C
    Voxel v4(0, 1, 2);  // S
    Voxel v5(1, 0, 1);  // S
    Voxel v6(1, 0, 2);  // N
    Voxel v7(1, 1, 1);  // C
    Voxel v8(1, 1, 2);  // S
    Voxel v9(0, 2, 0);  // S
    Voxel v10(0, 2, 1); // C
    Voxel v11(0, 3, 0); // N
    Voxel v12(0, 3, 1); // S
    Voxel v13(1, 4, 2); // N

    voxels.push_back(v0);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v1);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v2);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v3);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v4);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v5);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v6);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v7);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v8);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v9);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v10);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v11);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v12);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v13);
    minLevels.push_back(Shape::NEIGHBORHOOD);
}

// Euler Characteristic: 1
// Number of components: 2
// Number of loops: 1
// Number of voids: 0
template<class Shape, class Voxel>
static void buildExample1Translated(
        std::vector<Voxel>& voxels, std::vector<typename Shape::Level>& minLevels) {
    Voxel v0(1, 1, 2);  // N
    Voxel v1(1, 1, 3);  // S
    Voxel v2(1, 1, 4);  // S
    Voxel v3(1, 2, 3);  // C
    Voxel v4(1, 2, 4);  // S
    Voxel v5(2, 1, 3);  // S
    Voxel v6(2, 1, 4);  // N
    Voxel v7(2, 2, 3);  // S
    Voxel v8(2, 2, 4);  // C
    Voxel v9(1, 3, 2);  // S
    Voxel v10(1, 3, 3); // C
    Voxel v11(1, 4, 2); // N
    Voxel v12(1, 4, 3); // S
    Voxel v13(2, 5, 4); // C
    Voxel v14(1, 5, 3); // S
    Voxel v15(1, 5, 2); // S
    Voxel v16(1, 3, 1); // S
    Voxel v17(1, 4, 1); // C
    Voxel v18(1, 5, 1); // S
    Voxel v19(2, 5, 5); // N
    Voxel v20(1, 5, 4); // N

    voxels.push_back(v0);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v1);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v2);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v3);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v4);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v5);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v6);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v7);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v8);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v9);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v10);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v11);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v12);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v13);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v14);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v15);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v16);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v17);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v18);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v19);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v20);
    minLevels.push_back(Shape::NEIGHBORHOOD);
}

// Number of components: 2
// Number of loops: 1
// Number of voids: 1
template<class Shape, class Voxel>
static void buildExample1Extended(
        std::vector<Voxel>& voxels, std::vector<typename Shape::Level>& minLevels) {
    Voxel v0(1, 1, 2);  // N
    Voxel v1(1, 1, 3);  // S
    Voxel v2(1, 1, 4);  // S
    Voxel v3(1, 2, 3);  // C
    Voxel v4(1, 2, 4);  // S
    Voxel v5(2, 1, 3);  // S
    Voxel v6(2, 1, 4);  // N
    Voxel v7(2, 2, 3);  // S
    Voxel v8(2, 2, 4);  // C
    Voxel v9(1, 3, 2);  // S
    Voxel v10(1, 3, 3); // C
    Voxel v11(1, 4, 2); // N
    Voxel v12(1, 4, 3); // S
    Voxel v13(2, 5, 4); // C
    Voxel v14(1, 5, 3); // S
    Voxel v15(1, 5, 2); // S
    Voxel v16(1, 3, 1); // S
    Voxel v17(1, 4, 1); // S
    Voxel v18(1, 5, 1); // S
    Voxel v19(2, 5, 5); // N
    Voxel v20(1, 5, 4); // N

    voxels.push_back(v0);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v1);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v2);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v3);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v4);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v5);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v6);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v7);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v8);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v9);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v10);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v11);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v12);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v13);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v14);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v15);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v16);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v17);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v18);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v19);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v20);
    minLevels.push_back(Shape::NEIGHBORHOOD);

    for (int x = 10; x < 14; x++) {
        for (int y = 10; y < 14; y++) {
            for (int z = 10; z < 14; z++) {
                voxels.push_back(Voxel(x, y, z));
                if (x > 10 && x < 13 && y > 10 && y < 13 && z > 10 && z < 13)
                    minLevels.push_back(Shape::NEIGHBORHOOD);
                else
                    minLevels.push_back(Shape::MAIN_SHAPE);
            }
        }
    }
}

// Euler Characteristic: 3
// Number of components: 2
// Number of loops: 1
// Number of voids: 2
template<class Shape, class Voxel>
static void buildExample2(
        std::vector<Voxel>& voxels, std::vector<typename Shape::Level>& minLevels) {
    for (int x = 0; x < 2; x++){
        for (int y = 3; y < 7; y++){
            for (int z = 8; z < 12; z++){
                Voxel v(x, y, z);
                voxels.push_back(v);
                if (x == 1 && (y == 4 || y == 5) && (z == 9 || z == 10))
                    minLevels.push_back(Shape::MAIN_SHAPE);
                else
                    minLevels.push_back(Shape::NEIGHBORHOOD);
            }
        }
    }
    for (int y = 3; y < 7; y++){
        for (int z = 8; z < 11; z++){
            Voxel v(2, y, z);
            voxels.push_back(v);
            if ((y == 4 || y == 5) && (z == 9 || z == 10))
                minLevels.push_back(Shape::MAIN_SHAPE);
            else
                minLevels.push_back(Shape::NEIGHBORHOOD);
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
                        minLevels.push_back(Shape::CORE);
                    } else {
                        minLevels.push_back(Shape::MAIN_SHAPE);
                    }
                } else {
                    minLevels.push_back(Shape::NEIGHBORHOOD);
                }
            }
        }
    }
}

template<class Shape, class Voxel>
void buildMiniExample1(
        std::vector<Voxel>& voxels, std::vector<typename Shape::Level>& minLevels) {
    Voxel v0(0, 0, 0);  //C
    Voxel v1(0, 1, 0);  //S
    Voxel v2(1, 1, 1);  //C
    Voxel v3(1, 0, 2);  //N

    voxels.push_back(v0);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v1);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v2);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v3);
    minLevels.push_back(Shape::NEIGHBORHOOD);
}

template<class Shape, class Voxel>
void buildMiniExample2(
        std::vector<Voxel>& voxels, std::vector<typename Shape::Level>& minLevels) {
    Voxel v0(0, 0, 0);  //C
    Voxel v1(0, 1, 0);  //N
    Voxel v2(2, 0, 3);  //S
    Voxel v3(0, 2, 1);  //C

    voxels.push_back(v0);
    minLevels.push_back(Shape::CORE);
    voxels.push_back(v1);
    minLevels.push_back(Shape::NEIGHBORHOOD);
    voxels.push_back(v2);
    minLevels.push_back(Shape::MAIN_SHAPE);
    voxels.push_back(v3);
    minLevels.push_back(Shape::CORE);
}

template<class Shape, class Voxel>
void buildRandomExample(
        unsigned int nberOfVoxels,
        std::vector<Voxel> &voxels,
        std::vector<typename Shape::Level> &minLevels,
        std::unordered_map<Voxel, typename Shape::Level,
        std::hash<Voxel>> &realMinLevel) {
    std::mt19937 randomGenerator(0); // Repeatable seed
    std::uniform_int_distribution<int> unicoord(0, 100);
    std::uniform_int_distribution<int> unilevel(0, 3);

    for (unsigned int i = 0; i < nberOfVoxels; ++i) {
        voxels.push_back(Voxel(unicoord(randomGenerator), unicoord(randomGenerator),
                               unicoord(randomGenerator)));
        minLevels.push_back(static_cast<typename Shape::Level>(unilevel(randomGenerator)));
        if (realMinLevel.find(voxels.back()) == realMinLevel.end())
            realMinLevel.emplace(voxels.back(), minLevels.back());
        else
            realMinLevel.at(voxels.back()) =
                    std::min(realMinLevel.at(voxels.back()), minLevels.back());
    }
}

