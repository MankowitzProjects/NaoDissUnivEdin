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
#include "ipoint.h"

class OSFeatureExtraction {
public:
	OSFeatureExtraction(float angle, int distance);
	virtual ~OSFeatureExtraction();
	void calculateMatchingScore(const IplImage &image, IpVec &ipt1, IpVec &ipt2, IpPairVec & matches);
	bool verifyMatch1D(const IplImage &image,Ipoint &ipt1, Ipoint &ipt2);
	void performRansac(IpPairVec & matches);

	//Get matching stats
	Ipoint ip1;
	Ipoint ip2;
	float distanceMatch;
	int totalNumValidMatches;
	float imageMatchingScore;
	bool isCorrectMatch;
	int totalNumMatches;

private:
	//RANSAC Parameters
	float x_stored;
	float b_d;
	float b_s;
	float x_test;
	float x_test_expected;
	float difference;
	float threshold;

	//Set the thresholds for angle and distance if needed
	float angleThreshold;
	int distanceThreshold;



};

#endif /* OSFEATUREEXTRACTION_H_ */
