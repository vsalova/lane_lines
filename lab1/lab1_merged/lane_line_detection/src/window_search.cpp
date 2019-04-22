// #include <opencv2/opencv.hpp>
// #include <iostream>
// #include "signal_proc.hpp"
// #include "window_search.hpp"
// #include "cv_helper.hpp"
//
// const int WINDOW_BORDER_WIDTH = 6;
// const cv::Scalar WINDOW_COLOR(0, 255, 0);
// const std::vector<cv::Scalar> WAYPOINT_COLORS{cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 255), cv::Scalar(0, 255, 0), cv::Scalar(211, 0, 148), cv::Scalar(0, 0, 255), cv::Scalar(0, 127, 255)};
//
// //void print_cv_exception(cv::Exception& e);
//
// void window_search(cv::Mat &src, cv::Mat &window_img, std::vector<std::unique_ptr<LaneLine>> &lane_lines,
//                    std::vector<int> &peaks, int n_windows, int margin, int minpix)
// {
//     if (n_windows < 1)
//     {
//         std::cerr << "THE NUMBER OF WINDOWS MUST BE POSITIVE";
//         throw std::invalid_argument("");
//     }
//     if (lane_lines.size() != peaks.size())
//     {
//         std::cerr << "THE NUMBER OF LANE LINES AND THE NUMBER OF PEAKS MUST BE THE SAME" << std::endl;
//         std::cerr << "number of lane lines: " << lane_lines.size() << "\t number of peaks" << peaks.size() << std::endl;
//         throw std::invalid_argument("");
//     }
//
//     int image_height = src.rows;
//     int window_width = margin * 2;
//     int window_height = image_height / n_windows;
//     int current_y = image_height; //hold the BOTTOM of the current window
//
//     //TODO
//     //radius of curvature
//     //try -O3 AT END in CMakeLists.txt TO OPTIMIZE
//     //COLOR THE LANE LINES RED AND BLUE IN THE VIDEO
//     //SUBMIT HW to CANVAS
//
//     //reuse points for drawing the windows
//     cv::Point2i first_point;
//     cv::Point2i second_point;
//
//     //main window search loop
//     for (int peak_ind = 0; peak_ind < peaks.size(); peak_ind++)
//     { //peak index, also lane line index
//         current_y = image_height;
//         int peak_x_val = peaks[peak_ind];
//         double previous_window_x_center = peak_x_val;
//         for (int num_w = 0; num_w < n_windows; num_w++)
//         {
//             //set y values
//             first_point.y = current_y - window_height;
//             second_point.y = current_y;
//
//             //set x values
//             if (num_w == 0) {  //first point
//                 first_point.x = peak_x_val - margin;
//                 second_point.x = peak_x_val + margin;
//             } else {    //all other windows, use last saved x value
//                 first_point.x = previous_window_x_center - margin;
//                 second_point.x = previous_window_x_center + margin;
//             }
//
//             //draw window
//             cv::rectangle(window_img, first_point, second_point, WINDOW_COLOR, WINDOW_BORDER_WIDTH);
//
//             // Find all white pixels in the temporary window, and update previous_window_x_center
//             // Get rectangle that describes the window
//             // MAKE SURE RECT IS WITHIN POINTS OF SRC
//             cv::Rect window_rect = get_rect_in_bounds(first_point.x, first_point.y, window_width, window_height, src);
//
//             cv::Mat window_pixels(src, window_rect);
//             // Get non-zero pixels in the rectangle of interest
//             std::vector<cv::Point> non_zero_pixels;
//
//             cv::findNonZero(window_pixels, non_zero_pixels);
//
//             //recenter origin back to origin of main image
//             for (int i = 0; i < non_zero_pixels.size(); i++) {
//                 non_zero_pixels[i].x = non_zero_pixels[i].x + first_point.x;
//                 non_zero_pixels[i].y = non_zero_pixels[i].y + first_point.y;
//             }
//
//             // Color the pixels in the window
//             for (Point pnt : non_zero_pixels) {
//                 window_img.at<cv::Vec3b>(pnt)[0] = lane_lines[peak_ind]->color[0];
//                 window_img.at<cv::Vec3b>(pnt)[1] = lane_lines[peak_ind]->color[1];
//                 window_img.at<cv::Vec3b>(pnt)[2] = lane_lines[peak_ind]->color[2];
//             }
//
//             // Get average x for all the pixels, save all points for fit function later on
//             if (non_zero_pixels.size() > 0) {
//                 double total = 0;
//                 double min = non_zero_pixels[0].x; //only used for testing
//                 double max = non_zero_pixels[0].x; //only used for testing
//                 for (Point pnt : non_zero_pixels) {
//                     total += pnt.x;
//                     if (pnt.x < min)
//                         min = pnt.x;
//                     if (pnt.x > max)
//                         max = pnt.x;
//                     lane_lines[peak_ind]->window_pts_x.push_back(pnt.x);
//                     lane_lines[peak_ind]->window_pts_y.push_back(pnt.y);
//                 }
//                 double avg = total * 1.0 / non_zero_pixels.size();
//
//                 previous_window_x_center = avg;
//             }
//             //shift window up for next iteration of windows directly above
//             current_y -= window_height;
//         }
//     }
// }
//
// std::vector<std::pair<double, double>> generate_waypoints(cv::Mat &overlay_img, std::vector<std::unique_ptr<LaneLine>> &lane_lines, double start_fraction, double stop_fraction, int num_waypoints, int radius, double xm_per_pix, double ym_per_pix)
// {
//     std::vector<std::pair<double, double>> waypoints_meters;
//     cv::Mat middle_y_pix;
//     cv::Mat fit_left_x_pix;
//     cv::Mat fit_right_x_pix;
//
//     double start = overlay_img.rows * start_fraction;
//     double stop = overlay_img.rows * (1 - stop_fraction);
//
//     linspace(start, stop, num_waypoints, middle_y_pix);
//
//     //linspace always starts at 0 :(
//     for (int i = 0; i < middle_y_pix.rows; i++) //need to add start_fraction to each point in middle_y_pix
//         middle_y_pix.at<double>(i) += start;
//
//
//     polyVal(middle_y_pix, fit_left_x_pix, lane_lines[0]->current_fit_pix);
//     polyVal(middle_y_pix, fit_right_x_pix, lane_lines[1]->current_fit_pix);
//
//     cv::Mat middle_x_pix = (fit_left_x_pix + fit_right_x_pix) / 2.0; //CHECK
//     for (int i = 0; i < num_waypoints; i++)
//     {
//         cv::Point center = cv::Point(middle_x_pix.at<double>(i), middle_y_pix.at<double>(i));
//         cv::circle(overlay_img, center, radius, WAYPOINT_COLORS[i % WAYPOINT_COLORS.size()], CV_FILLED);
//
//         waypoints_meters.push_back(std::pair<double, double>(middle_x_pix.data[i] * xm_per_pix, middle_y_pix.data[i] * ym_per_pix));
//     }
//
//     return waypoints_meters;
// }
//
// void draw_lanes_waypoints(cv::Mat &src, cv::Mat &dst)
// {
//     const double xm_per_pix = 3.7 / 700;
//     const double ym_per_pix = 30. / 720;
//     const int LANE_ORDER = 2;
//     cv::Mat histogram;
//     cv::Mat image_to_draw_on;
//
//     //find peaks
//     std::vector<int> peaks;
//
//     peaks = findPeaks(src, histogram);
//     if (peaks.size() != 2) {
//         std::cout << "NEED TWO PEAKS" << std::endl;
//         return;
//     }
//     //make vector of lane lines
//     std::vector<std::unique_ptr<LaneLine>> lanes;
//     std::unique_ptr<LaneLine> left(new LaneLine(xm_per_pix, ym_per_pix, LANE_ORDER, cv::Scalar(0, 0, 255)));
//     lanes.push_back(std::move(left));
//     std::unique_ptr<LaneLine> right(new LaneLine(xm_per_pix, ym_per_pix, LANE_ORDER, cv::Scalar(255, 0, 0)));
//     lanes.push_back(std::move(right));
//
//     /*
//     for (int i = 2; i < peaks.size(); i++)
//     { //if for some reason there are more that two peaks in the histogram
//         std::err << "MORE THAN TWO LANE LINES" << std::endl;
//         std::unique_ptr<LaneLine> temp(new LaneLine(xm_per_pix, ym_per_pix, LANE_ORDER, cv::Scalar(0, 255, 0)));
//         lanes.push_back(std::move(temp));
//     }
//     */
//
//     cv::cvtColor(src, image_to_draw_on, cv::COLOR_GRAY2BGR);
//
//     int margin = 100;
//     window_search(src, image_to_draw_on, lanes, peaks, 10, margin, 0);
//
//     int lane_margin = 20;
//     dst = cv::Mat::zeros(image_to_draw_on.rows, image_to_draw_on.cols, image_to_draw_on.type());
//
//     // Fit each laneline, fit best fit line, draw the laneline on destination
//     for (int i = 0; i < lanes.size(); i++) {
//         lanes[i]->fit(src.rows);
//
//         cv::Mat pts1;
//         cv::Mat ploty = lanes[i]->ploty_pix;
//         cv::Mat fitx = lanes[i]->fitx_pix;
//         cv::hconcat(fitx, ploty, pts1);
//         cv::Mat converted_pts1;
//         pts1.convertTo(converted_pts1, CV_32S);
//
//         // Draw thick best-fit line on blank image to be warped back for final result
//         cv::polylines(dst, converted_pts1, 0, lanes[i]->color, lane_margin * 2);
//     }
//
//     //draw waypoints
//     generate_waypoints(dst, lanes, 0.05, 0.05, 8, 10, xm_per_pix, ym_per_pix);
// }
//
// void print_cv_exception_2(cv::Exception& e) {
//     std::cout << "exception caught: " << e.what() << std::endl;
//     std::cout << "ERR: " << e.err << std::endl;
//     std::cout << "FILE: " << e.file << std::endl;
//     std::cout << "FUNC: " << e.func << std::endl;
//     std::cout << "LINE: " << e.line << std::endl;
//     std::cout << "MESSAGE: " << e.msg << std::endl;
// }
//
// cv::Rect get_rect_in_bounds(int x, int y, int width, int height, cv::Mat &src) {
//     int src_height = src.rows;
//     int src_width  = src.cols;
//
//     if (width < 0) width = 0;
//     if (height < 0) height = 0;
//     if (x < 0) x = 0;
//     if (y < 0) y = 0;
//     if (x + width >= src_width) width = src_width - x - 1;
//     if (y + height >= src_height) height = src_height - y - 1;
//
//     cv::Rect rect (x, y, width, height);
//     //do bounds checking with src
//     return rect;
// }



