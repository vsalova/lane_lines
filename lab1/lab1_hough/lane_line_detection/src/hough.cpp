#include <ctime>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
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
        throw std::runtime_error("window_size doesn't divice the source width or height\n");
    }
    else if (source.channels() != 1) {
        throw std::runtime_error("source image should does not have exactly one channel\n");
    }

    int num_windows_x = source.cols / window_size;
    int num_windows_y = source.rows / window_size;

    for (int window_num_y = 0; window_num_y < num_windows_y; window_num_y++) {
    for (int window_num_x = 0; window_num_x < num_windows_x; window_num_x++) {
        int src_top_y = window_num_y * window_size;
        int src_bottom_y = src_top_y + window_size - 1;     // Bottom is inside the window
        int src_left_x = window_num_x * window_size;
        int src_right_x = src_left_x + window_size;
        int src_middle_x = src_left_x + (window_size / 2);
        //int src_middle_y = src_top_y + (window_size / 2);
        // Find all non-zero points in the top rectangle
        cv::Mat window = source(cv::Range(src_top_y, src_top_y + num_rows_to_find_angle), cv::Range(src_left_x, src_right_x));
        std::vector<Point> non_zero;
        cv::findNonZero(window, non_zero);
        if (non_zero.size() == 0)     // Non zero pixels not found
            continue;

        // Get average point of non zero pixels
        double total_y = 0;
        double total_x = 0;
        for (unsigned int i = 0; i < non_zero.size(); i++) {
            Point p = non_zero[i];
            total_y += p.y;
            total_x += p.x;
        }
        double avg_y = total_y / non_zero.size();
        double avg_x = total_x / non_zero.size();

        // Calculate angle
        double delta_y = window_size - avg_y;   // Referrence is the bottom of the window
        double delta_x = avg_x - (window_size / 2); // Referrence is the middle x of the window
        double theta = atan2(delta_y, delta_x);

        result.push_back(std::make_tuple(src_middle_x, src_bottom_y, theta));
    }}
}


void draw_vector_field(cv::Mat& dest, std::vector<std::tuple<double, double, double>>& vectors, double line_length)
{
    cv::RNG rng(12345);
    for (auto v : vectors) {
        double x1 = std::get<0>(v);
        double y1 = std::get<1>(v);
        double theta = std::get<2>(v);
        double x2 = x1 + cos(theta) * line_length;
        double y2 = y1 - sin(theta) * line_length;

        cv::circle(dest, Point(x1, y1), 2, Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255)), CV_FILLED);
        cv::arrowedLine(dest, Point(x1, y1), Point(x2, y2), Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255)), 2, 8, 0, 0.3);
    }
}

void draw_grid(cv::Mat& dest, int width)
{
    cv::Scalar color (100,100,100); //BGR color
    for (int y = 0; y < dest.rows; y += width) {
        // Draw horizontal line
        cv::line(dest, Point(0, y), Point(dest.cols, y), color);
    }
    for (int x = 0; x < dest.cols; x += width) {
        // Draw verticle line
        cv::line(dest, Point(x, 0), Point(x, dest.rows), color);
    }
}
