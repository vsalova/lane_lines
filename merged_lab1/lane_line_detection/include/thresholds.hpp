#ifndef THRESHOLDS_HPP
#define THRESHOLDS_HPP

#include <opencv2/opencv.hpp>
#include <iostream>

// applies sobel algorithm with respect to x or y and applies thresholds
void abs_sobel_thresh(cv::Mat &src, cv::Mat &dst, char orient, int sobel_kernel, int thresh_min, int thresh_max);

// uses sobel algorithm with respect to x and y to find direction of gradient and applies thresholds
// NOTE: thresh_min and thresh_max are in radians
void sobel_mag_dir_thresh(cv::Mat &src, cv::Mat &mag, cv::Mat &angle, int sobel_kernel, int mag_thresh_min, int mag_thresh_max, float dir_thresh_min, float dir_thresh_max);

/*
 * TODO:
 * Make function prototype here for your apply_thresholds function
 */

void apply_thresholds(cv::Mat &src, cv::Mat &dst, std::string file_yellow, std::string file_white);

#endif /* THRESHOLDS_HPP */
