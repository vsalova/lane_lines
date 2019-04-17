#ifndef LANE_LINE_HPP
#define LANE_LINE_HPP

class LaneLine {
 public:
  // Constructor
  LaneLine();
  // Destructor
  //~LaneLine();
  // Updates best-fit line of lane line
  void fit(cv::Mat& line_inds_x, cv::Mat& line_inds_y, cv::Mat& best_fit, cv::Mat& ploty, cv::Mat& fitx, cv::Mat& binary_warped);
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

};

#endif /* LANE_LINE_HPP */
