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
 * @file Connected_Components_test.cpp
 * Test file for the Connected Component class.
 **/

#include <gtest/gtest.h>
#include <unordered_set>
#include <vector>

#include "shape/ConnectedComponents.h"
#include "util/UnionFind.h"
#include "util/UnionFindDelete.h"
#include "shape/Voxel.h"

using shape_simplification::shape::ConnectedComponents;
using shape_simplification::shape::ConnectedComponentsWithRemoval;
using shape_simplification::util::UnionFind;
using shape_simplification::util::UnionFindDelete;
using shape_simplification::shape::Voxel;

template <class Structure>
void verifyUnionModifier(std::string name)
{
    Structure cc;

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

    cc.clear();
    ASSERT_EQ(0, cc.size()) << name;
}

TEST(ConnectedComponentsTest, Modifier) {
    verifyUnionModifier<ConnectedComponents<Voxel> >("Standart");
    verifyUnionModifier<ConnectedComponentsWithRemoval<Voxel> >("Removal");
}

template <class Structure>
void verifyUnionContent(std::string name)
{
    Structure cc;

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

    Structure ccc(cc);

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

TEST(ConnectedComponentsTest, Content) {
    verifyUnionContent<ConnectedComponents<Voxel> >("Standart");
    verifyUnionContent<ConnectedComponentsWithRemoval<Voxel> >("Removal");
}

TEST(ConnectedComponentsTest, StandartSpecific) {
    ConnectedComponents<Voxel> cc;
    std::unordered_set<Voxel> roots;
    std::vector<std::vector<Voxel> > sets;

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

    cc.getRoots(roots);
    ASSERT_EQ(3, roots.size()) << "getRoots";
    ASSERT_TRUE(roots.find(v0) != roots.end() ||
            roots.find(v1) != roots.end() ||
            roots.find(v2) != roots.end() ||
            roots.find(v3) != roots.end() ||
            roots.find(v4) != roots.end()) << "getRoots";
    ASSERT_TRUE(roots.find(v5) != roots.end()) << "getRoots";
    ASSERT_TRUE(roots.find(v6) != roots.end() ||
            roots.find(v7) != roots.end()) << "getRoots";

    roots.clear();
    cc.getRoots(roots, 4);
    ASSERT_EQ(1, roots.size()) << "getRoots max 4";
    ASSERT_TRUE(roots.find(v0) != roots.end() ||
            roots.find(v1) != roots.end() ||
            roots.find(v2) != roots.end() ||
            roots.find(v3) != roots.end() ||
            roots.find(v4) != roots.end()) << "getRoots max 4";

    cc.getSets(sets);
    ASSERT_EQ(3, sets.size()) << "getSets";
    ASSERT_EQ(8, sets.at(0).size() + sets.at(1).size() + sets.at(2).size())
            << "getSets";
    ASSERT_TRUE(sets.at(0).size() == 5 ||
                sets.at(0).size() == 1 ||
                sets.at(0).size() == 2) << "getSets";
    ASSERT_TRUE(sets.at(1).size() == 5 ||
                sets.at(1).size() == 1 ||
                sets.at(1).size() == 2) << "getSets";
    ASSERT_TRUE(sets.at(2).size() == 5 ||
                sets.at(2).size() == 1 ||
                sets.at(2).size() == 2) << "getSets";
}

TEST(ConnectedComponentsTest, Removal) {
    ConnectedComponentsWithRemoval<Voxel> cc;

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

    Voxel p1 = cc.find(v0);
    Voxel p2 = cc.find(v6);

    ASSERT_TRUE(p1 == v0 || p1 == v1 || p1 == v2 || p1 == v3 || p1 == v4)
            << "find";
    ASSERT_TRUE(p2 == v6 || p2 == v7) << "find";

    std::vector<Voxel> children1;
    std::vector<Voxel> children2;
    std::vector<Voxel> children3;
    std::vector<Voxel> children4;
    cc.getUnion(v0, children1);
    cc.getUnion(v6, children2);
    cc.getUnion(v5, children3);

    ASSERT_EQ(5, children1.size()) << "getUnion";
    ASSERT_EQ(2, children2.size()) << "getUnion";
    ASSERT_EQ(1, children3.size()) << "getUnion";

    cc.remove(v4);
    children1.clear();
    cc.getUnion(v0, children1);
    ASSERT_FALSE(cc.contains(v4)) << "remove v4";
    ASSERT_EQ(4, children1.size()) << "remove v4";

    cc.remove(v0);
    cc.getUnion(v2, children4);
    ASSERT_FALSE(cc.contains(v0)) << "remove v0";
    ASSERT_EQ(3, children4.size()) << "remove v0";

    cc.remove(v5);
    ASSERT_FALSE(cc.contains(v5)) << "remove v5";
    ASSERT_EQ(2, cc.size()) << "remove v5";
}

TEST(ConnectedComponentsTest, RemovalSpecificContent) {
    ConnectedComponentsWithRemoval<Voxel> cc;

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

    ASSERT_EQ(5, cc.getComponentSize(v0)) << "getComponentSize v0";
    ASSERT_EQ(5, cc.getComponentSize(v1)) << "getComponentSize v1";
    ASSERT_EQ(5, cc.getComponentSize(v2)) << "getComponentSize v2";
    ASSERT_EQ(5, cc.getComponentSize(v3)) << "getComponentSize v3";
    ASSERT_EQ(5, cc.getComponentSize(v4)) << "getComponentSize v4";
    ASSERT_EQ(1, cc.getComponentSize(v5)) << "getComponentSize v5";
    ASSERT_EQ(2, cc.getComponentSize(v6)) << "getComponentSize v6";
    ASSERT_EQ(2, cc.getComponentSize(v7)) << "getComponentSize v7";

    std::vector<Voxel> component1;
    std::vector<Voxel> component2;
    std::vector<Voxel> component3;
    cc.getComponent(v0, component1);
    cc.getComponent(v5, component2);
    cc.getComponent(v6, component3);

    std::unordered_set<Voxel> compSet;
    for (Voxel v : component1) compSet.insert(v);

    ASSERT_TRUE(compSet.find(v0) != compSet.end()) << "getComponent v0";
    ASSERT_TRUE(compSet.find(v1) != compSet.end()) << "getComponent v1";
    ASSERT_TRUE(compSet.find(v2) != compSet.end()) << "getComponent v2";
    ASSERT_TRUE(compSet.find(v3) != compSet.end()) << "getComponent v3";
    ASSERT_TRUE(compSet.find(v4) != compSet.end()) << "getComponent v4";

    ASSERT_TRUE(component2.at(0) == v5) << "getComponent v5";

    ASSERT_TRUE(component3.at(0) == v6 || component3.at(0) == v7)
            << "getComponent";
    ASSERT_TRUE(component3.at(1) == v6 || component3.at(1) == v7)
            << "getComponent";
    ASSERT_TRUE(component3.at(0) != component3.at(1))
            << "getComponent";

    compSet.clear();
    for (auto it = cc.begin(v0); it != cc.end(v0); it++){
        compSet.insert(*it);
    }
    compSet.insert(*cc.end(v0));

    ASSERT_EQ(5, compSet.size());
    ASSERT_TRUE(compSet.find(v0) != compSet.end()) << "iterator v0";
    ASSERT_TRUE(compSet.find(v1) != compSet.end()) << "iterator v1";
    ASSERT_TRUE(compSet.find(v2) != compSet.end()) << "iterator v2";
    ASSERT_TRUE(compSet.find(v3) != compSet.end()) << "iterator v3";
    ASSERT_TRUE(compSet.find(v4) != compSet.end()) << "iterator v4";
}

