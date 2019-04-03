#ifndef WINDOW_SEARCH_HPP
#define WINDOW_SEARCH_HPP
#include <utility>
#include "lane_line.hpp"

void window_search(cv::Mat& binary_warped, cv::Mat& window_img, std::vector<std::unique_ptr<LaneLine>>& lane_lines, 
                   std::vector<int>& peaks, int n_windows, int margin, int minpix, cv::Mat& fitx1, cv::Mat& ploty1, cv::Mat& fitx2, cv::Mat& final_result_img);

std::vector<std::pair<double, double>> generate_waypoints(cv::Mat& window_img, cv::Mat& fitx1, cv::Mat& ploty1, cv::Mat& fitx2);













#endif /* WINDOW_SEARCH_HPP */