#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include "cv_helper.hpp"
#include "overloader.hpp"
#include "thresholds.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: ./thresholds_tester > <path_to_warped_images\\*.png> \n";
    return 1;
  }

  // create save path for final thresholded images
  std::string save_path = "../images/thresholded_images/";
  if (!boost::filesystem::exists(save_path)) {
    boost::filesystem::create_directory(save_path);
  }
  std::vector<cv::String> image_paths;
  cv::glob(argv[1], image_paths);

  cv::Mat sobelx_img;
  cv::Mat sobel_mag_img;
  cv::Mat sobel_dir_img;
  cv::Mat img;
  std::vector<cv::Mat> axs((image_paths.size()));
  std::string filename;

  int i = 0;

  for (cv::String path : image_paths) {
    img = cv::imread(path);
    apply_thresholds(img, axs[i]);
    std::string j = std::to_string(i);
    filename = "../images/thresholded_images/thresholded" + j + ".png";
    imwrite(filename, axs[i]);
    i++;
  }
}