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
 * @file ShapeOverlay.h
 * Contains the declaration of the \ref ShapeOverlay class.
 **/

#include <functional>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "shape/ConnectedComponents.h"
#include "shape/ShapeUtilities.h"
#include "shape/VoxelSkeletonIterators.h"

namespace shape_simplification{ namespace shape {

template <class AugmentedShape, class Voxel>
/**
 * class ShapeOverlay
 * @brief Overlay for an augmented shape,
 * enabling testing of the consequences of adding a set of voxels
 * to a shape without actually adding the voxels to the shape.
 * @tparam AugmentedShape Augmented shape type.
 * @tparam Voxel Voxel class used by the shape.
 **/
class ShapeOverlay {
public:
    using voxel_container = typename AugmentedShape::voxel_container;
    using collar_container = typename AugmentedShape::collar_container;
    using collar_iterator = typename AugmentedShape::collar_iterator;
    template <Level level>
    using neighbor_iterator =
        typename AugmentedShape::template neighbor_iterator<level>;
    template <Level level>
    using extended_neighbor_iterator =
        typename AugmentedShape::template extended_neighbor_iterator<level>;
    using CC = ConnectedComponents<Voxel>;

    /**
     * @brief Constructor.
     * @param shape Shape to test.
     */
    ShapeOverlay(AugmentedShape &shape);

    /**
     * @brief Computes the new Euler characteristic and
     * the new number of components, loops and voids if
     * the given set of voxels were to be added into the
     * main shape. The results are saved until this
     * function is called again. Does not have any effect
     * if called just after \ref removeVoxelsFromMainShape:
     * call \ref clearModifications before.
     * @param voxels Voxels to be tested.
     * @pre Assumes that the given voxels are already
     * in the neighborhood of the shape and the the number
     * of components does not increase.
     */
    void addVoxelsInMainShape(voxel_container &voxels);
    /**
     * @brief Computes the new Euler characteristic and
     * the new number of voids and loops if
     * the given set of voxels were to be added into the
     * main shape. The set of voxels has to guarantee
     * that it does not affect H0, otherwise,
     * the number of loops is falsified.
     * The results are saved until this function is
     * called again. Does not have any effect
     * if called just after \ref removeVoxelsFromMainShape:
     * call \ref clearModifications before.
     * @param voxels Voxels to be tested.
     * @pre Assumes that the given voxels are already
     * in the neighborhood of the shape and the the number
     * of components does not increase.
     */
    void addVoxelsInMainShapeWithH0Guarantee(voxel_container &voxels);
    /**
     * @brief Computes the new Euler characteristic and
     * the new number of components and loops if
     * the given set of voxels were to be added into the
     * main shape. The set of voxels has to guarantee
     * that it does not affect H2, otherwise,
     * the number of loops is falsified.
     * The results are saved until this function is
     * called again. Does not have any effect
     * if called just after \ref removeVoxelsFromMainShape:
     * call \ref clearModifications before.
     * @param voxels Voxels to be tested.
     * @pre Assumes that the given voxels are already
     * in the neighborhood of the shape and the the number
     * of components does not increase.
     */
    void addVoxelsInMainShapeWithH2Guarantee(voxel_container &voxels);
    /**
     * @brief Computes the new Euler characteristic and
     * the new number of loops if
     * the given set of voxels were to be added into the
     * main shape. The set of voxels has to guarantee
     * that it does not affect H0 and H2, otherwise,
     * the number of loops is falsified.
     * The results are saved until this function is
     * called again. Does not have any effect
     * if called just after \ref removeVoxelsFromMainShape:
     * call \ref clearModifications before.
     * @param voxels Voxels to be tested.
     * @pre Assumes that the given voxels are already
     * in the neighborhood of the shape and the the number
     * of components does not increase.
     */
    void addVoxelsInMainShapeWithH0H2Guarantee(voxel_container &voxels);
    /**
     * @brief Computes the new Euler characteristic and
     * the new number of components, loops and voids if
     * the given set of voxels were to be removed from the
     * main shape. The results are saved until this
     * function is called again. Does not have any effect
     * if called just after \ref addVoxelsInMainShape:
     * call \ref clearModifications before.
     * @param voxels Voxels to be tested.
     * @pre Assumes that the the number
     * of components does not increase
     * (use \ref removeVoxelsAndReturnH0Only to test).
     */
    void removeVoxelsFromMainShape(voxel_container &voxels);
    /**
     * @brief Computes the new Euler characteristic and
     * the new number of voids and loops if
     * the given set of voxels were to be removed from the
     * main shape. The set of voxels has to guarantee
     * that it does not affect H0, otherwise,
     * the number of loops is falsified.
     * The results are saved until this function is
     * called again. Does not have any effect
     * if called just after \ref addVoxelsInMainShape:
     * call \ref clearModifications before.
     * @param voxels Voxels to be tested.
     * @pre Assumes that the the number
     * of components does not increase
     * (use \ref removeVoxelsAndReturnH0Only to test).
     */
    void removeVoxelsFromMainShapeWithH0Guarantee(voxel_container &voxels);
    /**
     * @brief Computes the new Euler characteristic and
     * the new number of components and loops if
     * the given set of voxels were to be removed from the
     * main shape. The set of voxels has to guarantee
     * that it does not affect H2, otherwise,
     * the number of loops is falsified. But if the new H2
     * is at most smaller, than the number of loops computed
     * will also not be higher than the true value.
     * The results are saved until this function is
     * called again. Does not have any effect
     * if called just after \ref addVoxelsInMainShape:
     * call \ref clearModifications before.
     * @param voxels Voxels to be tested.
     * @pre Assumes that the the number
     * of components does not increase
     * (use \ref removeVoxelsAndReturnH0Only to test).
     */
    void removeVoxelsFromMainShapeWithH2Guarantee(voxel_container &voxels);
    /**
     * @brief Computes the new Euler characteristic and
     * the new number of loops if
     * the given set of voxels were to be removed from the
     * main shape. The set of voxels has to guarantee
     * that it does not affect H0 and H2, otherwise,
     * the number of loops is falsified. But if the new H0 and H2
     * are at most smaller, than the number of loops computed
     * will also not be higher than the true value.
     * The results are saved until this function is
     * called again. Does not have any effect
     * if called just after \ref addVoxelsInMainShape:
     * call \ref clearModifications before.
     * @param voxels Voxels to be tested.
     * @pre Assumes that the the number
     * of components does not increase
     * (use \ref removeVoxelsAndReturnH0Only to test).
     */
    void removeVoxelsFromMainShapeWithH0H2Guarantee(voxel_container &voxels);

