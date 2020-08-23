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
 * @file Connected_Components_test.cpp
 * Test file for the Connected Component class.
 **/

#include <algorithm>
#include <gtest/gtest.h>
#include <random>
#include <unordered_set>

#include "shape/ConnectedComponents.h"
#include "shape/Voxel.h"

using shape_simplification::shape::ConnectedComponents;
using shape_simplification::shape::ConnectedComponentsWithRemoval;
using shape_simplification::shape::Voxel;

template<class CC> void verifyCCInitialization(std::string name);
template<class CC> void verifyCCMerge(std::string name);
template<class CC> void verifyCCRemoval(std::string name);
template<class CC> void verifyCCChildren(std::string name);
template<class CC> void verifyCCGetComponents(std::string name);

TEST(ConnectedComponentsTest, Initialization) {
    verifyCCInitialization<ConnectedComponents<Voxel> >("ConnectedComponents");
    verifyCCInitialization<ConnectedComponentsWithRemoval<Voxel> >("ConnectedComponentsWithRemoval");
}

template <class CC>
void verifyCCInitialization(std::string name){
    CC cc;

    Voxel v0(0, 0, 0);
    Voxel v1(0, 1, 0);
    Voxel v2(1, 1, 1);
    Voxel v3(1, 0, 2);

    ASSERT_EQ(0, cc.size()) << name;

    cc.initialize(v0);
    ASSERT_EQ(1, cc.size()) << name;
    ASSERT_EQ(v0, cc.find(v0)) << name;

    cc.initialize(v1);
    ASSERT_EQ(2, cc.size()) << name;
    ASSERT_EQ(v1, cc.find(v1)) << name;

    cc.initialize(v0);
    ASSERT_EQ(2, cc.size()) << name;

    cc.initialize(v2);
    ASSERT_EQ(3, cc.size()) << name;
    ASSERT_EQ(v2, cc.find(v2)) << name;

    cc.initialize(v3);
    ASSERT_EQ(4, cc.size()) << name;
    ASSERT_EQ(v3, cc.find(v3)) << name;

    cc.initialize(v1);
    ASSERT_EQ(4, cc.size()) << name;

    ASSERT_EQ(v0, cc.find(v0)) << name;
    ASSERT_EQ(v1, cc.find(v1)) << name;
    ASSERT_EQ(v2, cc.find(v2)) << name;
    ASSERT_EQ(v3, cc.find(v3)) << name;

    ASSERT_TRUE(cc.contains(v0)) << name;
    ASSERT_TRUE(cc.contains(v1)) << name;
    ASSERT_TRUE(cc.contains(v2)) << name;
    ASSERT_TRUE(cc.contains(v3)) << name;
    ASSERT_FALSE(cc.contains(Voxel(0,1,3))) << name;

    CC ccc(cc);

    ASSERT_EQ(4, ccc.size()) << name;

    ASSERT_EQ(v0, ccc.find(v0)) << name;
    ASSERT_EQ(v1, ccc.find(v1)) << name;
    ASSERT_EQ(v2, ccc.find(v2)) << name;
    ASSERT_EQ(v3, ccc.find(v3)) << name;

    ASSERT_TRUE(ccc.contains(v0)) << name;
    ASSERT_TRUE(ccc.contains(v1)) << name;
    ASSERT_TRUE(ccc.contains(v2)) << name;
    ASSERT_TRUE(ccc.contains(v3)) << name;
    ASSERT_FALSE(ccc.contains(Voxel(0,1,3))) << name;
}

TEST(ConnectedComponentsTest, Merge) {
    verifyCCMerge<ConnectedComponents<Voxel> >("ConnectedComponents");
    verifyCCMerge<ConnectedComponentsWithRemoval<Voxel> >("ConnectedComponentsWithRemoval");
}

