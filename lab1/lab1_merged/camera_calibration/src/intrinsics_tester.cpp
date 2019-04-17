#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include "overloader.hpp"
#include "cv_helper.hpp"

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: ./intrinsics_tester <yaml_file> <path/to/images/\\*.png> \n <fisheye or udacity>";
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

  cv::Mat distorted_img;
  cv::Mat undistorted_img;

  std::vector<cv::Mat> src_imgs;
  std::vector<cv::Mat> results_imgs;
  unsigned int n_images_verify = 5;

  for (unsigned int i = 0; i < n_images_verify; i++)
  {
    std::cout << i << std::endl;
    distorted_img = cv::imread(image_paths[i]);
    undistort(distorted_img, undistorted_img, mtx, dist);
    src_imgs.push_back(distorted_img);
    results_imgs.push_back(std::move(undistorted_img));
  }
  std::cout << src_imgs.size() << std::endl;
  src_imgs.insert(src_imgs.end(), results_imgs.begin(), results_imgs.end());
  subplot("compare", src_imgs, results_imgs.size(), 2);

  return 0;
}
