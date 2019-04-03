#include <opencv2/opencv.hpp>
#include <iostream>
#include <matplotlibcpp.h>
#include "overloader.hpp"
#include "cv_helper.hpp"
#include "thresholds.hpp"
#include "window_search.hpp"
#include "signal_proc.hpp"
#include "lane_line.hpp"

namespace plt = matplotlibcpp;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: ./window_search_tester <path_to_thresholded_images\\*.png> \n";
    return 1;
  }
  std::vector<cv::String> image_paths;
  cv::glob(argv[1], image_paths);
  cv::Mat binary_warped, histogram;


  cv::Mat output;
  binary_warped = cv::imread(image_paths[0], CV_LOAD_IMAGE_GRAYSCALE);
  cv::cvtColor(binary_warped, output, cv::COLOR_GRAY2BGR);

  std::vector<int> peaks;
  peaks = findPeaks(binary_warped, histogram);
  // Feel free to comment out the lines below. They are for visualizing histogram.
  std::vector<double> histogram_vis;
  histogram.copyTo(histogram_vis);
  // plt::plot(histogram_vis);
  // plt::show();

  unique_ptr<LaneLine> line(new LaneLine());
  std::vector<unique_ptr<LaneLine>> lane_lines;
  lane_lines.push_back(std::move(line));

  cv::Mat fitx1, ploty1, fitx2;


  cv::Mat final_result_img(720, 1280, CV_8UC3, Scalar(0,0,0));

  std::vector<std::pair<double, double>> waypoints_meters;

  window_search(binary_warped, output, lane_lines, peaks,  9, 100, 75, fitx1, ploty1, fitx2, final_result_img);
  waypoints_meters = generate_waypoints(final_result_img, fitx1, ploty1, fitx2);

  cout << waypoints_meters << endl;
  cv::namedWindow("disp", WINDOW_AUTOSIZE);
  cv::imshow("disp", output);
  cv::waitKey(0);
  cv::namedWindow("disp2", WINDOW_AUTOSIZE);
  cv::imshow("disp2", final_result_img);
  cv::waitKey(0);


}