#include <opencv2/opencv.hpp>
#include <iostream>
#include "window_search.hpp"
#include "cv_helper.hpp"

// what's window_img - the image pixels inside that window
//1280x720 image size (80 rows and 160 columns)

// TODO: Add comments in this file

void window_search(cv::Mat& binary_warped, cv::Mat& window_img,
                   std::vector<std::unique_ptr<LaneLine>>& lane_lines, std::vector<int>& peaks,
                   int n_windows, int margin, int minpix, cv::Mat& fitx1, cv::Mat& ploty1,
                   cv::Mat& fitx2, cv::Mat& final_result_img) {
    for (int j = 0; j < 2; j++) {
        int x_current = peaks[j];
        int win_left; // x
        int win_bottom; // y
        int win_right; // x
        int win_top; // y
        cv::Mat x_totalv;
        cv::Mat y_totalv;
        cv::Mat best_fit;

        cv::Mat ploty, fitx;

        for (int i = 0; i < n_windows; i++) {
            win_left = x_current - margin;
            win_right = x_current + margin;
            win_bottom = binary_warped.rows - (i)*(binary_warped.rows / n_windows);
            win_top = binary_warped.rows - (i+1)*(binary_warped.rows / n_windows);

            // Place rectangle
            cv::rectangle(window_img, cv::Point(win_left, win_bottom),
            cv::Point(win_right, win_top), cv::Scalar(0,255,0), 2);

            // Rind the average of the lane line points
            cv::Mat cropped = binary_warped(cv::Range(win_top, win_bottom), cv::Range(win_left, win_right));

            cv::Mat averaged, averaged_x, x_total, y_total;
            cv::findNonZero(cropped, averaged);

            if (!averaged.empty()) {
                cv::extractChannel(averaged, x_total, 0);
                cv::extractChannel(averaged, y_total, 1);

                x_total += win_left;
                y_total += win_top;

                x_totalv.push_back(x_total);
                y_totalv.push_back(y_total);

                if (cv::mean(x_total)[0] != 0) {
                    x_current = (int)cv::mean(x_total)[0];
                }
            }

            window_img(cv::Range(win_top,win_bottom),
                cv::Range(win_left,win_right)).setTo(lane_lines[0]->color,
                binary_warped(cv::Range(win_top,win_bottom),
                cv::Range(win_left,win_right)) != 0);
        }
        x_totalv.convertTo(x_totalv, CV_64F);
        y_totalv.convertTo(y_totalv, CV_64F);
        lane_lines[j]->fit(x_totalv, y_totalv, best_fit, ploty, fitx, binary_warped);
        if (j == 0) {
            fitx1 = fitx;
            ploty1 = ploty;
        }
        else {
            fitx2 = fitx;
            ploty1 = ploty;
        }
        //cout << fitx << endl;
                    /*
         * NOTE:
         * In this context, left_line and right_line are not lane lines!
         * Instead, they define the left and right line boundaries of one lane line.
         * Also, margin is the margin variable used in the window search algorithm.
         */
        cv::Mat pts1, pts2, left_line, right_line, poly_img;

        // make left line boundary of lane line
        std::vector<cv::Mat> left_merge = {fitx-margin, ploty};
        cv::merge(left_merge, left_line);

        // make right line boundary of lane line
        std::vector<cv::Mat> right_merge = {fitx+margin, ploty};
        cv::merge(right_merge, right_line);
        cv::flip(right_line, right_line, 0);
        cv::vconcat(left_line, right_line, pts2);
        pts2.convertTo(pts2, CV_32S);
        std::array<cv::Mat,1> poly = { pts2 };

        // draw lane line as polygon
        poly_img = cv::Mat::zeros(window_img.rows, window_img.cols, window_img.type());
        cv::fillPoly(poly_img, poly, cv::Scalar(255,0,0));
        cv::hconcat(fitx, ploty, pts1);
        pts1.convertTo(pts1, CV_32S);
        int thickness1 = 1;
        int thickness2 = 10;
        // draw thin best-fit line on image with window search
        cv::polylines(window_img, pts1, 0, cv::Scalar(255,0,0), thickness1);
        cv::addWeighted(window_img, 1.0, poly_img, 0.3, 0.0, window_img);
        // draw thick best-fit line on blank image to be warped back for final result
        cv::polylines(final_result_img, pts1, 0, cv::Scalar(255,0,0), thickness2);
    }
}


