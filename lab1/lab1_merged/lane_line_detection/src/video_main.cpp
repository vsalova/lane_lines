// #include <opencv2/opencv.hpp>
// #include <iostream>
// #include <utility>
// #include "thresholds.hpp"
// #include "signal_proc.hpp"
// #include "window_search.hpp"
// #include "lane_line.hpp"
// #include "cv_helper.hpp"
// #include "overloader.hpp"
//
// void print_cv_exception(cv::Exception& e);
//
// int main(int argc, char *argv[])
// {
//     if (argc != 5)
//     {
//         std::cerr << "Usage: ./video_main <path_to_matrix> <path_to_yellow> <path_to_white> <path_to_video.mp4> \n";
//         return 1;
//     }
//
//     // Collect arguments
//     std::string file_matrix = argv[1];
//     std::string file_yellow = argv[2];
//     std::string file_white = argv[3];
//     std::string file_video = argv[4];
//
//     //Create video file
//     cv::FileStorage file(file_matrix, cv::FileStorage::READ);
//     VideoCapture cap(file_video);
//     int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
//     int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
//
//     cv::Mat dist, mtx;
//     std::vector<cv::Point2f> src_points, dst_points;
//     VideoWriter video("../videos/final.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(frame_width, frame_height));
//
//     file["distortion coefficients"] >> dist;
//     file["camera matrix"] >> mtx;
//     file["source points"] >> src_points;
//     file["destination points"] >> dst_points;
//
//     cv::Mat undistorted, transformed, thresholded, dst, summed;
//     cv::Mat transform_matrix = cv::getPerspectiveTransform(src_points, dst_points);
//     cv::Mat reverse_matrix = cv::getPerspectiveTransform(dst_points, src_points);
//
//     // Check if camera opened successfully
//     if (!cap.isOpened()) {
//         std::cout << "Error opening video stream or file" << std::endl;
//         return -1;
//     }
//
//     while (true) {
//         Mat frame, way_point_img;
//         // Capture frame-by-frame
//         cap >> frame;
//
//         // If the frame is empty, break immediately
//         if (frame.empty())
//             break;
//
//         cv::undistort(frame, undistorted, mtx, dist);
//         cv::warpPerspective(undistorted, transformed, transform_matrix, Size(1280, 720));
//         apply_thresholds(transformed, thresholded, file_yellow, file_white);
//         draw_lanes_waypoints(thresholded, way_point_img);
//
//         cv::warpPerspective(way_point_img, dst, reverse_matrix, Size(1280, 720));
//         //cv::Mat color;
//         //cv::cvtColor(thresholded, color, cv::COLOR_GRAY2BGR);
//         //cv::warpPerspective(color, dst, reverse_matrix, Size(1280, 720));
//
//         cv::addWeighted(undistorted, 01., dst, 1.0, 0, summed);
//
//         // Display the resulting
//         cv::imshow("Frame", summed);
//         //cv::imshow("thresholded testing", thresholded);
//         //cv::imshow("waypoint image testing", way_point_img);
//         //cv::imshow("warped perspective", transformed);
//         //cv::waitKey();    // Pause at every frame
//         video.write(summed);
//
//         // Press  ESC on keyboard to exit
//         char c = (char) waitKey(25);
//         if (c == 27)
//             break;
//     }
//
//     // When everything done, release the video capture object
//     cap.release();
//     video.release();
//
//     // Closes all the frames
//     destroyAllWindows();
//     return 0;
// }
//
// void print_cv_exception(cv::Exception& e) {
//     std::cout << "exception caught: " << e.what() << std::endl;
//     std::cout << "ERR: " << e.err << std::endl;
//     std::cout << "FILE: " << e.file << std::endl;
//     std::cout << "FUNC: " << e.func << std::endl;
//     std::cout << "LINE: " << e.line << std::endl;
//     std::cout << "MESSAGE: " << e.msg << std::endl;
// }



#include <opencv2/opencv.hpp>
#include <iostream>
#include <utility>
#include "thresholds.hpp"
#include "signal_proc.hpp"
#include "window_search.hpp"
#include "lane_line.hpp"
#include "cv_helper.hpp"
#include "overloader.hpp"

