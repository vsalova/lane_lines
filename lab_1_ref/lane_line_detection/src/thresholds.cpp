#include "thresholds.hpp"
#include "cv_helper.hpp"

void abs_sobel_thresh(cv::Mat &src, cv::Mat &dst, char orient, int sobel_kernel = 7, int thresh_min = 40, int thresh_max = 100)
{
  cv::Mat gray;
  cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

  if (orient == 'x')
  {
    // TODO: Make the appropriate call to cv::Sobel
    cv::Sobel(gray, dst, CV_32F, 1, 0, sobel_kernel);
  }
  else if (orient == 'y')
  {
    // TODO: Make the appropriate call to cv::Sobel
    cv::Sobel(gray, dst, CV_32F, 0, 1, sobel_kernel);
  }
  else
  {
    std::stringstream ss;
    ss << orient << "is invalid for argument orient. Must be either 'x' or 'y'.\n";
    throw std::runtime_error(ss.str());
  }
  /*
   * TODO:
   * Finish the rest of this function definition
   */
  dst = cv::abs(dst);                                    // make it absolute
  cv::normalize(dst, dst, 0, 255, NORM_MINMAX, CV_8UC1); // normalize
  dst = thresh_min<dst & thresh_max> dst;                // applying threshold
}

void sobel_mag_dir_thresh(cv::Mat &src, cv::Mat &mag, cv::Mat &dir, int sobel_kernel, int mag_thresh_min = 40, int mag_thresh_max = 100, float dir_thresh_min = 0.7, float dir_thresh_max = 1.3)
{
  cv::Mat gray;
  cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
  /*
   * TODO:
   * Finish the rest of this function definition
   */
  cv::Mat gradx, grady;
  cv::Sobel(gray, gradx, CV_32F, 1, 0, sobel_kernel);
  cv::Sobel(gray, grady, CV_32F, 0, 1, sobel_kernel);
  gradx = cv::abs(gradx); // make it absolute
  grady = cv::abs(grady); // make it absolute

  cv::cartToPolar(gradx, grady, mag, dir);
  cv::normalize(mag, mag, 0, 255, NORM_MINMAX, CV_8UC1); // normalize the magnitude
  mag = mag_thresh_min<mag & mag_thresh_max> mag;        // applying threshold
  dir = dir_thresh_min<dir & dir_thresh_max> dir;        // applying threshold
}

/*
 * TODO:
 * Implement your apply_thresholds function here
 */

void apply_thresholds(cv::Mat &src, cv::Mat &dst, std::string file_yellow, std::string file_white)
{
  cv::Mat gradx, grady, mag, dir;
  std::vector<cv::Mat> axs;
  HSVmanager hsvy, hsvw;
  hsvy.img2HSV(src, file_yellow);
  hsvw.img2HSV(src, file_white);
  cv::add(hsvy.hsvImage, hsvw.hsvImage, dst);
  cv::medianBlur(dst, dst, 7);
  abs_sobel_thresh(src, gradx, 'x');
  cv::addWeighted(dst, 0.8, gradx, 0.5, 0, dst);

  cv::medianBlur(dst, dst, 7);

  //abs_sobel_thresh(src, grady, 'y');
  // sobel_mag_dir_thresh(src, mag, dir, 7);

  // // debug
  // axs.push_back(src);
  // axs.push_back(mag);
  // axs.push_back(dir);
  // axs.push_back(hsvy.hsvImage);
  // axs.push_back(hsvw.hsvImage);
  // axs.push_back(dst);
  // //axs.push_back(hsvw.hsvImage);

  //subplot("test", axs, 2, 1);
  //cv::waitKey();
}