std::vector<std::pair<double, double>> generate_waypoints(cv::Mat& window_img, cv::Mat& fitx1,
                                                          cv::Mat& ploty1, cv::Mat& fitx2) {
    //std::vector<std::pair<double, double>> ret;
    cv::RNG rng(12345);

    //cout << fitx1 << endl;
    int newx;
    double xm_per_pix = 3.7/700;
    double ym_per_pix = 30./720;

    double center = window_img.cols / 2;
    double bottom = window_img.rows;

    std::pair<double, double> mpair;
    std::vector<std::pair<double, double>> waypoints_meters;

    newx = (int)(fitx1.rows*0.05) + (int)((fitx1.rows*0.95 - fitx1.rows*0.05)/7);
    for (int i = 0; i < fitx1.rows; i++){
        double midx = (fitx2.at<double>(0,i) - fitx1.at<double>(0,i))/2 + fitx1.at<double>(0,i);
        double yval = ploty1.at<double>(0,i);

        if (i == (int)(fitx1.rows*0.05) || i == newx){
            cv::circle(window_img, Point(midx, yval),15, Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255)), CV_FILLED);

            mpair.first = (midx-center) * xm_per_pix;
            mpair.second = (bottom-yval) * ym_per_pix;
            waypoints_meters.push_back(mpair);

            cout << "x: " << midx << endl;
            cout << "y: " << yval << endl;
            if (i == newx){
                newx += (int)((fitx1.rows*0.95 - fitx1.rows*0.05)/7);
            }

        }
    }

    std::reverse(waypoints_meters.begin(),waypoints_meters.end());
    return waypoints_meters;
}
