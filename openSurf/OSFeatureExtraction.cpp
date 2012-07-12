/*
 * OSFeatureExtraction.cpp
 *
 *  Created on: 14 Jun 2012
 *      Author: daniel
 */

#include "OSFeatureExtraction.h"
#define PI 3.14159265
#define FEATURE_DEBUG_MODE 0

OSFeatureExtraction::OSFeatureExtraction(float angle, int distance) {
	// TODO Auto-generated constructor stub
	this->angleThreshold = angle;
	this->distanceThreshold = distance;

}

OSFeatureExtraction::~OSFeatureExtraction() {
	// TODO Auto-generated destructor stub
}


void OSFeatureExtraction::calculateMatchingScore(const IplImage &image, IpVec &ipt1, IpVec &ipt2, IpPairVec & matches)
{

	//Initialise the scoring variables
	imageMatchingScore = 0;

	//For matching
	distanceMatch = 0;

#if (FEATURE_DEBUG_MATCHES)
	cout<<"The total number of keypoints in image 1 is: "<<ipt1.size()<<endl;
	cout<<"The total number of keypoints in image 2 is: "<<ipt2.size()<<endl;
#endif

	//Loops through all of the matches
	for( size_t i = 0; i < matches.size(); i++ )
	{

		//Initialise the variables
		ip1 = matches[i].first;
		ip2 = matches[i].second;
		//Determine the distance between matches
		distanceMatch = ip1 - ip2;

		//Give a constant reward for being under a certain threshold
		float matchingScore = 0;
		if (distanceMatch==0)
			matchingScore=100;
		else
			matchingScore = 1/distanceMatch;

		if(ip1.x!=-1 && ip2.x!=-1)
		imageMatchingScore = imageMatchingScore + matchingScore;

	}

}


//Verfify that a match is indeed correct
//Needs the image size and the keypoints
bool OSFeatureExtraction::verifyMatch1D(const IplImage &image,Ipoint &ipt1, Ipoint &ipt2)
{

	//Store the keypoint coordinates
	//Image Left
	int x1 = ipt1.x;//col
	int y1 = ipt1.y;//row


	//Image Right
	int x2 = ipt2.x;//col
	int y2 = ipt2.y;//row


#if (FEATURE_DEBUG_MODE)
	cout<<"rowR, colR: "<<y1<<", "<<x1<<endl;
	cout<<"rowL, colL: "<<y2<<", "<<x2<<endl;
#endif

	//Store the image col and rows
	int rows = image.height;
	int cols = image.width;

#if (FEATURE_DEBUG_MODE)
	cout<<"image rows: "<<rows<<endl;
	cout<<"image cols: "<<cols<<endl;
#endif

	//Find the absolute distance between the two coordinates if the images were placed together
	double xdistance, ydistance;
	xdistance = (cols - x1) + x2;//cols
	ydistance = y2-y1;//rows

#if (FEATURE_DEBUG_MODE)
	cout<<"col distance: "<<xdistance<<endl;
	cout<<"row distance: "<<ydistance<<endl;
#endif

	//Calculate the angle of the line
	double angle = 0;
	angle = atan2(ydistance, xdistance)*180/PI;

#if (FEATURE_DEBUG_MODE)
	cout<<"angle: "<<angle<<endl;
	//cout<<"Negative angle: "<<atan2(-10, 50)*180/PI<<endl;
#endif

	//If the angle is greater than a threshold, then invalid match
	if((angle<-angleThreshold || angle > angleThreshold) || (xdistance>(image.width+distanceThreshold) || xdistance<(image.width-distanceThreshold)))
		return false;
	else
		return true;

}

//Perform RANSAC to extract the useful interest points
void OSFeatureExtraction::performRansac(IpPairVec & matches){

	//Store the total number of matches before the validation procedure is performed
	totalNumMatches  =matches.size();

	totalNumValidMatches = 0;
	for (int i = 0;i< matches.size();i++)
	{
		//The RANSAC function
		x_stored = matches[i].first.x;
		//Set the scaling and displacement parameters
		b_s = 1;
		b_d = 0;

		x_test = matches[i].second.x;

		//Set the acceptance threshold
		threshold = 30;

		x_test_expected = b_s*x_stored + b_d;
		difference = x_test - x_test_expected;

		//Remove the match
		if(difference > threshold || difference < -threshold){
			matches[i].first.x = -1;
			matches[i].second.x = -1;
		}
		else
			totalNumValidMatches++;
	}//end for loop

}

