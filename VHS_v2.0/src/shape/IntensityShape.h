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
 * @file IntensityShape.h
 * Contains the declaration of the \ref IntensityShape class.
 **/

#include <array>
#include <unordered_map>
#include <cmath>

#include "shape/Voxel.h"
#include "util/ProcessedImageStack.h"

namespace shape_simplification{ namespace shape {

using util::ProcessedImageStack;

/**
 * class IntensityShape
 * @brief Stores the neighborhood voxels by intensity values independently
 * from its position in the core, shape or neighborhood. But as the different
 * thresholds are still stored, the class can nevertheless iterate over the
 * desired level.
 **/
class IntensityShape {
public:
    /// Enum class for the different levels in the shape.
    enum Level {
        CORE,               ///< Core of the shape.
        MAIN_SHAPE,         ///< Shape to be simplified.
        NEIGHBORHOOD,       ///< Neighborhood of the shape.
        OUTSIDE,            ///< Inside none of the above levels.
        EXTENDEDN,          ///< Extended neighborhood.
                            ///< Still considered outside, but can fill holes.
        EX_EXTENDEDN,       ///< Extended neighborhood - neighborhood
                            ///< (or vise versa).
        EX_NEIGHBORHOOD,    ///< Neighborhood - shape.
        EX_MAIN_SHAPE,      ///< Shape - core.
    };

    using voxel_container = std::vector<Voxel>;
    template <Level level>
    /**
     * @brief Iterator class for the shape at given level by intensity value.
     * @tparam level Level to iterate.
     */
    class LevelIterator {
    public:
        /**
         * @brief Constructor.
         * @param shape IntensityShape to iterate.
         * @param end If true, indicates that the iterator is an end iterator.
         */
        LevelIterator(const IntensityShape &shape, bool end = false);

        LevelIterator &operator++();
        LevelIterator operator++(int);

        const Voxel &operator*() const;
        bool operator==(const LevelIterator &other) const;
        bool operator!=(const LevelIterator &other) const;

    private:
        const IntensityShape& _shape;   ///< Reference to the shape.
        unsigned int _currentLevel;     ///< Current level.
        unsigned int _currentIt;        ///< Current position.
        unsigned int _endLevel;         ///< End index of the current level.
    };
    template <Level level = EXTENDEDN>
    using voxel_iterator = class LevelIterator<level>;

    template <Level level>
    /**
     * @brief Reverse iterator class for the shape at given level by
     * intensity value.
     * @tparam level Level to reverse iterate.
     */
    class ReverseLevelIterator {
    public:
        /**
         * @brief Constructor.
         * @param shape IntensityShape to reverse iterate.
         * @param end If true, indicates that the reverse iterator is an
         * end iterator.
         */
        ReverseLevelIterator(const IntensityShape &shape, bool end = false);

        ReverseLevelIterator &operator++();
        ReverseLevelIterator operator++(int);

        const Voxel &operator*() const;
        bool operator==(const ReverseLevelIterator &other) const;
        bool operator!=(const ReverseLevelIterator &other) const;

    private:
        const IntensityShape& _shape;   ///< Reference to the shape.
        int _currentLevel;              ///< Current level.
        int _currentIt;                 ///< Current position.
        int _endLevel;                  ///< End index of the current level.
    };
    template <Level level = EXTENDEDN>
    using voxel_reverse_iterator = class ReverseLevelIterator<level>;

    /**
     * @brief Constructor for an empty shape.
     */
    IntensityShape();
    /**
     * @brief Constructor.
     * @param stack Stack of PNG images to load. Assumes that the threshold
     * value of the core is heigher than the one of the shape which is higher
     * than the one of the neighborhood.
     * @param extendedNThreshold Threshold value of the extended Neighborhood.
     * Has to be lower than the shape threshold.
     */
    IntensityShape(ProcessedImageStack &stack,
                   unsigned char extendedNThreshold);

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
     * *It will fail compiling if called for this class.*
     * @tparam minLevel Minimum level the voxel has to be added to.
     * @param v Voxel to be added.
     */
    template <Level minLevel> void addVoxel(Voxel v);

    /**
     * @brief Removes a voxel from the shape from level @p maxLevel and lower.
     * *It will fail compiling if called for this class.*
     * @tparam maxLevel Maximum level the voxel has to be removed from.
     * @param v Voxel to be removed.
     */
    template <Level maxLevel> void removeVoxel(Voxel v);