    /**
     * @brief Computes the new Euler characteristic if
     * the given set of voxels were to be added into the
     * main shape. The result is returned but *not stored*.
     * @param voxels Voxels to add.
     * @return The new Euler characteristic.
     */
    int addVoxelsAndReturnEulerOnly(voxel_container &voxels);
    /**
     * @brief Computes the new Euler characteristic if
     * the given set of voxels were to be removed from the
     * main shape. The result is returned but *not stored*.
     * @param voxels Voxels to remove.
     * @return The new Euler characteristic.
     */
    int removeVoxelsAndReturnEulerOnly(voxel_container &voxels);
    /**
     * @brief Computes the new number of components if
     * the given set of voxels were to be added into the
     * main shape. The result is returned but *not stored*.
     * @param voxels Voxels to add.
     * @return The new number of components.
     */
    unsigned int addVoxelsAndReturnH0Only(voxel_container &voxels);
    /**
     * @brief Computes the new number of components if
     * the given set of voxels were to be removed from the
     * main shape. The result is returned but *not stored*.
     * @param voxels Voxels to remove.
     * @return The new number of components.
     */
    unsigned int removeVoxelsAndReturnH0Only(voxel_container &voxels);
    /**
     * @brief Computes the new number of voids if
     * the given set of voxels were to be added into the
     * main shape. The result is returned but *not stored*.
     * @param voxels Voxels to add.
     * @return The new number of voids.
     * @pre Assumes that the the number of components does
     * not increase.
     */
    unsigned int addVoxelsAndReturnH2Only(voxel_container &voxels);
    /**
     * @brief Computes the new number of voids if
     * the given set of voxels were to be removed from the
     * main shape. The result is returned but *not stored*.
     * @param voxels Voxels to remove.
     * @return The new number of voids.
     * @pre Assumes that the the number of components does
     * not increase (use \ref removeVoxelsAndReturnH0Only
     * to test).
     */
    unsigned int removeVoxelsAndReturnH2Only(voxel_container &voxels);

