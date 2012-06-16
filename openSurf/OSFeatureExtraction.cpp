/*
 * OSFeatureExtraction.cpp
 *
 *  Created on: 14 Jun 2012
 *      Author: daniel
 */

#include "OSFeatureExtraction.h"
#define PI 3.14159265
#define FEATURE_DEBUG_MODE 1

OSFeatureExtraction::OSFeatureExtraction() {
	// TODO Auto-generated constructor stub

}

OSFeatureExtraction::~OSFeatureExtraction() {
	// TODO Auto-generated destructor stub
}

//Verfify that a match is indeed correct
//Needs the image size and the keypoints
bool OSFeatureExtraction::verifyMatch(const IplImage &image,Ipoint &ipt1, Ipoint &ipt2)
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
	if((angle<-5 || angle > 5) || (xdistance>(640+200) || xdistance<(640-200)))
		return false;
	else
		return true;

}
