#ifndef WINDOW_SEARCH_HPP
#define WINDOW_SEARCH_HPP
#include <utility>
#include "lane_line.hpp"

/*
binary_warped = bitmap image of (hopefully) the lanelines
window_img = actual image of the lane, colored?  using this as the image that window_search draws to
lane_lines = list of lane lines
peaks = list of peaks
n_windows = number of windows to use
margin = half of window width
minpix = meters in pixels
*/

void window_search(cv::Mat &binary_warped, cv::Mat &window_img, std::vector<std::unique_ptr<LaneLine>> &lane_lines,
                   std::vector<int> &peaks, int n_windows, int margin, int minpix);

std::vector<std::pair<double, double>> generate_waypoints(cv::Mat &overlay_img, std::vector<std::unique_ptr<LaneLine>> &lane_lines,
                                                          double start_fraction, double stop_fraction, int num_waypoints, int radius,
                                                          double xm_per_pix, double ym_per_pix);

void draw_lanes_waypoints(cv::Mat &src, cv::Mat &dst);

void print_cv_exception_2(cv::Exception& e);

cv::Rect get_rect_in_bounds(int x, int y, int width, int height, cv::Mat &src);
#endif /* WINDOW_SEARCH_HPP */
