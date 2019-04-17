#ifndef CV_HELPER_HPP
#define CV_HELPER_HPP

#include <opencv2/opencv.hpp>
#include <sstream>
#include <stdio.h>
#include <stdarg.h>

using namespace std;
using namespace cv;

// Returns type of cv::Mat i.e. 8UC3, 8SC3, 64FC1, etc.
std::string type(cv::Mat &img);

// Returns 3-dimensional shape of image
cv::Point3i shape(cv::Mat &img);

// Used to plot many images at once on one window
void subplot(std::string title, std::vector<cv::Mat> &v, unsigned int w, unsigned int h);

class HSVmanager
{
  public:
    cv::Scalar hsv_min;
    cv::Scalar hsv_max;
    cv::Mat hsvImage;

    int min_h, min_s, min_v;
    int max_h, max_s, max_v;

    void initHSVvalues(std::string filename);
    void setHSVvalues();
    void setHSVImage(cv::Mat &img);
    void extractColor();
    void img2HSV(cv::Mat &img, std::string filename);
};

template <typename T>

class ValueTracker
{
  public:
    class V
    {
      public:
        int slider;
        T *value;
        T max_value;
    };
    cv::FileStorage file;
    std::string filename;
    std::map<std::string, V> values;

    ValueTracker(std::string filename)
    {
        this->filename = filename;
    };

    void get_value(std::string value_name, T *value)
    {
        using namespace cv;
        file.open(filename, FileStorage::READ);
        file[value_name] >> *value;
        file.release();
    };

    void set_value(std::string value_name, T *value, T max_value)
    {
        V val;
        val.slider = 0;
        get_value(value_name, value);
        val.value = value;
        val.max_value = max_value;
        values[value_name] = val;
    };

    void save_value()
    {
        using namespace cv;
        file.open(filename, FileStorage::WRITE);
        while (!values.empty())
        {
            auto itr = values.begin();
            file << itr->first << *itr->second.value;
            values.erase(values.begin());
        }
        file.release();
    };
    static void onTrackbar(int, void *object)
    {
        using namespace std;
        V *val = (V *)object;
        *val->value = (T)val->slider;
    };
    void trackbar(std::string windowName)
    {
        using namespace cv;
        cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
        for (auto i = values.begin(); i != values.end(); ++i)
        {
            createTrackbar(i->first, windowName, &i->second.slider, (int)i->second.max_value, &ValueTracker::onTrackbar, &i->second);
        }
    };
};

void adjustValues(cv::Mat &img, std::string filename);
#endif // CV_HELPER_HPP
