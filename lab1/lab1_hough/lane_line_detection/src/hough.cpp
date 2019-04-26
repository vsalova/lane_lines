#include <ctime>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <utility>
#include "thresholds.hpp"
#include "signal_proc.hpp"
#include "lane_line.hpp"
#include "cv_helper.hpp"
#include "overloader.hpp"

/*
 * Takes @source, which should be 1 channel image (all non zero pixels are treated
 * as a 1) and finds a rough vector field by dividing the image into window_size
 * squares, taking the center of each window and taking the average of the top
 * rows (@num_rows_to_find_angle rows) to find the angle of the vector
 * @window_size must evenly divide the dimensions of source, and be >= 4
 * @num_rows_to_find_angle will must be >= window_size
 * @result is a vector of (x, y, angle) (angle in radians)
 * the origin on the vector is the bottom center of each window
 */
void mat_to_vector_field(cv::Mat& source, std::vector<std::tuple<double, double, double>>& result,
                         int window_size, int num_rows_to_find_angle)
{
    if (window_size < 4) {
        throw std::runtime_error("Window size must be >= 4\n");
    }
    else if (num_rows_to_find_angle > window_size) {
        throw std::runtime_error("num_rows_to_find_angle must be >= window_size\n");
    }
    else if (source.rows % window_size != 0 || source.cols % window_size != 0) {
        throw std::runtime_error("window_size doesn't divice the source width or height");
    }
    //TODO else if number of channels is not equal to 1 throw error

    int num_windows_x = source.cols / window_size;
    int num_windows_y = source.rows / window_size;

    for (int window_num_y = 0; window_num_y < num_windows_y; window_num_y++) {
    for (int window_num_x = 0; window_num_x < num_windows_x; window_num_x++) {
        int src_top_y = window_num_y * window_size;
        int src_bottom_y = src_top_y + window_size - 1;     // Bottom is inside the window
        int src_left_x = window_num_x * window_size;
        int src_right_x = src_left_x + window_size;
        //int src_middle_x = src_left_x + (window_size / 2);
        // Find all non-zero points in the top rectangle
        cv::Mat window = source(cv::Range(src_top_y, src_bottom_y), cv::Range(src_left_x, src_right_x));
        cv::Mat non_zero;
        cv::findNonZero(window, non_zero);
        if (non_zero.rows == 0 || non_zero.cols == 0)     // Non zero pixels not found
            continue;

        std::cout << "Non zero dimensions: " << non_zero.size() << "\t" << "channels: " << non_zero.channels() << std::endl;
        // Get average point of non zero pixels
        double total_y = 0;
        double total_x = 0;
        // for all point, add up in non_zero channels 0 and 1
        double avg_y = total_y / non_zero.rows;
        double avg_x = total_x / non_zero.rows;
        // Calculate angle

        // result.push_back(...)
    }}
}
