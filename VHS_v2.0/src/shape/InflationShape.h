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
 * @file InflationShape.h
 * Contains the declaration of the \ref Shape class.
 **/

#include <array>
#include <unordered_map>
#include <cmath>

#include "shape/Voxel.h"
#include "shape/ShapeUtilities.h"
#include "shape/ConnectedComponents.h"
#include "shape/ShapeUtilities.h"
#include "util/ProcessedImageStack.h"

namespace shape_simplification{ namespace shape {

using util::ProcessedImageStack;

/**
 * class InflationShape
 * @brief Stores the shape and neighborhood voxels by their distance from
 * the shape. I.e., all shape voxels are considered to be at distance 0 and
 * all remaining neighborhood voxels are sorted by distance (manhatten)
 * from the nearest shape voxel. The neighborhood is defined over the set
 * of voxels in the usual intensity neighborhood, but also over voxels
 * within voids inside the neighborhood. Then, only voxels at a given or
 * computed inflation value of the shape are kept.
 **/
class InflationShape {
public:
    using voxel_container = std::vector<Voxel>;

    template <Level level>
    /**
     * @brief Iterator class for the shape at given level by distance.
     * @tparam level Level to iterate.
     */
    class LevelIterator {
    public:
        /**
         * @brief Constructor.
         * @param shape InflationShape to iterate.
         * @param end If true, indicates that the iterator is an end iterator.
         */
        LevelIterator(const InflationShape &shape, bool end = false);

        LevelIterator &operator++();
        LevelIterator operator++(int);

        const Voxel &operator*() const;
        bool operator==(const LevelIterator &other) const;
        bool operator!=(const LevelIterator &other) const;

    private:
        const InflationShape& _shape;   ///< Reference to the shape.
        unsigned int _currentLevel;     ///< Current level.
        unsigned int _currentIt;        ///< Current position.
        unsigned int _endLevel;         ///< End index of the current level.
    };
    template <Level level = NEIGHBORHOOD>
    using voxel_iterator = class LevelIterator<level>;

    template <Level level>
    /**
     * @brief Reverse Iterator class for the shape at given level by distance.
     * @tparam level Level to reverse iterate.
     */
    class ReverseLevelIterator {
    public:
        /**
         * @brief Constructor.
         * @param shape InflationShape to reverse iterate.
         * @param end If true, indicates that the iterator is an end iterator.
         */
        ReverseLevelIterator(const InflationShape &shape, bool end = false);

        ReverseLevelIterator &operator++();
        ReverseLevelIterator operator++(int);

        const Voxel &operator*() const;
        bool operator==(const ReverseLevelIterator &other) const;
        bool operator!=(const ReverseLevelIterator &other) const;

    private:
        const InflationShape& _shape;   ///< Reference to the shape.
        int _currentLevel;              ///< Current level.
        int _currentIt;                 ///< Current position.
        int _endLevel;                  ///< End index of the current level.
    };
    template <Level level = NEIGHBORHOOD>
    using voxel_reverse_iterator = class ReverseLevelIterator<level>;

    /**
     * @brief Constructor for an empty shape.
     */
    InflationShape();
    /**
     * @brief Constructor.
     * @param stack Stack of PNG images to load. Assumes that the threshold
     * value of the core is heigher than the one of the shape which is higher
     * than the one of the neighborhood.
     * @param inflationValue Maximal distance of a voxel to store as
     * neighborhood. If the given value is 0, the maximal distance is defined
     * as the maximal distance of a neighborhood voxel from the shape in the
     * original stack.
     * @param nonVoidLocalMaxima If true, also stores sets of minimum size
     * of voxels in local maxima outside of the neighborhood collar as collar
     * elements.
     */
    InflationShape(ProcessedImageStack &stack,
                   unsigned int inflationValue,
                   bool nonVoidLocalMaxima);
    /**
     * @brief Constructor. The inflation value is computed such that every
     * intensity neighborhood voxel is included in the inflation neighborhood.
     * @param stack Stack of PNG images to load. Assumes that the threshold
     * value of the core is heigher than the one of the shape which is higher
     * than the one of the neighborhood.
     * @param nonVoidLocalMaxima If true, also stores sets of minimum size
     * of voxels in local maxima outside of the neighborhood collar as
     * collar elements.
     */
    InflationShape(ProcessedImageStack &stack,
                   bool nonVoidLocalMaxima);

    /**
     * @brief Tests if the class has the property to add voxels to the shape
     * after instanciation.
     * @return False.
     */
    static constexpr bool isAddable();

    /**
     * @brief Tests if the class has the property to remove voxels from the
     * shape after instanciation.
     * @return False.
     */
    static constexpr bool isRemovable();

    /**
     * @brief Adds a voxel to the shape to level @p minLevel and higher.
     * @tparam minLevel Minimum level the voxel has to be added to.
     * @param v Voxel to be added.
     */
    template <Level minLevel>
    void addVoxel(Voxel v);

    /**
     * @brief Removes a voxel from the shape from level @p maxLevel and lower.
     * *It will fail compiling if called for this class.*
     * @tparam maxLevel Maximum level the voxel has to be removed from.
     * @param v Voxel to be removed.
     */
    template <Level maxLevel>
    void removeVoxel(Voxel v);

    /**
     * @brief Tests if the shape contains a voxel at level @p level.
     * @tparam level Level to be tested.
     * @param x X-coordinate of the voxel.
     * @param y Y-coordinate of the voxel.
     * @param z Z-coordinate of the voxel.
     * @return True if the shape contains the given voxel at level @p level,
     * false otherwise.
     */
    template <Level level = NEIGHBORHOOD>
    bool contains(int x, int y, int z) const;

