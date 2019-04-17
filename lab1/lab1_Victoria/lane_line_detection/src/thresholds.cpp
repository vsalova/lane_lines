#include "thresholds.hpp"
#include "cv_helper.hpp"

// img_thresholded = img > 20 & img < 100; between 20 and 100 should be 255, the rest should be 0

void abs_sobel_thresh(cv::Mat& src, cv::Mat& dst, char orient, int sobel_kernel=7, int thresh_min=40, int thresh_max=100) {
  cv::Mat gray;
  cv::Mat sobel;
  double min_val;
  double max_val;
  cv::Mat binary;
  cv::Mat img_thresholded;

  cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

  if (orient == 'x') {
    // TODO: Make the appropriate call to cv::Sobel
    cv::Sobel(gray, sobel, CV_8U, 1, 0);

  }
  else if (orient == 'y') {
    // TODO: Make the appropriate call to cv::Sobel
    cv::Sobel(gray, sobel, CV_8U, 0, 1);
  }
  else {
    std::stringstream ss;
    ss << orient << "is invalid for argument orient. Must be either 'x' or 'y'.\n";
    throw std::runtime_error(ss.str());
  }

  // take absolute value of the gradient
  cv::Mat abs_sobel = cv::abs(sobel);

  //Normalize all values to be between 0-255, cv::minMaxLoc
  cv::minMaxLoc(sobel, &min_val, &max_val, NULL, NULL);
  cv::Mat normalized = 0 + (sobel - min_val)*(255-0) / (max_val - min_val); // cv::bitwise_and, cv::bitwise_or

  // convert to binary? cv::Mat::convertTo, Convert image to type CV_8U (8-bit unnsigned cv::Mat)
  normalized.convertTo(binary, CV_8U);

  // apply more upper/lower thresholds
  img_thresholded = binary > thresh_min & binary < thresh_max;

  dst = img_thresholded;

}



void sobel_mag_dir_thresh(cv::Mat& src, cv::Mat& mag, cv::Mat& dir, int sobel_kernel, int mag_thresh_min=40, int mag_thresh_max=100, float dir_thresh_min=0.7, float dir_thresh_max=1.3) {
  cv::Mat gray;
  cv::Mat sobelx;
  cv::Mat sobely;
  double min_val_grad;
  double max_val_grad;
  cv::Mat binary_gradient;
  cv::Mat grad_mag;
  cv::Mat grad_dir;

  cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

  //Find both the gradient with respect to x and y CHECK DEPTH??
  cv::Sobel(gray, sobelx, CV_32F, 1, 0);
  cv::Sobel(gray, sobely, CV_32F, 0, 1);

  //Take the absolute value of the gradient images
  cv::Mat gradx = cv::abs(sobelx);
  cv::Mat grady = cv::abs(sobely);

  //Calculate the magnitude and direction of the gradient
  cv::cartToPolar(grady, gradx, grad_mag, grad_dir, false);

  //Normalize the magnitude of the gradient to be between 0-255
  cv::minMaxLoc(grad_mag, &min_val_grad, &max_val_grad, NULL, NULL);
  cv::Mat normalized_gradient = 0 + (grad_mag - min_val_grad)*(255-0) / (max_val_grad - min_val_grad);

  //Convert the magnitude of the gradient to type CV_8U
  normalized_gradient.convertTo(binary_gradient, CV_8U);

  //Apply thresholds for both the magnitude and direction
  mag = normalized_gradient > mag_thresh_min & normalized_gradient < mag_thresh_max;
  dir = grad_dir > dir_thresh_min & grad_dir < dir_thresh_max;

}

void apply_thresholds(cv::Mat& src, cv::Mat& out) {
    cv::Mat img;
    cv::Mat hsv;
    cv::Mat sobelx_img;
    cv::Mat channel;

    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    abs_sobel_thresh(src, sobelx_img, 'x', 15, 40, 100);
    cv::extractChannel(hsv, channel, 2);
    cv::inRange(channel, 220, 255, out);
}