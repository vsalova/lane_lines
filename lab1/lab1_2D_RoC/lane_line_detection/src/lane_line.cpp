#include <opencv2/opencv.hpp>
#include <iostream>
#include "lane_line.hpp"
#include "signal_proc.hpp"

LaneLine::LaneLine() {
    xm_per_pix = 3.7/700;
    ym_per_pix = 30./720;
    order = 2;
    color = cv::Scalar(0, 255, 0);
}

LaneLine::LaneLine(double xm_per_pix, double ym_per_pix, int order, cv::Scalar color) {
    this->xm_per_pix = xm_per_pix;
    this->ym_per_pix = ym_per_pix;
    this->order = order;
    this->color = color;
}


// Updates best-fit line of lane line
void LaneLine::fit(cv::Mat& line_inds_x, cv::Mat& line_inds_y, cv::Mat& best_fit, cv::Mat& ploty, cv::Mat& fitx, cv::Mat& binary_warped) {
    polyFit(line_inds_y, line_inds_x, best_fit, 2);
    // generate y-values for plotting best-fit line
    linspace(0, binary_warped.rows, binary_warped.rows-1, ploty);
    // generate x values of best-fit line
    polyVal(ploty, fitx, best_fit);
}
