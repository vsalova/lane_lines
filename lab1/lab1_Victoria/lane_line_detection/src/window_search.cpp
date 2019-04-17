#include <opencv2/opencv.hpp>
#include <iostream>
#include "window_search.hpp"
#include "cv_helper.hpp"

// what's window_img - the image pixels inside that window
//1280x720 image size (80 rows and 160 columns)


void window_search(cv::Mat& binary_warped, cv::Mat& window_img,
std::vector<std::unique_ptr<LaneLine>>& lane_lines, std::vector<int>& peaks,
int n_windows, int margin, int minpix, cv::Mat& fitx1, cv::Mat& ploty1, cv::Mat& fitx2, cv::Mat& final_result_img) {
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


          // draw rectangle
          cv::rectangle(window_img, cv::Point(win_left, win_bottom),
          cv::Point(win_right, win_top), cv::Scalar(0,255,0), 2);

          // find the average of the lane line points
          cv::Mat cropped = binary_warped(cv::Range(win_top, win_bottom), cv::Range(win_left, win_right));

          cv::Mat averaged, averaged_x, x_total, y_total;
          cv::findNonZero(cropped, averaged);

          if (!averaged.empty())
          {

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
		if (j == 0){
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


std::vector<std::pair<double, double>> generate_waypoints(cv::Mat& window_img, cv::Mat& fitx1, cv::Mat& ploty1, cv::Mat& fitx2) {
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