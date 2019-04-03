#include <opencv2/opencv.hpp>
#include <iostream>
#include <utility>
#include "thresholds.hpp"
#include "signal_proc.hpp"
#include "window_search.hpp"
#include "lane_line.hpp"
#include "cv_helper.hpp"
#include "overloader.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: ./video_main <yaml_file> \n";
    return 1;
  }

  cv::VideoCapture cap("../videos/project_video.mp4");

  cv::VideoWriter video("../videos/final_output.mp4", 0x00000021, 25, Size(1280, 720));

  if (!cap.isOpened()){
  	cout << "Error opening video stream";
  	return -1;
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

  int i = 0;

  while (1) {

  	cout << "Generating frame " << i << endl;
  	i++;

    cv::Mat frame, new_frame;
    // Capture frame-by-frame
    cap >> frame;

    cv::Mat warpedImg, thresholdedImg;
  	Mat perspective_transform_matrix = cv::getPerspectiveTransform(src_points, dst_points);
  	Mat Minv = cv::getPerspectiveTransform(dst_points, src_points);

  	cv::Mat img1, img, binary_warped, final_warped;

    cv::undistort(frame, img, mtx, dist, mtx);
    cv::warpPerspective(img, warpedImg, perspective_transform_matrix, img.size());

    // Applying thresholds
    apply_thresholds(warpedImg, binary_warped);

    cv::Mat histogram, output;

  	cv::cvtColor(binary_warped, output, cv::COLOR_GRAY2BGR);

  	// if (i == 617){
  	// 	imshow("Frame", binary_warped);
  	// 	cv::waitKey(0);
  	// 	cap.release();
  	// 	video.release();
  	// }

  	std::vector<int> peaks;
  	peaks = findPeaks(binary_warped, histogram);

	unique_ptr<LaneLine> line(new LaneLine());
  	std::vector<unique_ptr<LaneLine>> lane_lines;
  	lane_lines.push_back(std::move(line));

  	cv::Mat fitx1, ploty1, fitx2;
  	cv::Mat final_result_img(720, 1280, CV_8UC3, Scalar(0,0,0));
  	std::vector<std::pair<double, double>> waypoints_meters;

  	try{
	  	window_search(binary_warped, output, lane_lines, peaks,  9, 100, 75, fitx1, ploty1, fitx2, final_result_img);
	  	waypoints_meters = generate_waypoints(final_result_img, fitx1, ploty1, fitx2);
	  	cv::warpPerspective(final_result_img, final_warped, Minv, final_result_img.size());
	  	cv::addWeighted(img, 1, final_warped, 4, 0, new_frame);
	}
	catch(Exception e){
		imshow("Frame", binary_warped);
  		cv::waitKey(0);
  		cap.release();
  		video.release();
	}

  	cout << waypoints_meters << endl;
    video.write(new_frame);

    // // Display the resulting frame
    // imshow( "Frame", new_frame );
    // cv::waitKey(0);

    // Press  ESC on keyboard to exit
    char c=(char)waitKey(25);
    if(c==27)
      break;
  }

  // When everything done, release the video capture object
  cap.release();
  video.release();

  // Closes all the frames
  destroyAllWindows();
  return 0;

}