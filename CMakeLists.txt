 cmake_minimum_required(VERSION 3.9)

project(ORBFeatureExtractor)

set(ORBFeatureExtractor_VERSION "1.0.0")

set(LIBRARY_NAME orb_feature_extractor)


################################################################################
# Paths
################################################################################

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(ORBFeatureExtractor_INC_INSTALL_DIR "/usr/local/include/${PROJECT_NAME}")
set(ORBFeatureExtractor_LIBRARY_INSTALL_DIR "/usr/local/lib")
set(ORBFeatureExtractor_ARCHIVE_INSTALL_DIR "/usr/local/lib")
set(ORBFeatureExtractor_FRAMEWORK_INSTALL_DIR "/usr/local/lib")
set(ORBFeatureExtractor_CMAKE_CONFIG_INSTALL_DIR "/usr/local/lib/cmake/ORBFeatureExtractor")


################################################################################
# Include CMake dependencies
################################################################################

include(CheckCXXCompilerFlag)
include(CheckLanguage)
include(CMakePackageConfigHelpers)


################################################################################
# Compiler specific configuration
################################################################################

if(CMAKE_BUILD_TYPE)
   message(STATUS "Build type specified as ${CMAKE_BUILD_TYPE}")
else()
   message(STATUS "Build type not specified, using Release")
   set(CMAKE_BUILD_TYPE Release)
   set(IS_DEBUG OFF)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


################################################################################
# Flags
################################################################################

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall   -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall   -O3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)


################################################################################
# Dependencies
################################################################################

find_package(OpenCV 3)
if(NOT OpenCV_FOUND)
   find_package(OpenCV 2.4.3 QUIET)
   if(NOT OpenCV_FOUND)
      message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
   endif()
endif()

find_package(yaml-cpp REQUIRED)


################################################################################
# Sources
################################################################################

include_directories(
   ${PROJECT_SOURCE_DIR}/include/orb_feature_extractor
)

set(HEADERS
        include/orb_feature_extractor/orb_extractor.h
        include/orb_feature_extractor/orb_extractor_node.h
        include/orb_feature_extractor/orb_params.h
        include/orb_feature_extractor/orb_point_pairs.h
)

set(SOURCES
        src/orb_extractor.cc
        src/orb_extractor_node.cc
        src/orb_params.cc
)

add_library(${LIBRARY_NAME} SHARED
       ${HEADERS}
       ${SOURCES}
)

target_include_directories(${LIBRARY_NAME} PUBLIC
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> # for headers when building
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # for headers when building
       $<INSTALL_INTERFACE:${ORBFeatureExtractor_INC_INSTALL_DIR}> # for client in install mode
)

target_link_libraries(${LIBRARY_NAME}
      PUBLIC
         ${OpenCV_LIBS}
         yaml-cpp
)

################################################################################
# Install Targets
################################################################################


set_target_properties(${LIBRARY_NAME} PROPERTIES OUTPUT_NAME "${LIBRARY_NAME}-${ORBFeatureExtractor_VERSION}")

install(TARGETS ${LIBRARY_NAME} EXPORT ORBFeatureExtractorTargets
      RUNTIME       DESTINATION ${ORBFeatureExtractor_RUNTIME_INSTALL_DIR}
      LIBRARY       DESTINATION ${ORBFeatureExtractor_LIBRARY_INSTALL_DIR}
      ARCHIVE       DESTINATION ${ORBFeatureExtractor_ARCHIVE_INSTALL_DIR}
      FRAMEWORK     DESTINATION ${ORBFeatureExtractor_FRAMEWORK_INSTALL_DIR}
      )

# Headers
install(
      DIRECTORY include/${LIBRARY_NAME}
      DESTINATION ${ORBFeatureExtractor_INC_INSTALL_DIR}
      FILES_MATCHING
      PATTERN "*.h"
      PATTERN "*.hpp"
)

################################################################################
# Install rest
################################################################################

# Create and install version file
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/ORBFeatureExtractorConfigVersion.cmake")
write_basic_package_version_file(${version_file}
      VERSION ${ORBFeatureExtractor_VERSION}
      COMPATIBILITY ExactVersion
)

install(FILES ${version_file}
      DESTINATION ${ORBFeatureExtractor_CMAKE_CONFIG_INSTALL_DIR}
)

install(
      EXPORT ORBFeatureExtractorTargets
      DESTINATION ${ORBFeatureExtractor_CMAKE_CONFIG_INSTALL_DIR}
      FILE ORBFeatureExtractorConfig.cmake
)

################################################################################
# Test
################################################################################


if(BUILD_TESTS)
 enable_testing()

 include(cmake/googletest.cmake)
 fetch_googletest(
        ${PROJECT_SOURCE_DIR}/cmake
        ${PROJECT_BINARY_DIR}/googletest)

 include(GoogleTest)

 add_executable(run_orb_feature_extractor_tests
         test/test_orb_feature_extractor.cpp
         test/extraction_test.cpp
         )

 # Standard linking to gtest stuff.
 target_link_libraries(run_orb_feature_extractor_tests gtest_main)

 # Extra linking for the project.
 target_link_libraries(run_orb_feature_extractor_tests ${LIBRARY_NAME})

 add_custom_command(TARGET run_orb_feature_extractor_tests PRE_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/test/data $<TARGET_FILE_DIR:run_orb_feature_extractor_tests>
 )

 # This is so you can do 'make test' to see all your tests run, instead of
 # manually running the executable run_unit_tests to see those specific tests.
 add_test(
         NAME
            run_orb_feature_extractor_unit_tests
         COMMAND
            ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/run_orb_feature_extractor_tests
 )
endif()