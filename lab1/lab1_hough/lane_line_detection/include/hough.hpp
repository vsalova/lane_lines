#ifndef HOUGH_HPP
#define HOUGH_HPP
#include <utility>
#include "lane_line.hpp"

void mat_to_vector_field(cv::Mat& source, std::vector<std::tuple<double, double, double>>& result,
                         int window_size, int num_rows_to_find_angle);

void draw_vector_field(cv::Mat& dest, std::vector<std::tuple<double, double, double>>& vectors, double line_length);

void draw_grid(cv::Mat& dest, int width);

#endif /* HOUGH_HPP */
