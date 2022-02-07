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
 * @file Simplifier.h
 * Contains the declaration of the \ref Simplifier class.
 **/

#include <ctime>
#include <iostream>

#include "shape/EditableShape.h"
#include "shape/AugmentedShape.h"
#include "shape/RestrictedShapeCopy.h"
#include "shape/InvertedRestrictedShapeCopy.h"
#include "shape/ClosedRestrictedShapeCopy.h"
#include "shape/ClosedInvertedRestrictedShapeCopy.h"
#include "shape/Voxel.h"
#include "shape/ShapeUtilities.h"
#include "simplifier/AdditiveCandidateGenerator.h"
#include "simplifier/SubstractiveCandidateGenerator.h"
#include "simplifier/CandidateValidator.h"
#include "simplifier/DistancePseudoVoidsCreator.h"
#include "util/FileIO.h"
#include "util/ProcessedImageStack.h"
#include "util/RawImageStack.h"

namespace shape_simplification{ namespace simplifier {

using shape_simplification::shape::Voxel;
using shape_simplification::shape::NeighborsInIterator;
using shape_simplification::shape::AddableShape;
using shape_simplification::shape::EditableShape;
using shape_simplification::shape::AugmentedShape;
using shape_simplification::shape::RestrictedShapeCopy;
using shape_simplification::shape::InvertedRestrictedShapeCopy;
using shape_simplification::shape::ClosedRestrictedShapeCopy;
using shape_simplification::shape::ClosedInvertedRestrictedShapeCopy;
using shape_simplification::util::ProcessedImageStack;
using shape_simplification::util::RawImageStack;

/**
 * class Simplifier
 * @brief Interface to simplify a shape from a given stack of PNG images.
 **/
class Simplifier {
public:
    using Shape = EditableShape<Voxel>;
    using AugShape = AugmentedShape<Shape,Voxel>;
    using RestrShape = RestrictedShapeCopy<AugShape,Voxel>;
    using InvRestrShape = InvertedRestrictedShapeCopy<AugShape,Voxel>;
    using ClRestrShape = ClosedRestrictedShapeCopy<AugShape,Voxel>;
    using ClInvRestrShape = ClosedInvertedRestrictedShapeCopy<AugShape,Voxel>;

    using voxel_container =
        typename AdditiveCandidateGenerator<AugShape,Voxel>::voxel_container;
    using candidate_container =
        typename AdditiveCandidateGenerator<AugShape,Voxel>::candidate_container;
    using valid_candidate_container =
        CandidateValidator<AugShape,Voxel>::valid_candidate_container;

    using lower_point_container = std::unordered_set<Point>;

    /**
     * @brief Structure to store the different parameters and options
     * to use to load the image stack.
     */
    struct ImageData{
        /**
         * @brief Input base name for input images.
         * That is, the part of the absolute or relative file paths
         * before "xxxx.png", where 'xxxx' has to be the numbering
         * of the images with 4 digits.
         * E.g. 'directory/path/flower_' if the files are named
         * 'directory/path/flower_xxxx.png'.
         * See README for more details.
         */
        std::string inputFileNameBase;
        /**
         * @brief Output base name for output images.
         * That is, the part of the absolute or relative file paths
         * before "xxxx.png", where 'xxxx' will be the numbering
         * of the images with 4 digits.
         * E.g. 'directory/path/flower_' if the files should be named
         * 'directory/path/flower_xxxx.png'.
         * See README for more details.
         */
        std::string outputFileNameBase;
        unsigned int numberOfImages = 0;            ///< Number of images to load.
        unsigned int minNumberOfComponents = 1;     ///< Optimal value for Betti_0
        unsigned int minNumberOfLoops = 0;          ///< Optimal value for Betti_1
        unsigned int minNumberOfVoids = 0;          ///< Optimal value for Betti_2
        double minRatio = 100.0;                    ///< Percentage of errors to resolve.
        unsigned int downsampling = 1;              ///< Downsampling value for raw images.
        unsigned int noise = 0;                     ///< Noise to add for raw images.
        unsigned char coreThreshold = 200;          ///< Core Threshold.
        unsigned char shapeThreshold = 150;         ///< Shape Threshold.
        unsigned char neighborhoodThreshold = 100;  ///< Neighborhood Threshold.
    };

    /**
     * @brief Constructor.
     */
    Simplifier(bool silence = false);
    /**
     * @brief Destructor.
     */
    ~Simplifier();

    /**
     * @brief Loads image stack from given data.
     * Ignores values for downsampling and noise.
     * @param data Structure containing all necessary
     * information to load the image stack.
     */
    void loadPreprocessedImage(ImageData &data, bool pseudovoids = false);
    /**
     * @brief Loads image stack from given data with
     * downsampling and possibly noise addition.
     * @param data Structure containing all necessary
     * information to load the image stack.
     */
    void loadRawImage(ImageData &data);
    /**
     * @brief Writes the result of the simplification
     * as an png image stack using the file name base
     * given by @p data.
     * @param data Structure containing all necessary
     * information to write the image stack.
     */
    void saveImage(ImageData &data);

