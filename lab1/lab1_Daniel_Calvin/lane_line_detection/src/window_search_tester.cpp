#include <opencv2/opencv.hpp>
#include <iostream>
#include <memory>
#include <matplotlibcpp.h>
#include "overloader.hpp"
#include "cv_helper.hpp"
#include "thresholds.hpp"
#include "window_search.hpp"
#include "signal_proc.hpp"
#include "lane_line.hpp"

namespace plt = matplotlibcpp;

const double xm_per_pix = 3.7 / 700;
const double ym_per_pix = 30. / 720;
const int LANE_ORDER = 2;

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        std::cerr << "Usage: ./window_search_tester <path_to_thresholded_images\\*.png> \n";
        return 1;
    }
    std::vector<cv::String> image_paths;
    cv::glob(argv[1], image_paths);
    std::vector<cv::Mat> disp_imgs;

    for (int i = 0; i < image_paths.size(); i++)
    { //go over all images
        cv::Mat binary_warped, histogram;

        //binary_warped = cv::imread(image_paths[0]);
        binary_warped = cv::imread(image_paths[i], CV_LOAD_IMAGE_GRAYSCALE);

        //find peaks
        std::vector<int> peaks;
        peaks = findPeaks(binary_warped, histogram);

        //make vector of lane lines
        std::vector<std::unique_ptr<LaneLine>> lanes;
        if (peaks.size() >= 2)
        {
            std::unique_ptr<LaneLine> left(new LaneLine(xm_per_pix, ym_per_pix, LANE_ORDER, cv::Scalar(0, 0, 255)));
            lanes.push_back(std::move(left));
            std::unique_ptr<LaneLine> right(new LaneLine(xm_per_pix, ym_per_pix, LANE_ORDER, cv::Scalar(255, 0, 0)));
            lanes.push_back(std::move(right));
        }
        for (int i = 2; i < peaks.size(); i++)
        { //if for some reason there are more that two peaks in the histogram
            std::unique_ptr<LaneLine> temp(new LaneLine(xm_per_pix, ym_per_pix, LANE_ORDER, cv::Scalar(0, 255, 0)));
            lanes.push_back(std::move(temp));
        }

        //std::cout << peaks << std::endl;
        // Feel free to comment out the lines below. They are for visualizing histogram.
        //std::vector<double> histogram_vis;
        //histogram.copyTo(histogram_vis);
        //plt::plot(histogram_vis);
        //plt::show();

        //convert binary_warped to a color image, so we can draw some stuff on it and see some color
        //cv::Mat binary_warped_color;
        //cv::cvtColor(binary_warped, binary_warped_color, cv::COLOR_GRAY2BGR);

        //apply window search
        cv::Mat image_to_draw_on; //copy binary warped, convert to colored colorspace
        cv::cvtColor(binary_warped, image_to_draw_on, cv::COLOR_GRAY2BGR);
        int margin = 100;

        try {
            window_search(binary_warped, image_to_draw_on, lanes, peaks, 10, margin, 0);
        }
        catch (cv::Exception& e ) {
            std::cout << "\n\nEXCEPTION IN WINDOW SEARCH\n\n\n";
            std::cout << "exception caught: " << e.what() << std::endl;
        }

        //fix for subplot error, convert greyscale to color
        //cv::Mat color;
        //cv::cvtColor(binary_warped, color, cv::COLOR_GRAY2BGR);

        //show thresholded images and the window search algorithm
        //subplot("threshold image and window search algo", src_imgs, 1, 1);
        //cv::waitKey();

        //USE THIS IF SUBPLOT DOES NOT WORK
        //cv::imshow("threshold image and window search algo", image_to_draw_on);
        //cv::waitKey();
        //cv::Mat poly_img;
        try {
            int lane_margin = 20;
            cv::Mat final_img(image_to_draw_on.rows, image_to_draw_on.cols, image_to_draw_on.type());

            for (int i = 0; i < lanes.size(); i++)
            {
                lanes[i]->fit(binary_warped.rows);
                //std::cout << lanes[i]->fitx_pix << std::endl;
                //std::cout << lanes[i]->ploty_pix << std::endl;
                cv::Mat pts1, pts2, left_line, right_line, poly_img;

                cv::Mat ploty = lanes[i]->ploty_pix;
                cv::Mat fitx = lanes[i]->fitx_pix;
                // make left line boundary of lane line
                std::vector<cv::Mat> left_merge = {fitx - margin, ploty};
                cv::merge(left_merge, left_line);

                //make right line boundary of lane line
                std::vector<cv::Mat> right_merge = {fitx + margin, ploty};
                cv::merge(right_merge, right_line);
                cv::flip(right_line, right_line, 0);
                cv::vconcat(left_line, right_line, pts2);
                pts2.convertTo(pts2, CV_32S);
                std::array<cv::Mat, 1> poly = {pts2};

                //draw lane line as polygon
                poly_img = cv::Mat::zeros(image_to_draw_on.rows, image_to_draw_on.cols, image_to_draw_on.type());
                cv::fillPoly(poly_img, poly, cv::Scalar(0, 255, 0));
                cv::hconcat(fitx, ploty, pts1);
                pts1.convertTo(pts1, CV_32S);

                // draw thin best-fit line on image with window search
                cv::polylines(image_to_draw_on, pts1, 0, cv::Scalar(0, 255, 255), 10);
                cv::addWeighted(image_to_draw_on, 1.0, poly_img, 0.3, 0.0, image_to_draw_on);
                // draw thick best-fit line on blank image to be warped back for final result
                cv::polylines(final_img, pts1, 0, lanes[i]->color, lane_margin * 2);
            }
            cv::imshow("imge_to_draw_on", image_to_draw_on);
            generate_waypoints(final_img, lanes, 0.05, 0.05, 8, 10, xm_per_pix, ym_per_pix);
            cv::imshow("final", final_img);
            cv::waitKey();
        }
        catch (cv::Exception& e ) {
            std::cout << "\n\nEXCEPTION LATER ON\n\n";
            std::cout << "exception caught: " << e.what() << std::endl;
        }
    }
}
