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
