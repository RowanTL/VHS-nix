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
 * @file DistancePseudoVoidsCreator.h
 * Contains the declaration of the \ref DistancePseudoVoidsCreator class.
 **/

#include <iostream>
#include <iomanip>
#include <ctime>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <queue>

#include "shape/ShapeUtilities.h"
#include "shape/Voxel.h"
#include "shape/Cell.h"
#include "util/ProcessedImageStack.h"
#include "shape/ConnectedComponents.h"
#include "shape/IntensityShape.h"
#include "shape/InflationShape.h"

namespace shape_simplification{ namespace simplifier {

using shape_simplification::shape::Voxel;
using shape_simplification::shape::ConnectedComponents;
using shape_simplification::util::ProcessedImageStack;

/**
 * class DistancePseudoVoidsCreator
 * @brief Realizes and fills pseudo-voids in the given shape.
 **/
class DistancePseudoVoidsCreator {
public:
    template <int dim>
    using cell_iterator = CellContainer::cell_iterator<dim>;

    /**
     * @brief Constructor.
     * @param stack Stack of PNG images to load.
     * @param inflationValue Maximum distance the pseudo-voids have to be
     * searched in. If the given value is 0, the maximal distance is
     * defined as the maximal distance of a neighborhood voxel from the
     * shape in the original stack.
     * @param silent If true, no text is output in the terminal.
     */
    DistancePseudoVoidsCreator(ProcessedImageStack& stack,
                               unsigned int inflationValue = 0,
                               bool silent = false);

    /**
     * @brief Adds at the top an additional layer to the stack of
     * images with only shape pixels within a rectangle of minimal size
     * containing all shape voxels in the adjacent layer.
     */
    void closeTop();
    /**
     * @brief Removes the additional layer previously added with
     * the call of \ref closeTop.
     */
    void removeTop();

    /**
     * @brief Builds pseudo-voids as real voids in the shape.
     */
    void createPseudoVoids();

    /**
     * @brief Fill all voids in the current shape. This takes into
     * account the pseudo-voids if \ref createPseudoVoids was called
     * before.
     */
    void fillVoids();

private:
    ProcessedImageStack& _stack;    ///< Image stack of the shape.
    unsigned int _inflationValue;   ///< Inflation value.

    clock_t _time;                  ///< Stores time for timing.
    bool _silent;                   ///< True if no output in the terminal.

    /**
     * @brief Helper method for \ref createPseudoVoids.
     * @param patchCC Empty container for candidates to finalize pseudo-voids.
     * @param shape Inflation shape to search in.
     */
    void _computeVoidPatchComponents(
            ConnectedComponents<Voxel>& patchCC,
            InflationShape& shape);
    /**
     * @brief Helper method for \ref createPseudoVoids.
     * @param cc Empty container for the inflation starting components used
     * to indentify pseudo-voids.
     * @param shape Inflation shape to search in.
     */
    void _computeSeedCollarComponents(
            ConnectedComponents<Voxel>& cc,
            InflationShape& shape);
    /**
     * @brief Helper method for \ref createPseudoVoids.
     * @param patchCC Candidates for pseudo-voids realizations.
     * @param patchs Empty container for the valid pseudo-voids patches.
     * @param shape Inflation shape to search in.
     */
    void _computeValidPatches(
            ConnectedComponents<Voxel>& patchCC,
            std::vector<std::vector<Voxel> >& patchs,
            InflationShape& shape);

    /**
     * @brief Tests if a neighborhood voxel is directly adjacent to the shape.
     * @param v Voxel to test.
     * @param shape Inflation shape containing the neighborhood.
     * @return True if the voxel is adjacent to the shape, false otherwise.
     */
    bool _isAdjacentToShape(Voxel &v, InflationShape& shape);

    /**
     * @brief Builds the void from the valid pseudo-void patch candidates.
     * @param patch Valid pseudo-void patch candidates.
     */
    void _createVoid(std::vector<Voxel>& patch);

    /**
     * @brief Computes the shape collar somponents.
     * @param cc Empty container for the connected components.
     * @param shape Shape to compute the collar of.
     * @return The most top right voxel in the collar,
     * i.e., a representative of the outside collar.
     */
    Voxel _computeCollarComponents(
            ConnectedComponents<Voxel>& cc,
            IntensityShape& shape);
    /**
     * @brief Helper method for \ref _computeCollarComponents.
     * @param cc
     * @param ccRep
     * @param shape
     */
    void _mergeSameVoidCollarComponents(
            ConnectedComponents<Voxel>& cc,
            std::unordered_map<Voxel,Voxel>& ccRep,
            IntensityShape& shape);

    /**
     * @brief Resets the timer.
     */
    void _resetTimer();
    /**
     * @brief Prints the current mesured time with the given message.
     * @param message Message to display before the time value.
     */
    void _printTime(std::string message);
    /**
     * @brief Prints the current mesured time with the given message
     * and resets the timer.
     * @param message Message to display before the time value.
     */
    void _printAndResetTime(std::string message);

    /**
     * @brief Prints the given message.
     * @param message Message to print.
     */
    void _printMessage(std::string message);
};

}} // namespace shape_simplification::simplifier
