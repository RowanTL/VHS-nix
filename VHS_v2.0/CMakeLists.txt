cmake_minimum_required(VERSION 3.12)

project(VHS VERSION 2.0
		DESCRIPTION "VHS -- Voxelized Homological Simplification"
        LANGUAGES CXX)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")
set(CMAKE_CXX_STANDARD 17)

set(SRC_FILES 
    src/util/PackedData.cpp
    src/util/BaseImageStack.cpp
    src/util/ProcessedImageStack.cpp
    src/util/RawImageStack.cpp

	src/simplifier/Simplifier.cpp

    src/shape/Voxel.cpp

    src/persistence/BoundaryMatrix.cpp
)

set(INCLUDE_FILES
    src/util/PackedData.h
    src/util/Point.h
    src/util/UnionFind.h
	src/util/UnionFindDelete.h
    src/util/BaseImageStack.h
    src/util/ProcessedImageStack.h
    src/util/RawImageStack.h
    src/util/FileIO.h

    src/simplifier/CandidateValidator.h
	src/simplifier/BaseCandidateGenerator.h
    src/simplifier/AdditiveH2CandidateGenerator.h
    src/simplifier/AdditiveH1CandidateGenerator.h
    src/simplifier/AdditiveH0CandidateGenerator.h
	src/simplifier/SubstractiveH2CandidateGenerator.h
	src/simplifier/SubstractiveH1CandidateGenerator.h
	src/simplifier/SubstractiveH0CandidateGenerator.h
	src/simplifier/Simplifier.h

    src/shape/Voxel.h
	src/shape/VoxelSkeletonIterators.h
	src/shape/BaseShape.h
	src/shape/AddableShape.h
	src/shape/RemovableShape.h
	src/shape/EditableShape.h
	src/shape/RestrictedShapeCopy.h
	src/shape/InvertedRestrictedShapeCopy.h
	src/shape/ClosedRestrictedShapeCopy.h
	src/shape/ClosedInvertedRestrictedShapeCopy.h
	src/shape/AugmentedShape.h
	src/shape/ShapeOverlay.h
	src/shape/ConnectedComponents.h
	src/shape/Cell.h
	src/shape/CellAdjacencies.h
	src/shape/BaseSkeleton.h
	src/shape/DualSkeleton.h
	src/shape/PrimalSkeleton.h

	src/persistence/BoundaryMatrix.h
    src/persistence/PersistentHomology.h

	misc/theoretical_best/TheoreticalOptimum.h
)

set(TEMPLATE_CODE_FILES
    src/util/UnionFind.txx
	src/util/UnionFindDelete.txx
    
    src/simplifier/CandidateValidator.txx
	src/simplifier/BaseCandidateGenerator.txx
    src/simplifier/AdditiveH2CandidateGenerator.txx
    src/simplifier/AdditiveH1CandidateGenerator.txx
    src/simplifier/AdditiveH0CandidateGenerator.txx
	src/simplifier/SubstractiveH2CandidateGenerator.txx
	src/simplifier/SubstractiveH1CandidateGenerator.txx
	src/simplifier/SubstractiveH0CandidateGenerator.txx

	src/shape/VoxelSkeletonIterators.txx
	src/shape/BaseShape.txx
	src/shape/AddableShape.txx
	src/shape/RemovableShape.txx
	src/shape/RestrictedShapeCopy.txx
	src/shape/InvertedRestrictedShapeCopy.txx
	src/shape/ClosedRestrictedShapeCopy.txx
	src/shape/ClosedInvertedRestrictedShapeCopy.txx
	src/shape/AugmentedShape.txx
	src/shape/ShapeOverlay.txx
	src/shape/ConnectedComponents.txx
	src/shape/Cell.txx
	src/shape/BaseSkeleton.txx
	src/shape/DualSkeleton.txx
	src/shape/PrimalSkeleton.txx

    src/persistence/PersistentHomology.txx
)

set(IMG_FILES
	src/util/BaseImageStack.cpp
	src/util/RawImageStack.cpp
)

set(OPT_FILES
	src/util/BaseImageStack.cpp
	src/util/ProcessedImageStack.cpp
	src/util/PackedData.cpp

	src/shape/Voxel.cpp
	src/shape/DualSkeleton.txx
	src/shape/Cell.txx

	misc/theoretical_best/TheoreticalOptimum.cpp
)

