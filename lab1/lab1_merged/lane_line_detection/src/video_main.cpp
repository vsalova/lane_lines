#include <ctime>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <utility>
#include "thresholds.hpp"
#include "signal_proc.hpp"
#include "window_search.hpp"
#include "lane_line.hpp"
#include "cv_helper.hpp"
#include "overloader.hpp"

unsigned int num_frames = 0;
clock_t total_time = 0;
clock_t total_time_undistort = 0;
clock_t total_time_warp = 0;
clock_t total_time_thresh = 0;
clock_t total_time_convert_color = 0;
clock_t total_time_find_peaks = 0;
clock_t total_time_window_search = 0;
clock_t total_time_waypoints = 0;
clock_t total_time_distort_back = 0;
clock_t total_time_add_weighted = 0;


void print_cv_exception(cv::Exception& e) {
    std::cout << "exception caught: " << e.what() << std::endl;
    std::cout << "ERR: " << e.err << std::endl;
    std::cout << "FILE: " << e.file << std::endl;
    std::cout << "FUNC: " << e.func << std::endl;
    std::cout << "LINE: " << e.line << std::endl;
    std::cout << "MESSAGE: " << e.msg << std::endl;
}

void print_time(clock_t total)
{
    float time = (float) total / CLOCKS_PER_SEC;
    float fps = (num_frames == 0) ? std::numeric_limits<float>::infinity() : num_frames / time;
    std::cout << "total: " << time << " seconds, \tFPS: " << fps << " /s\n";
}

void print_benchmark_progress() {
    std::cout << "Total time-- ";
    print_time(total_time);
    std::cout << "total_time_undistort-- ";
    print_time(total_time_undistort);
    std::cout << "total_time_warp-- ";
    print_time(total_time_warp);
    std::cout << "total_time_thresh-- ";
    print_time(total_time_thresh);
    std::cout << "total_time_convert_color-- ";
    print_time(total_time_convert_color);
    std::cout << "total_time_find_peaks-- ";
    print_time(total_time_find_peaks);
    std::cout << "total_time_window_search-- ";
    print_time(total_time_window_search);
    std::cout << "total_time_waypoints-- ";
    print_time(total_time_waypoints);
    std::cout << "total_time_distort_back-- ";
    print_time(total_time_distort_back);
    std::cout << "total_time_add_weighted-- ";
    print_time(total_time_add_weighted);
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 5 || argc > 7) {
        std::cerr << "Usage: ./video_main <path_to_matrix> <path_to_yellow> <path_to_white> <path_to_video.mp4> [-q] [-b]" << std::endl;
        std::cerr << "-q flag for quiet" << std::endl;
        std::cerr << "-b flag for benchmark" << std::endl;
        return 1;
    }

    // Collect arguments
    std::string file_matrix = argv[1];
    std::string file_yellow = argv[2];
    std::string file_white  = argv[3];
    std::string file_video  = argv[4];


    bool quiet = false;
    bool benchmark = false;
    if (argc == 6) {
        std::string opt_arg_str (argv[5]);
        if (opt_arg_str == "-q")
            quiet = true;
        else if (opt_arg_str == "-b")
            benchmark = true;
    }
    else if (argc == 7) {
        std::string opt_arg_str1 (argv[5]);
        std::string opt_arg_str2 (argv[6]);
        if (opt_arg_str1 == "-q")
            quiet = true;
        else if (opt_arg_str1 == "-b")
            benchmark = true;

        if (opt_arg_str2 == "-q")
            quiet = true;
        else if (opt_arg_str2 == "-b")
            benchmark = true;
    }

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

        num_frames += 1;

        // Processing the video frame starts here, this is where the benchmark will get start time
        clock_t total_start = clock();
        clock_t undistort_start = total_start;  // Same time

        cv::undistort(frame, undistorted, mtx, dist);
        total_time_undistort += clock() - undistort_start;

        clock_t warp_start = clock();
        cv::warpPerspective(undistorted, transformed, transform_matrix, Size(1280, 720));
        total_time_warp += clock() - warp_start;

        clock_t thresh_start = clock();
        apply_thresholds(transformed, thresholded, file_yellow, file_white);
        total_time_thresh += clock() - thresh_start;

        //draw_lanes_waypoints(thresholded, way_point_img);
        cv::Mat histogram, output;
        std::vector<int> peaks;

        clock_t convert_color_start = clock();
        cv::cvtColor(thresholded, output, cv::COLOR_GRAY2BGR);
        total_time_convert_color += clock() - convert_color_start;

        clock_t find_peaks_start = clock();
        peaks = findPeaks(thresholded, histogram);
        total_time_find_peaks += clock() - find_peaks_start;

        unique_ptr<LaneLine> line(new LaneLine());
        std::vector<unique_ptr<LaneLine>> lane_lines;
        lane_lines.push_back(std::move(line));

        cv::Mat fitx1, ploty1, fitx2;
        cv::Mat final_result_img(720, 1280, CV_8UC3, Scalar(0,0,0));
        std::vector<std::pair<double, double>> waypoints_meters;

        try {
            clock_t window_search_start = clock();
            window_search(thresholded, output, lane_lines, peaks,  9, 100, 75, fitx1, ploty1, fitx2, final_result_img);
            total_time_window_search += clock() - window_search_start;

            clock_t generate_waypoints_start = clock();
            waypoints_meters = generate_waypoints(final_result_img, fitx1, ploty1, fitx2);
            total_time_waypoints += clock() - generate_waypoints_start;

            clock_t distort_back_start = clock();
            cv::warpPerspective(final_result_img, dst, reverse_matrix, final_result_img.size());
            total_time_distort_back += clock() - distort_back_start;

            clock_t add_weighted_start = clock();
            cv::addWeighted(undistorted, 1, dst, 4, 0, summed);
            total_time_add_weighted += clock() - add_weighted_start;

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

        // frame processes, end time for the benchmarks frame
        total_time += clock() - total_start;
        if (benchmark && !quiet)
            print_benchmark_progress();

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

    std::cout << "DONE WITH MAKING VIDEO" << std::endl;
    if (benchmark)
        print_benchmark_progress();

    // When everything done, release the video capture object
    cap.release();
    video.release();

    // Closes all the frames
    destroyAllWindows();
    return 0;
}