template <class CC>
void verifyCCMerge(std::string name){
    CC cc;

    Voxel v0(0, 0, 0);
    Voxel v1(0, 1, 0);
    Voxel v2(1, 1, 1);
    Voxel v3(1, 0, 2);
    Voxel v4(1, 2, 1);

    ASSERT_EQ(0, cc.size()) << name;

    cc.initialize(v0);
    ASSERT_EQ(1, cc.size()) << name;
    ASSERT_EQ(v0, cc.find(v0)) << name;

    cc.initialize(v1);
    ASSERT_EQ(2, cc.size()) << name;
    ASSERT_EQ(v1, cc.find(v1)) << name;

    cc.merge(v0, v1);
    ASSERT_EQ(1, cc.size()) << name;
    ASSERT_TRUE(cc.find(v0) == v0 || cc.find(v0) == v1) << name;
    ASSERT_TRUE(cc.find(v1) == v0 || cc.find(v1) == v1) << name;
    ASSERT_TRUE(cc.find(v0) == cc.find(v1)) << name;

    cc.initialize(v2);
    ASSERT_EQ(2, cc.size()) << name;
    ASSERT_EQ(v2, cc.find(v2)) << name;

    cc.initialize(v3);
    ASSERT_EQ(3, cc.size()) << name;
    ASSERT_EQ(v3, cc.find(v3)) << name;

    CC ccc(cc);

    cc.initialize(v4);
    ASSERT_EQ(4, cc.size()) << name;
    ASSERT_EQ(v4, cc.find(v4)) << name;

    cc.merge(v2, v4);
    ASSERT_EQ(3, cc.size());
    ASSERT_TRUE(cc.find(v2) == v2 || cc.find(v2) == v4) << name;
    ASSERT_TRUE(cc.find(v4) == v2 || cc.find(v4) == v4) << name;
    ASSERT_TRUE(cc.find(v2) == cc.find(v4)) << name;

    ASSERT_TRUE(cc.find(v0) == cc.find(v1)) << name;
    ASSERT_EQ(v3, cc.find(v3)) << name;

    ASSERT_TRUE(cc.contains(v0)) << name;
    ASSERT_TRUE(cc.contains(v1)) << name;
    ASSERT_TRUE(cc.contains(v2)) << name;
    ASSERT_TRUE(cc.contains(v3)) << name;
    ASSERT_TRUE(cc.contains(v4)) << name;
    ASSERT_FALSE(cc.contains(Voxel(0,1,3))) << name;

    ASSERT_EQ(3, ccc.size()) << name;
    ASSERT_EQ(v3, ccc.find(v3)) << name;

    ccc.initialize(v4);
    ASSERT_EQ(4, ccc.size()) << name;
    ASSERT_EQ(v4, ccc.find(v4)) << name;

    ccc.merge(v2, v4);

    ASSERT_EQ(3, ccc.size());
    ASSERT_TRUE(ccc.find(v2) == v2 || ccc.find(v2) == v4) << name;
    ASSERT_TRUE(ccc.find(v4) == v2 || ccc.find(v4) == v4) << name;
    ASSERT_TRUE(ccc.find(v2) == ccc.find(v4)) << name;

    ASSERT_TRUE(ccc.find(v0) == ccc.find(v1)) << name;
    ASSERT_EQ(v3, ccc.find(v3)) << name;

    ASSERT_TRUE(ccc.contains(v0)) << name;
    ASSERT_TRUE(ccc.contains(v1)) << name;
    ASSERT_TRUE(ccc.contains(v2)) << name;
    ASSERT_TRUE(ccc.contains(v3)) << name;
    ASSERT_TRUE(ccc.contains(v4)) << name;
    ASSERT_FALSE(ccc.contains(Voxel(0,1,3))) << name;
}

TEST(ConnectedComponentsTest, Removal) {
    verifyCCRemoval<ConnectedComponentsWithRemoval<Voxel> >("ConnectedComponentsWithRemoval");
}

