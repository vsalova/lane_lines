#include <opencv2/opencv.hpp>
#include <iostream>
#include "signal_proc.hpp"

void polyFit(const cv::Mat &src_x, const cv::Mat &src_y, cv::Mat &dst, int order) {
  if (src_x.cols != 1 && src_x.channels() != 1 && src_y.cols != 1 && src_y.channels() != 1) {
    std::stringstream ss;
    ss << "Dimensions of inputs (src_x, src_y) are invalid. Expected dimensions are of the form [1 n 1] (width, height, channels).\n";
    throw std::invalid_argument(ss.str());
  }
  if (src_x.type() != 6 || src_y.type() != 6) {
    std::stringstream ss;
    ss << "Input types are invalid. src_x and src_y must be CV_64F.\n";
    throw std::invalid_argument(ss.str());
  }
  cv::Mat X = cv::Mat::zeros(src_x.rows, order+1, CV_64FC1);
  cv::Mat copy;
  for(int i = 0; i <= order; i++) {
    copy = src_x.clone();
    cv::pow(copy,i,copy);
    copy.col(0).copyTo(X.col(i));
  }
  cv::Mat X_t;
  cv::transpose(X,X_t);
  cv::Mat temp = X_t*X;
  cv::Mat temp2;
  cv::invert (temp,temp2);
  cv::Mat temp3 = temp2*X_t;
  dst = temp3*src_y;
}

void polyVal(const cv::Mat& src, cv::Mat& dst, const cv::Mat& best_fit) {
  if (src.cols != 1 && src.channels() != 1 && best_fit.cols != 1 && best_fit.channels() != 1) {
    std::stringstream ss;
    ss << "Dimensions of inputs (src, best_fit) are invalid. Expected dimensions are of the form [1 n 1] (width, height, channels).\n";
    throw std::invalid_argument(ss.str());
  }
  if (src.type() != 6 || best_fit.type() != 6) {
    std::stringstream ss;
    ss << "Input types are invalid. src and best_fit must be CV_64F.\n";
    throw std::invalid_argument(ss.str());
  }/*
  cv::Mat temp;
  dst = cv::Mat::zeros(src.rows, src.cols, src.type());
  for (int i = best_fit.rows-1; i >= 0; i--) {
    cv::pow(src, i, temp);
    dst += best_fit.col(0).at<double>(i)*temp;
  }
  */
  // evaluate polynomial using Horner's method
  dst = cv::Mat::zeros(src.rows, src.cols, src.type());
  for (int i = best_fit.rows-1; i>=0; i--) {
    cv::multiply(src,dst,dst);
    dst += best_fit.col(0).at<double>(i);
  }
}

void linspace(double start, double stop, unsigned int num, cv::Mat& dst) {
  if (stop <= start) {
    std::stringstream ss;
    std::cout << "start: " << start << std::endl;
    std::cout << "stop: " << stop << std::endl;
    ss << "linspace(): stop must be greater than start.\n";
    throw std::invalid_argument(ss.str());
  }
  double step = (stop - start) / (num - 1);
  dst = cv::Mat::zeros(static_cast<int>(num), 1, CV_64F);
  dst.forEach<double> (
    [step](double& pixel, const int* position) -> void {
      pixel = (*position) * step;
    }
  );
}

std::vector<int> findPeaks(cv::Mat& binary_warped, cv::Mat& histogram) {
  // NOTE: this funtion does assume max value of pixels is 255.
  //std::vector<int> peaks(2);
  cv::reduce(binary_warped.rowRange(binary_warped.rows/2,binary_warped.rows)/255, histogram, 0, cv::REDUCE_SUM, CV_64F);

  int midpoint = histogram.cols/2;
  double left_min_val, left_max_val, right_min_val, right_max_val;
  cv::Point left_min_loc, left_max_loc, right_min_loc, right_max_loc;
  cv::minMaxLoc(histogram.colRange(0,midpoint), &left_min_val, &left_max_val, &left_min_loc, &left_max_loc);
  cv::minMaxLoc(histogram.colRange(midpoint,histogram.cols), &right_min_val, &right_max_val, &right_min_loc, &right_max_loc);

  std::vector<int> peaks = {left_max_loc.x, right_max_loc.x + midpoint};
  return peaks;

}