    /**
     * @brief Saves the given voxels as voxels added to the shape.
     * Enables to temporary "validate" modifications which were
     * tested before.
     * @param voxels Set of voxels to save.
     */
    void saveAddedVoxels(voxel_container &voxels);
    /**
     * @brief Saves the given voxels as voxels added to the shape.
     * Enables to temporary "validate" modifications which were
     * tested before.
     * @param voxels Set of voxels to save.
     * @pre Assumes that the voxels do not modify H2.
     */
    void saveAddedVoxelsWithH2Guarantee(voxel_container &voxels);
    /**
     * @brief Saves the given voxels as voxels removed from the shape.
     * Enables to temporary "validate" modifications which were
     * tested before.
     * @param voxels Set of voxels to save.
     */
    void saveRemovedVoxels(voxel_container &voxels);
    /**
     * @brief Saves the given voxels as voxels removed from the shape.
     * Enables to temporary "validate" modifications which were
     * tested before.
     * @param voxels Set of voxels to save.
     * @pre Assumes that the voxels do not modify H2.
     */
    void saveRemovedVoxelsWithH2Guarantee(voxel_container &voxels);
    /**
     * @brief Saves the given voxels as voxels removed from the shape.
     * Enables to temporary "validate" modifications which were
     * tested before.
     * @param voxels Set of voxels to save.
     * @pre Assumes that the voxels do not modify H0.
     */
    void saveRemovedVoxelsWithH0Guarantee(voxel_container &voxels);
    /**
     * @brief Saves the given voxels as voxels removed from the shape.
     * Enables to temporary "validate" modifications which were
     * tested before.
     * @param voxels Set of voxels to save.
     * @pre Assumes that the voxels do not modify H0 and H2.
     */
    void saveRemovedVoxelsWithH0H2Guarantee(voxel_container &voxels);
    /**
     * @brief Clears all modifications generated by the family of
     * \ref addVoxelsInMainShape, \ref removeVoxelsFromMainShape
     * and \ref saveVoxels methods.
     */
    void clearModifications();

    /**
     * @brief Returns the Euler characteristic
     * of the main shape with the saved voxels.
     * @return The actual Euler characteristic
     * of the main shape.
     */
    int getInitialEulerCharacteristic() const;

    /**
     * @brief Returns the number of connected
     * components in the main shape with the saved voxels.
     * @return The actual number of connected
     * components in the main shape.
     */
    unsigned int getInitialNumberOfComponents() const;

    /**
     * @brief Returns the number of loops
     * in the main shape with the saved voxels.
     * @return The actual number of loops
     * in the main shape.
     */
    unsigned int getInitialNumberOfLoops();

    /**
     * @brief Returns the number of voids
     * in the main shape with the saved voxels.
     * @return The actual number of voids
     * in the main shape.
     */
    unsigned int getInitialNumberOfVoids();

    /**
     * @brief Returns the Euler characteristic
     * of the main shape after calling \ref addVoxelsInMainShape
     * or \ref removeVoxelsFromMainShape.
     * @return The new Euler characteristic of the main
     * shape after adding or removing.
     */
    int getCurrentEulerCharacteristic() const;

    /**
     * @brief Returns the number of connected components
     * in the main shape after calling \ref addVoxelsInMainShape
     * or \ref removeVoxelsFromMainShape.
     * @return The new number of connected components
     * in the main shape after adding or removing.
     */
    unsigned int getCurrentNumberOfComponents() const;

    /**
     * @brief Returns the number of loops
     * in the main shape after calling \ref addVoxelsInMainShape
     * or \ref removeVoxelsFromMainShape.
     * @return The new number of loops
     * in the main shape after adding or removing.
     */
    unsigned int getCurrentNumberOfLoops();

