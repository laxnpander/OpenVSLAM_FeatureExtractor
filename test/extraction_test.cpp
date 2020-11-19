

// gtest
#include <gtest/gtest.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <orb_extractor.h>

TEST(OrbFeatureExtractor, Basic)
{
  openvslam::feature::orb_extractor extractor(3000, 1.2, 8, 15, 3);

  cv::Mat img = cv::imread("image.jpg");

  cv::Mat descriptor;
  std::vector<cv::KeyPoint> features;

  extractor.extract(img, cv::Mat(), features, descriptor);

  cv::Mat img_features;
  cv::drawKeypoints(img, features, img_features);

  cv::namedWindow("Features", CV_WINDOW_NORMAL);
  cv::imshow("Features", img_features);
  cv::waitKey(0);
}
