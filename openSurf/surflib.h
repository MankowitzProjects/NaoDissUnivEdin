/*********************************************************** 
 *  --- OpenSURF ---                                       *
 *  This library is distributed under the GNU GPL. Please   *
 *  use the contact form at http://www.chrisevansdev.com    *
 *  for more information.                                   *
 *                                                          *
 *  C. Evans, Research Into Robust Visual Features,         *
 *  MSc University of Bristol, 2008.                        *
 *                                                          *
 ************************************************************/

#ifndef SURFLIB_H
#define SURFLIB_H

#include <cv.h>
#include <highgui.h>

#include "integral.h"
#include "fasthessian.h"
#include "surf.h"
#include "ipoint.h"
#include "utils.h"

#include <ctime>

#define TIME_DEBUG 0


inline timespec diffSurf(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

//! Library function builds vector of described interest points
inline void surfDetDes(IplImage *img,  /* image to find Ipoints in */
		std::vector<Ipoint> &ipts, /* reference to vector of Ipoints */
		bool upright = false, /* run in rotation invariant mode? */
		int octaves = OCTAVES, /* number of octaves to calculate */
		int intervals = INTERVALS, /* number of intervals per octave */
		int init_sample = INIT_SAMPLE, /* initial sampling step */
		float thres = THRES /* blob response threshold */)
{
	// Create integral-image representation of the image
	IplImage *int_img = Integral(img);
	cvWaitKey(0);
	// Create Fast Hessian Object
	FastHessian fh(int_img, ipts, octaves, intervals, init_sample, thres);

	// Extract interest points and store in vector ipts
	fh.getIpoints();

	// Create Surf Descriptor Object
	Surf des(int_img, ipts);

	// Extract the descriptors for the ipts
	des.getDescriptors(upright);

	// Deallocate the integral image
	cvReleaseImage(&int_img);
}


//! Library function builds vector of interest points
inline void surfDet(IplImage *img,  /* image to find Ipoints in */
		std::vector<Ipoint> &ipts, /* reference to vector of Ipoints */
		int octaves = OCTAVES, /* number of octaves to calculate */
		int intervals = INTERVALS, /* number of intervals per octave */
		int init_sample = INIT_SAMPLE, /* initial sampling step */
		float thres = THRES /* blob response threshold */)
{
	timespec ts, te, hs, he, is, ie;
	//Transform the image into a 1D image
	//IplImage *one_int_Image = getRowOfPixels(img);

	// Create integral image representation of the image
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);

	IplImage *int_img = Integral(img);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &te);
	float intTime = diffSurf(ts,te).tv_nsec/1000;

	// Create Fast Hessian Object.
	//MC: The Fast Hessian is computed using box filters and the integral image
	//MC: We construct and convolve box filters with integral image
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &hs);

	FastHessian fh(int_img, ipts, octaves, intervals, init_sample, thres);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &he);
	float hessTime = diffSurf(hs,he).tv_nsec/1000;

	// Extract interest points and store in vector ipts
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &is);

	fh.getIpoints();

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ie);
	float ipointTime = diffSurf(is,ie).tv_nsec/1000;

#if (TIME_DEBUG)
	cout<<"Integral Time: "<<intTime<<" us"<<endl;
	cout<<"Hessian Time: "<<hessTime<<" us"<<endl;
	cout<<"Ipoint time: "<<ipointTime<<" us"<<endl;
#endif
	// Deallocate the integral image
	cvReleaseImage(&int_img);
}




//! Library function describes interest points in vector
inline void surfDes(IplImage *img,  /* image to find Ipoints in */
		std::vector<Ipoint> &ipts, /* reference to vector of Ipoints */
		bool upright = false) /* run in rotation invariant mode? */
{ 

	timespec ts, te, ds, de, is, ie;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);

	// Create integral image representation of the image
	IplImage *int_img = Integral(img);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &te);
	double intTime = diffSurf(ts,te).tv_nsec/1000;


	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ds);
	// Create Surf Descriptor Object
	Surf des(int_img, ipts);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &de);
	double descripTime = diffSurf(ds,de).tv_nsec/1000;


	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &is);
	// Extract the descriptors for the ipts
	des.getDescriptors(upright);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ie);
	double ipointTime = diffSurf(is,ie).tv_nsec/1000;

#if (TIME_DEBUG)
	cout<<"Integral 2 Time: "<<intTime<<" us"<<endl;
	cout<<"Descript Time: "<<descripTime<<" us"<<endl;
	cout<<"Ipoint 2 time: "<<ipointTime<<" us"<<endl;
#endif
	// Deallocate the integral image
	cvReleaseImage(&int_img);
}


#endif
