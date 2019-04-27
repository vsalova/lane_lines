#include <ctime>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>  // For Hough tranform in OpenCV
#include <iostream>
#include <string>
#include <utility>
#include "thresholds.hpp"
#include "signal_proc.hpp"
#include "lane_line.hpp"
#include "cv_helper.hpp"
#include "overloader.hpp"
#include "hough.hpp"

unsigned int num_frames = 0;
clock_t total_time = 0;
clock_t total_time_undistort = 0;
clock_t total_time_warp = 0;
clock_t total_time_thresh = 0;
clock_t total_time_convert_color = 0;
clock_t total_time_hough = 0;
clock_t total_time_distort_back = 0;
clock_t total_time_add_weighted = 0;

const int ESC_ASCII = 27;

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
    std::cout << "total_time_hough-- ";
    print_time(total_time_hough);
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
        // Capture frame-by-frame
        cv::Mat frame;
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        num_frames += 1;

        cv::Mat way_point_img, histogram, output, hough_lines;
        std::vector<int> peaks;

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

        clock_t convert_color_start = clock();
        cv::cvtColor(thresholded, output, cv::COLOR_GRAY2BGR);
        total_time_convert_color += clock() - convert_color_start;

        //clock_t find_peaks_start = clock();
        //peaks = findPeaks(thresholded, histogram);
        //total_time_find_peaks += clock() - find_peaks_start;

        unique_ptr<LaneLine> line(new LaneLine());
        std::vector<unique_ptr<LaneLine>> lane_lines;
        lane_lines.push_back(std::move(line));

        cv::Mat fitx1, ploty1, fitx2;
        cv::Mat final_result_img(720, 1280, CV_8UC3, Scalar(0,0,0));
        std::vector<std::pair<double, double>> waypoints_meters;
        cv::Mat vector_field_thresh;

        try {
            //cv::imshow("INPUT INTO HOUGH TRANS", output);
            //char b = (char) waitKey(3000);
            //if (b == ESC_ASCII)
            //    break;

            // Convert output which is 8U3C to 8U1C
            cvtColor(output, output, COLOR_BGR2GRAY);

            // Hough transform VIA OpenCV
            /*
            vector<Vec2f> lines;
            cv::HoughLines(output, lines, 1, CV_PI/180, 150, 0, 0 );

            // Draw the lines
            for(size_t i = 0; i < lines.size(); i++) {
                float rho = lines[i][0], theta = lines[i][1];
                Point pt1, pt2;
                double a = cos(theta), b = sin(theta);
                double x0 = a * rho;
                double y0 = b * rho;
                pt1.x = cvRound(x0 + 1000 * (-b));
                pt1.y = cvRound(y0 + 1000 * (a));
                pt2.x = cvRound(x0 - 1000 * (-b));
                pt2.y = cvRound(y0 - 1000 * (a));
                cv::line(final_result_img, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
            }
            */

            // Vector field to Hough like space
            const int grid_size = 16;
            std::vector<tuple<double, double, double>> vector_field;
            clock_t hough_start = clock();
            mat_to_vector_field(output, vector_field, grid_size, 2);
            total_time_hough += clock() - hough_start;

            cv::cvtColor(output, vector_field_thresh, cv::COLOR_GRAY2BGR);
            draw_vector_field(vector_field_thresh, vector_field, (int) (grid_size * 0.8));
            draw_grid(vector_field_thresh, grid_size);
            //cv::imshow("vector field", vector_field_thresh);
            //waitKey(10000000);

            // Clustering
            // @Daniel, you're information is in vector_field




            clock_t distort_back_start = clock();
            cv::warpPerspective(final_result_img, dst, reverse_matrix, final_result_img.size());
            total_time_distort_back += clock() - distort_back_start;

            clock_t add_weighted_start = clock();
            cv::addWeighted(undistorted, 1, dst, 4, 0, summed);
            total_time_add_weighted += clock() - add_weighted_start;

            ////////////////////////////////////////////////////
            // Frame is now processesed, end time for the benchmarks frame
            total_time += clock() - total_start;
            if (benchmark && !quiet)
                print_benchmark_progress();

            // Display the results
            cv::Mat window_mat(cv::Size(1800,900), CV_8UC3);
            //std::cout << "Type of summed, thresholded, transformed, vector_field_thresh: " << summed.type() << " " << thresholded.type() << " " << transformed.type() << " " << vector_field_thresh.type() << std::endl;

            cv::Mat summed_small, thresholded_small, transformed_small, vector_field_thresh_small;
            cv::resize(summed, summed_small, cv::Size(900,450));
            cv::resize(thresholded, thresholded_small, cv::Size(900,450));
            cv::resize(transformed, transformed_small, cv::Size(900,450));
            cv::resize(vector_field_thresh, vector_field_thresh_small, cv::Size(900,450));
            //cv::cvtColor(summed_small, summed_small, CV_GRAY2RGB);
            cv::cvtColor(thresholded_small, thresholded_small, CV_GRAY2RGB);
            //cv::cvtColor(transformed_small, transformed_small, CV_GRAY2RGB);
            //cv::cvtColor(vector_field_thresh_small, vector_field_thresh_small, CV_GRAY2RGB);
            summed_small.             copyTo(window_mat(cv::Rect(   0,   0, 900, 450)));
            thresholded_small.        copyTo(window_mat(cv::Rect( 900,   0, 900, 450)));
            transformed_small.        copyTo(window_mat(cv::Rect(   0, 450, 900, 450)));
            vector_field_thresh_small.copyTo(window_mat(cv::Rect( 900, 450, 900, 450)));
            cv::imshow("Outputs", window_mat);

            //cv::imshow("thresholded testing", thresholded);
            //cv::imshow("waypoint image testing", way_point_img);
            //cv::imshow("warped perspective", transformed);
            //cv::waitKey();    // Pause at every frame
            video.write(summed);

            // Press  ESC on keyboard to exit
            char c = (char) waitKey(5);
            if (c == ESC_ASCII)
                break;

        } catch(Exception e){
            std::cerr << "Exception occurred:" << std::endl;
            std::cerr << e.what() << std::endl;
            imshow("Exception occurred, last good frame:", thresholded);
            video.write(summed);
            cap.release();
            video.release();
            cv::waitKey(0);
        }
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