    /**
     * @brief Returns the number of voids
     * in the main shape after calling \ref addVoxelsInMainShape
     * or \ref removeVoxelsFromMainShape.
     * @return The new number of voids
     * in the main shape after adding or removing.
     */
    unsigned int getCurrentNumberOfVoids();

    template <Level level>
    /**
     * @brief Tests if the given voxel is contained at given level,
     * taking into account the saved voxels.
     * @tparam level Level to test.
     * @param v Given voxel.
     * @return True is contained, false otherwise.
     */
    bool contains(Voxel v) const;

private:
    AugmentedShape &_shape;                     ///< Underlying shape.
    voxel_container _savedVoxels;               ///< Saved voxels.
    int _currentState;                          ///< -1 when saved voxels have to be removed,
                                                ///< 1 when saved voxels have to be added,
                                                ///< 0 when no status yet.
    int _initialEulerCharacteristic;            ///< Initial Euler characteristic.
    unsigned int _initialNumberOfComponents;    ///< Initial number of connected components.
    unsigned int _initialNumberOfLoops;         ///< Initial number of loops.
    unsigned int _initialNumberOfVoids;         ///< Initial number of voids.
    int _currentEulerCharacteristic;            ///< New Euler characteristic after calling
                                                ///< a test method.
    unsigned int _currentNumberOfComponents;    ///< New number of connected components after
                                                ///< calling a test method.
    unsigned int _currentNumberOfLoops;         ///< New number of loops after calling a test method.
    unsigned int _currentNumberOfVoids;         ///< New number of voids after calling a test method.
    collar_container _savedCollar;              ///< Collar of the shape with saved voxels.
    unsigned int _shapeNumberOfComponents;      ///< Number of connected components in the
                                                ///< underlying shape, without saved voxels.
    bool _savedSinceLastUpdate;                 ///< True if saved since last H2 update.
    bool _testedSinceLastSave;                  ///< True if test method was called since last save.

    /**
     * @brief Common part of all save methods.
     * @param voxels Voxels to save.
     */
    void _commonSaveVoxels(voxel_container &voxels);
    /**
     * @brief Common part of all save H2-safe methods.
     * @param voxels Voxels to save.
     */
    void _commonSaveVoxelsWithH2Guarantee(voxel_container &voxels);

    /**
     * @brief Computes the Euler characteristic after
     * calling \ref addVoxelsInMainShape or \ref removeVoxelsFromMainShape.
     * @param voxels The set of voxels which are added or removed.
     */
    void _updateEulerCharacteristic(voxel_container &voxels);
    /**
     * @brief Computes the Euler characteristic after
     * calling \ref addVoxelsInMainShape or \ref removeVoxelsFromMainShape.
     * @param voxels The set of voxels which are added or removed.
     * @param res Reference to store the new value.
     */
    void _updateEulerCharacteristic(voxel_container &voxels, int &res);
    /**
     * @brief Computes the Euler characteristic to save given voxels.
     * @param voxels Voxels to save.
     */
    void _updateEulerCharacteristicAndSave(voxel_container &voxels);

    /**
     * @brief Computes the number of connected components
     * after calling \ref addVoxelsInMainShape.
     * @param voxels The set of voxels which are added.
     */
    void _updateNumberOfComponentsAfterAdding(voxel_container &voxels);
    /**
     * @brief Computes the number of connected components after
     * calling \ref addVoxelsInMainShape.
     * @param voxels The set of voxels which are added.
     * @param res Reference to store the new value.
     */
    void _updateNumberOfComponentsAfterAdding(
            voxel_container &voxels, unsigned int &res);
    /**
     * @brief Computes the number of connected components to save given voxels.
     * @param voxels Voxels to add and save.
     */
    void _updateNumberOfComponentsAfterAddingAndSave(voxel_container &voxels);
    /**
     * @brief Computes the number of connected components
     * after calling \ref removeVoxelsFromMainShape.
     * @param voxels The set of voxels which are removed.
     */
    void _updateNumberOfComponentsAfterRemoving(voxel_container &voxels);
    /**
     * @brief Computes the number of connected components after
     * calling \ref removeVoxelsFromMainShape.
     * @param voxels The set of voxels which are removed.
     * @param res Reference to store the new value.
     */
    void _updateNumberOfComponentsAfterRemoving(
            voxel_container &voxels, unsigned int &res);
    /**
     * @brief Computes the number of connected components to save given voxels.
     * @param voxels Voxels to remove and save.
     */
    void _updateNumberOfComponentsAfterRemovingAndSave(voxel_container &voxels);

