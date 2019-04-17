#ifndef CV_HELPER_HPP
#define CV_HELPER_HPP

#include <opencv2/opencv.hpp>
#include <sstream>
#include <stdio.h>
#include <stdarg.h>

using namespace std;
using namespace cv;

// Returns type of cv::Mat i.e. 8UC3, 8SC3, 64FC1, etc.
std::string type(cv::Mat& img);

// Returns 3-dimensional shape of image
cv::Point3i shape(cv::Mat& img);

// Used to plot many images at once on one window
void subplot(std::string title, std::vector<cv::Mat>& v, unsigned int w, unsigned int h);

#endif // CV_HELPER_HPP
