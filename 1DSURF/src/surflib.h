

#ifndef SURFLIB_H
#define SURFLIB_H

#include <cv.h>
#include <highgui.h>

#include "integral.h"
#include "fasthessian.h"
#include "surf.h"
#include "ipoint.h"
#include <ctime>

#define RUNSWIFT 0

class TestImage{

public:

	TestImage(  IplImage *_img,
			int _left,
			int _right,
			int _direction,
			string _filename){
		img=_img;
		left=_left;
		right=_right;
		direction=_direction;
		filename = _filename;

	}

	IplImage *img;
	int left;
	int right;
	int direction;
	string filename;
	IpVec ipts;
	double tt_extraction;
	double tt_integral;
	double tt_interestpoints;
	double tt_descriptors;
};

//-------------------------------------------------------

//! Builds vector of described interest points on a horizon
inline void surfDetDes(TestImage &image,
		int horizon_width = WIDTH, /* number of pixels on the horizon to average */
		int octaves = 4, /* number of octaves to calculate */
		int intervals = 3, /* number of intervals per octave */
		int init_sample = INIT_SAMPLE, /* initial sampling step */
		float thres = THRES /* blob response threshold */
)
{
	timespec detectors, detectore, extractors, extractore;
#if RUNSWIFT
	double time = (double)cvGetTickCount();
#else
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectors);
#endif

	// Create integral-image representation of the image
	std::vector<int> y_coords;
	IplImage *int_img = Integral(image.img, image.left, image.right, y_coords, horizon_width);
#if RUNSWIFT
	double integral_time = (double)cvGetTickCount();
	image.tt_integral = (integral_time - time)/(cvGetTickFrequency()*1000.f);
#endif
	// Create Fast Hessian Object
	FastHessian fh(y_coords, int_img, image.ipts, octaves, intervals, init_sample, thres);

	// Extract interest points and store in vector ipts
	fh.getIpoints();

#if RUNSWIFT
	  double extraction_time = (double)cvGetTickCount();
	  image.tt_interestpoints = (extraction_time - integral_time)/(cvGetTickFrequency()*1000.f);
#else
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectore);
	Ipoint::detectionTime = Ipoint::diffSurf(detectors,detectore).tv_nsec/1000000.0f;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractors);

#endif
	// Create Surf Descriptor Object
	Surf des(int_img, image.ipts);

	// Extract the descriptors for the ipts
	des.getHorizonDescriptors();
#if RUNSWIFT
	double description_time = (double)cvGetTickCount();
	image.tt_descriptors = (description_time - extraction_time)/(cvGetTickFrequency()*1000.f);
#else
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractore);
	Ipoint::extractionTime = Ipoint::diffSurf(extractors,extractore).tv_nsec/1000000.0f;
#endif
	// Deallocate the integral image
	cvReleaseImage(&int_img);
}


#endif
