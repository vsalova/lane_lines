#include "cv_helper.hpp"

std::string type(cv::Mat &img)
{
  int type = img.type();
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch (depth)
  {
  case CV_8U:
    r = "8U";
    break;
  case CV_8S:
    r = "8S";
    break;
  case CV_16U:
    r = "16U";
    break;
  case CV_16S:
    r = "16S";
    break;
  case CV_32S:
    r = "32S";
    break;
  case CV_32F:
    r = "32F";
    break;
  case CV_64F:
    r = "64F";
    break;
  default:
    r = "User";
    break;
  }

  r += "C";
  r += (chans + '0');

  return r;
}

cv::Point3i shape(cv::Mat &img)
{
  cv::Point3i result(img.cols, img.rows, img.channels());
  return result;
}

void subplot(std::string title, std::vector<cv::Mat> &v, unsigned int w, unsigned int h)
{
  if (w * h < v.size())
  {
    std::stringstream ss;
    ss << "Grid size is too small for number of images. Please choose appropriate numbers for w and h.\n";
    throw std::runtime_error(ss.str());
  }

  int size;
  int i;
  int m, n;
  int x, y;
  int nArgs = v.size();

  // w - Maximum number of images in a row
  // h - Maximum number of images in a column
  //int w, h;

  // scale - How much we have to resize the image
  float scale;
  int max;

  if (nArgs <= 0)
  {
    std::stringstream ss;
    ss << "Number of arguments too small.\n";
    throw std::underflow_error(ss.str());
  }
  else if (nArgs > 40)
  {
    std::stringstream ss;
    ss << "Number of arguments too large.\n";
    throw std::overflow_error(ss.str());
  }
  // Determine the size of the image,
  // and the number of rows/cols
  // from number of arguments
  else if (nArgs == 1)
  {
    //w = h = 1;
    size = 500;
  }
  else if (nArgs == 2)
  {
    //w = 2; h = 1;
    size = 500;
  }
  else if (nArgs == 3 || nArgs == 4)
  {
    //w = 2; h = 2;
    size = 400;
  }
  else if (nArgs == 5 || nArgs == 6)
  {
    //w = 3; h = 2;
    size = 350;
  }
  else if (nArgs == 7 || nArgs == 8)
  {
    //w = 4; h = 2;
    size = 275;
  }
  else if (nArgs >= 9 && nArgs <= 12)
  {
    //w = 4; h = 3;
    size = 225;
  }
  else if (nArgs >= 13 && nArgs <= 16)
  {
    //w = 4; h = 4;
    size = 175;
  }
  else if (nArgs >= 17 && nArgs <= 20)
  {
    //w = 5; h = 4;
    size = 175;
  }

  else if (nArgs >= 21 && nArgs <= 25)
  {
    //w = 5; h = 5;
    size = 150;
  }

  else if (nArgs >= 26 && nArgs <= 30)
  {
    //w = 6; h = 5;
    size = 150;
  }

  else if (nArgs >= 31 && nArgs <= 35)
  {
    //w = 7; h = 5;
    size = 150;
  }

  else
  {
    //w = 8; h = 5;
    size = 150;
  }

  cv::Mat img = v[0];
  // Check whether it is NULL or not
  // If it is NULL, release the image, and return
  if (img.empty())
  {
    std::stringstream ss;
    ss << "Invalid Image. Image is empty.\n";
    throw std::invalid_argument(ss.str());
  }
  x = img.cols;
  y = img.rows;
  max = (x > y) ? x : y;
  scale = (float)((float)max / size);
  int type = 16;
  int step = 20;

  cv::Mat DispImage = cv::Mat::zeros(cv::Size((int)((x / scale) * w + 2 * step + (w - 1) * step), (int)((y / scale) * h + 2 * step + (h - 1) * step)), type);
  DispImage = cv::Scalar(235, 235, 235);
  cv::Mat temp;
  // Loop for nArgs number of arguments
  for (i = 0, m = step, n = step; i < nArgs; i++, m += (step + (int)(x / scale)))
  {
    img = v[i];

    // Check whether it is NULL or not
    // If it is NULL, release the image, and return
    if (img.empty())
    {
      std::stringstream ss;
      ss << "Invalid Image. Image is empty.\n";
      throw std::invalid_argument(ss.str());
    }
    /*
    if (img.rows != y || img.cols != x) {
      std::stringstream ss;
      ss << "Image shapes must all match.\n";
      throw std::runtime_error(ss.str());
    }
    */
    if (img.channels() == 1)
    {
      cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
    }
    else if (img.channels() != 3)
    {
      std::stringstream ss;
      ss << "Invalid number of image channels. Number of image channels must be 3 or 1.\n"
         << "Image " << i << " has " << img.channels() << ".\n";
      throw std::invalid_argument(ss.str());
    }

    if (img.type() != type)
    {
      img.convertTo(img, type);
    }

    // Used to Align the images
    if (i % w == 0 && m != step)
    {
      m = step;
      n += step + (int)(y / scale);
    }
    // Set the image ROI to display the current image
    // Resize the input image and copy it to the Single Big Image
    cv::Rect ROI(m, n, (int)(x / scale), (int)(y / scale));
    cv::resize(img, temp, cv::Size(ROI.width, ROI.height));
    temp.copyTo(DispImage(ROI));
  }

  // Create a new window, and show the Single Big Image
  cv::namedWindow(title, 1);
  cv::imshow(title, DispImage);
}

// HSVmanager
void HSVmanager::initHSVvalues(std::string filename)
{
  cv::FileStorage file(filename, cv::FileStorage::READ);
  file["min_h"] >> min_h;
  file["min_s"] >> min_s;
  file["min_v"] >> min_v;
  file["max_h"] >> max_h;
  file["max_s"] >> max_s;
  file["max_v"] >> max_v;
};
void HSVmanager::setHSVvalues()
{
  hsv_min = cv::Scalar(min_h, min_s, min_v);
  hsv_max = cv::Scalar(max_h, max_s, max_v);
};
void HSVmanager::setHSVImage(cv::Mat &img) { cv::cvtColor(img, hsvImage, CV_RGB2HSV); };
void HSVmanager::extractColor()
{
  cv::inRange(hsvImage, hsv_min, hsv_max, hsvImage);
};
void HSVmanager::img2HSV(cv::Mat &img, std::string filename)
{
  this->initHSVvalues(filename);
  this->setHSVvalues();
  this->setHSVImage(img);
  this->extractColor();
}

void adjustValues(cv::Mat &img, std::string filename)
{
  std::cout << "o: save values \nq: quit\ns: stop" << std::endl;

  HSVmanager hsv;
  ValueTracker<int> track(filename);
  hsv.initHSVvalues(filename);

  track.set_value("min_h", &hsv.min_h, 180);
  track.set_value("min_s", &hsv.min_s, 255);
  track.set_value("min_v", &hsv.min_v, 255);
  track.set_value("max_h", &hsv.max_h, 180);
  track.set_value("max_s", &hsv.max_s, 255);
  track.set_value("max_v", &hsv.max_v, 255);
  track.trackbar("HSV");

  std::vector<cv::Mat> axs;
  while (1)
  {
    axs.clear();
    axs.push_back(img.clone());
    hsv.setHSVvalues();
    hsv.setHSVImage(img);
    hsv.extractColor();
    axs.push_back(hsv.hsvImage);
    subplot("hsv", axs, 2, 1);

    auto key = cv::waitKey(100);
    if (key == 'o')
    {
      track.save_value();
      std::cout << "saved values!" << std::endl;
    }
    else if (key == 'q')
      break;
    else if (key == 's')
      cv::waitKey(0);
  }
};
