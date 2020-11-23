//#include <opencv2/core.hpp>
//#include <opencv2/imgproc.hpp>
#include <cstdio>
#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <type_traits>
#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <orb_extractor.h>
#include <opencv2/core/types.hpp>
#include <string>
#include "ndarray_converter.h"


namespace py = pybind11;
using namespace std;


string cv_version() {
    return CV_VERSION;
}



template<class T>
cv::Mat vector2Mat(vector<vector<T>> v) {
    /* convert std vector into cv::Mat */
    size_t rn = v.size();       // row number
    size_t cn = v[0].size();    // column number

    // image type selection based on vector type
    int img_type;
    if (is_same<T, uchar>::value) {
        img_type = CV_8UC1;
    }
    if (is_same<T, float>::value) {
        img_type = CV_32FC1;
    }

    cv::Mat m(rn,cn,img_type);   // output mat

    for (size_t r=0; r<rn; r++) {
        // copy one line
        memcpy(m.data + r*cn*sizeof(T), v[r].data(), cn*sizeof(T));
    }
    return m;
}


template<class T>
vector<vector<T>> Mat2Vector(cv::Mat m) {
    /* convert cv::Mat into std vector */
    size_t rn = m.rows;         // row number
    size_t cn = m.cols;         // column number
    vector<vector<T>> v(rn);    // output mat

    for (size_t r=0; r<rn; r++) {
        v[r] = vector<T>(cn);
        // copy une line
        memcpy(v[r].data() , m.data + r*cn*sizeof(T), cn*sizeof(T));
    } 
    return v;
}


cv::Mat showkeyp(const std::string& input) {
    openvslam::feature::orb_extractor extractor(3000, 1.2, 8, 15, 3);

    //cv::Mat img = m;
    cv::Mat img = cv::imread(input);

    cv::Mat descriptor;
    std::vector<cv::KeyPoint> features;

    extractor.extract(img, cv::Mat(), features, descriptor);

    // cv::Mat img_features;
    // cv::drawKeypoints(img, features, img_features);
    cv::Mat m1 = cv::Mat(img.rows,img.cols,  CV_8U, 255);
    for (int i = 0; i < features.size(); ++i) {
    cv::Point2f p = features[i].pt;
    m1.at<int>((int)p.y,(int)p.x) = 1;
    }
    return m1;

}

cv::Mat showkeyp2(cv::Mat img) {
    openvslam::feature::orb_extractor extractor(3000, 1.2, 8, 15, 3);


    cv::Mat descriptor;
    std::vector<cv::KeyPoint> features;

    extractor.extract(img, cv::Mat(), features, descriptor);

    
    cv::Mat m1 = cv::Mat(img.rows,img.cols,  CV_8U, 255);
    for (int i = 0; i < features.size(); ++i) {
    cv::Point2f p = features[i].pt;
    m1.at<int>((int)p.y,(int)p.x) = 1;
    }
    return m1;

}

cv::Mat keypointsnp(cv::Mat img, int max_num_keyp, double scale_factor, int num_levels, int ini_fast_thr, int min_fast_thr) {
    openvslam::feature::orb_extractor extractor(max_num_keyp, scale_factor, num_levels, ini_fast_thr, min_fast_thr);

    cv::Mat descriptor;
    std::vector<cv::KeyPoint> features;

    extractor.extract(img, cv::Mat(), features, descriptor);

    cv::Mat m2 = cv::Mat(features.size(), 2,  CV_16SC1);
    for (int i = 0; i < features.size(); ++i) {
        m2.at<int>(i,0,0) = (int) features[i].pt.x;
        m2.at<int>(i,1,0) = (int) features[i].pt.y;
    }
    return m2;
}


/* ====== PYBIND CODE ====== */
PYBIND11_MODULE(cvMat, m) {
    NDArrayConverter::init_numpy();
    m.doc() = "cvMat <-> Python"; // optional module docstring
    m.def("cv_version", &cv_version, "get the current OpenCV version");
    m.def("drawKeypoints", &showkeyp);
    m.def("Keypointm", &showkeyp2, "return keypoint mask for input image", py::arg("img"));
    m.def("Kmat", &keypointsnp, "return keypoint coordinates for input image", py::arg("img"),py::arg("max_num_keyp"),py::arg("scale_factor"),py::arg("num_levels"), py::arg("ini_fast_thr"),py::arg("min_fast_thr"));
}