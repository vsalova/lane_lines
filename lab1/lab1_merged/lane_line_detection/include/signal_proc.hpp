#ifndef SIGNAL_PROC_HPP
#define SIGNAL_PROC_HPP


void polyFit(const cv::Mat& src_x, const cv::Mat& src_y, cv::Mat &dst, int order);

void polyVal(const cv::Mat& src, cv::Mat& dst, const cv::Mat& best_fit);

void linspace(double start, double stop, unsigned int num, cv::Mat& dst);

std::vector<int> findPeaks(cv::Mat& binary_warped, cv::Mat& histogram);
















#endif /* SIGNAL_PROC_HPP */
