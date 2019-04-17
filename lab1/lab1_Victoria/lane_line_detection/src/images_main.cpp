#include <opencv2/opencv.hpp>
#include <iostream>
#include <utility>
#include "thresholds.hpp"
#include "signal_proc.hpp"
#include "window_search.hpp"
#include "lane_line.hpp"
#include "cv_helper.hpp"
#include "overloader.hpp"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: ./images_main <yaml_file> <path/to/images/\\*.png> \n";
    return 1;
  }

    // read in yaml file
  cv::FileStorage file(argv[1], cv::FileStorage::READ);
  // declare camera matrix and list of distortion coefficients
  cv::Mat dist, mtx;
  // declare source points and destination points
  std::vector<cv::Point2f> src_points, dst_points;

  file["distortion coefficients"] >> dist;
  file["camera matrix"] >> mtx;
  file["source points"] >> src_points;
  file["destination points"] >> dst_points;

  std::vector<cv::String> image_paths;
  cv::glob(argv[2], image_paths);

  cv::Mat warpedImg, thresholdedImg;
  Mat perspective_transform_matrix = cv::getPerspectiveTransform(src_points, dst_points);
  Mat Minv = cv::getPerspectiveTransform(dst_points, src_points);

  cv::Mat img1, img, binary_warped, final_warped, final_overlay;

  for (cv::String path : image_paths) {
  	// Perspective Transformation
    img1 = cv::imread(path);
    cv::undistort(img1, img, mtx, dist, mtx);
    cv::warpPerspective(img, warpedImg, perspective_transform_matrix, img.size());

    // Applying thresholds
    apply_thresholds(warpedImg, binary_warped);

    cv::Mat histogram, output;

  	cv::cvtColor(binary_warped, output, cv::COLOR_GRAY2BGR);

  	std::vector<int> peaks;
  	peaks = findPeaks(binary_warped, histogram);

	unique_ptr<LaneLine> line(new LaneLine());
  	std::vector<unique_ptr<LaneLine>> lane_lines;
  	lane_lines.push_back(std::move(line));

  	cv::Mat fitx1, ploty1, fitx2;
  	cv::Mat final_result_img(720, 1280, CV_8UC3, Scalar(0,0,0));
  	std::vector<std::pair<double, double>> waypoints_meters;

  	window_search(binary_warped, output, lane_lines, peaks,  9, 100, 75, fitx1, ploty1, fitx2, final_result_img);
  	waypoints_meters = generate_waypoints(final_result_img, fitx1, ploty1, fitx2);
  	cv::warpPerspective(final_result_img, final_warped, Minv, final_result_img.size());

  	cout << waypoints_meters << endl;
  	cv::addWeighted(img, 1, final_warped, 4, 0, final_overlay);
  	cv::imshow("disp2", final_overlay);
  	cv::waitKey(0);
  }
  return 0;
}
