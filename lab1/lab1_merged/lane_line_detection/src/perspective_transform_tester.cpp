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
  std::string save_path = "../images/warped_images";
  std::string saveTo;
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


  cv::Mat img;
  std::vector<cv::Mat> axs(image_paths.size());

  int i = 0;
  for (cv::String path : image_paths) {

    img = cv::imread(path);
    cv::undistort(img, axs[i], mtx, dist);
    //axs[i] = img;
    draw_viewing_window(axs[i], dst_points, cv::Scalar(0,255,0), 5);



    /*
     * TODO:
     * Finish this for-loop to undistort the images and populate axs so you can
     * subplot your drawn source points.
     */
    i++;
  }
  // TODO: uncomment the line below when you're ready to visualize src points
  //subplot("source points", axs, 4, 2);
  //cv::imshow("Test image", axs[0]);
  //cv::waitKey();

  //axs.clear();
  /*
   * TODO:
   * Obtain your perspective transform matrix M
   */
  Mat M = cv::getPerspectiveTransform(src_points, dst_points);
  //cout << M;
  i = 0;
  for (cv::String path : image_paths) {
    img = cv::imread(path);
    //cv::undistort(img, img, mtx, dist);
    cv::warpPerspective(img, axs[i], M, Size(1280, 720));
    //cout << axs.size();
    draw_viewing_window(axs[i], dst_points, cv::Scalar(0,255,0), 5);
    saveTo = save_path;
    saveTo +="/image";
    saveTo += std::to_string(i);
    saveTo += ".jpg";
    cout << endl << saveTo << endl;

    cv::imwrite(saveTo, axs[i]);
    /*
     * TODO:
     * Finish this for-loop to undistort the images and apply the perspective
     * transformation. Be sure to draw your destination points as well, so you
     * can easily tune them! axs should be re-populated with the destination
     * points images
     */
    i++;
  }
  // TODO: uncomment the line below when you're ready to visualize dst points
  subplot("destination points", axs, 4, 2);
  //cv::imshow("Test image", axs[0]);
  //cv::waitKey();

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