template <class CC>
void verifyCCRemoval(std::string name){
    CC cc;

    Voxel v0(0, 0, 0);
    Voxel v1(0, 1, 0);
    Voxel v2(0, 0, 1);
    Voxel v3(0, 1, 1);
    Voxel v4(0, 1, 2);
    Voxel v5(0, 3, 1);
    Voxel v6(0, 2, 4);
    Voxel v7(0, 3, 4);

    cc.initialize(v0);
    cc.initialize(v1);
    cc.initialize(v2);
    cc.initialize(v3);
    cc.initialize(v4);
    cc.merge(v0, v1);
    cc.merge(v2, v3);
    cc.merge(v2, v4);
    cc.merge(v0, v2);

    cc.initialize(v5);

    cc.initialize(v6);
    cc.initialize(v7);
    cc.merge(v6, v7);

    CC ccc(cc);

    Voxel p1 = cc.find(v0);
    Voxel p2 = cc.find(v6);

    ASSERT_TRUE(p1 == v0 || p1 == v1 || p1 == v2 || p1 == v3 || p1 == v4) << name;
    ASSERT_TRUE(p2 == v6 || p2 == v7) << name;

    std::vector<Voxel> children1;
    std::vector<Voxel> children2;
    std::vector<Voxel> children3;
    std::vector<Voxel> children4;
    cc.getUnion(v0, children1);
    cc.getUnion(v6, children2);
    cc.getUnion(v5, children3);

    ASSERT_EQ(5, children1.size()) << name;
    ASSERT_EQ(2, children2.size()) << name;
    ASSERT_EQ(1, children3.size()) << name;

    ASSERT_TRUE(cc.contains(v0)) << name;
    ASSERT_TRUE(cc.contains(v1)) << name;
    ASSERT_TRUE(cc.contains(v2)) << name;
    ASSERT_TRUE(cc.contains(v3)) << name;
    ASSERT_TRUE(cc.contains(v4)) << name;
    ASSERT_TRUE(cc.contains(v5)) << name;
    ASSERT_TRUE(cc.contains(v6)) << name;
    ASSERT_TRUE(cc.contains(v7)) << name;

    cc.remove(v4);
    children1.clear();
    cc.getUnion(v0, children1);
    ASSERT_FALSE(cc.contains(v4)) << name;
    ASSERT_EQ(4, children1.size()) << name;

    cc.remove(v0);
    cc.getUnion(v2, children4);
    ASSERT_FALSE(cc.contains(v0)) << name;
    ASSERT_EQ(3, children4.size()) << name;

    cc.remove(v5);
    ASSERT_FALSE(cc.contains(v5)) << name;
    ASSERT_EQ(2, cc.size()) << name;

    Voxel p1c = ccc.find(v0);
    Voxel p2c = ccc.find(v6);

    ASSERT_TRUE(p1c == v0 || p1c == v1 || p1c == v2 || p1c == v3 || p1c == v4) << name;
    ASSERT_TRUE(p2c == v6 || p2c == v7) << name;

    std::vector<Voxel> children1c;
    std::vector<Voxel> children2c;
    std::vector<Voxel> children3c;
    std::vector<Voxel> children4c;
    ccc.getUnion(v0, children1c);
    ccc.getUnion(v6, children2c);
    ccc.getUnion(v5, children3c);

    ASSERT_EQ(5, children1c.size()) << name;
    ASSERT_EQ(2, children2c.size()) << name;
    ASSERT_EQ(1, children3c.size()) << name;

    ASSERT_TRUE(ccc.contains(v0)) << name;
    ASSERT_TRUE(ccc.contains(v1)) << name;
    ASSERT_TRUE(ccc.contains(v2)) << name;
    ASSERT_TRUE(ccc.contains(v3)) << name;
    ASSERT_TRUE(ccc.contains(v4)) << name;
    ASSERT_TRUE(ccc.contains(v5)) << name;
    ASSERT_TRUE(ccc.contains(v6)) << name;
    ASSERT_TRUE(ccc.contains(v7)) << name;

    ccc.remove(v4);
    children1c.clear();
    ccc.getUnion(v0, children1c);
    ASSERT_FALSE(ccc.contains(v4)) << name;
    ASSERT_EQ(4, children1c.size()) << name;

    ccc.remove(v0);
    ccc.getUnion(v2, children4c);
    ASSERT_FALSE(ccc.contains(v0)) << name;
    ASSERT_EQ(3, children4c.size()) << name;

    ccc.remove(v5);
    ASSERT_FALSE(ccc.contains(v5)) << name;
    ASSERT_EQ(2, ccc.size()) << name;
}

