

#ifndef INTEGRAL_H
#define INTEGRAL_H

#include <algorithm>  // req'd for std::min/max
#include <cv.h>

#define SUBSAMPLE 4

//! Computes the 1d integral image of the specified horizon line 
//! in image img.  Assumes source image to be a 
//! 32-bit floating point.  Returns IplImage in 32-bit float form.
IplImage *Integral(IplImage *img, int left_horizon, int right_horizon, std::vector<int> &y_coords, int horizon_width);

// Convert horizon of image to single channel 32F
IplImage *getGrayHorizon(const IplImage *img, int left_horizon, int right_horizon, std::vector<int> &y_coords, int horizon_width, int subsample, bool divide);

//! Computes the sum of pixels within the row specified by the left start
//! co-ordinate and width
inline float BoxIntegral(IplImage *img, int col, int cols) 
{
  float *data = (float *) img->imageData;

  // The subtraction by one for col because col is inclusive.
  int c1 = std::min(col,          img->width)  - 1;
  int c2 = std::min(col + cols,   img->width)  - 1;

  float A(0.0f), B(0.0f);
  if (c2 >= 0) A = data[c2];
  if (c1 >= 0) B = data[c1];

  return std::max(0.f, A - B);
}



#endif