    /**
     * @brief Tests if the shape contains a voxel at level @p level.
     * @tparam level Level to be tested.
     * @param v Voxel to be tested.
     * @return True if the shape contains @p v at level @p level, false
     * otherwise.
     */
    template <Level level = EXTENDEDN>
    bool contains(Voxel v) const;
    /**
     * @brief Tests if the shape contains a voxel at level @p level.
     * @tparam level Level to be tested.
     * @param x X-coordinate of the voxel.
     * @param y Y-coordinate of the voxel.
     * @param z Z-coordinate of the voxel.
     * @return True if the shape contains the given voxel at level @p level,
     * false otherwise.
     */
    template <Level level = EXTENDEDN>
    bool contains(int x, int y, int z) const;

    /**
     * @brief Returns an iterator at the start of the voxels contained at
     * level @p level. The voxels are ordered by pixel grayscale value.
     * @tparam level Level of the voxels to iterate over.
     * @return Starting iterator for level @p level.
     */
    template <Level level = EXTENDEDN>
    voxel_iterator<level> begin() const;
    /**
     * @brief Returns the end iterator of the voxels contained at level
     * @p level.
     * @tparam level Level of the voxels to iterate over.
     * @return End iterator for level @p level.
     */
    template <Level level = EXTENDEDN>
    voxel_iterator<level> end() const;

    /**
     * @brief Returns a reverse iterator at the end of the voxels contained at
     * level @p level. The voxels are ordered by pixel grayscale value.
     * @tparam level Level of the voxels to iterate over.
     * @return Starting iterator for level @p level.
     */
    template <Level level = EXTENDEDN>
    voxel_reverse_iterator<level> rbegin() const;
    /**
     * @brief Returns the end reverse iterator of the voxels contained at
     * level @p level.
     * @tparam level Level of the voxels to iterate over.
     * @return End reverse iterator for level @p level.
     */
    template <Level level = EXTENDEDN>
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
     * @brief Returns number of voxels in the core of the shape.
     * @return Number of voxels contained in the core.
     */
    unsigned int getCoreSize();

    /**
     * @brief Empties the shape.
     */
    void clear();

    /**
     * @brief Returns the core threshold value.
     * @return Core threshold value.
     */
    unsigned int getCoreThreshold() const;
    /**
     * @brief Sets the core threshold.
     * @param coreThreshold New core threshold.
     */
    void setCoreThreshold(unsigned int coreThreshold);

    /**
     * @brief Returns the shape threshold value.
     */
    unsigned int getShapeThreshold() const;
    /**
     * @brief Sets the shape threshold.
     * @param shapeThreshold New shape threshold.
     */
    void setShapeThreshold(unsigned int shapeThreshold);

    /**
     * @brief Returns the neighborhood threshold value.
     * @return Neighborhood threshold value.
     */
    unsigned int getNeighborhoodThreshold() const;
    /**
     * @brief Sets the neighborhood threshold.
     * @param neighborhoodeThreshold New neighborhood threshold.
     */
    void setNeighborhoodThreshold(unsigned int neighborhoodThreshold);

    /**
     * @brief Returns the extended neighborhood threshold value.
     * @return Extended neighborhood threshold value.
     */
    unsigned int getExtendedNeighborhoodThreshold() const;
    /**
     * @brief Sets the extended neighborhood threshold.
     * @param extendedNeighborhoodThreshold New extended neighborhood threshold.
     */
    void setExtendedNeighborhoodThreshold(
            unsigned int extendedNeighborhoodThreshold);

    /**
     * @brief Returns voxels with given intensity value.
     * @param grayscaleValue Intensity value.
     * @return Reference to the set of voxels with given intensity value.
     */
    const voxel_container& getVoxelsWithGrayscale(
            unsigned char grayscaleValue) const;

private:
    std::unordered_map<Voxel, unsigned char> _levels;   ///< Map from Voxel to its grayscale value.
    std::array<voxel_container, 256> _voxels;           ///< Voxels contained in the shape.
    std::array<unsigned int, 3> _shapeDimensions;       ///< Maximal value of a Voxel coordinate.
    unsigned char _coreThreshold;                       ///< Core threshold value.
    unsigned char _shapeThreshold;                      ///< Shape threshold value.
    unsigned char _neighborhoodThreshold;               ///< Neighborhood threshold value.
    unsigned char _extendedNeighborhoodThreshold;       ///< Extended neighborhood threshold value.
};

#include "IntensityShape.txx"

}} // namespace shape_simplification::shape
