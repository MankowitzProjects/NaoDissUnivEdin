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
#define MATCHES_MODE 1
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
	//The angle used for matching validation
	double angle = 10;
	//The distance threshold
	double distance = 200;

	//Determine if the KNN validation criterion is necessary
	bool usingKnnCriterion = false;

	//Declare the horizon line above which the image is processed
	int horizonLine = 300;

	//Removes all matches except the first ones if true
	bool removeMatches = true;

	//Validates the matches
	bool performValidation=true;

	//Choose the number of KNN matches to display
	bool useDifferentNumMatches = false ;
	int numMatchesToDisplay = 0;

	//Set the arguments
	//std::string feat_dMetector = "SURF";
	//int threshold = 1000;
	bool hamming=true;
	std::string feat_detector = "BRISK";
	double threshold = 75;//46.25
	double hammingDistance = 121.25;//BRISK BRISK
	double radius = 0.28;//BRISK SURF
	std::string feat_descriptor = "U-BRISK";

	//For changing the threshold
	int testThreshold = 10;

	//Create the Feature extraction object
	FeatureExtraction feature(angle, distance, usingKnnCriterion);

	//Create data analysis object
	DataAnalysis dataAnalysis;

	// Declare the extractor. Only needs to be performed once since it computes lookup
	//tables for each of the various patterns on initialisation
	//*****************************************************************
	cv::Ptr<cv::DescriptorExtractor> descriptorExtractor;
	descriptorExtractor = feature.getExtractor(argc, feat_descriptor, hamming, descriptorExtractor);
	//*****************************************************************

	//Start by creating the stored image
	//****************************************************************************************
	//Find the directory where the image is stored
	std::string dir = "../images/PicsOGValidation/Matching_Pics_Right_Overlapping";
	std::string dir1 = "../images/PicsMGValidation/Matching_Pics_Right_Overlapping";//PicsOG/Matching_Images_OG_Left

	//Original Dataset Left light off
//		std::string dir = "../images/PicsMG/Matching_Pics_Left_Overlapping";
//		std::string dir1 = "../images/Pics1MGLeftLightOff/Matching_Pics_Left_Overlapping";//PicsOG/Matching_Images_OG_Left


	//Original dataset Right light off
//		std::string dir = "../images/PicsMG/Matching_Pics_Right_Overlapping";
//		std::string dir1 = "../images/Pics2MGRightLightOff/Matching_Pics_Right_Overlapping";//PicsOG/Matching_Images_OG_Left


	//Original dataset both lights off
//		std::string dir = "../images/Pics3OGBothLightsOff/Matching_Pics_Right_Overlapping";
//		std::string dir1 = "../images/Pics3OGBothLightsOff/Matching_Pics_Right_Overlapping";//PicsOG/Matching_Images_OG_Left

	//Dataset 2
//		std::string dir = "../images/Dataset2_Overlapping_1";
//		std::string dir1 = "../images/Dataset2_Overlapping_2";

	//Dataset 3
//		std::string dir = "../images/Dataset3_Overlapping_1";
//		std::string dir1 = "../images/Dataset3_Overlapping_2";

	//Google Street View dataset
