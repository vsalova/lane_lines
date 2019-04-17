#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <array>
#include "overloader.hpp"
#include "cv_helper.hpp"

// draws the four source or destination points on an image
void draw_viewing_window(cv::Mat& dst, std::vector<cv::Point2f>& points, cv::Scalar color, int thickness);

int main(int argc, char* argv[]) {

  if (argc != 3) {
    std::cerr << "Usage: ./perspective_transform_tester <yaml_file> <path/to/images/\\*.png> \n";
    return 1;
  }

  // create save path for final birds-eye view images
  std::string save_path = "../images/warped_images/";
  if (!boost::filesystem::exists(save_path)) {
    boost::filesystem::create_directory(save_path);
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

  cv::Mat img, img1;
  std::vector<cv::Mat> axs;
  for (cv::String path : image_paths) {
    img1 = cv::imread(path);
    cv::undistort(img1, img, mtx, dist, mtx);
    draw_viewing_window(img, src_points, cv::Scalar(0,255,0), 5);
    axs.push_back(img);
    img.release();
    /*
     * TODO:
     * Finish this for-loop to undistort the images and populate axs so you can
     * subplot your drawn source points.
     */
  }
  // TODO: uncomment the line below when you're ready to visualize src points
  subplot("source points", axs, 4, 2);

  axs.clear();

  /*
   * TODO:
   * Obtain your perspective transform matrix M
   */

  cv::Mat warpedImg;
  Mat perspective_transform_matrix = cv::getPerspectiveTransform(src_points, dst_points);

  int i = 0;
  for (cv::String path : image_paths) {
    img1 = cv::imread(path);
    cv::undistort(img1, img, mtx, dist, mtx);
    cv::warpPerspective(img, warpedImg, perspective_transform_matrix, img.size());
    imwrite(save_path + "warped" + std::to_string(i) + ".png", warpedImg);
    draw_viewing_window(warpedImg, dst_points, cv::Scalar(0,255,0), 5);
    axs.push_back(warpedImg);
    warpedImg.release();
    i++;
    /*
     * TODO:
     * Finish this for-loop to undistort the images and apply the perspective
     * transformation. Be sure to draw your destination points as well, so you
     * can easily tune them! axs should be re-populated with the destination
     * points images
     */
  }
  // TODO: uncomment the line below when you're ready to visualize dst points
  subplot("destination points", axs, 4, 2);

  return 0;
}

void draw_viewing_window(cv::Mat& dst, std::vector<cv::Point2f>& points, cv::Scalar color, int thickness) {
  if (points.size() != 4) {
    std::stringstream ss;
    ss << "Invalid number of points. There must be exactly 4 points.\n";
    throw std::runtime_error(ss.str());
  }
  cv::line(dst, points[0], points[1], color, thickness);
  cv::line(dst, points[1], points[2], color, thickness);
  cv::line(dst, points[2], points[3], color, thickness);
  cv::line(dst, points[3], points[0], color, thickness);
}