TEST(ConnectedComponentsTest, Children) {
    verifyCCChildren<ConnectedComponentsWithRemoval<Voxel> >("ConnectedComponentsWithRemoval");
}

template <class CC>
void verifyCCChildren(std::string name){
    CC cc;

    Voxel v0(0, 0, 0);
    Voxel v1(0, 1, 0);
    Voxel v2(0, 0, 1);
    Voxel v3(0, 1, 1);
    Voxel v4(0, 1, 2);
    Voxel v5(0, 3, 1);
    Voxel v6(0, 2, 4);
    Voxel v7(0, 3, 4);

    cc.initialize(v0);
    cc.initialize(v1);
    cc.initialize(v2);
    cc.initialize(v3);
    cc.initialize(v4);
    cc.merge(v0, v1);
    cc.merge(v2, v3);
    cc.merge(v2, v4);
    cc.merge(v0, v2);

    cc.initialize(v5);

    cc.initialize(v6);
    cc.initialize(v7);
    cc.merge(v6, v7);

    CC ccc(cc);

    std::vector<Voxel> children1;
    std::vector<Voxel> children2;
    std::vector<Voxel> children3;
    cc.getUnion(v0, children1);
    cc.getUnion(v6, children2);
    cc.getUnion(v5, children3);

    ASSERT_EQ(5, children1.size()) << name;
    ASSERT_EQ(2, children2.size()) << name;
    ASSERT_EQ(1, children3.size()) << name;

    std::unordered_set<Voxel> chilSet;
    for (Voxel v : children1) chilSet.insert(v);

    ASSERT_TRUE(chilSet.find(v0) != chilSet.end()) << name;
    ASSERT_TRUE(chilSet.find(v1) != chilSet.end()) << name;
    ASSERT_TRUE(chilSet.find(v2) != chilSet.end()) << name;
    ASSERT_TRUE(chilSet.find(v3) != chilSet.end()) << name;
    ASSERT_TRUE(chilSet.find(v4) != chilSet.end()) << name;

    ASSERT_TRUE(children2.at(0) == v6 || children2.at(0) == v7) << name;
    ASSERT_TRUE(children2.at(1) == v6 || children2.at(1) == v7) << name;
    ASSERT_TRUE(children2.at(0) != children2.at(1)) << name;

    ASSERT_TRUE(children3.at(0) == v5) << name;

    std::vector<Voxel> children1c;
    std::vector<Voxel> children2c;
    std::vector<Voxel> children3c;
    ccc.getUnion(v0, children1c);
    ccc.getUnion(v6, children2c);
    ccc.getUnion(v5, children3c);

    ASSERT_EQ(5, children1c.size()) << name;
    ASSERT_EQ(2, children2c.size()) << name;
    ASSERT_EQ(1, children3c.size()) << name;

    std::unordered_set<Voxel> chilSetc;
    for (Voxel v : children1c) chilSetc.insert(v);

    ASSERT_TRUE(chilSetc.find(v0) != chilSetc.end()) << name;
    ASSERT_TRUE(chilSetc.find(v1) != chilSetc.end()) << name;
    ASSERT_TRUE(chilSetc.find(v2) != chilSetc.end()) << name;
    ASSERT_TRUE(chilSetc.find(v3) != chilSetc.end()) << name;
    ASSERT_TRUE(chilSetc.find(v4) != chilSetc.end()) << name;

    ASSERT_TRUE(children2c.at(0) == v6 || children2c.at(0) == v7) << name;
    ASSERT_TRUE(children2c.at(1) == v6 || children2c.at(1) == v7) << name;
    ASSERT_TRUE(children2c.at(0) != children2c.at(1)) << name;

    ASSERT_TRUE(children3c.at(0) == v5) << name;
}

TEST(ConnectedComponentsTest, GetComponents) {
    verifyCCGetComponents<ConnectedComponentsWithRemoval<Voxel> >("ConnectedComponentsWithRemoval");
}