set(TEST_FILES 
    test/test_all.cpp

	test/Voxel_test.cpp
	test/Shape_test.cpp
	test/Restricted_Shapes_test.cpp
	test/Augmented_Shape_test.cpp
	test/Shape_Overlay_test.cpp
	test/Connected_Components_test.cpp
	test/Cell_test.cpp
	test/Skeleton_test.cpp
	test/Boundary_Matrix_test.cpp
	test/Persistence_test.cpp
	test/H2_Candidates_Generator_test.cpp
	test/H1_Candidates_Generator_test.cpp
	test/H0_Candidates_Generator_test.cpp
	test/Validator_test.cpp
)

# Library setup
find_package(PNG REQUIRED)
set(PHAT external/phat-code/include/)
set(PNGPP external/png++-0.2.9/)

include_directories(
	src
	${PHAT}
	${PNG_INCLUDE_DIR}
	${PNGPP})

# Setup test target
find_package(GTest REQUIRED)

add_executable(test 
               ${TEST_FILES}
               ${SRC_FILES}
               ${TEMPLATE_CODE_FILES}
)
target_include_directories(test PUBLIC test)
target_link_libraries(test ${GTEST_BOTH_LIBRARIES} ${PNG_LIBRARIES})
target_compile_features(test PRIVATE cxx_std_17)

# Image Processing
add_executable(imgproc
			   ${IMG_FILES}
			   misc/image_processing/main.cpp
)
target_include_directories(imgproc PUBLIC)
target_link_libraries(imgproc ${PNG_LIBRARIES})
target_compile_features(imgproc PRIVATE cxx_std_17)

# Theoretical best
add_executable(optsol
			   ${OPT_FILES}
			   misc/theoretical_best/main.cpp
)
target_include_directories(optsol PUBLIC)
target_link_libraries(optsol ${PNG_LIBRARIES})
target_compile_features(optsol PRIVATE cxx_std_17)

# MAIN
add_executable(simplifier
			   ${SRC_FILES}
			   ${TEMPLATE_CODE_FILES}
			   src/main.cpp
)
target_include_directories(simplifier PUBLIC)
target_link_libraries(simplifier ${PNG_LIBRARIES})
target_compile_features(simplifier PRIVATE cxx_std_17)

# Doxygen documentation
find_package(Doxygen
             REQUIRED dot
             OPTIONAL_COMPONENTS mscgen dia)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)"
	${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif()

  set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
  set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/docs)

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)

  add_custom_target(doc
                    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating API documentation with Doxygen"
                    VERBATIM)
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif()

# Clang format support
find_program(CLANG_FORMAT NAMES clang-format)
if (CLANG_FORMAT)
  set(CLANG_FILES_TO_FORMAT
	  ${SRC_FILES}
	  ${INCLUDE_FILES}
	  ${TEMPLATE_CODE_FILES}
	  ${TEST_FILES}
	  ${OPT_FILES}
	  ${IMG_FILES})
  list(TRANSFORM CLANG_FILES_TO_FORMAT PREPEND "${CMAKE_SOURCE_DIR}/")
  message(${CLANG_FILES_TO_FORMAT})
  add_custom_target(format
                    COMMAND ${CLANG_FORMAT}
                    -i ${CLANG_FILES_TO_FORMAT}) 
endif ()


# Clang tidy support
find_program(CLANG_TIDY NAMES clang-tidy)
if (CLANG_TIDY)
  set(CLANG_FILES_TO_CHECK ${SRC_FILES})
  list(TRANSFORM CLANG_FILES_TO_CHECK PREPEND "${CMAKE_SOURCE_DIR}/")
  add_custom_target(tidy
                    COMMAND ${CLANG_TIDY}
                    -checks=-*,clang-analyzer-core.*,clang-analyzer-cplusplus.*,clang-analyzer-optin.*,cppcoreguidelines-*,modernize-*,-modernize-use-trailing-return-type
                    -extra-arg=-std=c++17
                    -extra-arg=-I${CMAKE_SOURCE_DIR}/src
                    -header-filter=.*
                    ${CLANG_FILES_TO_CHECK})
endif ()
