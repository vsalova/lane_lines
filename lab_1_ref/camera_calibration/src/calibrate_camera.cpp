#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include "overloader.hpp"
#include "cv_helper.hpp"

int main(int argc, char *argv[])
{

  if (argc != 3)
  {
    std::cerr << "Usage: ./calibrate_camera <yaml_file> <path/to/images/\\*.png> \n";
    return 1;
  }

  // distortion coefficients and camera matrix
  cv::Mat dist, mtx;

  // read in all checkerboard images
  std::vector<cv::String> image_paths;
  cv::glob(argv[2], image_paths);

  /*
   * TODO:
   * Change nx and ny to their appropriate values. nx is the number of inner
   * corners per row. ny is the number of inner corners per column.
   */

  unsigned int nx;
  unsigned int ny;

  if (std::string(argv[1]).find("fisheye") != std::string::npos) // for fisheye camera
  {
    nx = 10;
    ny = 7;
  }
  else
  { // for udacity images
    nx = 9;
    ny = 6;
  }
  cv::Size boardSize(nx, ny);
  cv::Size imageSize = cv::imread(image_paths[0]).size();

  // list of image points (locations of inner corners in pixel space)
  std::vector<cv::Point2f> imgp;
  // list of object points (locations of inner corners in real-world 3D space)
  std::vector<cv::Point3f> objp;

  for (float y = 0; y < ny; y++)
  {
    for (float x = 0; x < nx; x++)
    {
      objp.push_back(cv::Point3f(y, x, 0));
    }
  }

  /*
   * TODO:
   * Write the necessary nested for-loop to initialize the object points.
   * The object points should look like this:
   * 0, 0, 0
   * 0, 1, 0
   * 0, 2, 0
   * 0, 3, 0
   * ...
   * 8, 8, 0
   * 8, 9, 0
   * Assuming nx is 10 and ny is 9. Notice how the z-dimension is always zero.
   */

  // List of list of object points
  std::vector<std::vector<cv::Point3f>> obj_points;
  // List of list of image points
  std::vector<std::vector<cv::Point2f>> img_points;
  // Input image
  cv::Mat image;
  // Gray Image
  cv::Mat gray;
  // create save path for drawn checkerboard corners.
  std::string save_path = "../drawn_corners/";
  if (!boost::filesystem::exists(save_path))
  {
    boost::filesystem::create_directory(save_path);
  }

  /*
   * TODO:
   * Finish the for-loop below to populate obj_points, img_points, and draw
   * the chessboard corners. Save the images of drawn chessboard corners to the
   * folder ../drawn_corners. Drawing the corners is for debugging. To do this,
   * you'll probably have to look up the API for cv::findChessboardCorners() and
   * cv::drawChessboardCorners(). You'll have to define some of your own variables * this time. Remember to actually check if OpenCV found the chessboard corners
   * before populating obj_points and img_points!
   */

  std::string img_name;
  for (cv::String path : image_paths)
  {
    // Your code goes here...
    image = cv::imread(path);
    cv::cvtColor(image, gray, CV_BGR2GRAY);
    bool patternfound = cv::findChessboardCorners(gray, boardSize, imgp, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
    img_name = path.substr(path.rfind('/') + 1);
    std::cout << img_name << " ,patternfound: " << patternfound << std::endl;
    if (patternfound)
    {
      cv::cornerSubPix(gray, imgp, boardSize, cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.01));
      obj_points.push_back(objp);
      img_points.push_back(imgp);
      cv::cvtColor(gray, gray, CV_GRAY2BGR);
      drawChessboardCorners(gray, boardSize, imgp, patternfound);
      cv::imwrite(save_path + img_name, gray);
    }
  }

  std::cout << img_points.size() << std::endl;
  std::vector<cv::Mat> rvecs, tvecs;

  /*
   * TODO:
   * Make the necessary call to cv::calibrateCamera() to generate your camera
   * matrix (mtx) and list of distortion coefficients (dist).
   */

  calibrateCamera(obj_points, img_points, imageSize, mtx, dist, rvecs, tvecs);

  // Save camera matrix and list of distortion coefficients to yaml file.
  if (dist.rows > 0 && mtx.rows > 0)
  {
    std::cout << "dist: " << dist << std::endl;
    std::cout << "mtx: " << mtx << std::endl;

    cv::FileStorage file(argv[1], cv::FileStorage::WRITE);
    file << "distortion coefficients" << dist;
    file << "camera matrix" << mtx;
    file.release();
  }

  return 0;
}
