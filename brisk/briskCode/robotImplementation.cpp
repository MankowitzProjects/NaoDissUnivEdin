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

#define DEBUG_MODE 1
#define OUTPUT 1
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


int main(int argc, char ** argv) {


	//Set the arguments
	//std::string feat_detector = "SURF";
	//int threshold = 1000;
	bool hamming=true;
	std::string feat_detector = "BRISK";
	int threshold = 100;
	int hammingDistance = 85;//BRISK BRISK
	double radius = 0.15;//BRISK SURF
	std::string feat_descriptor = "BRISK";

	//Create the Feature extraction object
	FeatureExtraction feature;

	//Create data analysis object
	DataAnalysis dataAnalysis;

	// Declare the extractor. Only needs to be performed once since it computes lookup
	//tables for each of the various patterns on initialisation
	//*****************************************************************
	cv::Ptr<cv::DescriptorExtractor> descriptorExtractor;
	descriptorExtractor = feature.getExtractor(argc, feat_descriptor, hamming, descriptorExtractor);
	//*****************************************************************

	//The directory where the files are stored
	std::string dir = "../../images/PicsMG/Matching_Pics_Right_Overlapping";
	std::string dir1 = "../../images/PicsMG/Matching_Pics_Right_Overlapping";//PicsOG/Matching_Images_OG_Left
	//Names of the two image files
	std::string name1 = "9";
	std::string name2 = "2";

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
	cv::Mat imgRGB3;
	bool do_rot=false;
	// standard file extensions
	std::vector<std::string> fextensions;
	fextensions.push_back(".jpeg");
	fextensions.push_back(".jpg");

	timespec ts, te, matchings, matchinge, detectors, detectore, extractors, extractore;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);


	// if no arguments are passed:
	//Read in images
	//*****************************************************************
	int i=0;
	int fextensions_size=fextensions.size();
	while(imgRGB1.empty()||imgRGB2.empty()){
		fname1 = dir+"/"+name1+".jpg";
		fname2 = dir1+"/"+name2+".jpg";
		imgRGB1 = cv::imread(fname1);
		imgRGB2 = cv::imread(fname2);
		i++;
		if(i>=fextensions_size) break;
	}
	if (imgRGB2.empty()||imgRGB2.empty())
	{
		std::cout<<"image(s)"<<fname1<<", "<<fname2<<" not found." << std::endl;
		return 2;
	}
	//*****************************************************************

	// convert to grayscale
	//*****************************************************************
	cv::Mat imgGray1;
	cv::cvtColor(imgRGB1, imgGray1, CV_BGR2GRAY);
	cv::Mat imgGray2;
	if(!do_rot){
		cv::cvtColor(imgRGB2, imgGray2, CV_BGR2GRAY);
	}
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
	// first image. Computes the descriptor for each of the keypoints.
	//Outputs a 64 bit vector describing the keypoints.
	descriptorExtractor->compute(imgGray2,keypoints2,descriptors2);
	// and the second one
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
		descriptorMatcher->radiusMatch(descriptors2,descriptors,matches,hammingDistance);
	else{
		//Messing with the maxdistance value will drastically reduce the number of matches
		descriptorMatcher->radiusMatch(descriptors2,descriptors,matches,radius);
		//descriptorMatcher->knnMatch(descriptors2,descriptors,matches,3);
	}
	//For the above method, we could use KnnMatch. All values less than 0.21 max distance are selected

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchinge);
	double matchingTime = diff(matchings,matchinge).tv_nsec/1000;
	//*****************************************************************


	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &te);
	double overallTime = diff(ts,te).tv_nsec/1000000;

	//Image created for drawing
	cv::Mat outimg;


	//Write the data to a file
	//    ofstream writeFile;
	//
	//    std::string filename = "../../data/implementation/matchingData.txt";
	//    writeFile.open(filename.c_str(), ios::app);//ios::app

	// Variables for matching statistics
	float imageMatchingScore = 0;
	float imageMatchingScoreBest = 0;
	int totalNumMatches = 0;
	int totalNumValidMatches = 0;
	int totalNumInvalidMatches = 0;
	int totalNumBestMatches = 0;

	//To store the incorrect matches
#if(DISPLAY || DEBUG_MODE)
	std::vector<cv::Point2f> leftPoints;
	std::vector<cv::Point2f> rightPoints;
#endif

#if (DEBUG_MODE)
	cout<<"The total number of keypoints in image 1 is: "<<keypoints.size()<<endl;
	cout<<"The total number of keypoints in image 2 is: "<<keypoints2.size()<<endl;