    /**
     * @brief Simplifies the latest loaded shape.
     * @param boxWidth Width of boxes to use for persistence.
     * @param ccSize Maximal size of a connected component to remove.
     * Give negative value if no limit.
     * @param noover True if the boxes should not overlap, false otherwise.
     * See README file for more detailed explanation.
     */
    void simplify(
            unsigned int boxWidth, int ccSize = -1, bool noover = false);
    /**
     * @brief Simplifies the latest loaded shape,
     * but searches for removals first (inverse of \ref simplify).
     * @param boxWidth Width of boxes to use for persistence.
     * @param ccSize Maximal size of a connected component to remove.
     * Give negative value if no limit.
     * @param noover True if the boxes should not overlap, false otherwise.
     * See README file for more detailed explanation.
     */
    void simplifyWithSubFirst(
            unsigned int boxWidth, int ccSize = -1, bool noover = false);
    /**
     * @brief Simplifies the latest loaded shape,
     * but uses only additions.
     * @param boxWidth Width of boxes to use for persistence.
     * @param ccSize Maximal size of a connected component to remove.
     * Give negative value if no limit.
     * @param noover True if the boxes should not overlap, false otherwise.
     * See README file for more detailed explanation.
     */
    void simplifyOnlyAdditively(
            unsigned int boxWidth, bool noover = false);
    /**
     * @brief Simplifies the latest loaded shape,
     * but uses only removals.
     * @param boxWidth Width of boxes to use for persistence.
     * @param ccSize Maximal size of a connected component to remove.
     * Give negative value if no limit.
     * @param noover True if the boxes should not overlap, false otherwise.
     * See README file for more detailed explanation.
     */
    void simplifyOnlySubstractively(
            unsigned int boxWidth, int ccSize = -1, bool noover = false);

    /**
     * @brief Prints basic data of the current state of the shape:
     *
     * dimensions, number of voxels in the core, number of non-core voxels
     * in the shape, total number of voxels in the shape, number of voxels
     * in the neighborhood, Euler characteristic, number of components,
     * number of loops, number of voids.
     */
    void printShapeData();

private:
    AugShape _shape;                        ///< Latest loaded shape.
    unsigned int _boxWidth;                 ///< Box width.
    bool _noover;                           ///< True if the boxes should not overlap.
    Point _maxCoordinate;                   ///< Maximal coordinate of a voxel.
    unsigned int _minNumberOfComponents;    ///< Goal for number of connected components.
    unsigned int _minNumberOfLoops;         ///< Goal for number of loops.
    unsigned int _minNumberOfVoids;         ///< Goal for number of voids.
    unsigned int _optNumberOfVoids;         ///< Best possible number of voids.

    clock_t _time;                          ///< Stores time for timing.
    bool _silent;                           ///< True if no output in the terminal.

    bool _simplifyWithAdditions(
            AdditiveCandidateGenerator<AugShape,Voxel>& gen,
            CandidateValidator<AugShape,Voxel>& val,
            lower_point_container& lowerPoints);
    bool _simplifyWithSubstractions(
            SubstractiveCandidateGenerator<AugShape,Voxel>& gen,
            CandidateValidator<AugShape,Voxel>& val,
            lower_point_container& lowerPoints);

    bool _applyAdditiveSimplification(const valid_candidate_container& voxels);
    bool _applyAdditiveSimplification(const candidate_container& voxelSets);
    bool _applyAdditiveSimplification(const valid_candidate_container& voxels,
                                      lower_point_container& currentLP);
    bool _applyAdditiveSimplification(const candidate_container& voxelSets,
                                      lower_point_container& currentLP);

    bool _applySubstractiveSimplification(
            const valid_candidate_container& voxels);
    bool _applySubstractiveSimplification(
            const candidate_container& voxelSets);
    bool _applySubstractiveSimplification(
            const valid_candidate_container& voxels,
            lower_point_container& currentLP);
    bool _applySubstractiveSimplification(
            const candidate_container& voxelSets,
            lower_point_container& currentLP);

    void _computeInitialLowerPoints(lower_point_container& lowerPoints);
    void _getCorrespondingLowerPoints(Voxel v, lower_point_container& points);

    void _resetTimer();
    void _printTime(std::string message);
    void _printAndResetTime(std::string message);

    void _printMessage(std::string message);

    void _printParameterData(std::string message);
    void _printParameterData(int ccSize, std::string message);
};

}} // namespace shape_simplification::simplifier
