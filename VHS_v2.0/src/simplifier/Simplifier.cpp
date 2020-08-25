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

#include "Simplifier.h"

namespace shape_simplification::simplifier {

Simplifier::Simplifier(bool silence) : _silent(silence)
{}

Simplifier::~Simplifier()
{}

void Simplifier::loadPreprocessedImage(ImageData &data)
{
    ProcessedImageStack proc;
    _shape.clear();
    _resetTimer();

    _printMessage("--Load image.");

    proc.load(data.inputFileNameBase, data.numberOfImages);
    proc.setCoreThreshold(data.coreThreshold);
    proc.setShapeThreshold(data.shapeThreshold);
    proc.setNeighborhoodThreshold(data.neighborhoodThreshold);

    _printAndResetTime("Image load time:");
    _printMessage("--Load shape.");

    proc >> _shape;

    _printTime("Shape load time:");

    const std::array<unsigned int, 3> &dim = _shape.getShapeDimensions();
    _maxCoordinate[0] = static_cast<int>(dim[0]);
    _maxCoordinate[1] = static_cast<int>(dim[1]);
    _maxCoordinate[2] = static_cast<int>(dim[2]);

    _minNumberOfComponents = std::floor(
                (data.minNumberOfComponents * data.minRatio +
                 _shape.getNumberOfComponents() * (100.0 - data.minRatio))
                / 100.0
                );
    _minNumberOfLoops = std::floor(
                (data.minNumberOfLoops * data.minRatio +
                 _shape.getNumberOfLoops() * (100.0 - data.minRatio))
                / 100.0
                );
    _minNumberOfVoids = std::floor(
                (data.minNumberOfVoids * data.minRatio +
                 _shape.getNumberOfVoids() * (100.0 - data.minRatio))
                / 100.0
                );
    _optNumberOfVoids = data.minNumberOfVoids;
}

void Simplifier::loadRawImage(ImageData &data)
{
    RawImageStack raw(data.downsampling, data.noise);
    _shape.clear();
    _resetTimer();

    _printMessage("--Load image.");

    raw.load(data.inputFileNameBase, data.numberOfImages);
    raw.setCoreThreshold(data.coreThreshold);
    raw.setShapeThreshold(data.shapeThreshold);
    raw.setNeighborhoodThreshold(data.neighborhoodThreshold);

    _printAndResetTime("Image load time:");
    _printMessage("--Load shape.");

    raw >> _shape;

    _printTime("Shape load time:");

    const std::array<unsigned int, 3> &dim = _shape.getShapeDimensions();
    _maxCoordinate[0] = static_cast<int>(dim[0]);
    _maxCoordinate[1] = static_cast<int>(dim[1]);
    _maxCoordinate[2] = static_cast<int>(dim[2]);

    _minNumberOfComponents = std::floor(
                (data.minNumberOfComponents * data.minRatio +
                 _shape.getNumberOfComponents() * (100.0 - data.minRatio))
                / 100.0
                );
    _minNumberOfLoops = std::floor(
                (data.minNumberOfLoops * data.minRatio +
                 _shape.getNumberOfLoops() * (100.0 - data.minRatio))
                / 100.0
                );
    _minNumberOfVoids = std::floor(
                (data.minNumberOfVoids * data.minRatio +
                 _shape.getNumberOfVoids() * (100.0 - data.minRatio))
                / 100.0
                );
    _optNumberOfVoids = data.minNumberOfVoids;
}

void Simplifier::saveImage(ImageData &data){
    _printMessage("--Write image.");
    _resetTimer();

    data.outputFileNameBase << _shape;

    _printTime("Image writing time:");
}

void Simplifier::simplify(unsigned int boxWidth, int ccSize){
    static_assert(AugShape::isAddable(), "Addition of voxels not enabled.");
    static_assert(AugShape::isRemovable(), "Removal of voxels not enabled.");

    _boxWidth = boxWidth;

    _printParameterData(ccSize, "Standard Simplification");

    AdditiveH0CandidateGenerator<AugShape,Voxel> ah0(_shape);
    AdditiveH1CandidateGenerator<AugShape,Voxel> ah1(_shape);
    AdditiveH2CandidateGenerator<AugShape,Voxel> ah2(_shape);
    SubstractiveH0CandidateGenerator<AugShape,Voxel> sh0(_shape);
    SubstractiveH1CandidateGenerator<AugShape,Voxel> sh1(_shape);
    SubstractiveH2CandidateGenerator<AugShape,Voxel> sh2(_shape);
    CandidateValidator<AugShape,Voxel> val(_shape);

    lower_point_container ah0LowerPoints, ah1LowerPoints,
            sh1LowerPoints, sh2LowerPoints;
    _computeInitialLowerPoints(ah0LowerPoints);
    _computeInitialLowerPoints(ah1LowerPoints);
    _computeInitialLowerPoints(sh1LowerPoints);
    _computeInitialLowerPoints(sh2LowerPoints);

    _resetTimer();

    // Void filling. Validation can be skipped.
    _applyAdditiveSimplification(ah2.generateVoidFillings());
    _printAndResetTime("Void filling time:");

    // Component removal. Validation can be skipped.
    _applySubstractiveSimplification(
                sh0.generateSubstractibleComponents(ccSize)
                );
    _printTime("Remove Connected Component time:");
    printShapeData();
    _resetTimer();

    // One Voxel candidates.
    while (_applyAdditiveSimplification(
               val.verifyAdditiveCandidatesValidity(
                   ah0.generateOneVoxelCandidates()
                   )));
    _printTime("AddH0 one voxel time:");
    printShapeData();
    _resetTimer();

    // Persistence candidates.
    bool ah0Changed = true;
    bool ah1Changed = true;
    bool sh1Changed = true;
    bool sh2Changed = true;
    while (ah1Changed || ah0Changed || sh1Changed || sh2Changed) {
        if (ah0Changed)
            ah0Changed = _simplifyH0WithAdditions(
                        ah0, val, ah0LowerPoints);
        if (ah1Changed)
            ah1Changed = _simplifyH1WithAdditions(
                        ah1, val, ah1LowerPoints);
        if (sh2Changed)
            sh2Changed = _simplifyH2WithSubstractions(
                        sh2, val, sh2LowerPoints);
        if (sh1Changed)
            sh1Changed = _simplifyH1WithSubstractions(
                        sh1, val, sh1LowerPoints);
    }

    _printTime("Persistence time:");
}


void Simplifier::simplifyWithSubFirst(unsigned int boxWidth,
                                      int ccSize){
    static_assert(AugShape::isAddable(), "Addition of voxels not enabled.");
    static_assert(AugShape::isRemovable(), "Removal of voxels not enabled.");

    _boxWidth = boxWidth;

    _printParameterData(ccSize, "Standard Simplification");

    AdditiveH0CandidateGenerator<AugShape,Voxel> ah0(_shape);
    AdditiveH1CandidateGenerator<AugShape,Voxel> ah1(_shape);
    AdditiveH2CandidateGenerator<AugShape,Voxel> ah2(_shape);
    SubstractiveH0CandidateGenerator<AugShape,Voxel> sh0(_shape);
    SubstractiveH1CandidateGenerator<AugShape,Voxel> sh1(_shape);
    SubstractiveH2CandidateGenerator<AugShape,Voxel> sh2(_shape);
    CandidateValidator<AugShape,Voxel> val(_shape);

    lower_point_container ah0LowerPoints, ah1LowerPoints,
            sh1LowerPoints, sh2LowerPoints;
    _computeInitialLowerPoints(ah0LowerPoints);
    _computeInitialLowerPoints(ah1LowerPoints);
    _computeInitialLowerPoints(sh1LowerPoints);
    _computeInitialLowerPoints(sh2LowerPoints);

    _resetTimer();

    // Void filling. Validation can be skipped.
    _applyAdditiveSimplification(ah2.generateVoidFillings());
    _printAndResetTime("Void filling time:");

    // Component removal. Validation can be skipped.
    _applySubstractiveSimplification(
                sh0.generateSubstractibleComponents(ccSize)
                );
    _printTime("Remove Connected Component time:");
    printShapeData();
    _resetTimer();

    // One Voxel candidates.
    while (_applyAdditiveSimplification(
               val.verifyAdditiveCandidatesValidity(
                   ah0.generateOneVoxelCandidates()
                   )));
    _printTime("AddH0 one voxel time:");
    printShapeData();
    _resetTimer();

    // Persistence candidates.
    bool ah0Changed = true;
    bool ah1Changed = true;
    bool sh1Changed = true;
    bool sh2Changed = true;
    while (ah1Changed || ah0Changed || sh1Changed || sh2Changed) {
        if (sh2Changed)
            sh2Changed = _simplifyH2WithSubstractions(
                        sh2, val, sh2LowerPoints);
        if (sh1Changed)
            sh1Changed = _simplifyH1WithSubstractions(
                        sh1, val, sh1LowerPoints);
        if (ah0Changed)
            ah0Changed = _simplifyH0WithAdditions(
                        ah0, val, ah0LowerPoints);
        if (ah1Changed)
            ah1Changed = _simplifyH1WithAdditions(
                        ah1, val, ah1LowerPoints);
    }

    _printTime("Persistence time:");
}

void Simplifier::simplifyOnlyAdditively(unsigned int boxWidth){
    static_assert(AugShape::isAddable(), "Addition of voxels not enabled.");

    _boxWidth = boxWidth;

    _printParameterData("Additions Only Simplification");

    AdditiveH0CandidateGenerator<AugShape,Voxel> ah0(_shape);
    AdditiveH1CandidateGenerator<AugShape,Voxel> ah1(_shape);
    AdditiveH2CandidateGenerator<AugShape,Voxel> ah2(_shape);
    CandidateValidator<AugShape,Voxel> val(_shape);

    lower_point_container ah0LowerPoints, ah1LowerPoints;
    _computeInitialLowerPoints(ah0LowerPoints);
    _computeInitialLowerPoints(ah1LowerPoints);

    _resetTimer();

    // Void filling. Validation can be skipped.
    _applyAdditiveSimplification(ah2.generateVoidFillings());
    _printTime("Void filling time:");
    printShapeData();
    _resetTimer();

    // One Voxel candidates.
    while (_applyAdditiveSimplification(
               val.verifyAdditiveCandidatesValidity(
                   ah0.generateOneVoxelCandidates()
                   )));
    _printTime("AddH0 one voxel time:");
    printShapeData();
    _resetTimer();

    // Persistence candidates.
    bool ah0Changed = true;
    bool ah1Changed = true;
    while (ah1Changed || ah0Changed) {
        if (ah0Changed)
            ah0Changed = _simplifyH0WithAdditions(
                        ah0, val, ah0LowerPoints);
        if (ah1Changed)
            ah1Changed = _simplifyH1WithAdditions(
                        ah1, val, ah1LowerPoints);
    }

    _printTime("Persistence time:");
}

void Simplifier::simplifyOnlySubstractively(unsigned int boxWidth, int ccSize){
    static_assert(AugShape::isRemovable(), "Removal of voxels not enabled.");

    _boxWidth = boxWidth;

    _printParameterData(ccSize, "Substractions Only Simplification");

    SubstractiveH0CandidateGenerator<AugShape,Voxel> sh0(_shape);
    SubstractiveH1CandidateGenerator<AugShape,Voxel> sh1(_shape);
    SubstractiveH2CandidateGenerator<AugShape,Voxel> sh2(_shape);
    CandidateValidator<AugShape,Voxel> val(_shape);

    lower_point_container sh1LowerPoints, sh2LowerPoints;
    _computeInitialLowerPoints(sh1LowerPoints);
    _computeInitialLowerPoints(sh2LowerPoints);

    _resetTimer();

    // Component removal. Validation can be skipped.
    _applySubstractiveSimplification(
                sh0.generateSubstractibleComponents(ccSize)
                );
    _printTime("Remove Connected Component time:");
    printShapeData();
    _resetTimer();

    // Persistence candidates.
    bool sh1Changed = true;
    bool sh2Changed = true;
    while (sh1Changed || sh2Changed) {
        if (sh2Changed)
            sh2Changed = _simplifyH2WithSubstractions(
                        sh2, val, sh2LowerPoints);
        if (sh1Changed)
            sh1Changed = _simplifyH1WithSubstractions(
                        sh1, val, sh1LowerPoints);
    }

    _printTime("Persistence time:");
}

void Simplifier::printShapeData(){
    if (_silent) return;

    std::cout << "\n";
    std::cout << "-- Shape Data --\n";
    std::cout << "\n";
    std::cout << "Dimensions: " << _maxCoordinate[0]
              << " x " <<  _maxCoordinate[1]
              << " x " <<  _maxCoordinate[2] << "\n";
    std::cout << "Number of voxels in the core: "
              << _shape.getCoreSize() << "\n";
    std::cout << "Number of non-core voxels in the shape: "
              << (_shape.getShapeSize() - _shape.getCoreSize()) << "\n";
    std::cout << "Total number of voxels in the shape: "
              << _shape.getShapeSize() << "\n";
    std::cout << "Number of voxels in the neighborhood: "
              << (_shape.getNeighborhoodSize() - _shape.getShapeSize()) << "\n";
    std::cout << "Euler Characteristic: "
              << _shape.getEulerCharacteristic() << "\n";
    std::cout << "Number of components: "
              << _shape.getNumberOfComponents() << "\n";
    std::cout << "Number of loops: "
              << _shape.getNumberOfLoops() << "\n";
    std::cout << "Number of voids: "
              << _shape.getNumberOfVoids() << "\n";
    std::cout << "\n";
    std::cout << "----------------\n";
    std::cout << "\n";
}

bool Simplifier::_simplifyH0WithAdditions(
        AdditiveH0CandidateGenerator<AugShape, Voxel>& gen,
        CandidateValidator<AugShape,Voxel>& val,
        lower_point_container& lowerPoints)
{
    _printMessage("__positive H0");

    if (_shape.getNumberOfComponents() <= _minNumberOfComponents)
        return false;

    bool changed = _applyAdditiveSimplification(
                val.verifyAdditiveCandidatesValidity(
                    gen.template generatePeristenceCandidates<RestrShape>(
                        _boxWidth, lowerPoints, _silent)),
                lowerPoints);

    printShapeData();

    return changed;
}


bool Simplifier::_simplifyH1WithAdditions(
        AdditiveH1CandidateGenerator<AugShape, Voxel>& gen,
        CandidateValidator<AugShape,Voxel>& val,
        lower_point_container& lowerPoints)
{
    _printMessage("__positive H1");

    if (_shape.getNumberOfLoops() <= _minNumberOfLoops)
        return false;

    bool changed = _applyAdditiveSimplification(
                val.verifyAdditiveCandidatesValidity(
                    gen.template generatePeristenceCandidates<RestrShape>(
                        _boxWidth, lowerPoints, _silent)),
                lowerPoints);

    printShapeData();

    return changed;
}


bool Simplifier::_simplifyH1WithSubstractions(
        SubstractiveH1CandidateGenerator<AugShape, Voxel>& gen,
        CandidateValidator<AugShape,Voxel>& val,
        lower_point_container& lowerPoints)
{
    _printMessage("__negative H1");

    if (_shape.getNumberOfLoops() <= _minNumberOfLoops)
        return false;

    bool changed;

    if (_shape.getNumberOfVoids() == _optNumberOfVoids){
        changed = _applySubstractiveSimplification(
                    val.verifySubstractiveCandidatesValidityWithH2Guarantee(
                        gen.template generatePeristenceCandidates<InvRestrShape>(
                            _boxWidth, lowerPoints, _silent)),
                    lowerPoints);
    } else {
        changed = _applySubstractiveSimplification(
                    val.verifySubstractiveCandidatesValidity(
                        gen.template generatePeristenceCandidates<InvRestrShape>(
                            _boxWidth, lowerPoints, _silent)),
                    lowerPoints);
    }

    printShapeData();

    return changed;
}


bool Simplifier::_simplifyH2WithSubstractions(
        SubstractiveH2CandidateGenerator<AugShape, Voxel>& gen,
        CandidateValidator<AugShape,Voxel>& val,
        lower_point_container& lowerPoints)
{
    if (_shape.getNumberOfVoids() <= _minNumberOfVoids)
        return false;

    _printMessage("__negative H2");
    bool changed = _applySubstractiveSimplification(
                val.verifySubstractiveCandidatesValidity(
                    gen.template generatePeristenceCandidates<InvRestrShape>(
                        _boxWidth, lowerPoints, _silent)),
                lowerPoints);

    printShapeData();

    return changed;
}

bool Simplifier::_applyAdditiveSimplification(
        const valid_candidate_container& voxels)
{
    if (voxels.size() == 0) return false;

    for (Voxel v : voxels){
        _shape.addVoxel<AugShape::CORE>(v);
    }

    return true;
}

bool Simplifier::_applyAdditiveSimplification(
        const candidate_container& voxelSets)
{
    if (voxelSets.size() == 0) return false;

    for (auto set : voxelSets){
        for (Voxel v : set){
            _shape.addVoxel<AugShape::CORE>(v);
        }
    }

    return true;
}

bool Simplifier::_applyAdditiveSimplification(
        const valid_candidate_container& voxels,
        lower_point_container& currentLP)
{
    currentLP.clear();
    if (voxels.size() == 0) return false;

    for (Voxel v : voxels){
        _shape.addVoxel<AugShape::CORE>(v);
        _getCorrespondingLowerPoints(v, currentLP);
    }

    return true;
}

bool Simplifier::_applyAdditiveSimplification(
        const candidate_container& voxelSets,
        lower_point_container& currentLP)
{
    currentLP.clear();
    if (voxelSets.size() == 0) return false;

    for (auto set : voxelSets){
        for (Voxel v : set){
            _shape.addVoxel<AugShape::CORE>(v);
            _getCorrespondingLowerPoints(v, currentLP);
        }
    }

    return true;
}

bool Simplifier::_applySubstractiveSimplification(
        const valid_candidate_container& voxels)
{
    if (voxels.size() == 0) return false;

    for (Voxel v : voxels){
        _shape.removeVoxel<AugShape::NEIGHBORHOOD>(v);
    }

    return true;
}

bool Simplifier::_applySubstractiveSimplification(
        const candidate_container& voxelSets)
{
    if (voxelSets.size() == 0) return false;

    for (auto set : voxelSets){
        for (Voxel v : set){
            _shape.removeVoxel<AugShape::NEIGHBORHOOD>(v);
        }
    }

    return true;
}

bool Simplifier::_applySubstractiveSimplification(
        const valid_candidate_container& voxels,
        lower_point_container& currentLP)
{
    currentLP.clear();
    if (voxels.size() == 0) return false;

    for (Voxel v : voxels){
        _shape.removeVoxel<AugShape::NEIGHBORHOOD>(v);
        _getCorrespondingLowerPoints(v, currentLP);
    }

    return true;
}

bool Simplifier::_applySubstractiveSimplification(
        const candidate_container& voxelSets,
        lower_point_container& currentLP)
{
    currentLP.clear();
    if (voxelSets.size() == 0) return false;

    for (auto set : voxelSets){
        for (Voxel v : set){
            _shape.removeVoxel<AugShape::NEIGHBORHOOD>(v);
            _getCorrespondingLowerPoints(v, currentLP);
        }
    }

    return true;
}

void Simplifier::_computeInitialLowerPoints(lower_point_container& lowerPoints){
    Point lower;
    int shift = _boxWidth / 2;

    for (lower[0] = 0; lower[0] <= _maxCoordinate[0]; lower[0] += shift) {
        for (lower[1] = 0; lower[1] <= _maxCoordinate[1]; lower[1] += shift) {
            for (lower[2] = 0; lower[2] <= _maxCoordinate[2]; lower[2] += shift) {
                lowerPoints.insert(lower);
            }
        }
    }
}

void Simplifier::_getCorrespondingLowerPoints(Voxel v, lower_point_container& points){
    int shift = _boxWidth / 2;
    int x = std::floor(static_cast<double>(v[0]) / static_cast<double>(shift)) * shift;
    int y = std::floor(static_cast<double>(v[1]) / static_cast<double>(shift)) * shift;
    int z = std::floor(static_cast<double>(v[2]) / static_cast<double>(shift)) * shift;
    points.insert(Point({x, y, z}));

    if (x - shift >= 0){
        points.insert(Point({x - shift, y, z}));
        if (y - shift >= 0){
            points.insert(Point({x - shift, y - shift, z}));
            if (z - shift >= 0) points.insert(Point({x - shift, y - shift, z - shift}));
        }
        if (z - shift >= 0) points.insert(Point({x - shift, y, z - shift}));
    }
    if (y - shift >= 0){
        points.insert(Point({x, y - shift, z}));
        if (z - shift >= 0) points.insert(Point({x, y - shift, z - shift}));
    }
    if (z - shift >= 0) points.insert(Point({x, y, z - shift}));
}

void Simplifier::_resetTimer(){
    if (_silent) return;

    _time = clock();
}

void Simplifier::_printTime(std::string message){
    if (_silent) return;

    std::cout << message << " " << std::setprecision(10)
              << (static_cast<float>(clock() - _time)) / CLOCKS_PER_SEC
              << " seconds.\n";
}

void Simplifier::_printAndResetTime(std::string message){
    if (_silent) return;

    _printTime(message);
    _resetTimer();
}

void Simplifier::_printMessage(std::string message){
    if (_silent) return;

    std::cout << message << std::endl;
}

void Simplifier::_printParameterData(std::string message){
    if (_silent) return;

    std::cout << "\n";
    std::cout << "-- " << message << " --\n";
    std::cout << "\n";
    std::cout << "Box Width: " << _boxWidth << "\n";
    std::cout << "Shift value: " << (_boxWidth / 2) << "\n";
    std::cout << "\n";
    std::cout << "-------------------------------------------\n";
    std::cout << "\n";
}

void Simplifier::_printParameterData(int ccSize, std::string message){
    if (_silent) return;

    std::cout << "\n";
    std::cout << "-- " << message << " --\n";
    std::cout << "\n";
    std::cout << "Box Width: " << _boxWidth << "\n";
    std::cout << "Shift value: " << (_boxWidth / 2) << "\n";
    if (ccSize >= 0)
        std::cout << "Maximum size for removed components: " << ccSize << "\n";
    else
        std::cout << "Maximum size for removed components: no limit\n";
    std::cout << "\n";
    std::cout << "-------------------------------------------\n";
    std::cout << "\n";
}

} // namespace shape_simplification::simplifier




