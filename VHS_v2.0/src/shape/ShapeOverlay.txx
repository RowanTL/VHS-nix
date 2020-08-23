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
 * @file ShapeOverlay.txx
 * Contains the implementation of the methods in the \ref ShapeOverlay class.
 **/

#include "ShapeOverlay.h"

using namespace shape_simplification::shape;

template <class AugmentedShape, class Voxel>
ShapeOverlay<AugmentedShape, Voxel>::ShapeOverlay(AugmentedShape &shape)
    : _shape(shape),
      _currentState(0),
      _initialEulerCharacteristic(shape.getEulerCharacteristic()),
      _initialNumberOfComponents(shape.getNumberOfComponents()),
      _initialNumberOfLoops(shape.getNumberOfLoops()),
      _initialNumberOfVoids(shape.getNumberOfVoids()),
      _currentEulerCharacteristic(_initialEulerCharacteristic),
      _currentNumberOfComponents(_initialNumberOfComponents),
      _currentNumberOfLoops(_initialNumberOfLoops),
      _currentNumberOfVoids(_initialNumberOfVoids),
      _shapeNumberOfComponents(_initialNumberOfComponents),
      _savedCollar(shape.getCollar()),
      _savedSinceLastUpdate(false),
      _testedSinceLastSave(false)
{}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::addVoxelsInMainShape(
        voxel_container &voxels) {
    if (_currentState == -1) return;
    _currentState = 1;

    _updateNumberOfComponentsAfterAdding(voxels);
    _updateNumberOfVoidsAfterAdding(voxels);
    _updateEulerCharacteristic(voxels);
    _updateNumberOfLoops();
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::addVoxelsInMainShapeWithH0Guarantee(
        voxel_container &voxels) {
    if (_currentState == -1) return;
    _currentState = 1;

    _updateNumberOfVoidsAfterAdding(voxels);
    _updateEulerCharacteristic(voxels);
    _updateNumberOfLoops();
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::addVoxelsInMainShapeWithH2Guarantee(
        voxel_container &voxels) {
    if (_currentState == -1) return;
    _currentState = 1;

    _updateNumberOfComponentsAfterAdding(voxels);
    _updateEulerCharacteristic(voxels);
    _updateNumberOfLoops();
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::addVoxelsInMainShapeWithH0H2Guarantee(
        voxel_container &voxels) {
    if (_currentState == -1) return;
    _currentState = 1;

    _updateEulerCharacteristic(voxels);
    _updateNumberOfLoops();
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::removeVoxelsFromMainShape(
        voxel_container &voxels) {
    static_assert(AugmentedShape::isRemovable(), "Removal of voxel not enabled.");

    if (_currentState == 1) return;
    _currentState = -1;

    _updateNumberOfComponentsAfterRemoving(voxels);
    _updateNumberOfVoidsAfterRemoving(voxels);
    _updateEulerCharacteristic(voxels);
    _updateNumberOfLoops();
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::removeVoxelsFromMainShapeWithH0Guarantee(
        voxel_container &voxels) {
    static_assert(AugmentedShape::isRemovable(), "Removal of voxel not enabled.");

    if (_currentState == 1) return;
    _currentState = -1;

    _updateNumberOfVoidsAfterRemoving(voxels);
    _updateEulerCharacteristic(voxels);
    _updateNumberOfLoops();
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::removeVoxelsFromMainShapeWithH2Guarantee(
        voxel_container &voxels) {
    static_assert(AugmentedShape::isRemovable(), "Removal of voxel not enabled.");

    if (_currentState == 1) return;
    _currentState = -1;

    _updateNumberOfComponentsAfterRemoving(voxels);
    _updateEulerCharacteristic(voxels);
    _updateNumberOfLoops();
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::removeVoxelsFromMainShapeWithH0H2Guarantee(
        voxel_container &voxels) {
    static_assert(AugmentedShape::isRemovable(), "Removal of voxel not enabled.");

    if (_currentState == 1) return;
    _currentState = -1;

    _updateEulerCharacteristic(voxels);
    _updateNumberOfLoops();
}

template <class AugmentedShape, class Voxel>
int ShapeOverlay<AugmentedShape, Voxel>::
addVoxelsAndReturnEulerOnly(voxel_container &voxels){
    if (_currentState == -1) return _currentEulerCharacteristic;
    _currentState = 1;

    int res = _initialEulerCharacteristic;;
    _updateEulerCharacteristic(voxels, res);

    return res;
}

template <class AugmentedShape, class Voxel>
int ShapeOverlay<AugmentedShape, Voxel>::
removeVoxelsAndReturnEulerOnly(voxel_container &voxels){
    static_assert(AugmentedShape::isRemovable(), "Removal of voxel not enabled.");

    if (_currentState == 1) return _currentEulerCharacteristic;
    _currentState = -1;

    int res = _initialEulerCharacteristic;
    _updateEulerCharacteristic(voxels, res);

    return res;
}

template <class AugmentedShape, class Voxel>
unsigned int ShapeOverlay<AugmentedShape, Voxel>::
addVoxelsAndReturnH0Only(voxel_container &voxels){
    if (_currentState == -1) return _currentNumberOfComponents;
    _currentState = 1;

    unsigned int res;
    _updateNumberOfComponentsAfterAdding(voxels, res);

    return res;
}

template <class AugmentedShape, class Voxel>
unsigned int ShapeOverlay<AugmentedShape, Voxel>::
removeVoxelsAndReturnH0Only(voxel_container &voxels){
    static_assert(AugmentedShape::isRemovable(), "Removal of voxel not enabled.");

    if (_currentState == 1) return _currentNumberOfComponents;
    _currentState = -1;

    unsigned int res;
    _updateNumberOfComponentsAfterRemoving(voxels, res);

    return res;
}

template <class AugmentedShape, class Voxel>
unsigned int ShapeOverlay<AugmentedShape, Voxel>::
addVoxelsAndReturnH2Only(voxel_container &voxels){
    if (_currentState == -1) return _currentNumberOfVoids;
    _currentState = 1;

    unsigned int tmp = _currentNumberOfVoids;
    unsigned int res;
    _updateNumberOfVoidsAfterAdding(voxels);
    res = _currentNumberOfVoids;
    _currentNumberOfVoids = tmp;
    return res;
}

template <class AugmentedShape, class Voxel>
unsigned int ShapeOverlay<AugmentedShape, Voxel>::
removeVoxelsAndReturnH2Only(voxel_container &voxels){
    static_assert(AugmentedShape::isRemovable(), "Removal of voxel not enabled.");

    if (_currentState == 1) return _currentNumberOfVoids;
    _currentState = -1;

    unsigned int tmp = _currentNumberOfVoids;
    unsigned int res;
    _updateNumberOfVoidsAfterRemoving(voxels);
    res = _currentNumberOfVoids;
    _currentNumberOfVoids = tmp;
    return res;
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::saveAddedVoxels(voxel_container &voxels){
    if (_currentState != 1) return;

    _updateNumberOfComponentsAfterAddingAndSave(voxels);
    _updateNumberOfVoidsAfterAddingComputeCollarChanges(
                voxels, _savedCollar);
    _commonSaveVoxels(voxels);
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::saveRemovedVoxels(voxel_container &voxels){
    static_assert(AugmentedShape::isRemovable(), "Removal of voxel not enabled.");

    if (_currentState != -1) return;

    _updateNumberOfComponentsAfterRemovingAndSave(voxels);
    _updateNumberOfVoidsAfterRemovingComputeCollarChanges(
                voxels, _savedCollar);
    _commonSaveVoxels(voxels);
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::saveAddedVoxelsWithH2Guarantee(
        voxel_container &voxels){
    if (_currentState != 1) return;

    _updateNumberOfComponentsAfterAddingAndSave(voxels);
    _updateNumberOfVoidsAfterAddingComputeCollarChanges(
                voxels, _savedCollar);
    //has to be last
    _commonSaveVoxelsWithH2Guarantee(voxels);
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::saveRemovedVoxelsWithH2Guarantee(
        voxel_container &voxels){
    static_assert(AugmentedShape::isRemovable(), "Removal of voxel not enabled.");

    if (_currentState != -1) return;

    _updateNumberOfComponentsAfterRemovingAndSave(voxels);
    _updateNumberOfVoidsAfterRemovingComputeCollarChanges(
                voxels, _savedCollar);
    //has to be last
    _commonSaveVoxelsWithH2Guarantee(voxels);
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::saveRemovedVoxelsWithH0Guarantee(
        voxel_container &voxels){
    static_assert(AugmentedShape::isRemovable(), "Removal of voxel not enabled.");

    if (_currentState != -1) return;

    _updateNumberOfVoidsAfterRemovingComputeCollarChanges(
                voxels, _savedCollar);
    //has to be last
    _commonSaveVoxels(voxels);
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::saveRemovedVoxelsWithH0H2Guarantee(
        voxel_container &voxels){
    static_assert(AugmentedShape::isRemovable(), "Removal of voxel not enabled.");

    if (_currentState != -1) return;

    _updateNumberOfVoidsAfterRemovingComputeCollarChanges(
                voxels, _savedCollar);
    //has to be last
    _commonSaveVoxelsWithH2Guarantee(voxels);
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::clearModifications(){
    _savedVoxels.clear();
    _currentState = 0;
    _initialEulerCharacteristic = _shape.getEulerCharacteristic();
    _initialNumberOfComponents = _shape.getNumberOfComponents();
    _initialNumberOfLoops = _shape.getNumberOfLoops();
    _initialNumberOfVoids = _shape.getNumberOfVoids();
    _currentEulerCharacteristic = _initialEulerCharacteristic;
    _currentNumberOfComponents = _initialNumberOfComponents;
    _currentNumberOfLoops = _initialNumberOfLoops;
    _currentNumberOfVoids = _initialNumberOfVoids;
    _shapeNumberOfComponents = _initialNumberOfComponents;
    _savedCollar.clear();
    for (Voxel v : _shape.getCollar()) _savedCollar.insert(v);
}

template <class AugmentedShape, class Voxel>
inline int ShapeOverlay<AugmentedShape, Voxel>::getInitialEulerCharacteristic() const {
    return _initialEulerCharacteristic;
}

template <class AugmentedShape, class Voxel>
inline unsigned int
ShapeOverlay<AugmentedShape, Voxel>::getInitialNumberOfComponents() const {
    return _initialNumberOfComponents;
}

template <class AugmentedShape, class Voxel>
inline unsigned int
ShapeOverlay<AugmentedShape, Voxel>::getInitialNumberOfLoops() {
    if (_savedSinceLastUpdate){
        _updateNumberOfLoopsAndSave();
    }
    return _initialNumberOfLoops;
}

template <class AugmentedShape, class Voxel>
inline unsigned int
ShapeOverlay<AugmentedShape, Voxel>::getInitialNumberOfVoids() {
    if (_savedSinceLastUpdate){
        if (_currentState == 1){
            std::function<bool(Voxel)> isInShape = [this](Voxel v) {
                return _shape.template contains<MAIN_SHAPE>(v) ||
                        _savedVoxels.find(v) != _savedVoxels.end();
            };
            _updateNumberOfVoidsAndSave(_savedCollar, isInShape);
        } else {
            std::function<bool(Voxel)> isInShape = [this](Voxel v) {
                return _shape.template contains<MAIN_SHAPE>(v) &&
                        _savedVoxels.find(v) == _savedVoxels.end();
            };
            _updateNumberOfVoidsAndSave(_savedCollar, isInShape);
        }

        _savedSinceLastUpdate = false;
    }
    return _initialNumberOfVoids;
}

template <class AugmentedShape, class Voxel>
inline int ShapeOverlay<AugmentedShape, Voxel>::getCurrentEulerCharacteristic() const {
    return _currentEulerCharacteristic;
}

template <class AugmentedShape, class Voxel>
inline unsigned int
ShapeOverlay<AugmentedShape, Voxel>::getCurrentNumberOfComponents() const {
    return _currentNumberOfComponents;
}

template <class AugmentedShape, class Voxel>
inline unsigned int
ShapeOverlay<AugmentedShape, Voxel>::getCurrentNumberOfLoops() {
    if (!_testedSinceLastSave){
        _updateNumberOfLoops();
    }
    return _currentNumberOfLoops;
}

template <class AugmentedShape, class Voxel>
inline unsigned int
ShapeOverlay<AugmentedShape, Voxel>::getCurrentNumberOfVoids() {
    if (!_testedSinceLastSave){
        _testedSinceLastSave = true;
        _currentNumberOfVoids = getInitialNumberOfVoids();
    }
    return _currentNumberOfVoids;
}

template <class AugmentedShape, class Voxel>
template <typename ShapeOverlay<AugmentedShape, Voxel>::Level level>
bool ShapeOverlay<AugmentedShape, Voxel>::contains(Voxel v) const{
    if constexpr (level == OUTSIDE){
        if (_currentState == -1)
            return _shape.template contains<level>(v) ||
                    (_savedVoxels.find(v) != _savedVoxels.end());
        if (_currentState == 1)
            return _shape.template contains<level>(v) &&
                    (_savedVoxels.find(v) == _savedVoxels.end());
    } else {
        if (_currentState == 1)
            return _shape.template contains<level>(v) ||
                    (_savedVoxels.find(v) != _savedVoxels.end());
        if (_currentState == -1)
            return _shape.template contains<level>(v) &&
                    (_savedVoxels.find(v) == _savedVoxels.end());
        return _shape.template contains<level>(v);
    }
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateEulerCharacteristic(
        voxel_container &voxels) {
    _currentEulerCharacteristic = _initialEulerCharacteristic;
    _updateEulerCharacteristic(voxels, _currentEulerCharacteristic);
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateEulerCharacteristicAndSave(
        voxel_container &voxels) {
    _updateEulerCharacteristic(voxels, _initialEulerCharacteristic);
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_commonSaveVoxels(voxel_container &voxels){
    _updateEulerCharacteristicAndSave(voxels);

    for (Voxel v : voxels) _savedVoxels.insert(v);

    _currentEulerCharacteristic = _initialEulerCharacteristic;
    _currentNumberOfComponents = _initialNumberOfComponents;
    _currentNumberOfLoops = _initialNumberOfLoops;

    _savedSinceLastUpdate = true;
    _testedSinceLastSave = false;
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_commonSaveVoxelsWithH2Guarantee(voxel_container &voxels){
    _commonSaveVoxels(voxels);
    _savedSinceLastUpdate = false;
    _testedSinceLastSave = true;
    //has to be last
    _updateNumberOfLoopsAndSave();

    _currentNumberOfVoids = _initialNumberOfVoids;
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateEulerCharacteristic(
        voxel_container &voxels, int &res) {
    int genNumberOfVertices = voxels.size();
    int genNumberOfEdges = 0;
    int genNumberOfSquares = 0;
    int genNumberOfCubes = 0;
    std::unordered_set<Voxel> done;
    for (Voxel v : voxels) {
        genNumberOfEdges +=
                _computeNumberOfGeneratedCells<DualEdgeIterator<Voxel>>(v, done, voxels);
        genNumberOfSquares +=
                _computeNumberOfGeneratedCells<DualSquareIterator<Voxel>>(v, done, voxels);
        genNumberOfCubes +=
                _computeNumberOfGeneratedCells<DualCubeIterator<Voxel>>(v, done, voxels);
        done.insert(v);
    }

    int altSum = genNumberOfVertices - genNumberOfEdges +
            genNumberOfSquares - genNumberOfCubes;
    if (_currentState == 1) res += altSum;
    else res -= altSum;
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateNumberOfComponentsAfterAdding(
        voxel_container &voxels) {
    _updateNumberOfComponentsAfterAdding(voxels, _currentNumberOfComponents);
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateNumberOfComponentsAfterAddingAndSave(
        voxel_container &voxels) {
    _updateNumberOfComponentsAfterAdding(voxels, _initialNumberOfComponents);
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateNumberOfComponentsAfterAdding(
        voxel_container &voxels, unsigned int &res)
{
    CC updatedComponents;
    std::unordered_set<Voxel> reusedComponents;
    voxel_container allVoxels(_savedVoxels);
    for (Voxel v : voxels) allVoxels.insert(v);

    for (Voxel v : allVoxels) {
        updatedComponents.initialize(v);
        for (neighbor_iterator<NEIGHBORHOOD> it(v, _shape);
             !it.isEnd();
             it.next()) {
            Voxel cur = it.getCurrentValue();
            if (allVoxels.find(cur) != allVoxels.end()) {
                updatedComponents.initialize(cur);
                updatedComponents.merge(v, cur);
            } else if (_shape.template contains<MAIN_SHAPE>(cur)) {
                Voxel rep = _shape.getComponentRepresentative(cur);
                reusedComponents.insert(rep);
                updatedComponents.initialize(rep);
                updatedComponents.merge(v, rep);
            }
        }
    }

    res = _shapeNumberOfComponents + updatedComponents.size() - reusedComponents.size();
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateNumberOfComponentsAfterRemoving(
        voxel_container &voxels) {
    _updateNumberOfComponentsAfterRemoving(voxels, _currentNumberOfComponents);
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateNumberOfComponentsAfterRemovingAndSave(
        voxel_container &voxels) {
    _updateNumberOfComponentsAfterRemoving(voxels, _initialNumberOfComponents);
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateNumberOfComponentsAfterRemoving(
        voxel_container &voxels, unsigned int &res)
{
    CC updatedComponents;
    std::unordered_set<Voxel> done;
    voxel_container allVoxels(_savedVoxels);
    for (Voxel v : voxels) allVoxels.insert(v);

    for (Voxel v : allVoxels){
        Voxel rep = _shape.getComponentRepresentative(v);
        if (done.find(rep) == done.end()){
            done.insert(rep);
            std::vector<Voxel> component;
            _shape.getComponent(rep, component);
            for (Voxel cur : component){
                if (allVoxels.find(cur) == allVoxels.end()){
                    updatedComponents.initialize(cur);
                    for (neighbor_iterator<MAIN_SHAPE> nIt(cur, _shape);
                         !nIt.isEnd();
                         nIt.next()){
                        Voxel n = nIt.getCurrentValue();
                        if (allVoxels.find(n) == allVoxels.end()){
                            updatedComponents.initialize(n);
                            updatedComponents.merge(cur, n);
                        }
                    }
                }
            }
        }
    }

    res = _shapeNumberOfComponents + updatedComponents.size() - done.size();
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateNumberOfLoops() {
    // Assumes that the other values were already updated!
    _currentNumberOfLoops = _currentNumberOfComponents -
            _currentEulerCharacteristic + getCurrentNumberOfVoids();
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateNumberOfLoopsAndSave() {
    // Assumes that the other values were already updated!
    _initialNumberOfLoops = _initialNumberOfComponents -
            _initialEulerCharacteristic + getInitialNumberOfVoids();
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateNumberOfVoidsAfterAdding(
        voxel_container &voxels){
    collar_container currentCollar(_savedCollar);

    _updateNumberOfVoidsAfterAddingComputeCollarChanges(
                voxels,
                currentCollar);

    std::function<bool(Voxel)> isInShape = [&voxels, this](Voxel v) {
        return _shape.template contains<MAIN_SHAPE>(v) ||
                voxels.find(v) != voxels.end() ||
                _savedVoxels.find(v) != _savedVoxels.end();
    };

    _updateNumberOfVoids(currentCollar, isInShape);
    _testedSinceLastSave = true;
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::
_updateNumberOfVoidsAfterAddingComputeCollarChanges(
        voxel_container &voxels,
        collar_container &currentCollar)
{
    for (Voxel v : voxels) {
        if (currentCollar.find(v) != currentCollar.end())
            currentCollar.erase(v);
        for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
            Voxel cur = it.getCurrentValue();
            if (!(_shape.template contains<MAIN_SHAPE>(cur)) &&
                    voxels.find(cur) == voxels.end() &&
                    currentCollar.find(v) == currentCollar.end()){
                currentCollar.insert(cur);
            }
        }
    }
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateNumberOfVoidsAfterRemoving(
        voxel_container &voxels){
    collar_container currentCollar(_savedCollar);

    _updateNumberOfVoidsAfterRemovingComputeCollarChanges(
                voxels,
                currentCollar);

    std::function<bool(Voxel)> isInShape = [&voxels, this](Voxel v) {
        return _shape.template contains<MAIN_SHAPE>(v) &&
                voxels.find(v) == voxels.end() &&
                _savedVoxels.find(v) == _savedVoxels.end();
    };

    _updateNumberOfVoids(currentCollar, isInShape);
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::
_updateNumberOfVoidsAfterRemovingComputeCollarChanges(
        voxel_container &voxels,
        collar_container &currentCollar)
{
    for (Voxel v : voxels) {
        bool toAdd = false;
        for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
            Voxel cur = it.getCurrentValue();
            if (voxels.find(cur) == voxels.end()){
                if (_shape.template contains<MAIN_SHAPE>(cur)){
                    toAdd = true;
                } else if (currentCollar.find(cur) != currentCollar.end()) {
                    ExtendedNeighborsIterator<Voxel> nIt(cur);
                    while (!nIt.isEnd() &&
                           !(_shape.template contains<MAIN_SHAPE>(nIt.getCurrentValue()) &&
                             voxels.find(nIt.getCurrentValue()) == voxels.end())) {
                        nIt.next();
                    }
                    if (nIt.isEnd()){
                        currentCollar.erase(cur);
                    }
                }
            }
        }
        if (toAdd) {
            currentCollar.insert(v);
        }
    }
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateNumberOfVoids(
        collar_container &currentCollar,
        std::function<bool(Voxel)>& isInShape)
{
    CC newCollarComponents;
    std::unordered_map<Voxel,Voxel> componentRep;

    _updateNumberOfVoidsFindCollarComponents(currentCollar,
                                             newCollarComponents);
    _updateNumberOfVoidsFindComponentRepresentative(currentCollar,
                                                    newCollarComponents,
                                                    componentRep);
    _updateNumberOfVoidsMergeSameVoidComponents(isInShape,
                                                newCollarComponents,
                                                componentRep);

    _currentNumberOfVoids = newCollarComponents.size() - 1;
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::_updateNumberOfVoidsAndSave(
        collar_container &currentCollar,
        std::function<bool(Voxel)>& isInShape)
{
    CC newCollarComponents;
    std::unordered_map<Voxel,Voxel> componentRep;

    _updateNumberOfVoidsFindCollarComponents(currentCollar,
                                             newCollarComponents);
    _updateNumberOfVoidsFindComponentRepresentative(currentCollar,
                                                    newCollarComponents,
                                                    componentRep);
    _updateNumberOfVoidsMergeSameVoidComponents(isInShape,
                                                newCollarComponents,
                                                componentRep);

    _initialNumberOfVoids = newCollarComponents.size() - 1;
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::
_updateNumberOfVoidsFindCollarComponents(
        collar_container &currentCollar,
        CC &collarComponents)
{
    for (Voxel v : currentCollar) {
        collarComponents.initialize(v);
        for (ExtendedNeighborsIterator<Voxel> it(v); !it.isEnd(); it.next()) {
            Voxel cur = it.getCurrentValue();
            collarComponents.merge(v, cur);
        }
    }
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::
_updateNumberOfVoidsFindComponentRepresentative(
        collar_container &currentCollar,
        CC &collarComponents,
        std::unordered_map<Voxel,Voxel>& componentRep)
{
    for (Voxel v : currentCollar) {
        Voxel component = collarComponents.find(v);
        if (componentRep.find(component) == componentRep.end())
            componentRep[component] = v;
        else
            componentRep[component] = std::max(v, componentRep[component]);
    }
}

template <class AugmentedShape, class Voxel>
void ShapeOverlay<AugmentedShape, Voxel>::
_updateNumberOfVoidsMergeSameVoidComponents(
        std::function<bool(Voxel)>& isInShape,
        CC &collarComponents,
        std::unordered_map<Voxel,Voxel>& componentRep) {
    for (auto iter : componentRep) {
        Voxel v = iter.second;
        Point p = {v[0], v[1], v[2]};

        bool done = false;
        for (unsigned i = 0; (i < 3) && !done; i++) {
            while (!done && p[i] <= _shape.getShapeDimensions()[i]) {
                p[i]++;
                Voxel cur(p);
                if (!collarComponents.contains(cur) && !isInShape(cur)) {
                    collarComponents.initialize(cur);
                    for (ExtendedNeighborsIterator<Voxel> it(cur); !it.isEnd();
                         it.next()) {
                        Voxel n = it.getCurrentValue();
                        collarComponents.merge(cur, n);
                    }
                } else {
                    done = true;
                }
            }
        }
    }
}

template <class AugmentedShape, class Voxel>
template <class CellIterator>
unsigned int ShapeOverlay<AugmentedShape, Voxel>::_computeNumberOfGeneratedCells(
        Voxel v, std::unordered_set<Voxel> &done,
        voxel_container &voxels) {
    unsigned int nberOfGenCells = 0;

    for (CellIterator it(v); !it.isEnd(); it.next()) {
        typename CellIterator::coordinate_type points = it.getCurrentValue();
        auto p = points.begin();
        bool valid = true;

        while (p != points.end() && valid){
            Voxel cur(*p);
            if ((voxels.find(cur) == voxels.end() &&
                 _savedVoxels.find(cur) == _savedVoxels.end() &&
                 !(_shape.template contains<MAIN_SHAPE>(cur))) ||
                    done.find(cur) != done.end()) {
                valid = false;
            }
            ++p;
        }
        if (valid)
            nberOfGenCells++;
    }

    return nberOfGenCells;
}