//		std::string dir = "../images/GoogleMaps/NaoSide1";
//		std::string dir1 = "../images/GoogleMaps/GoogleSide1";//PicsOG/Matching_Images_OG_Left



	//Names of the two image files
	std::string name1 = "15";
	std::string name2 = "3";

	//Get the first gray image
	cv::Mat imgGray1Full;
	cv::Mat imgGray1;

	//Initialise the filename variable of the first image
	std::string fname1;

	//Get the image from the directory
	while(imgGray1Full.empty())
	{
		fname1 = dir+"/"+name1+".jpg";
		imgGray1Full = cv::imread(fname1.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	}
	if (imgGray1Full.empty())
	{
		std::cout<<"image(s)"<<fname1<<" not found." << std::endl;
		return 2;
	}

	//Declare the horizon variable and only get the keypoints above the horizon
	imgGray1 = imgGray1Full(cv::Rect(0, 0, imgGray1Full.cols, horizonLine));

	//Generate a set of keypoints
	std::vector<cv::KeyPoint> keypoints;

	cv::Ptr<cv::FeatureDetector> detector;
	detector = feature.getDetector(argc, feat_detector, detector, threshold, testThreshold,1);
	//Run the detector
	detector->detect(imgGray1,keypoints);

	//Initialise the descriptors
	cv::Mat descriptors;

	//Create the descriptors
	// and the first one
	descriptorExtractor->compute(imgGray1,keypoints,descriptors);

	//***********************************************************************************************
	//***********************************************************************************************
	//***********************************************************************************************

	timespec ts, te, matchings, matchinge, detectors, detectore, extractors, extractore, verifys, verifye;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);

	//Now extract the keypoints from the test image
	//***********************************************************************
	// names of the two images
	std::string fname2;

	cv::Mat imgGray2Full;
	cv::Mat imgGray2;

	//Read in images
	//*****************************************************************

	while(imgGray2Full.empty()){
		fname2 = dir1+"/"+name2+".jpg";
		imgGray2Full = cv::imread(fname2.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	}
	if (imgGray2Full.empty())
	{
		std::cout<<"image(s)"<<fname2<<" not found." << std::endl;
		return 2;
	}
	//*****************************************************************

	//We only need the keypoints above the horizon
	imgGray2 = imgGray2Full(cv::Rect(0, 0, imgGray2Full.cols, horizonLine));


//	cv::namedWindow("First Gray image");
//	cv::imshow("First Gray image", imgGray1);
//	cv::imwrite("../images/gray1.jpg",imgGray1);
//	cv::waitKey();
//	cv::namedWindow("Second Gray Image");
//	cv::imshow("Second Gray Image", imgGray2);
//	cv::imwrite("../images/gray2.jpg",imgGray2);
//	cv::waitKey();

	//cout<<"Image in directory 1: "<<name1<<", Image in directory 2: "<<name2<<endl;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectors);
	//MC: Generate a vector of keypoints
	std::vector<cv::KeyPoint> keypoints2;

	// create the detector:
	//*****************************************************************
	cv::Ptr<cv::FeatureDetector> detector1;
	detector1 = feature.getDetector(argc, feat_detector, detector, threshold, testThreshold,1);
	//*****************************************************************

	// run the detector:
	//*****************************************************************
	detector1->detect(imgGray2,keypoints2);
	//*****************************************************************
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectore);
	float detectionTime = diff(detectors,detectore).tv_nsec/1000000.0f;


	//*****************************************************************
	//*****************************************************************

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractors);
	// get the descriptors
	cv::Mat descriptors2;
	std::vector<cv::DMatch> indices;
	// second image. Computes the descriptor for each of the keypoints.
	//Outputs a 64 bit vector describing the keypoints.
	descriptorExtractor->compute(imgGray2,keypoints2,descriptors2);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractore);
	float extractionTime = diff(extractors,extractore).tv_nsec/1000000.0f;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchings);

	// matching
	//*****************************************************************
	std::vector<std::vector<cv::DMatch> > matches;
	cv::Ptr<cv::DescriptorMatcher> descriptorMatcher;
	if(hamming)
		descriptorMatcher = new cv::BruteForceMatcher<cv::HammingSse>();
	else
		descriptorMatcher = new cv::BruteForceMatcher<cv::L2<float> >();

	if(hamming){
		//The first parameter is the query descriptor. The second parameter is the train descriptor
		if (usingKnnCriterion)
		{
			if (descriptors2.rows>0)
				descriptorMatcher->knnMatch(descriptors,descriptors2,matches,2);
			else
				matches.clear();
		}
		else
		{
			if (descriptors2.rows>0)
				descriptorMatcher->radiusMatch(descriptors,descriptors2,matches,hammingDistance);
			else
				matches.clear();
		}

	}else{
		if (usingKnnCriterion)
		{
			cout<<"The number of left descriptors: "<<descriptors2.rows<<endl;
			cout<<"The number of right descriptors: "<<descriptors2.rows<<endl;

			if (descriptors2.rows>0)
				descriptorMatcher->knnMatch(descriptors,descriptors2,matches,2);
			else
				matches.clear();
		}
		else
		{
			if (descriptors2.rows>0)
				descriptorMatcher->radiusMatch(descriptors,descriptors2,matches,hammingDistance);//radiusMatch radius
			else
				matches.clear();
		}


	}
	//*****************************************************************

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &verifys);

	//Perform the matching verification
	//*************************************************************************
	if (performValidation)
	feature.performMatchingValidation(imgGray1,keypoints, keypoints2, matches, hamming);
	//*************************************************************************
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &verifye);
	float verifyTime = diff(verifys,verifye).tv_nsec/1000000.0f;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchinge);
	float matchingTime = diff(matchings,matchinge).tv_nsec/1000000.0f;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &te);
	float overallTime = diff(ts,te).tv_nsec/1000000.0f;


