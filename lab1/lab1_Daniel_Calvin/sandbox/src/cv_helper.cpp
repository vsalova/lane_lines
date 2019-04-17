#include "cv_helper.hpp"

std::string type(cv::Mat& img) {
  int type = img.type();
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

cv::Point3i shape(cv::Mat& img) {
  cv::Point3i result(img.cols, img.rows, img.channels());
  return result;
}

void subplot(std::string title, std::vector<cv::Mat>& v, unsigned int w, unsigned int h) {
if (w*h < v.size()) {
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

if(nArgs <= 0) {
    std::stringstream ss;
    ss << "Number of arguments too small.\n";
    throw std::underflow_error(ss.str());
}
else if(nArgs > 40) {
    std::stringstream ss;
    ss << "Number of arguments too large.\n";
    throw std::overflow_error(ss.str());
}
// Determine the size of the image,
// and the number of rows/cols
// from number of arguments
else if (nArgs == 1) {
    //w = h = 1;
    size = 500;
}
else if (nArgs == 2) {
    //w = 2; h = 1;
    size = 500;
}
else if (nArgs == 3 || nArgs == 4) {
    //w = 2; h = 2;
    size = 400;
}
else if (nArgs == 5 || nArgs == 6) {
    //w = 3; h = 2;
    size = 350;
}
else if (nArgs == 7 || nArgs == 8) {
    //w = 4; h = 2;
    size = 275;
}
else if (nArgs >= 9 && nArgs <= 12) {
    //w = 4; h = 3;
    size = 225;
}
else if (nArgs >= 13 && nArgs <= 16) {
  //w = 4; h = 4;
  size = 175;
}
else if (nArgs >=17 && nArgs <= 20) {
  //w = 5; h = 4;
  size = 175;
}

else if (nArgs >=21 && nArgs <= 25) {
  //w = 5; h = 5;
  size = 150;
}

else if (nArgs >=26 && nArgs <= 30) {
  //w = 6; h = 5;
  size = 150;
}

else if (nArgs >=31 && nArgs <= 35) {
  //w = 7; h = 5;
  size = 150;
}

else {
  //w = 8; h = 5;
  size = 150;
}


cv::Mat img = v[0];
// Check whether it is NULL or not
// If it is NULL, release the image, and return
if(img.empty()) {
    std::stringstream ss;
    ss << "Invalid Image. Image is empty.\n";
    throw std::invalid_argument(ss.str());
}
x = img.cols;
y = img.rows;
max = (x > y) ? x: y;
scale = (float) ( (float) max / size );
int type = img.type();
int step = 20;

cv::Mat DispImage = cv::Mat::zeros(cv::Size((int)((x/scale)*w + 2*step + (w-1)*step), (int)((y/scale)*h + 2*step + (h-1)*step)), type);
DispImage = cv::Scalar(235,235,235);
cv::Mat temp;
// Loop for nArgs number of arguments
for (i = 0, m = step, n = step; i < nArgs; i++, m += (step + (int)(x/scale))) {
    img = v[i];

    // Check whether it is NULL or not
    // If it is NULL, release the image, and return
    if(img.empty()) {
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
    if (img.channels() == 1) {
      cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
    }
    else if (img.channels() != 3) {
      std::stringstream ss;
      ss << "Invalid number of image channels. Number of image channels must be 3 or 1.\n" << "Image " << i << " has " << img.channels() << ".\n";
      throw std::invalid_argument(ss.str());
    }

    if (img.type() != type) {
      img.convertTo(img, type);
    }

    // Used to Align the images
    if( i % w == 0 && m!= step) {
        m = step;
        n += step + (int)(y/scale);
    }
    // Set the image ROI to display the current image
    // Resize the input image and copy it to the Single Big Image
    cv::Rect ROI(m, n, (int)(x/scale), (int)(y/scale));
    cv::resize(img, temp, cv::Size(ROI.width, ROI.height));
    temp.copyTo(DispImage(ROI));
}

// Create a new window, and show the Single Big Image
cv::namedWindow(title, 1);
cv::imshow(title, DispImage);
cv::waitKey();

}
