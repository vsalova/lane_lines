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
                   cv::Mat& fitx2, cv::Mat& final_result_img, cv::Mat& best_fit_l, cv::Mat& best_fit_r) {
    for (int j = 0; j < 2; j++) {
        int x_current = peaks[j];
        int win_left; // x
        int win_bottom; // y
        int win_right; // x
        int win_top; // y
        cv::Mat x_totalv;
        cv::Mat y_totalv;
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
        if (j == 0) {
            lane_lines[j]->fit(x_totalv, y_totalv, best_fit_l, ploty, fitx, binary_warped);
            fitx1 = fitx;
            ploty1 = ploty;
        }
        else {
            lane_lines[j]->fit(x_totalv, y_totalv, best_fit_r, ploty, fitx, binary_warped);
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


void window_search_2D(cv::Mat& binary_warped, cv::Mat& window_img,
                   std::vector<std::unique_ptr<LaneLine>>& lane_lines, std::vector<int>& peaks,
                   int n_windows_max, int margin, int window_height, int minpix, double disp, double max_angle_change, cv::Mat& fitx1, cv::Mat& ploty1,
                   cv::Mat& fitx2, cv::Mat& final_result_img, cv::Mat& best_fit_l, cv::Mat& best_fit_r) {

    int half_window_height = window_height / 2;
    double disp_sqr = disp * disp;     // To make distance calculations not need sqrt
    int stop_margin = 30;
    int stop_left = stop_margin;
    int stop_top = stop_margin;
    int stop_right = window_img.cols - stop_margin;

    for (int j = 0; j < 2; j++) {   // For each lane
        // Center first points to reduce oscillations
        double center_x = (double) peaks[j];
        cv::Rect first_rect = get_rect_in_bounds(center_x - margin, binary_warped.rows - window_height, margin * 2, window_height, window_img);
        cv::Mat first_rect_pixels = binary_warped(cv::Range(first_rect.y, first_rect.y + first_rect.height),
                                                  cv::Range(first_rect.x, first_rect.x + first_rect.width));
        cv::findNonZero(first_rect_pixels, first_rect_pixels); // Find pixels in rectangle
        cv::Mat xvals;
        cv::extractChannel(first_rect_pixels, xvals, 0);
        if (xvals.rows > 0) {
            xvals += center_x - margin;
            int x_avg = (int) cv::mean(xvals)[0];
            center_x = x_avg;
        }

        cv::Point last_center = cv::Point(center_x, binary_warped.rows + (disp / 2));     // Stored in x,y form
        cv::Point current_center = cv::Point(center_x, binary_warped.rows - (disp / 2));

        std::vector<cv::Point> points;
        points.push_back(last_center);

        //int last_delta = 0;
        int win_left; // x
        int win_bottom; // y
        int win_right; // x
        int win_top; // y
        cv::Mat x_totalv;
        cv::Mat y_totalv;
        cv::Mat ploty, fitx;
        //try {   // Better way to stop?
        for (int i = 0; i < n_windows_max; i++) {
            points.push_back(current_center);
            win_left = current_center.x - margin;
            win_right = current_center.x + margin;
            win_bottom = current_center.y + half_window_height;
            win_top = current_center.y - half_window_height;

            // Get window (make sure inside bounds of image)
            cv::Rect window_rect = get_rect_in_bounds(win_left, win_top, margin * 2, window_height, window_img);
            win_left = window_rect.x;
            win_bottom = window_rect.y + window_rect.height;
            win_right = window_rect.x + window_rect.width;
            win_top = window_rect.y;

            // Place rectangle
            cv::rectangle(window_img, cv::Point(win_left, win_bottom), cv::Point(win_right, win_top), cv::Scalar(0,255,0), 2);
            // Find the average of the lane line points
            cv::Mat cropped = binary_warped(cv::Range(win_top, win_bottom), cv::Range(win_left, win_right));

            cv::Mat averaged, averaged_x, x_total, y_total;
            cv::findNonZero(cropped, averaged);                 // Find pixels in rectangle
            int num_pixels_in_window = averaged.rows;
            //std::cout << "Averaged: " << "rows: " << averaged.rows << " cols: " << averaged.cols << std::endl;
            //if (!averaged.empty()) {
            if (num_pixels_in_window >= minpix) {
                cv::extractChannel(averaged, x_total, 0);
                cv::extractChannel(averaged, y_total, 1);
                // Re-align pixels to original image coordinates
                x_total += win_left;
                y_total += win_top;
                // Convert pixels to vector
                x_totalv.push_back(x_total);
                y_totalv.push_back(y_total);

                int x_avg = (int) cv::mean(x_total)[0];
                int y_avg = (int) cv::mean(y_total)[0];

                //last_center = current_center;
                //current_center.x = x_avg;
                //current_center.y = y_avg;
                // Take disp step in new direction
                double prev_angle = atan2(last_center.y - current_center.y, last_center.x - current_center.x);

                double current_disp_sqr = std::pow(x_avg - last_center.x, 2) + std::pow(y_avg - last_center.y, 2);
                double r = disp_sqr / current_disp_sqr;
                double newx = current_center.x + r*(x_avg - last_center.x);
                double newy = current_center.y + r*(y_avg - last_center.y);
                last_center = current_center;
                current_center.x = newx;
                current_center.y = newy;

                // Make sure angle did not change too much
                double current_angle = atan2(last_center.y - current_center.y, last_center.x - current_center.x);
                if (abs(current_angle - prev_angle) > max_angle_change) {
                    // Limit angle to prev_angle +- max_angle
                    if (current_angle > prev_angle)     current_angle = prev_angle + max_angle_change;
                    else                                current_angle = prev_angle - max_angle_change;

                    // Calculate updated current center based on new current angle from last_center
                    current_center.y = sin(last_center.y - current_center.y) * r + last_center.y;
                    current_center.x = cos(last_center.x - current_center.x) * r + last_center.x;
                }
            }
            else {
                // Use last difference
                int diff_x = current_center.x - last_center.x;
                int diff_y = current_center.y - last_center.y;
                last_center = current_center;
                current_center.x += diff_x;
                current_center.y += diff_y;
            }

            // Color non-zero pixels
            window_img(cv::Range(win_top,win_bottom),
                cv::Range(win_left,win_right)).setTo(lane_lines[0]->color,
                binary_warped(cv::Range(win_top,win_bottom),
                cv::Range(win_left,win_right)) != 0);

            // Stop when reach left, top, or right of image
            if (current_center.x < stop_left || current_center.x > stop_right || current_center.y < stop_top)
                break;

        }
        //} catch (Exception e) {}

        x_totalv.convertTo(x_totalv, CV_64F);
        y_totalv.convertTo(y_totalv, CV_64F);

        if (j == 0) {
            lane_lines[j]->fit(x_totalv, y_totalv, best_fit_l, ploty, fitx, binary_warped);
            fitx1 = fitx;
            ploty1 = ploty;
        }
        else {
            lane_lines[j]->fit(x_totalv, y_totalv, best_fit_r, ploty, fitx, binary_warped);
            fitx2 = fitx;
            ploty1 = ploty;
        }


        // Draw arrows between center points
        for (unsigned int i = 0; i < points.size() - 1; i++)
            cv::arrowedLine(window_img, points[i], points[i+1], cv::Scalar(255,255,0), 3);

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

            //std::cout << "x: " << midx << endl;
            //std::cout << "y: " << yval << endl;
            if (i == newx) {
                newx += (int)((fitx1.rows*0.95 - fitx1.rows*0.05)/7);
            }

        }
    }

    std::reverse(waypoints_meters.begin(),waypoints_meters.end());
    return waypoints_meters;
}


void calc_RoC(cv::Mat& src, cv::Mat& best_fit_l, cv::Mat& best_fit_r)
{
    double eval_at_y = 719;
    //double xm_per_pix = 3.7/700;
    double ym_per_pix = 30./720;
    double max_radius_before_inf = 5000;
    double font_scale = 0.65;

    // best_fit__ in form [c,b,a]
    double left_roc = pow(1 + pow(2 * best_fit_l.at<double>(2) * eval_at_y * ym_per_pix + best_fit_l.at<double>(1), 2), 2) / abs(2 * best_fit_l.at<double>(2));
    double right_roc = pow(1 + pow(2 * best_fit_r.at<double>(2) * eval_at_y * ym_per_pix + best_fit_r.at<double>(1), 2), 2) / abs(2 * best_fit_r.at<double>(2));
    double avg = (left_roc + right_roc) / 2;

    string l_string = "left_roc:    " + std::to_string( (int) left_roc) + "m";
    string r_string = "right_roc:   " + std::to_string( (int) right_roc) + "m";
    string a_string = "average_roc: " + std::to_string( (int) avg) + "m";
    if (left_roc > max_radius_before_inf)
        l_string = "left_roc:    inf";
    if (right_roc > max_radius_before_inf)
        r_string = "right_roc:   inf";
    if (avg > max_radius_before_inf)
        a_string = "average_roc: inf";

    putText(src, l_string, Point(10,20), FONT_HERSHEY_DUPLEX, font_scale, Scalar(255,255,255), 1);
    putText(src, r_string, Point(10,40), FONT_HERSHEY_DUPLEX, font_scale, Scalar(255,255,255), 1);
    putText(src, a_string, Point(10,60), FONT_HERSHEY_DUPLEX, font_scale, Scalar(255,255,255), 1);

    // std::cout << "left_roc = " << left_roc << std::endl;
    // std::cout << "right_roc = " << right_roc << std::endl;
    // std::cout << "avg = " << avg << std::endl;
    // std::cout << std::endl;
}


void calc_vehicle_offset(cv::Mat& src, cv::Mat& best_fit_l, cv::Mat& best_fit_r)
{
    double eval_at_y = 719;     // bottom of frame
    double xm_per_pix = 3.7/700;
    //double ym_per_pix = 30./720;
    double font_scale = 0.65;
    double bottom_x_l = best_fit_l.at<double>(2) * pow(eval_at_y, 2) + best_fit_l.at<double>(1) * eval_at_y + best_fit_l.at<double>(0);
    double bottom_x_r = best_fit_r.at<double>(2) * pow(eval_at_y, 2) + best_fit_r.at<double>(1) * eval_at_y + best_fit_r.at<double>(0);
    double vehicle_offset = (src.cols / 2) - (bottom_x_l + bottom_x_r) / 2;
    vehicle_offset *= xm_per_pix;
    vehicle_offset = ((long) (vehicle_offset * 100)) / 100.0;
    string s = "offset: " + std::to_string( std::abs(vehicle_offset) ) + "m ";
    if (vehicle_offset < 0)
        s += "left";
    else if (vehicle_offset > 0)
        s += "right";
    else
        s = "offset: center";
    putText(src, s, Point(500,20), FONT_HERSHEY_DUPLEX, font_scale, Scalar(255,255,255), 1);
}


cv::Rect get_rect_in_bounds(int x, int y, int width, int height, cv::Mat &src) {
    int src_height = src.rows;
    int src_width  = src.cols;

    if (width < 0) width = 0;
    if (height < 0) height = 0;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + width >= src_width) width = src_width - x - 1;
    if (y + height >= src_height) height = src_height - y - 1;

    cv::Rect rect (x, y, width, height);
    //do bounds checking with src
    return rect;
}
