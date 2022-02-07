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
 * @file PseudoVoidsCreator.cpp
 * Contains the implementation of the methods in the
 * \ref PseudoVoidsCreator class.
 **/

#include "DistancePseudoVoidsCreator.h"

namespace shape_simplification::simplifier {

DistancePseudoVoidsCreator::DistancePseudoVoidsCreator(
        ProcessedImageStack& stack, unsigned int inflationValue, bool silent) :
    _stack(stack), _inflationValue(inflationValue), _silent(silent)
{}

void DistancePseudoVoidsCreator::closeTop()
{
    const std::array<unsigned int, 3> &dimensions = _stack.getDimensions();
    png::image<png::gray_pixel> image(dimensions[0], dimensions[1]);
    unsigned int minX = dimensions[0], maxX = 0;
    unsigned int minY = dimensions[1], maxY = 0;

    for (unsigned int y = 0; y < dimensions[1]; y++){
        for (unsigned int x = 0; x < dimensions[0]; x++) {
            if (_stack.getLevel(x, y, dimensions[2] - 1) >=
                    _stack.getShapeThreshold()){
                if (x < minX) minX = x;
                if (x > maxX) maxX = x;
                if (y < minY) minY = y;
                if (y > maxY) maxY = y;
            }
        }
    }

    _stack.addEmptyLayer();
    for (unsigned int y = minY; y <= maxY; y++){
        for (unsigned int x = minX; x <= maxX; x++) {            
            _stack.setValue(_stack.getShapeThreshold(), x, y,
                            dimensions[2] - 1);
        }
    }
}

void DistancePseudoVoidsCreator::removeTop()
{
    _stack.removeTopLayer();
}

void DistancePseudoVoidsCreator::createPseudoVoids()
{
    std::vector<std::vector<Voxel> > patchs;
    ConnectedComponents<Voxel> patchComponents;

    _resetTimer();
    _printMessage("---Load inflation shape.");

    InflationShape shape(_stack, _inflationValue, true);

    _printAndResetTime("Inflation shape loading time:");
    _printMessage("---Compute void patches.");

    _computeVoidPatchComponents(patchComponents, shape);
    _computeValidPatches(patchComponents, patchs, shape);

    _printAndResetTime("Void patchs computing time:");
    _printMessage("---Create pseudo-voids.");

    for (std::vector<Voxel>& p : patchs){
        _createVoid(p);
    }

    _printAndResetTime("Pseudo-voids creation time:");
}

void DistancePseudoVoidsCreator::_computeVoidPatchComponents(
        ConnectedComponents<Voxel>& patchCC,
        InflationShape& shape)
{
    ConnectedComponents<Voxel> cc = shape.getLocalMaxima();

    std::unordered_set<Voxel> rep;
    std::unordered_set<Voxel> nrep;
    std::unordered_set<Voxel> nlrep;
    int x, y, z;

    cc.getRepresentatives(rep);

    for (InflationShape::voxel_reverse_iterator<EX_NEIGHBORHOOD> it =
            shape.rbegin<EX_NEIGHBORHOOD>();
         it != shape.rend<EX_NEIGHBORHOOD>();
         it++)
    {
        Voxel v = *it;

        nrep.clear();
        nlrep.clear();
        for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
            it.getCurrentValue(x, y, z);
            Voxel cur(x, y, z);
            if (cc.contains(cur)){
                Voxel r = cc.find(cur);
                if (rep.find(r) == rep.end()) nlrep.insert(r);
                else nrep.insert(r);
            }
        }

        if (nrep.size() > 1){
            patchCC.initialize(v);
            for (NeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
                it.getCurrentValue(x, y, z);
                patchCC.merge(v, Voxel(x, y, z));
            }
        } else {
            cc.initialize(v);

            for (auto it = nlrep.begin(); it != nlrep.end(); it++){
                cc.merge(v, *it);
            }

            if (nrep.size() == 1){
                Voxel w = *(nrep.begin());
                cc.merge(v, w);
                Voxel r = cc.find(v);
                if (r != w){
                    rep.erase(w);
                    rep.insert(r);
                }
            }
        }
    }
}

void DistancePseudoVoidsCreator::_computeSeedCollarComponents(
        ConnectedComponents<Voxel>& cc,
        InflationShape &shape)
{
    int x, y, z;
    for (Voxel v : shape.getVoxelsAtDistance(_inflationValue))
    {
        for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
            it.getCurrentValue(x, y, z);
            Voxel cur(x, y, z);
            if (!cc.contains(cur) &&
                    !(shape.contains<NEIGHBORHOOD>(cur[0], cur[1], cur[2]))){
                cc.initialize(cur);
                for (ExtendedNeighborsIterator<Voxel> itn(cur);
                     !itn.isEnd();
                     itn.next())
                {
                    itn.getCurrentValue(x, y, z);
                    cc.merge(cur, Voxel(x, y, z));
                }
            }
        }
    }
}

void DistancePseudoVoidsCreator::_computeValidPatches(
        ConnectedComponents<Voxel>& patchCC,
        std::vector<std::vector<Voxel> >& patchs,
        InflationShape& shape)
{
    std::vector<std::vector<Voxel> > components;

    patchCC.getComponents(components);

    for (std::vector<Voxel>& comp : components){
        bool isValid = false;
        unsigned int i = 0;

        while (!isValid && i < comp.size()) {
            if (_isAdjacentToShape(comp.at(i), shape)) isValid = true;
            i++;
        }

        if (isValid) patchs.push_back(comp);
    }
}