#if (MATCHES_MODE)
	cout<<"The total number of keypoints in Left image: "<<keypoints.size()<<endl;
	cout<<"The total number of keypoints in Right image: "<<keypoints2.size()<<endl;
	cout<<"****************************************"<<endl;
	cout<<"The matching score for the image (condsidering all matches) is "<<feature.imageMatchingScore<<endl;
	cout<<"The matching score for the image (condsidering best match only) is "<<feature.imageMatchingScoreBest<<endl;
	cout<<"The total number of matches is "<<feature.totalNumMatches<<endl;
	cout<<"The total number of valid matches is "<<feature.totalNumValidMatches<<endl;
	cout<<"The total number of best matches is "<<feature.totalNumBestMatches<<endl;
	cout<<"****************************************"<<endl;
#endif
#if (DEBUG_TIMES)
	std::cout<<"The times:"<<endl;
	std::cout<<"Detection Time: "<<detectionTime<<" ms"<<endl;
	std::cout<<"Extraction Time: "<<extractionTime<<" ms"<<endl;
	std::cout<<"Matching Time: "<<matchingTime<<" ms"<<endl;
	std::cout<<"Verify Matches Time: "<<verifyTime<<" ms"<<endl;
	std::cout<<"Overall Time: "<<overallTime<<" ms"<<endl;
#endif

	//    writeFile <<threshold<<", "<<name1<<", "<<name2<<", "<<keypoints.size()<<", "<<keypoints2.size()<<", "<<imageMatchingScoreBest<<", "<<imageMatchingScore<<","<<totalNumMatches<<", "<<totalNumBestMatches<<"\n";
	//    //close the file
	//    writeFile.close();
#if (DEBUG_MODE)
	cout<<"The total number of keypoints in image 1 is: "<<keypoints.size()<<endl;
	cout<<"The total number of keypoints in image 2 is: "<<keypoints2.size()<<endl;
#endif
	//Image created for drawing
	cv::Mat outimg;

#if (DISPLAY)

	feature.removeSecondMatches(matches, numMatchesToDisplay, useDifferentNumMatches, removeMatches);

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

	//This function draws the circles around each of the matches

	//The number of matches to display
	int totalMatches = matches.size();

	for (int k = 0; k<totalMatches; k++)
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

	//Demo pics for the thesis
//	cv::imwrite("../images/MainRobocupDataset.jpg",outimg); //10,2 D: 3,3
//	cv::imwrite("../images/OfficeDataset.jpg",outimg); //10,6 D: 1,1
//	cv::imwrite("../images/LargeHallDataset.jpg",outimg); //10,5 D: 1,1
//	cv::imwrite("../images/NaoNikonRobocupDataset.jpg",outimg); //16,9 D: 3,3
//	cv::imwrite("../images/NaoNikonLargeHallDataset.jpg",outimg);//2,12 D:1,1
//	cv::imwrite("../images/VaryingIlluminationLeftDataset.jpg",outimg);//19,6 D:1,1
//	cv::imwrite("../images/VaryingIlluminationRightDataset.jpg",outimg);//2,12 D:2,2
//	cv::imwrite("../images/VaryingIlluminationBothDataset.jpg",outimg);//5,15 D:2,2
//	cv::imwrite("../images/GoogleStreetViewDataset.jpg",outimg);//10,10 D:1,1

	//Pics showing problems - Mainly in main Robocup dataset
