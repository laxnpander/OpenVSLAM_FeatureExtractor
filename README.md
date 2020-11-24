# OpenVSLAM_FeatureExtractor Python Bindings

Simple Python Bindings for OpenVSLAM_FeatureExtractor using pybind11 and cmake

Note:
Clone the repo with git clone --recursive to include pybind11 submodule

# Prerequisites:

Python Environment: python >=3.6, Open CV python package, numpy

For Cmake:

Open CV >= 4.0.0 Installation

yaml-cpp


## 1. Build & install OpenVSLAM_Feature Extractor

```
cd OpenVSLAM_FeatureExtractor
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
make -j4
sudo make install
```

## 2. Edit python_bindings/CMakeLists.txt

Edit Python Path sections

```
# set path to your python environment and numpy headers
set(PYTHON_EXECUTABLE "/opt/anaconda3/envs/slambind/bin/python")
set(PYTHON_INCLUDE_DIRS ${PYTHON_INCLUDE_DIRS} /opt/anaconda3/envs/slambind/lib/python3.6/site-packages/numpy/core/include)

...

# set path to PythonLibs
find_package(PythonLibs 3 PATHS /opt/anaconda3/envs/slambind/)

...

```
## 3. Install Python Bindings with pip

activate python environment and install package

```
cd python_bindings
pip install .

```

## Example Usage

```{python}
import cvMat
import numpy as np
import cv2

## get keypoint coordinates x,y as numpy array for input image
img = cv2.imread("image.jpg")
kparray = cvMat.Kmat(img, 3000, 1.2, 8, 15, 3).astype('uint8')

```

```
#Function arguments
cvMat.Kmat(img: numpy.ndarray, max_num_keyp: int, scale_factor: float, num_levels: int, ini_fast_thr: int, min_fast_thr: int)
```

see jupyter notebook in python_bindings folder for example usage

Code tested with OS X 10.15, python=3.6, Open CV 4.4.0

Python Bindings Code adapted from:

https://github.com/mpascucci/pybind_opencvMat
and
https://github.com/edmBernard/pybind11_opencv_numpy
