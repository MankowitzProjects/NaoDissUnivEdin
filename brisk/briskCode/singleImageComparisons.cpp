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
#define DEBUG_TIMES 0
#define DEBUG_MATCHES 1
#define OUTPUT 0
#define SINGLE_IMAGE_TEST 1
#define MULTIPLE_IMAGE_TEST 1
#define DISPLAY 0

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

	//The horizon line
	int horizonLine = 300;

	//Determine if the KNN validation criterion is necessary
	bool usingKnnCriterion = true;

	//To determine whether we are finding keypoint properties or KNN Ratio
	bool keypointProperties = true;
	//Set if you are matching or not matchingg
	bool isMatching = true;
	//Set the date and time
	string myDate = "11072012";
	string myTime = "2300";


	//Using the terminal
	bool terminal = false;

	//BRISK SURF2D
		bool hamming=false;
		std::string feat_detector = "BRISK";
		int threshold = 43.75;
		int hammingDistance = 40;//BRISK BRISK
		//double radius = 0.15;//BRISK SURF
		std::string feat_descriptor = "SURF";


	//Set the arguments
//	bool hamming=true;
//	std::string feat_detector = "BRISK";
//	int threshold = 55;
//	int hammingDistance = 40;//BRISK BRISK
//	//double radius = 0.15;//BRISK SURF
//	std::string feat_descriptor = "U-BRISK";

	//Create the Feature extraction object
	FeatureExtraction feature(angle, distance, usingKnnCriterion);

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

	//Declare the files
	string file,file1, file2, file3;

	if(terminal)
	{
		file.append("../");
		file1.append("../");
		file2.append("../");
		file3.append("../");
	}


	file = "../data/singleImage/singleImageData_keypoints_";
	file.append(feat_detector);
	file.append("_");
	file.append(feat_descriptor);
	file.append("_");
	if(usingKnnCriterion)
		file.append("KNN_");
	else
		file.append("Hamming_");
	file.append(myDate);
	file.append("_");
	file.append(myTime);


	file1 = "../data/singleImage/singleImageData_matches_";
	file1.append(feat_detector);
	file1.append("_");
	file1.append(feat_descriptor);
	file1.append("_");
	if(usingKnnCriterion)
		file1.append("KNN_");
	else
		file1.append("Hamming_");
	file1.append(myDate);
	file1.append("_");
	file1.append(myTime);


	file2 = "../data/singleImage/singleImageData_invalid_matches_KNN_";
	file2.append(feat_detector);
	file2.append("_");
	file2.append(feat_descriptor);
	file2.append("_");
	if(usingKnnCriterion)
		file2.append("KNN_");
	else
		file2.append("Hamming_");
	file2.append(myDate);
	file2.append("_");
	file2.append(myTime);


	file3 = "../data/singleImage/singleImageData_invalid_matches_AngleDistance_";
	file3.append(feat_detector);
	file3.append("_");
	file3.append(feat_descriptor);
	file3.append("_");
	if(usingKnnCriterion)
		file3.append("KNN_");
	else
		file3.append("Hamming_");
	file3.append(myDate);
	file3.append("_");
	file3.append(myTime);


	if(keypointProperties)
	{
		file.append("_keypointStats");
		file1.append("_keypointStats");
		file2.append("_keypointStats");
		file3.append("_keypointStats");

	}
	else{

		if(!isMatching)
		{
			file.append("_false");
			file1.append("_false");
			file2.append("_false");
			file3.append("_false");
		}
		else
		{
			file.append("_identical");
			file1.append("_identical");
			file2.append("_identical");
			file3.append("_identical");

		}
	}
	file.append(".txt");
	cout<<file<<endl;
	file1.append(".txt");
	cout<<file1<<endl;
	file2.append(".txt");
	cout<<file2<<endl;
	file3.append(".txt");
	cout<<file3<<endl;


	//*************************************

	//Matching parameters
	int k_start =0;
	int k_end = 0;
	int s_start = 0;
	int s_end = 0;

	int index = 0;

	if(isMatching)
	{
		k_start = 1;
		k_end = 4;
	}
	else
	{
		k_start = 1;
		k_end = 2;
		s_start = 3;
		s_end = 4;
	}


	for (int kk=k_start;kk<=k_end; kk++)
	{
		if(isMatching)
			s_start = s_end = kk;

		for (int ss=s_start;ss<=s_end;ss++)
		{

			std::string dir, dir1;
			dir = to_string<int>(kk);
			dir1 = to_string<int>(ss);

			std::string tempDir = to_string<int>(kk);
			std::string tempDir1 = to_string<int>(ss);

			//Names of the two image files
			std::string name1;
			std::string name2;

			//For changing the threshold
			int testThreshold = 10;

			//Set the directory names and determine the number of images in each directory
			int jpegCounter = dataAnalysis.getNumImagesInDirectory(&dir, terminal);
			int jpegCounter1 = dataAnalysis.getNumImagesInDirectory(&dir1, terminal);

			std::cout<<"The number of images in the directory is: "<<jpegCounter<<endl;


			// Declare the extractor. Only needs to be performed once since it computes lookup
			//tables for each of the various patterns on initialisation
			//*****************************************************************
			cv::Ptr<cv::DescriptorExtractor> descriptorExtractor;
			descriptorExtractor = feature.getExtractor(argc, feat_descriptor, hamming, descriptorExtractor);
			//*****************************************************************
			//Make sure that there are the same number of images in each frame
			if (isMatching)
			{
				jpegCounter=5;
//				if(jpegCounter>jpegCounter1)
//					jpegCounter = jpegCounter1;
//				else
//					jpegCounter1 = jpegCounter;
			}

			//Remember that for non-matches, we can compare 1,1;2,2;3,3...etc
			//Determine matches without repetition
			for (int ii = 1;ii<=jpegCounter;ii++)
			{
				if(isMatching)
					index = ii;
				else
					index = jpegCounter1;

				for (int jj = 1; jj<=index;jj++)//if int jj = index; jj<=index;jj++ then matching images to themselves
				{
					//Choose the images to compare
					name1 = to_string<int>(ii);
					name2 = to_string<int>(jj);

					cout<<"Image "<<ii<<", Image "<<jj<<endl;


					//For changing the threshold
					int testThreshold = 10;

					//cout<<"Image in directory 1: "<<name1<<", Image in directory 2: "<<name2<<endl;


					// names of the two images
					std::string fname1;
					std::string fname2;

					cv::Mat imgGray1Full;
					cv::Mat imgGray2Full;
					cv::Mat imgGray1;
					cv::Mat imgGray2;

					//Process the first image
					//****************************************************************
					while(imgGray1Full.empty()){
						fname1 = dir+"/"+name1+".jpg";
						imgGray1Full = cv::imread(fname1.c_str(), CV_LOAD_IMAGE_GRAYSCALE);

					}
					if (imgGray1Full.empty())
					{
						std::cout<<"image(s)"<<fname1<<" not found." << std::endl;
						return 2;
					}

					//Process only the image above the horizon line
					imgGray1 = imgGray1Full(cv::Rect(0, 0, imgGray1Full.cols, horizonLine));



					timespec ts, te, matchings, matchinge, detectors, detectore, extractors, extractore, verifys, verifye;
					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);

					// if no arguments are passed:
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

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &te);
					float imageProcessingTime = diff(ts,te).tv_nsec/1000000.0f;

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectors);
					//MC: Generate a vector of keypoints
					std::vector<cv::KeyPoint> keypoints, keypoints2;
					// create the detector:
					//*****************************************************************
					cv::Ptr<cv::FeatureDetector> detector;
					detector = feature.getDetector(argc, feat_detector, detector, threshold, testThreshold,1);
					//*****************************************************************
					detector->detect(imgGray2,keypoints2);

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectore);
					float detectionTime = diff(detectors,detectore).tv_nsec/1000000.0f;

					// run the detector:
					//*****************************************************************
					detector->detect(imgGray1,keypoints);

					//*****************************************************************

					//*****************************************************************


					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractors);
					// get the descriptors
					cv::Mat descriptors, descriptors2;
					std::vector<cv::DMatch> indices;


					// second image. Computes the descriptor for each of the keypoints.
					//Outputs a 64 bit vector describing the keypoints.
					//*****************************************************************
					descriptorExtractor->compute(imgGray2,keypoints2,descriptors2);
					//*****************************************************************
					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractore);
					float extractionTime = diff(extractors,extractore).tv_nsec/1000000.0f;

					//Compute the descriptors for the stored image
					//*****************************************************************
					descriptorExtractor->compute(imgGray1,keypoints,descriptors);
					//*****************************************************************

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
					float verifyTime = diff(verifys,verifye).tv_nsec/1000000.0f;

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchinge);
					float matchingTime = diff(matchings,matchinge).tv_nsec/1000000.0f;


					float overallTime = imageProcessingTime + detectionTime + extractionTime + matchingTime + verifyTime;


					//Write the data to a file
					ofstream writeFile;
					writeFile.open(file.c_str(), ios::app);

					//1. Write all the keypoints to the first image file

					//Swap hamming distance with radius
					for(int im1 = 0;im1<keypoints.size();im1++){
						//[image num, hammingdist, threshold, kp1x, kp1y, angle, size, octave]
						writeFile <<kk<<", "<<ss<<", "<<name1<<", "<<hammingDistance<<", "<<threshold<<", "<<im1<<", "<<keypoints[im1].pt.x<<", "<<keypoints[im1].pt.y<<", "<<keypoints[im1].angle<<", "<<keypoints[im1].size<<", "<<keypoints[im1].octave<<", "<<keypoints[im1].response<<"\n";
					}

					//2. Write all the keypoints to the second image file
					//Swap hamming distance with radius
					for(int im2 = 0;im2<keypoints2.size();im2++){
						//[image num, hammingdist, threshold, kp1x, kp1y, angle, size, octave]
						writeFile <<kk<<", "<<ss<<", "<<name2<<", "<<hammingDistance<<", "<<threshold<<", "<<im2<<", "<<keypoints2[im2].pt.x<<", "<<keypoints2[im2].pt.y<<", "<<keypoints2[im2].angle<<", "<<keypoints2[im2].size<<", "<<keypoints2[im2].octave<<", "<<keypoints2[im2].response<<"\n";
					}

					//Close the file
					writeFile.close();
					writeFile.clear();

					//Write the data to a file
					writeFile.open(file1.c_str(), ios::app);

					//Write all the matches data to a file
					//image num left, image num right, queryIdx, trainIdx,  keypoint1 x, kp1y, angle size, octave, response, kp2x, kp2y, angle, size, octave, neighbor num, distance
					//Swap hamming distance with radius
					for(size_t mIdx = 0;mIdx<matches.size();mIdx++){

						for(size_t nIdx = 0;nIdx<matches[mIdx].size();nIdx++)
						{
							int q = matches[mIdx][nIdx].queryIdx;
							int t = matches[mIdx][nIdx].trainIdx;

#if (DEBUG_MODE)
							cout<<"neighbor index: "<<nIdx<<endl;
							cout<<"Matches size: "<<matches[mIdx].size()<<endl;
							cout<<"Keypoint Left to be erased row,col : "<<keypoints[q].pt.y<<", "<<keypoints[q].pt.x<<endl;
							cout<<"Keypoint Right to be erased row,col : "<<keypoints2[t].pt.y<<", "<<keypoints2[t].pt.x<<endl;
							cout<<"The match distance is: "<<matches[mIdx][nIdx].distance<<endl;
#endif
							writeFile <<kk<<", "<<ss<<", "<<name1<<", "<<name2<<", "<<q<<", "<<t<<", "<<keypoints[q].pt.x<<", "<<keypoints[q].pt.y<<", "<<keypoints[q].angle<<", "<<keypoints[q].size<<", "<<keypoints[q].octave<<", "<<keypoints[q].response<<", "<<keypoints2[t].pt.x<<", "<<keypoints2[t].pt.y<<", "<<keypoints2[t].angle<<", "<<keypoints2[t].size<<", "<<keypoints2[t].octave<<", "<<keypoints2[t].response<<", "<<nIdx<<", "<<matches[mIdx][nIdx].distance<<"\n";
						}
					}
					//Close the file
					writeFile.close();
					writeFile.clear();

					//Write the invalid KNN matches data to a file
					writeFile.open(file2.c_str(), ios::app);

					for (size_t it = 0; it<feature.leftPointsKNN.size(); it++)
					{
						//						cout<<"it index: "<<it<<endl;
						//						cout<<"Matches size: "<<feature.leftPointsKNN.size()<<endl;
						//						cout<<"Keypoint Left to be erased row,col : "<<feature.leftPointsKNN[it].pt.y<<", "<<feature.leftPointsKNN[it].pt.x<<endl;
						//						cout<<"Keypoint Right to be erased row,col : "<<feature.rightPointsKNN[it].pt.y<<", "<<feature.rightPointsKNN[it].pt.x<<endl;
						//						cout<<"The match distance is: "<<feature.keypointDistanceKNN[it]<<endl;

						writeFile <<kk<<", "<<ss<<", "<<name1<<", "<<name2<<", "<<feature.leftPointsKNN[it].pt.x<<", "<<feature.leftPointsKNN[it].pt.y<<", "<<feature.leftPointsKNN[it].angle<<", "<<feature.leftPointsKNN[it].size<<", "<<feature.leftPointsKNN[it].octave<<", "<<feature.leftPointsKNN[it].response<<", "<<feature.rightPointsKNN[it].pt.x<<", "<<feature.rightPointsKNN[it].pt.y<<", "<<feature.rightPointsKNN[it].angle<<", "<<feature.rightPointsKNN[it].size<<", "<<feature.rightPointsKNN[it].octave<<", "<<feature.rightPointsKNN[it].response<<", "<<feature.neighborIndexKNN[it]<<", "<<feature.keypointDistanceKNN[it]<<"\n";
					}

					//Close the file
					writeFile.close();
					writeFile.clear();

					//Write the invalid angle and distance matches data to a file
					writeFile.open(file3.c_str(), ios::app);

					for (int it = 0; it<feature.leftPointsAngle.size(); it++)
					{
						writeFile <<kk<<", "<<ss<<", "<<name1<<", "<<name2<<", "<<feature.leftPointsAngle[it].pt.x<<", "<<feature.leftPointsAngle[it].pt.y<<", "<<feature.leftPointsAngle[it].angle<<", "<<feature.leftPointsAngle[it].size<<", "<<feature.leftPointsAngle[it].octave<<", "<<feature.leftPointsAngle[it].response<<", "<<feature.rightPointsAngle[it].pt.x<<", "<<feature.rightPointsAngle[it].pt.y<<", "<<feature.rightPointsAngle[it].angle<<", "<<feature.rightPointsAngle[it].size<<", "<<feature.rightPointsAngle[it].octave<<", "<<feature.rightPointsAngle[it].response<<", "<<feature.keypointDistanceAngle[it]<<"\n";
					}

					//Close the file
					writeFile.close();
					writeFile.clear();
#if  (DEBUG_MODE)
					cout<<"****************************************"<<endl;
					cout<<"The matching score for the image (condsidering all matches) is "<<feature.imageMatchingScore<<endl;
					cout<<"The matching score for the image (condsidering best match only) is "<<feature.imageMatchingScoreBest<<endl;
					cout<<"The total number of matches is "<<feature.totalNumMatches<<endl;
					cout<<"The total number of valid matches is "<<feature.totalNumValidMatches<<endl;
					cout<<"The total number of invalid matches is "<<feature.totalNumInvalidMatches<<endl;
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
				}//end of jj loop
			}//end of ii loop
		}//End of ss loop
	}//End of the kk loop

	return 0;
}
