#ifndef LANE_LINE_HPP
#define LANE_LINE_HPP

class LaneLine
{
public:
  // Constructor
  LaneLine(double xm_per_pix, double ym_per_pix, int order, cv::Scalar color);
  // Destructor
  //~LaneLine() {}
  // Updates best-fit line of lane line
  void fit(int img_rows);

  //THESE ARE THE PIXELS OF THE ACTUAL LANE LINE
  // y values over which to plot lane line in pixels
  cv::Mat ploty_pix;
  // y values over which to plot lane line in meters
  cv::Mat ploty_meters;
  //

  //THESE ARE THE PIXELS OF THE ACTUAL LANE LINE
  // x values of current best-fit line in pixels
  cv::Mat fitx_pix;
  // x values of current best-fit line in meters
  cv::Mat fitx_meters;

  //COEFFICIENTS FOR POLYNOMIAL BEST FIT
  // current best-fit coefficients for lane line in pixels
  cv::Mat current_fit_pix;
  // currrent best-fit coefficients for lane line in meters
  cv::Mat current_fit_meters;

  //pixel points for all of the pixels in the lane line that window search finds
  std::vector<int> window_pts_x;
  std::vector<int> window_pts_y;
  // the points on the lane line are (window_pts_x + margin, window_pts_y)

  // conversion factor for pixels to meters along x-axis
  double xm_per_pix;
  // conversion factor for pixels to meters along y-axis
  double ym_per_pix;
  // order of polynomial to be used for best-fit line
  int order;
  // color of lane lines pixels
  cv::Scalar color;

  //history of laneline NOT USED CURRENTLY
  /*
  cv::Mat ploty_pix_hist;
  cv::Mat ploty_meters_hist;
  cv::Mat fitx_pix_hist;
  cv::Mat fitx_meters_hist;
  */
};

#endif /* LANE_LINE_HPP */