//	cv::imwrite("../images/KNN_ratio.jpg",outimg);//Image 4,4 Datasets 1,1
//	cv::imwrite("../images/LightingBrisk4.jpg",outimg);//Img 30, 8 from dataset 3,3
//	cv::imwrite("../images/weakInterestPointMatch.jpg",outimg);//Img 22, 1 from dataset 2,2
//	cv::imwrite("../images/fpMatchBRISK0.jpg",outimg);//Img 29, 11 from dataset 2,2
//	cv::imwrite("../images/reflectionsBrisk0.jpg",outimg);//Img 8, 7 from dataset 3,3
//	cv::imwrite("../images/noMatchesBrisk4.jpg",outimg);//Img 11, 4 from dataset 2,2
//	cv::imwrite("../images/rotationsUBRISK.jpg",outimg);//Img 100, 101 from dataset 1,1
	//cv::imwrite("../images/match.jpg",outimg);
//	cv::imwrite("../images/GoogleStreetViewMatch.jpg",outimg);//Img 10, 8 dataset Nao1, Google1
	//cv::imwrite("../images/t_20_hd_55_OG_Left_MG_Right_2_12.jpg",outimg);


	//Dataset2
//	cv::imwrite("../images/dataset2_interestPoints.jpg",outimg);//Img 5,10 dataset 5,6

	//Dataset3
//	cv::imwrite("../images/dataset3_interestPoints.jpg",outimg);//Img 13,1 dataset 5,6

	//Varying Lighting
//	cv::imwrite("../images/dataset_lighting_incorrect_matches.jpg",outimg);//Img 9,10 dataset 1,1
//	cv::imwrite("../images/dataset_lighting_incorrect_matches_OG.jpg",outimg);//Img 9,10 dataset 3,3

	//Google Street View
//	cv::imwrite("../images/google_street_interest_points.jpg",outimg);//Img 9,10 dataset 3,3

	//Feature Matching Images
	//2-NN Matching and Radius Matching
//	cv::imwrite("../images/dataset1_without_validation_knn.jpg",outimg);//Img 8,7 dataset 1: 3,3
//	cv::imwrite("../images/dataset1_without_validation_knn_best.jpg",outimg);//Img 8,7 dataset 1: 3,3
//	cv::imwrite("../images/dataset1_without_validation_radius.jpg",outimg);//Img 10,7 dataset 3: 3,3
//	cv::imwrite("../images/dataset1_without_validation_radius_best.jpg",outimg);//Img 10,7 dataset 3: 3,3

	//Matching constraints - 2-NN Ratio
//	cv::imwrite("../images/dataset_2nn_matching_constraint_before.jpg",outimg);//Img 10,9 dataset 1: 3,3
//	cv::imwrite("../images/dataset_2nn_matching_constraint_applied.jpg",outimg);//Img 10,9 dataset 1: 3,3
//	cv::imwrite("../images/dataset_2nn_matching_constraint_after.jpg",outimg);//Img 10,9 dataset 1: 3,3

	//Angle Constraint
//	cv::imwrite("../images/dataset_angle_constraint_before.jpg",outimg);//Img 10,9 dataset 2: 1,1
//	cv::imwrite("../images/dataset_angle_constraint_after.jpg",outimg);//Img 10,9 dataset 2: 1,1

	//Distance constraint
//	cv::imwrite("../images/dataset_distance_constraint_before.jpg",outimg);//Img 20,9 dataset 1: 1,1
//	cv::imwrite("../images/dataset_distance_constraint_after.jpg",outimg);//Img 20,9 dataset 1: 1,1

	//Discussion
//	cv::imwrite("../images/dataset_similar_scene.jpg",outimg);//Img 10,2 dataset Street View: 2,1
//	cv::imwrite("../images/dataset_similar_scene_bad_matches.jpg",outimg);//Img 10,2 dataset Street View: 1,1
//	cv::imwrite("../images/dataset_similar_scene_bad_matches_smaller_angle.jpg",outimg);//Img 10,2 dataset Street View: 1,1
//	cv::imwrite("../images/dataset_valid_invalid_matches.jpg",outimg);//Img 10,2 dataset Street View: 1,1

	//My Question
//	cv::imwrite("../images/office_dataset_nonOverlapping.jpg",outimg);//Img 10,2 dataset Street View: 15,3
//	cv::imwrite("../images/mrd_dataset_overlapping.jpg",outimg);//Img 10,2 dataset Street View: 15,3
//	cv::imwrite("../images/mrd_dataset_nonOverlapping.jpg",outimg);//Img 10,2 dataset Street View: 15,3
#endif

	return 0;
}