bool DistancePseudoVoidsCreator::_isAdjacentToShape(
        Voxel &v, InflationShape& shape)
{
    int x, y, z;
    for (NeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
        it.getCurrentValue(x, y, z);
        if (shape.contains<MAIN_SHAPE>(x, y, z)) return true;
    }

    return false;
}

void DistancePseudoVoidsCreator::_createVoid(std::vector<Voxel>& patch)
{
    for (Voxel v : patch) {
        _stack.setValue(_stack.getShapeThreshold(), v[0], v[1], v[2]);
    }
}

void DistancePseudoVoidsCreator::fillVoids()
{
    int x, y, z;
    IntensityShape shape(_stack, _stack.getNeighborhoodThreshold());

    std::unordered_set<Voxel> seedPoints;
    ConnectedComponents<Voxel> collarComponents;

    Voxel maxCollar = _computeCollarComponents(collarComponents, shape);

    collarComponents.getRepresentatives(seedPoints);
    Voxel ext = collarComponents.find(maxCollar);
    seedPoints.erase(ext);

    _printMessage("Number of Voids filled: "
            + std::to_string(seedPoints.size()));

    for (Voxel seed : seedPoints) {
        std::queue<Voxel> queue;
        queue.push(seed);
        std::unordered_set<Voxel> found;
        found.insert(seed);
        while (!queue.empty()) {
            Voxel cur = queue.front();
            queue.pop();
            _stack.setValue(_stack.getShapeThreshold(), cur[0], cur[1], cur[2]);

            for (ExtendedNeighborsIterator<Voxel> it(cur);
                 !it.isEnd();
                 it.next())
            {
                it.getCurrentValue(x, y, z);
                Voxel n(x, y, z);
                if (!(shape.contains<IntensityShape::MAIN_SHAPE>(n)) &&
                        found.find(n) == found.end()) {
                    queue.push(n);
                    found.insert(n);
                }
            }
        }
    }
}

Voxel DistancePseudoVoidsCreator::_computeCollarComponents(
        ConnectedComponents<Voxel>& cc,
        IntensityShape& shape)
{
    Voxel maxCollar(0, 0, 0);
    std::vector<Voxel> collar;
    std::unordered_map<Voxel,Voxel> ccRep;
    int x, y, z;

    for (IntensityShape::voxel_iterator<IntensityShape::MAIN_SHAPE> it =
         shape.begin<IntensityShape::MAIN_SHAPE>();
         it != shape.end<IntensityShape::MAIN_SHAPE>();
         it++)
    {
        Voxel v = *it;
        for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
            it.getCurrentValue(x, y, z);
            Voxel cur(x, y, z);
            if (!(shape.contains<IntensityShape::MAIN_SHAPE>(cur)) &&
                    !cc.contains(cur)){
                cc.initialize(cur);
                collar.push_back(cur);
                for (ExtendedNeighborsIterator<Voxel> it2(cur);
                     !it2.isEnd();
                     it2.next())
                {
                    it2.getCurrentValue(x, y, z);
                    cc.merge(cur, Voxel(x, y, z));
                }
                maxCollar = std::max(maxCollar, cur);
            }
        }
    }

    for (Voxel v : collar) {
        Voxel cur = cc.find(v);
        if (ccRep.find(cur) == ccRep.end())
            ccRep[cur] = v;
        else
            ccRep[cur] = std::max(v, ccRep[cur]);
    }

    _mergeSameVoidCollarComponents(cc, ccRep, shape);

    return maxCollar;
}

void DistancePseudoVoidsCreator::_mergeSameVoidCollarComponents(
        ConnectedComponents<Voxel>& cc,
        std::unordered_map<Voxel,Voxel>& ccRep,
        IntensityShape& shape)
{
    int x, y, z;
    for (auto iter : ccRep) {
        Voxel v = iter.second;
        Point p = {v[0], v[1], v[2]};

        bool done = false;
        for (unsigned i = 0; (i < 3) && !done; i++) {
            while (!done &&
                   static_cast<unsigned int>(p[i]) <=
                   shape.getShapeDimensions()[i])
            {
                p[i]++;
                Voxel cur(p);
                if (!cc.contains(cur) &&
                        !(shape.contains<IntensityShape::MAIN_SHAPE>(cur)))
                {
                    cc.initialize(cur);
                    for (ExtendedNeighborsIterator<Voxel> it(cur);
                         !it.isEnd();
                         it.next()) {
                        it.getCurrentValue(x, y, z);
                        cc.merge(cur, Voxel(x, y, z));
                    }
                } else {
                    done = true;
                }
            }
        }
    }
}

void DistancePseudoVoidsCreator::_resetTimer(){
    if (_silent) return;

    _time = clock();
}

void DistancePseudoVoidsCreator::_printTime(std::string message){
    if (_silent) return;

    std::cout << message << " " << std::setprecision(10)
              << (static_cast<float>(clock() - _time)) / CLOCKS_PER_SEC
              << " seconds.\n";
}

void DistancePseudoVoidsCreator::_printAndResetTime(std::string message){
    if (_silent) return;

    _printTime(message);
    _resetTimer();
}

void DistancePseudoVoidsCreator::_printMessage(std::string message){
    if (_silent) return;

    std::cout << message << std::endl;
}

} // namespace pseudo_void_filler
