#include <opencv2/opencv.hpp>
#include <iostream>
#include "lane_line.hpp"
#include "signal_proc.hpp"

LaneLine::LaneLine(double xm_per_pix, double ym_per_pix, int order, cv::Scalar color)
{
    this->xm_per_pix = xm_per_pix;
    this->ym_per_pix = ym_per_pix;
    this->order = order;
    this->color = color;
    if (this->order < 1)
    {
        this->order = 1;
    }
}

void LaneLine::fit(int img_rows)
{
    cv::Mat mx = cv::Mat(this->window_pts_x);
    cv::Mat my = cv::Mat(this->window_pts_y);
    mx.convertTo(mx, CV_64F);
    my.convertTo(my, CV_64F);
    polyFit(my, mx, this->current_fit_pix, this->order);
    linspace(0, img_rows, img_rows - 1, this->ploty_pix);
    polyVal(this->ploty_pix, this->fitx_pix, this->current_fit_pix);
}
