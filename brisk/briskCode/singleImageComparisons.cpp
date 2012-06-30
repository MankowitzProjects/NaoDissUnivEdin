/*
    BRISK - Binary Robust Invariant Scalable Keypoints
    Reference implementation of
    [1] Stefan Leutenegger,Margarita Chli and Roland Siegwart, BRISK:
    	Binary Robust Invariant Scalable Keypoints, in Proceedings of
    	the IEEE International Conference on Computer Vision (ICCV2011).

    Copyright (C) 2011  The Autonomous Systems Lab (ASL), ETH Zurich,
    Stefan Leutenegger, Simon Lynen and Margarita Chli.

    This file is part of BRISK.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
         notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
         notice, this list of conditions and the following disclaimer in the
         documentation and/or other materials provided with the distribution.
 * Neither the name of the ASL nor the names of its contributors may be
         used to endorse or promote products derived from this software without
         specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <opencv2/opencv.hpp>
#include "include/brisk.h"
#include <fstream>
#include <iostream>
#include <list>
#include <iomanip>
#include "include/DataAnalysis.h"
#include "include/FeatureExtraction.h"


#include <string>
#include <sstream>

#define DEBUG_MODE 0
#define DEBUG_TIMES 1
#define DEBUG_MATCHES 1
#define OUTPUT 0
#define SINGLE_IMAGE_TEST 1
#define MULTIPLE_IMAGE_TEST 1
#define DISPLAY 1

template <class T>
inline std::string to_string (const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}


//Timer
timespec diff(timespec start, timespec end)
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



//standard configuration for the case of no file given
const int n=12;
const float r=2.5; // found 8-9-11, r=3.6, exponent 1.5

//Main function
int main(int argc, char ** argv) {


	//Set the arguments
	//std::string feat_dMetector = "SURF";
	//int threshold = 1000;
	bool hamming=true;
	std::string feat_detector = "BRISK";
	int threshold = 20;
	int hammingDistance = 400;//BRISK BRISK
	double radius = 0.15;//BRISK SURF
	std::string feat_descriptor = "BRISK";

	//Create the Feature extraction object
	FeatureExtraction feature;

	//Create data analysis object
	DataAnalysis dataAnalysis;

	//*************************************
	//Get the current time for saving purposes and set the file to save to
	//*************************************
	time_t rawtime;
	struct tm * timeinfo;
	char filename [80];
	string testThresholdString = to_string<int>(threshold);
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	string file = "../data/singleImage/singleImageData_threshold_26062012_BRISK_100_BRISK_KNN_keypoints";
	//		file.append(threshold.c_str());
	//		file.append("Directory_");
	//		file.append(tempDir.c_str());
	//		file.append("_");
	//		file.append(stringRad.c_str());
	file.append(".txt");
	cout<<file<<endl;
	string file1 = "../data/singleImage/singleImageData_threshold_26062012_BRISK_100_BRISK_KNN_matches";
	//		file.append(threshold.c_str());
	//		file.append("Directory_");
	//		file.append(tempDir.c_str());
	//		file.append("_");
	//		file.append(stringRad.c_str());
	file1.append(".txt");
	cout<<file1<<endl;
	//*************************************




	// Declare the extractor. Only needs to be performed once since it computes lookup
	//tables for each of the various patterns on initialisation
	//*****************************************************************
	cv::Ptr<cv::DescriptorExtractor> descriptorExtractor;
	descriptorExtractor = feature.getExtractor(argc, feat_descriptor, hamming, descriptorExtractor);
	//*****************************************************************
	timespec ts, te, matchings, matchinge, detectors, detectore, extractors, extractore, verifys, verifye;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);

	//The directory where the files are stored
	std::string dir = "../images/PicsMG/Matching_Pics_Right_Overlapping";
	std::string dir1 = "../images/PicsMG/Matching_Pics_Right_Overlapping";//PicsOG/Matching_Images_OG_Left

	//dir = to_string<int>(ss);
	std::string tempDir = to_string<int>(1);

	//Names of the two image files
	std::string name1 = "2";
	std::string name2 = "12";

	//For changing the threshold
	int testThreshold = 10;

	//Choose the images to compare
	//    name1 = to_string<int>(ii);
	//    if(ii==jj)
	//    continue;
	//
	//    name2 = to_string<int>(jj);

	cout<<"Image in directory 1: "<<name1<<", Image in directory 2: "<<name2<<endl;


	// names of the two images
	std::string fname1;
	std::string fname2;
	cv::Mat imgRGB1;
	cv::Mat imgRGB2;
	//	cv::Mat imgRGB3;
	cv::Mat imgGray1Full;
	cv::Mat imgGray2Full;
	cv::Mat imgGray1;
	cv::Mat imgGray2;

	bool do_rot=false;
	// standard file extensions
	std::vector<std::string> fextensions;
	fextensions.push_back(".jpeg");
	fextensions.push_back(".jpg");




	// if no arguments are passed:
	//Read in images
	//*****************************************************************
	int i=0;
	int fextensions_size=fextensions.size();
	while(imgGray1Full.empty()||imgGray2Full.empty()){
		fname1 = dir+"/"+name1+".jpg";
		fname2 = dir1+"/"+name2+".jpg";
		//imgRGB1 = cv::imread(fname1);
		//imgRGB2 = cv::imread(fname2);
		imgGray1Full = cv::imread(fname1.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
		imgGray2Full = cv::imread(fname2.c_str(), CV_LOAD_IMAGE_GRAYSCALE);

		i++;
		if(i>=fextensions_size) break;
	}
	if (imgGray1Full.empty()||imgGray2Full.empty())
	{
		std::cout<<"image(s)"<<fname1<<", "<<fname2<<" not found." << std::endl;
		return 2;
	}
	//*****************************************************************
	//We only need the keypoints above the horizon
	int horizonLine = 300;
	imgGray1 = imgGray1Full(cv::Rect(0, 0, imgGray1Full.cols, horizonLine));
	imgGray2 = imgGray2Full(cv::Rect(0, 0, imgGray2Full.cols, horizonLine));


	// convert to grayscale
	//*****************************************************************
	//	imgGray1;
	//	cv::cvtColor(imgRGB1, imgGray1, CV_BGR2GRAY);
	//	imgGray2;
	//	if(!do_rot){
	//		cv::cvtColor(imgRGB2, imgGray2, CV_BGR2GRAY);
	//	}
	//*****************************************************************


	//MC: Generate a vector of keypoints
	std::vector<cv::KeyPoint> keypoints, keypoints2;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectors);
	// create the detector:
	//*****************************************************************
	cv::Ptr<cv::FeatureDetector> detector;
	detector = feature.getDetector(argc, feat_detector, detector, threshold, testThreshold,1);
	//*****************************************************************

	// run the detector:
	//*****************************************************************
	detector->detect(imgGray1,keypoints);
	detector->detect(imgGray2,keypoints2);
	//*****************************************************************
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectore);
	double detectionTime = diff(detectors,detectore).tv_nsec/1000000;


	//*****************************************************************
	//This is where we add the openSURF
	//Convert Mat to IPlImage
	IplImage iplimg1 = imgGray1;
	IplImage iplimg2 = imgGray2;


	//*****************************************************************
	// get the descriptors
	cv::Mat descriptors, descriptors2;
	std::vector<cv::DMatch> indices;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractors);
	// second image. Computes the descriptor for each of the keypoints.
	//Outputs a 64 bit vector describing the keypoints.
	descriptorExtractor->compute(imgGray2,keypoints2,descriptors2);
	// and the first one
	descriptorExtractor->compute(imgGray1,keypoints,descriptors);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractore);
	double extractionTime = diff(extractors,extractore).tv_nsec/1000000;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchings);

	// matching
	//*****************************************************************
	std::vector<std::vector<cv::DMatch> > matches;
	cv::Ptr<cv::DescriptorMatcher> descriptorMatcher;
	if(hamming)
		descriptorMatcher = new cv::BruteForceMatcher<cv::HammingSse>();
	else
		descriptorMatcher = new cv::BruteForceMatcher<cv::L2<float> >();
	if(hamming)
		//descriptorMatcher->radiusMatch(descriptors,descriptors2,matches,hammingDistance);

		//The first parameter is the query descriptor. The second parameter is the train descriptor
		descriptorMatcher->knnMatch(descriptors,descriptors2,matches,2);
	else{
		//Messing with the maxdistance value will drastically reduce the number of matches
		descriptorMatcher->knnMatch(descriptors,descriptors2,matches,2);//radiusMatch radius
	}
	//For the above method, we could use KnnMatch. All values less than 0.21 max distance are selected

	//*****************************************************************
	//Image created for drawing
	cv::Mat outimg;



	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &verifys);

	//Perform the matching verification
	//*************************************************************************
	feature.performMatchingValidation(imgGray1,keypoints, keypoints2, matches, hamming);
	//*************************************************************************
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &verifye);
	double verifyTime = diff(verifys,verifye).tv_nsec/1000;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchinge);
	double matchingTime = diff(matchings,matchinge).tv_nsec/1000;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &te);
	double overallTime = diff(ts,te).tv_nsec/1000000;

	//Write the data to a file
	ofstream writeFile;
	writeFile.open(file.c_str(), ios::app);

	//1. Write all the keypoints to the first image file

	//Swap hamming distance with radius
	for(int im1 = 0;im1<keypoints.size();im1++){
		//[image num, hammingdist, threshold, kp1x, kp1y, angle, size, octave]
		writeFile <<name1<<", "<<hammingDistance<<", "<<testThreshold<<", "<<im1<<", "<<keypoints[im1].pt.x<<", "<<keypoints[im1].pt.y<<", "<<keypoints[im1].angle<<", "<<keypoints[im1].size<<", "<<keypoints[im1].octave<<", "<<keypoints[im1].response<<"\n";
	}

	//2. Write all the keypoints to the second image file
	//Swap hamming distance with radius
	for(int im2 = 0;im2<keypoints2.size();im2++){
		//[image num, hammingdist, threshold, kp1x, kp1y, angle, size, octave]
		writeFile <<name2<<", "<<hammingDistance<<", "<<testThreshold<<", "<<im2<<", "<<keypoints2[im2].pt.x<<", "<<keypoints2[im2].pt.y<<", "<<keypoints2[im2].angle<<", "<<keypoints2[im2].size<<", "<<keypoints2[im2].octave<<", "<<keypoints2[im2].response<<"\n";
	}

	//Close the file
	writeFile.close();
	writeFile.clear();

	//Write the data to a file
	writeFile.open(file1.c_str(), ios::app);

	//Write all the information to a file
	//image num left, image num right, queryIdx, trainIdx,  keypoint1 x, kp1y, angle size, octave, response, kp2x, kp2y, angle, size, octave, neighbor num, distance
	//Swap hamming distance with radius
	for(int mIdx = 0;mIdx<matches.size();mIdx++){
		for(int nIdx = 0;nIdx<matches[mIdx].size();nIdx++)
		{
			int q = matches[mIdx][nIdx].queryIdx;
			int t = matches[mIdx][nIdx].trainIdx;
			writeFile <<name1<<", "<<name2<<", "<<q<<", "<<t<<", "<<keypoints[q].pt.x<<", "<<keypoints[q].pt.y<<", "<<keypoints[q].angle<<", "<<keypoints[q].size<<", "<<keypoints[q].octave<<", "<<keypoints[q].response<<", "<<keypoints2[t].pt.x<<", "<<keypoints2[t].pt.y<<", "<<keypoints2[t].angle<<", "<<keypoints2[t].size<<", "<<keypoints2[t].octave<<", "<<keypoints2[t].response<<", "<<nIdx<<", "<<matches[q][t].distance<<"\n";
		}
	}
	//Close the file
	writeFile.close();
	writeFile.clear();

#if (DEBUG_MODE)
	cout<<"****************************************"<<endl;
	cout<<"The matching score for the image (condsidering all matches) is "<<feature.imageMatchingScore<<endl;
	cout<<"The matching score for the image (condsidering best match only) is "<<feature.imageMatchingScoreBest<<endl;
	cout<<"The total number of matches is "<<feature.totalNumMatches<<endl;
	cout<<"The total number of valid matches is "<<feature.totalNumValidMatches<<endl;
	cout<<"****************************************"<<endl;
#endif
#if (DEBUG_TIMES)
	std::cout<<"The times:"<<endl;
	std::cout<<"Detection Time: "<<detectionTime<<" ms"<<endl;
	std::cout<<"Extraction Time: "<<extractionTime<<" ms"<<endl;
	std::cout<<"Matching Time: "<<matchingTime<<" us"<<endl;
	std::cout<<"Verify Matches Time: "<<verifyTime<<" us"<<endl;
	std::cout<<"Overall Time: "<<overallTime<<" ms"<<endl;
#endif
	threshold = atoi(argv[3]+5);
	//    writeFile <<threshold<<", "<<name1<<", "<<name2<<", "<<keypoints.size()<<", "<<keypoints2.size()<<", "<<imageMatchingScoreBest<<", "<<imageMatchingScore<<","<<totalNumMatches<<", "<<totalNumBestMatches<<"\n";
	//    //close the file
	//    writeFile.close();
#if (DEBUG_MODE)
	cout<<"The total number of keypoints in image 1 is: "<<keypoints.size()<<endl;
	cout<<"The total number of keypoints in image 2 is: "<<keypoints2.size()<<endl;
#endif


#if (DISPLAY)
	drawMatches(imgGray1, keypoints, imgGray2, keypoints2,matches,outimg,
			cv::Scalar(0,255,0), cv::Scalar(0,0,255),
			std::vector<std::vector<char> >(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
	//NOT_DRAW_SINGLE_POINTS

	//Note: leftpoints correspond to keypoints - Image 1. rightpoints correspond to keypoints2 - Image 2.
	int colourChanger = 0;
	//	for (int k = 0; k<feature.rightPoints.size(); k++)
	//	{
	//
	//		circle(imgGray2,cv::Point(feature.rightPoints[k].x, feature.rightPoints[k].y), 5, cv::Scalar(colourChanger, 100, 255), 4, 8, 0);
	//
	//#if(DEBUG_MATCHES)
	//		cout<<"Incorrect coord Left row,col : "<<feature.rightPoints[k].y<<", "<<feature.rightPoints[k].x<<endl;
	//#endif
	//		colourChanger = colourChanger+30;
	//	}
	//	colourChanger = 0;
	//	for (int k = 0; k<feature.leftPoints.size(); k++)
	//	{
	//		circle(imgGray1,cv::Point(feature.leftPoints[k].x, feature.leftPoints[k].y), 5, cv::Scalar(colourChanger, 100, 255), 4, 8, 0);
	//#if(DEBUG_MATCHES)
	//		cout<<"Incorrect coord Right row,col : "<<feature.leftPoints[k].y<<", "<<feature.leftPoints[k].x<<endl;
	//#endif
	//		colourChanger = colourChanger+30;
	//	}
	for (int k = 0; k<matches.size(); k++)
	{
		for (int j=0;j<matches[k].size();j++)
		{
			int qi = matches[k][j].queryIdx;//Reference Point. Assumed to be for image left
			int ti = matches[k][j].trainIdx;//Assumed to be image right

			//The points in the left image
			int kp1x = (*(keypoints.begin()+qi)).pt.x;
			int kp1y = (*(keypoints.begin()+qi)).pt.y;

			//The points in the right image
			int kp2x = (*(keypoints2.begin()+ti)).pt.x;
			int kp2y = (*(keypoints2.begin()+ti)).pt.y;

			circle(outimg,cv::Point(kp1x, kp1y), 5, cv::Scalar(colourChanger, 100, 255), 4, 8, 0);
			circle(outimg,cv::Point(640+ kp2x, kp2y), 5, cv::Scalar(colourChanger, 100, 255), 4, 8, 0);

#if(DEBUG_MODE)
			cout<<"Correct coord Left row,col : "<<kp1y<<", "<<kp1x<<endl;
			cout<<"Correct coord Right row,col : "<<kp2y<<", "<<kp2x<<endl;
#endif
		}
		colourChanger = colourChanger+30;
	}



	cv::namedWindow("Matches");
	cv::imshow("Matches", outimg);
	//imgRGB1 is right. imgRGB2 is left
#if(DEBUG_MODE)
	cv::imshow("keypoints", imgGray1);
	cv::imshow("keypoints2", imgGray2);
#endif

	cv::waitKey();
#endif

	return 0;
}
