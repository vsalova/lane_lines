#include <opencv2/opencv.hpp>
#include <iostream>
#include <utility>
#include "thresholds.hpp"
#include "signal_proc.hpp"
#include "window_search.hpp"
#include "lane_line.hpp"
#include "cv_helper.hpp"
#include "overloader.hpp"

int main(int argc, char *argv[])
{
  if (argc != 5)
  {
    std::cerr << "Usage: ./video_main <path_to_matrix> <path_to_yellow> <path_to_white> <path/to/images/\\*.jpg> \n";
    return 1;
  }

  std::string file_matrix = argv[1];
  std::string file_yellow = argv[2];
  std::string file_white = argv[3];
  std::string file_images = argv[4];

  std::cout << file_yellow << file_white << file_images << std::endl;

  cv::FileStorage file(file_matrix, cv::FileStorage::READ);
  cv::Mat dist, mtx;
  std::vector<cv::Point2f> src_points, dst_points;

  file["distortion coefficients"] >> dist;
  file["camera matrix"] >> mtx;
  file["source points"] >> src_points;
  file["destination points"] >> dst_points;

  std::vector<cv::String> image_paths;
  cv::glob(file_images, image_paths);

  cv::Mat img, undistorted, transformed, thresholded;
  cv::Mat transform_matrix = cv::getPerspectiveTransform(src_points, dst_points);

  for (cv::String path : image_paths)
  {
    img = cv::imread(path);
    cv::undistort(img, undistorted, mtx, dist);
    cv::warpPerspective(undistorted, transformed, transform_matrix, Size(1280, 720));
    apply_thresholds(transformed, thresholded, file_yellow, file_white);
    cv::imshow("test", thresholded);
    cv::waitKey(0);
  }

  return 0;
}