template <class CC>
void verifyCCGetComponents(std::string name){
    CC cc;

    Voxel v0(0, 0, 0);
    Voxel v1(0, 1, 0);
    Voxel v2(0, 0, 1);
    Voxel v3(0, 1, 1);
    Voxel v4(0, 1, 2);
    Voxel v5(0, 3, 1);
    Voxel v6(0, 2, 4);
    Voxel v7(0, 3, 4);

    cc.initialize(v0);
    cc.initialize(v1);
    cc.initialize(v2);
    cc.initialize(v3);
    cc.initialize(v4);
    cc.merge(v0, v1);
    cc.merge(v2, v3);
    cc.merge(v2, v4);
    cc.merge(v0, v2);

    cc.initialize(v5);

    cc.initialize(v6);
    cc.initialize(v7);
    cc.merge(v6, v7);

    CC ccc(cc);

    ASSERT_EQ(5, cc.getComponentSize(v0)) << name;
    ASSERT_EQ(5, cc.getComponentSize(v1)) << name;
    ASSERT_EQ(5, cc.getComponentSize(v2)) << name;
    ASSERT_EQ(5, cc.getComponentSize(v3)) << name;
    ASSERT_EQ(5, cc.getComponentSize(v4)) << name;
    ASSERT_EQ(1, cc.getComponentSize(v5)) << name;
    ASSERT_EQ(2, cc.getComponentSize(v6)) << name;
    ASSERT_EQ(2, cc.getComponentSize(v7)) << name;

    std::vector<Voxel> component1;
    std::vector<Voxel> component2;
    std::vector<Voxel> component3;
    cc.getComponent(v0, component1);
    cc.getComponent(v5, component2);
    cc.getComponent(v6, component3);

    std::unordered_set<Voxel> compSet;
    for (Voxel v : component1) compSet.insert(v);

    ASSERT_TRUE(compSet.find(v0) != compSet.end()) << name;
    ASSERT_TRUE(compSet.find(v1) != compSet.end()) << name;
    ASSERT_TRUE(compSet.find(v2) != compSet.end()) << name;
    ASSERT_TRUE(compSet.find(v3) != compSet.end()) << name;
    ASSERT_TRUE(compSet.find(v4) != compSet.end()) << name;

    ASSERT_TRUE(component2.at(0) == v5) << name;

    ASSERT_TRUE(component3.at(0) == v6 || component3.at(0) == v7) << name;
    ASSERT_TRUE(component3.at(1) == v6 || component3.at(1) == v7) << name;
    ASSERT_TRUE(component3.at(0) != component3.at(1)) << name;

    ASSERT_EQ(5, ccc.getComponentSize(v0)) << name;
    ASSERT_EQ(5, ccc.getComponentSize(v1)) << name;
    ASSERT_EQ(5, ccc.getComponentSize(v2)) << name;
    ASSERT_EQ(5, ccc.getComponentSize(v3)) << name;
    ASSERT_EQ(5, ccc.getComponentSize(v4)) << name;
    ASSERT_EQ(1, ccc.getComponentSize(v5)) << name;
    ASSERT_EQ(2, ccc.getComponentSize(v6)) << name;
    ASSERT_EQ(2, ccc.getComponentSize(v7)) << name;

    std::vector<Voxel> component1c;
    std::vector<Voxel> component2c;
    std::vector<Voxel> component3c;
    ccc.getComponent(v0, component1c);
    ccc.getComponent(v5, component2c);
    ccc.getComponent(v6, component3c);

    std::unordered_set<Voxel> compSetc;
    for (Voxel v : component1c) compSetc.insert(v);

    ASSERT_TRUE(compSetc.find(v0) != compSetc.end()) << name;
    ASSERT_TRUE(compSetc.find(v1) != compSetc.end()) << name;
    ASSERT_TRUE(compSetc.find(v2) != compSetc.end()) << name;
    ASSERT_TRUE(compSetc.find(v3) != compSetc.end()) << name;
    ASSERT_TRUE(compSetc.find(v4) != compSetc.end()) << name;

    ASSERT_TRUE(component2c.at(0) == v5) << name;

    ASSERT_TRUE(component3c.at(0) == v6 || component3c.at(0) == v7) << name;
    ASSERT_TRUE(component3c.at(1) == v6 || component3c.at(1) == v7) << name;
    ASSERT_TRUE(component3c.at(0) != component3c.at(1)) << name;
}