#endif

	for( size_t i = 0; i < matches.size(); i++ )
	{

		if (matches[i].size()>0){
#if (DEBUG_MODE)
			cout<<"The number of matches is : "<<matches[i].size()<<endl;
			cout<<"****************************************"<<endl;
#endif
		}

		int allMatches = matches[i].size();
		int counter = 0;
		int matchSize = matches[i].size();
		bool isTrueMatchFound = false;

		//for(size_t j = 0; j < matches[i].size(); j++ )
		while(counter<matchSize)
		{


			//Corresponds to the first image
			int i1 = matches[i][counter].trainIdx;
			//Corresponds to the second image
			int i2 = matches[i][counter].queryIdx;
			//Determine the distance between matches
			float distanceMatch = matches[i][counter].distance;

			//Give a constant reward for being under a certain threshold
			float matchingScore = 0;
			if (distanceMatch==0)
				matchingScore=100;
			else
				matchingScore = 1/distanceMatch;

#if (DEBUG_MODE)
			cout<<"Keypoint indices i1, i2: "<<i1<<", "<<i2<<endl;
			cout<<"****************************"<<endl;
			cout<<"Keypoint Left  row,col : "<<(*(keypoints2.begin()+i2)).pt.y<<", "<<(*(keypoints2.begin()+i2)).pt.x<<endl;
			cout<<"Keypoint Right row,col : "<<(*(keypoints.begin() + i1)).pt.y<<", "<<(*(keypoints.begin() + i1)).pt.x<<endl;
			cout<<"****************************"<<endl;
#endif

#if (OUTPUT)
			dataAnalysis.displayOutput(keypoints2, keypoints, matchingScore, i1, i2);
#endif

			//Verify whether the match is correct or not
			//****************************************************
			bool correctMatch = feature.verifyMatch(imgRGB1, keypoints2[i2], keypoints[i1]);
#if (DEBUG_MODE)
			cout<<"CorrectMatch: "<<correctMatch<<endl;
#endif
			//****************************************************
			//If the match is incorrect, remove the invalid match
			if (correctMatch==false)
			{
#if (DEBUG_MODE)
				cout<<"Entered Here"<<endl;
				cout<<"Keypoint Left to be erased row,col : "<<(*(keypoints2.begin()+i2)).pt.y<<", "<<(*(keypoints2.begin()+i2)).pt.x<<endl;
				cout<<"Keypoint Right to be erased row,col : "<<(*(keypoints.begin() + i1)).pt.y<<", "<<(*(keypoints.begin() + i1)).pt.x<<endl;
#endif

#if(DEBUG_MODE)
				cout<<"The number of matches before removal is : "<<matches[i].size()<<endl;
				cout<<"-----------------------------------------"<<endl;
#endif
				//Erase the corresponding match
				matches[i].erase(matches[i].begin()+counter);

				matchSize = matches[i].size();
#if (DEBUG_MODE)
				cout<<"The number of matches after removal is : "<<matchSize<<endl;
				cout<<"counter is : "<<counter<<endl;
				cout<<"----------------------------------------"<<endl;
#endif

				totalNumInvalidMatches = totalNumInvalidMatches + 1;
#if (DEBUG_MODE)
				leftPoints.push_back(keypoints2[i2].pt);
				rightPoints.push_back(keypoints[i1].pt);
#endif
			}
			else
			{
				isTrueMatchFound = true;
				counter++;
			}
			//cv::waitKey(500);

			if(matches[i].size()==0)
				break;



			//This only considers the best correct match.
			if(isTrueMatchFound && counter ==1){
				totalNumBestMatches = totalNumBestMatches + 1;
				imageMatchingScoreBest = imageMatchingScoreBest + matchingScore;
				isTrueMatchFound = false;
			}

			imageMatchingScore = imageMatchingScore + matchingScore;

		}
		totalNumValidMatches = totalNumValidMatches + matches[i].size();
		totalNumMatches = totalNumMatches + allMatches;
	}
#if (DEBUG_MODE)
	cout<<"****************************************"<<endl;
	cout<<"The matching score for the image (condsidering all matches) is "<<imageMatchingScore<<endl;
	cout<<"The matching score for the image (condsidering best match only) is "<<imageMatchingScoreBest<<endl;
	cout<<"The total number of matches is "<<totalNumMatches<<endl;
	cout<<"The total number of valid matches is "<<totalNumValidMatches<<endl;
	cout<<"****************************************"<<endl;
#endif
#if (DEBUG_MODE)
	std::cout<<"The times:"<<endl;
	std::cout<<"Detection Time: "<<detectionTime<<" ms"<<endl;
	std::cout<<"Extraction Time: "<<extractionTime<<" ms"<<endl;
	std::cout<<"Matching Time: "<<matchingTime<<" us"<<endl;
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
	drawMatches(imgRGB2, keypoints2, imgRGB1, keypoints,matches,outimg,
			cv::Scalar(0,255,0), cv::Scalar(0,0,255),
			std::vector<std::vector<char> >(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
//NOT_DRAW_SINGLE_POINTS

	int colourChanger = 0;
	for (int k = 0; k<leftPoints.size(); k++)
	{

		circle(imgRGB2,cv::Point(leftPoints[k].x, leftPoints[k].y), 5, cv::Scalar(colourChanger, 100, 255), 4, 8, 0);

#if(DEBUG_MODE)
		cout<<"Incorrect coord Left row,col : "<<leftPoints[k].y<<", "<<leftPoints[k].x<<endl;
#endif
		colourChanger = colourChanger+30;
	}
	colourChanger = 0;
	for (int k = 0; k<rightPoints.size(); k++)
	{
		circle(imgRGB1,cv::Point(rightPoints[k].x, rightPoints[k].y), 5, cv::Scalar(colourChanger, 100, 255), 4, 8, 0);
#if(DEBUG_MODE)
		cout<<"Incorrect coord Right row,col : "<<rightPoints[k].y<<", "<<rightPoints[k].x<<endl;
#endif
		colourChanger = colourChanger+30;
	}



	cv::namedWindow("Matches");
	cv::imshow("Matches", outimg);
	//imgRGB1 is right. imgRGB2 is left
#if(DEBUG_MODE)
	cv::imshow("keypoints", imgRGB1);
	cv::imshow("keypoints2", imgRGB2);
#endif

	cv::waitKey();
#endif

	return 0;
}
