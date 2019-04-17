#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include "overloader.hpp"
#include "cv_helper.hpp"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: ./intrinsics_tester <yaml_file> <path/to/images/\\*.png> \n";
    return 1;
  }
  cv::FileStorage file(argv[1], cv::FileStorage::READ);
  cv::Mat dist, mtx;
  file["distortion coefficients"] >> dist;
  file["camera matrix"] >> mtx;

  std::cout << "dist: " << dist << std::endl;
  std::cout << "mtx: " << mtx << std::endl;

  std::vector<cv::String> image_paths;
  cv::glob(argv[2], image_paths);

  /*
   * TODO:
   * Undistort any 8 checkerboard images and show them along-side the
   * distorted images using the subplot() function provided in cv_helper.hpp.
   * This is to visually verify your camera matrix and distortion coefficients
   * are correct. If your undistorted checkerboards do not look correct, you
   * should redo your calibrate_camera.cpp and try again.
   */
  std::string save_path = "../testout/";
  if (!boost::filesystem::exists(save_path)) {
  boost::filesystem::create_directory(save_path);
  }
  cv::Mat image, out;

  std::vector<cv::Mat> imgout;
  for (int i = 0; i < 8; i++){
  image = cv::imread(image_paths[i]);
  cv::undistort(image, out, mtx, dist, mtx);
  imgout.push_back(out);
  imwrite("../testout/testout"+std::to_string(i)+".png", out);
  
  }
  subplot("test", imgout, 1, 8);
  
  return 0;
}
