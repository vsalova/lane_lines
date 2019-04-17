#include <opencv2/opencv.hpp>
#include <iostream>
#include "lane_line.hpp"
#include "signal_proc.hpp"


// Constructor
LaneLine::LaneLine() {
    xm_per_pix = 3.7/700;
    ym_per_pix = 30./720;
    order = 2;
    color = cv::Scalar(0, 255, 0);
}

// Destructor
// ~LaneLine();

// Updates best-fit line of lane line
void LaneLine::fit(cv::Mat& line_inds_x, cv::Mat& line_inds_y, cv::Mat& best_fit, cv::Mat& ploty, cv::Mat& fitx, cv::Mat& binary_warped) {
    polyFit(line_inds_y, line_inds_x, best_fit, 2);
      // generate y-values for plotting best-fit line
    linspace(0, binary_warped.rows, binary_warped.rows-1, ploty);

    // generate x values of best-fit line
    polyVal(ploty, fitx, best_fit);
}

// y values over which to plot lane line in pixels
cv::Mat ploty_pix;

// y values over which to plot lane line in meters
cv::Mat ploty_meters;

// x values of current best-fit line in pixels
cv::Mat fitx_pix;

// x values of current best-fit line in meters
cv::Mat fitx_meters;

// color of lane lines pixels
cv::Scalar color;

// current best-fit coefficients for lane line in pixels
cv::Mat current_fit_pix;

// currrent best-fit coefficients for lane line in meters
cv::Mat current_fit_meters;

// conversion factor for pixels to meters along x-axis
double xm_per_pix;

// conversion factor for pixels to meters along y-axis
double ym_per_pix;

// order of polynomial to be used for best-fit line
int order;