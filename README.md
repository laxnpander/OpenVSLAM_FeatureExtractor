# OpenVSLAM_FeatureExtractor

Standalone ORB detector implementation copied from https://github.com/xdspacelab/openvslam ,but installable with cmake.

## Build & install

```
git clone https://github.com/laxnpander/OpenVSLAM_FeatureExtractor.git
cd OpenVSLAM_FeatureExtractor
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
make -j4
sudo make install
```

## Example Usage

In CMakeLists.txt of your project use:

```
cmake_minimum_required(VERSION 3.9)

project(test)

find_package(ORBFeatureExtractor REQUIRED)

add_executable(test_bin test.cpp)
target_link_libraries(test_bin ORBFeatureExtractor)
```