// TODO: WHY THE COMMENTED OUT PART ABOVE?

void print_cv_exception(cv::Exception& e);

int main(int argc, char *argv[])
{
    if (argc != 5) {
        std::cerr << "Usage: ./video_main <path_to_matrix> <path_to_yellow> <path_to_white> <path_to_video.mp4>\n";
        return 1;
    }

    // Collect arguments
    std::string file_matrix = argv[1];
    std::string file_yellow = argv[2];
    std::string file_white  = argv[3];
    std::string file_video  = argv[4];

    //Create video file
    cv::FileStorage file(file_matrix, cv::FileStorage::READ);
    VideoCapture cap(file_video);
    int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    cv::Mat dist, mtx;
    std::vector<cv::Point2f> src_points, dst_points;
    VideoWriter video("../videos/final.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(frame_width, frame_height));

    file["distortion coefficients"] >> dist;
    file["camera matrix"] >> mtx;
    file["source points"] >> src_points;
    file["destination points"] >> dst_points;

    cv::Mat undistorted, transformed, thresholded, dst, summed;
    cv::Mat transform_matrix = cv::getPerspectiveTransform(src_points, dst_points);
    cv::Mat reverse_matrix = cv::getPerspectiveTransform(dst_points, src_points);

    // Check if camera opened successfully
    if (!cap.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    while (true) {
        Mat frame, way_point_img;
        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        cv::undistort(frame, undistorted, mtx, dist);
        cv::warpPerspective(undistorted, transformed, transform_matrix, Size(1280, 720));
        apply_thresholds(transformed, thresholded, file_yellow, file_white);
        //draw_lanes_waypoints(thresholded, way_point_img);
        cv::Mat histogram, output;
      	cv::cvtColor(thresholded, output, cv::COLOR_GRAY2BGR);
      	std::vector<int> peaks;
      	peaks = findPeaks(thresholded, histogram);
    	unique_ptr<LaneLine> line(new LaneLine());
      	std::vector<unique_ptr<LaneLine>> lane_lines;
      	lane_lines.push_back(std::move(line));

      	cv::Mat fitx1, ploty1, fitx2;
      	cv::Mat final_result_img(720, 1280, CV_8UC3, Scalar(0,0,0));
      	std::vector<std::pair<double, double>> waypoints_meters;

        try {
            window_search(thresholded, output, lane_lines, peaks,  9, 100, 75, fitx1, ploty1, fitx2, final_result_img);
            waypoints_meters = generate_waypoints(final_result_img, fitx1, ploty1, fitx2);


            cv::warpPerspective(final_result_img, dst, reverse_matrix, final_result_img.size());
            cv::addWeighted(undistorted, 1, dst, 4, 0, summed);
            // cv::warpPerspective(way_point_img, dst, reverse_matrix, Size(1280, 720));
            //cv::Mat color;
            //cv::cvtColor(thresholded, color, cv::COLOR_GRAY2BGR);
            //cv::warpPerspective(color, dst, reverse_matrix, Size(1280, 720));
            // cv::addWeighted(undistorted, 01., dst, 1.0, 0, summed);
        } catch(Exception e){
            imshow("Frame", thresholded);
            cv::waitKey(0);
            video.write(summed);
            cap.release();
            video.release();
        }
        // Display the resulting
        cv::imshow("Frame", summed);
        //cv::imshow("thresholded testing", thresholded);
        //cv::imshow("waypoint image testing", way_point_img);
        //cv::imshow("warped perspective", transformed);
        //cv::waitKey();    // Pause at every frame
        video.write(summed);

        // Press  ESC on keyboard to exit
        char c = (char) waitKey(25);
        if (c == 27)
            break;
    }

    // When everything done, release the video capture object
    cap.release();
    video.release();

    // Closes all the frames
    destroyAllWindows();
    return 0;
}

void print_cv_exception(cv::Exception& e) {
    std::cout << "exception caught: " << e.what() << std::endl;
    std::cout << "ERR: " << e.err << std::endl;
    std::cout << "FILE: " << e.file << std::endl;
    std::cout << "FUNC: " << e.func << std::endl;
    std::cout << "LINE: " << e.line << std::endl;
    std::cout << "MESSAGE: " << e.msg << std::endl;
}
