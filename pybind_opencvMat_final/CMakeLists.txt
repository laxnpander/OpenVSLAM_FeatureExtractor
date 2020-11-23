project( cvMat_pybind )
cmake_minimum_required(VERSION 3.15)
set (CMAKE_CXX_STANDARD 11)
set(PYBIND11_CPP_STANDARD -std=c++11)

# specify here which python interpreter you want to compile this module for
add_subdirectory(pybind11)
set(PYTHON_EXECUTABLE "/opt/anaconda3/envs/slambind/bin/python")
set(PYTHON_INCLUDE_DIRS ${PYTHON_INCLUDE_DIRS} /opt/anaconda3/envs/slambind/lib/python3.6/site-packages/numpy/core/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PYTHON_INCLUDE_DIRS})

# find the system installed OpenCV libraries
find_package(PythonLibs 3 PATHS /opt/anaconda3/envs/slambind/)
find_package(OpenCV 4.0.0 REQUIRED)
find_package(ORBFeatureExtractor REQUIRED)

# The following requires pybind installed in the compiling system
pybind11_add_module(cvMat main.cpp ndarray_converter.cpp)
find_package(yaml-cpp REQUIRED)

# Link the OpenCV libraries
target_link_libraries(cvMat PRIVATE  ${OpenCV_LIBS}  ${PYTHON_LIBRARIES} ORBFeatureExtractor)