    /**
     * @brief Returns an iterator at the start of the voxels contained at level
     * @p level. The voxels are ordered by distance from the shape.
     * @tparam level Level of the voxels to iterate over.
     * @return Starting iterator for level @p level.
     */
    template <Level level = NEIGHBORHOOD>
    voxel_iterator<level> begin() const;
    /**
     * @brief Returns the end iterator of the voxels contained at level
     * @p level.
     * @tparam level Level of the voxels to iterate over.
     * @return End iterator for level @p level.
     */
    template <Level level = NEIGHBORHOOD>
    voxel_iterator<level> end() const;

    /**
     * @brief Returns a reverse iterator at the end of the voxels contained
     * at level @p level. The voxels are ordered by distance from the shape.
     * @tparam level Level of the voxels to iterate over.
     * @return Reverse iterator for level @p level.
     */
    template <Level level = NEIGHBORHOOD>
    voxel_reverse_iterator<level> rbegin() const;
    /**
     * @brief Returns the end reverse iterator of the voxels contained at level
     * @p level.
     * @tparam level Level of the voxels to iterate over.
     * @return End reverse iterator for level @p level.
     */
    template <Level level = NEIGHBORHOOD>
    voxel_reverse_iterator<level> rend() const;

    /**
     * @brief Returns the lowest level the voxel @p v is contained in.
     * @param v Voxel for which the information is needed.
     * @return The lowest level @p v is contained in.
     */
    unsigned int getMinLevel(Voxel v);
    /**
     * @brief Returns the lowest level the given voxel is contained in.
     * @param x X-coordinate of the voxel.
     * @param y Y-coordinate of the voxel.
     * @param z Z-coordinate of the voxel.
     * @return The lowest level the given voxel is contained in.
     */
    unsigned int getMinLevel(int x, int y, int z);

    /**
     * @brief Returns an array width all three maximal coordinate
     * values of a voxel in the shape.
     * @return An array where [0] is the maximal x coordinate,
     * [1] the maximal y coordinate and [2] the maximal z coordinate.
     */
    const std::array<unsigned int, 3> &getShapeDimensions() const;

    /**
     * @brief Returns total number of voxels.
     * @return Total number of voxels contained in the class.
     */
    unsigned int getNeighborhoodSize();
    /**
     * @brief Returns number of voxels contained in the shape,
     * excluding neighborhood.
     * @return Number of voxels contained in the shape.
     */
    unsigned int getShapeSize();

    /**
     * @brief Empties the shape.
     */
    void clear();

    /**
     * @brief Returns the shape threshold value given by the initial stack.
     * @return Shape threshold value.
     */
    unsigned int getShapeThreshold() const;
    /**
     * @brief Returns the neighborhood threshold value given by the initial
     * stack.
     * @return Neighborhood threshold value.
     */
    unsigned int getNeighborhoodThreshold() const;

    /**
     * @brief Returns set of voxels which are exactly at the given distance
     * from the shape.
     * @param distance Distance of the voxels.
     * @return Reference to the container of voxels at distance @p distance
     * from the shape.
     */
    const voxel_container& getVoxelsAtDistance(unsigned int distance) const;

    /**
     * @brief Returns the inflation value the neighborhood was computed with.
     * @return Value of the inflation value.
     */
    unsigned int getInflationValue() const;

    /**
     * @brief Returns local maxima of the shape. By default, it consists
     * only of the neighborhood collar. To include all local maxima, see
     * constructor option.
     * @return Reference to the stored local maxima.
     */
    const ConnectedComponents<Voxel>& getLocalMaxima() const;

    /**
     * @brief Returns the maximal distance of a neighborhood voxel from the
     * shape. Can be different from the inflation value, if the latter is
     * higher than the maximal distance in the intensity neighborhood.
     * @return Maximal distance of a neighborhood voxel from the
     * shape.
     */
    unsigned int getNumberOfLevels();

private:
    std::unordered_map<Voxel, unsigned int> _levels;    ///< Map from Voxel to its distance value.
    std::vector<voxel_container> _voxels;               ///< Voxels contained in the shape.
    std::array<unsigned int, 3> _shapeDimensions;       ///< Maximal value of a Voxel coordinate.
    unsigned char _shapeThreshold;                      ///< Shape threshold.
    unsigned char _neighborhoodThreshold;               ///< Neighborhood threshold.
    ConnectedComponents<Voxel> _localMaxima;            ///< Local maxima in the neighborhood.

    /**
     * @brief Shape constructor.
     * @param stack Stack of PNG images to load.
     * @param inflationValue Maximal distance of a voxel to store as
     * neighborhood.
     * @return The number of voxels.
     */
    int _constructShape(ProcessedImageStack &stack,
                        unsigned int inflationValue);
    /**
     * @brief Shape constructor.
     * @param stack Stack of PNG images to load.
     * @return The number of voxels.
     */
    int _constructShape(ProcessedImageStack &stack);
    /**
     * @brief Helping methods to compute the collar.
     * @param collar
     * @param components
     * @param stack
     */
    void _mergeSameVoidCollarComponents(
            ConnectedComponents<Voxel>& collar,
            const std::vector<std::vector<Voxel> >& components,
            ProcessedImageStack &stack);
    /**
     * @brief Computes local maxima with only collar.
     */
    void _computeVoidCollar();
    /**
     * @brief Computes local maxima.
     */
    void _computeLocalMaxima();
    /**
     * @brief Adds voxel as being at given distance.
     * @param v Voxel to add.
     * @param level Distance.
     */
    void _addAtLevel(Voxel& v, unsigned int level);
};

#include "InflationShape.txx"

}} // namespace shape_simplification::shape
