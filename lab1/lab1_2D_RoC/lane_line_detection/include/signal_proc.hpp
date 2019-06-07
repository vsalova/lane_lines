#ifndef SIGNAL_PROC_HPP
#define SIGNAL_PROC_HPP

// Finds the beset fitting polynomial of order @order given a number of points
void polyFit(const cv::Mat& src_x, const cv::Mat& src_y, cv::Mat &dst, int order);

// Returns the values given a polynomial described by @best_fit from inputs in @src
void polyVal(const cv::Mat& src, cv::Mat& dst, const cv::Mat& best_fit);

// Linear step between start and stop with a certain number of points
void linspace(double start, double stop, unsigned int num, cv::Mat& dst);

// Returns the peaks of the histogram from a binary image
std::vector<int> findPeaks(cv::Mat& binary_warped, cv::Mat& histogram);

#endif /* SIGNAL_PROC_HPP */