    /**
     * @brief Computes the number of loops
     * after calling \ref addVoxelsInMainShape or
     * \ref removeVoxelsFromMainShape.
     * @pre Assumes that all the other _update functions
     * were already called.
     */
    void _updateNumberOfLoops();
    /**
     * @brief Computes the number of loops to save given voxels.
     * @param voxels Voxels to save.
     */
    void _updateNumberOfLoopsAndSave();

    /**
     * @brief Computes the number of voids
     * after calling \ref addVoxelsInMainShape.
     * @param voxels The set of voxels which are added.
     */
    void _updateNumberOfVoidsAfterAdding(voxel_container &voxels);
    /**
     * @brief Compute the changes of the collar after adding given voxels.
     * @param voxels
     * @param currentCollar
     */
    void _updateNumberOfVoidsAfterAddingComputeCollarChanges(
            voxel_container &voxels,
            collar_container &currentCollar);
    /**
     * @brief Computes the number of voids
     * after calling \ref removeVoxelsFromMainShape.
     * @param voxels The set of voxels which are removed.
     */
    void _updateNumberOfVoidsAfterRemoving(voxel_container &voxels);
    /**
     * @brief Compute the changes of the collar after removing given voxels.
     * @param voxels
     * @param currentCollar
     */
    void _updateNumberOfVoidsAfterRemovingComputeCollarChanges(
            voxel_container &voxels,
            collar_container &currentCollar);
    /**
     * @brief Computes number of voids after collar was updated.
     * @param currentCollar Updated collar.
     * @param isInShape Function returning true if and only if
     * a voxel is contained in the shape taking into account the changes.
     */
    void _updateNumberOfVoids(collar_container &currentCollar,
                              std::function<bool(Voxel)>& isInShape);
    /**
     * @brief Computes number of voids after collar was updated to be saved.
     * @param currentCollar Updated collar.
     * @param isInShape Function returning true if and only if
     * a voxel is contained in the shape taking into account the changes.
     */
    void _updateNumberOfVoidsAndSave(collar_container &currentCollar,
                                     std::function<bool(Voxel)>& isInShape);

    template <class CellIterator>
    /**
     * @brief Computes the number of new cells among those
     * given by the iterator class created by the insertion or removal
     * of the given voxel.
     * @tparam CellIterator Iterator over the candidate cells.
     * @param v Current voxel added or removed.
     * @param done Voxels already added previously.
     * @param voxels Set of voxels which are added.
     * @return The number of new cells created by the insertion of @p v.
     */
    unsigned int _computeNumberOfGeneratedCells(
            Voxel v,
            std::unordered_set<Voxel> &done,
            voxel_container &voxels);

    /**
     * @brief Auxiliary function of \ref _updateNumberOfVoids.
     * @param currentCollar
     * @param collarComponents
     */
    void _updateNumberOfVoidsFindCollarComponents(
            collar_container &currentCollar,
            CC &collarComponents);

    /**
     * @brief Auxiliary function of \ref _updateNumberOfVoids.
     * @param currentCollar
     * @param collarComponents
     * @param componentRep
     */
    void _updateNumberOfVoidsFindComponentRepresentative(
            collar_container &currentCollar,
            CC &collarComponents,
            std::unordered_map<Voxel,Voxel>& componentRep);

    /**
     * @brief Auxiliary function of \ref _updateNumberOfVoids.
     * @param isInShape
     * @param collarComponents
     * @param componentRep
     */
    void _updateNumberOfVoidsMergeSameVoidComponents(
            std::function<bool(Voxel)>& isInShape,
            CC &collarComponents,
            std::unordered_map<Voxel,Voxel>& componentRep);
};

#include "ShapeOverlay.txx"

}} // namespace shape_simplification::shape
