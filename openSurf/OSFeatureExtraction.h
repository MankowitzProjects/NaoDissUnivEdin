/*
 * OSFeatureExtraction.h
 *
 *  Created on: 14 Jun 2012
 *      Author: daniel
 */

#ifndef OSFEATUREEXTRACTION_H_
#define OSFEATUREEXTRACTION_H_

#include <fstream>
#include <iostream>
#include <list>
#include <iomanip>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "surflib.h"

class OSFeatureExtraction {
public:
	OSFeatureExtraction();
	virtual ~OSFeatureExtraction();
	bool verifyMatch(const IplImage &image,Ipoint &ipt1, Ipoint &ipt2);
	bool verifyMatch1D(const IplImage &image,Ipoint &ipt1, Ipoint &ipt2);
};

#endif /* OSFEATUREEXTRACTION_H_ */
