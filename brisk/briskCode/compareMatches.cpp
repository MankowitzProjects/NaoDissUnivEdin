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
#include <ctime>
#include "config_file.h"



#define DEBUG_MODE 0
#define DEBUG_TIMES 0
#define SINGLE_IMAGE_TEST 1
#define MULTIPLE_IMAGE_TEST 1
#define DISPLAY 0

enum lighting{
	LIGHT_LEFT,
	LIGHT_RIGHT,
	LIGHT_BOTH
};

template <class T>
inline std::string to_string (const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

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
	//The angle used for matching validation
	double angle = 10;
	//The distance threshold
	double distance = 200;

	//Determine if the KNN validation criterion is necessary
	bool usingKnnCriterion = false;

	//The horizon line
	int horizonLine = 300;

	//Using the terminal
	bool terminal = false;


	//For BRISK SURF Using radius = 0.20, threshold = 70
	//		bool hamming=false;
	//		std::string feat_detector = "BRISK";
	//		std::string feat_descriptor = "SURF";
	//		double hammingDistance = 0.28;
	//		//double threshold = 30;//45
	//		double threshold = 60;

	//For SBRISK SBRISK, hammingDistance = 85, Threshold = 100
	bool hamming=true;
	std::string feat_detector = "BRISK";
	std::string feat_descriptor = "U-BRISK";
	double threshold = 75;//46.25;//46.25 KNN
	//	double threshold = 78.75;//Hamming
	double hammingDistance = 121.25;//Hamming

	//For BRISK4 (4 octaves)
	//			bool hamming=true;
	//			std::string feat_detector = "BRISK4";
	//			std::string feat_descriptor = "BRISK4";
	//			//double threshold = 30; //KNN 51.25
	//			double threshold = 65;//Hamming
	//			double hammingDistance = 130;//Hamming

	//Set the date and time
	string myDate = "25072012";
	string myTime = "2232";

	//Set if you are matching or not matching
	bool isMatching = false;
	//Determine whether the MPS or CPS threshold is being used
	bool isMax  =  true;
	//Are we comparing camera pics to the Nao pics
	bool usingCamera = false;

	//Lighting parameter
	lighting light = LIGHT_RIGHT;

	//NB *************************************
	//CHOOSE THE DATASET TO USE
	int dataset=4;
	//dataset =1 for original robocup pics
	//dataset = 2 for office pics and large hall pics
	//dataset = 3 for varying illumination and for comparing the robocup dataset with the camera robocup dataset
	//dataset = 4 Nao Street View 27-28 (directory indices) with Google Street view
	// AND second dataset 19-20 with camera pic indices 25-26


	//Create the non-matching upper and lower bounds
	int upperBound = 0;
	int lowerBound = 0;

	//Matching parameters
	int k_start =0;
	int k_end = 0;
	int s_start = 0;
	int s_end = 0;

	int index = 0;

	//Counters used to ensure standard output for processing in Matlab
	int tempDirCounterkk = 0;
	int tempDirCounterss = 0;

	//USED FOR LIGHTING AND FOR COMPARING NAO CAMERA PICS
	int step = 0;

	//If we are performing the matching routine
	if(isMatching)
	{

		if (dataset==1)//The original dataset
		{
			//Counters used to ensure standard output for processing in Matlab
			tempDirCounterkk = 1;
			tempDirCounterss = 1;

			k_start = 13;//Left dataset: 5; Right Dataset: 9; Both Dataset: 13
			k_end = 16;//Left dataset: 8; Right Dataset 12; Both Dataset: 16
		}
		else if (dataset==2)//Additional datasets
		{
			//Counters used to ensure standard output for processing in Matlab
			tempDirCounterkk = 5;
			tempDirCounterss = 5;

			k_start = 19; //Dataset 2: 17; Dataset 3: 19
			k_end = 20; //Dataset 2: 18; Dataset 3: 20
		}
		else if (dataset==3) //Varying lighting
		{
			//Counters used to ensure standard output for processing in Matlab
			tempDirCounterkk = 1;
			tempDirCounterss = 1;

			k_start = 1;//Left dataset: 5; Right Dataset: 9; Both Dataset: 13
			k_end = 4;//Left dataset: 6; Right Dataset: 10; Both Dataset: 14

			//Determines which dataset to use. Either lighting datasets or camera datasets
			if(!usingCamera)
			{
				if(light == LIGHT_LEFT)
					step = 4;
				else if(light == LIGHT_RIGHT)
					step = 8;
				else if (light ==LIGHT_BOTH)
					step=12;
			}
			else //Camera dataset
				step=20;

			//Camera: 20 for camera pics
		}
		else if (dataset==4) //Google Street View
		{
			//Street view 27,28,29,30
			//In matching, we want to match 27-29 and 28-30
			//Used for matching the correct images
			step = 2;
			//Counters used to ensure standard output for processing in Matlab
			tempDirCounterkk = 1;
			tempDirCounterss = 1;

			if(!usingCamera)
			{
				k_start = 27;
				k_end = 28;
			}
			else
			{
				k_start = 25;
				k_end = 26;
			}
		}

	}
	else
	{
		if (dataset==1)
		{
			//Counters used to ensure standard output for processing in Matlab
			tempDirCounterkk = 1;
			tempDirCounterss = 3;

			k_start = 13;//Left dataset: 5; Right Dataset: 9; Both Dataset: 13
			k_end = 14;//Left dataset: 6; Right Dataset: 10; Both Dataset: 14
			s_start = 15;//Left dataset: 7; Right Dataset: 11; Both Dataset: 15
			s_end = 16;//Left dataset: 8; Right Dataset: 12; Both Dataset: 16
		}
		else if (dataset==2)
		{
			//Counters used to ensure standard output for processing in Matlab
			tempDirCounterkk = 5;
			tempDirCounterss = 6;

			k_start = 19; //Dataset 2: 17; Dataset 3: 19
			k_end = 19; //Dataset 2: 17; Dataset 3: 19
			s_start = 20; //Dataset 2: 18; Dataset 3: 20
			s_end = 20; //Dataset 2: 18; Dataset 3: 20
		}
		else if(dataset==3) //Varying lighting or comparing the Nao robocup pics with the Nikon Camera pics
		{
			//s_start; s_end
			//Left Light off dataset: 5; Left Light off Dataste 8;
			//Right Light off Dataset: 9; Right Light off Dataset: 12;
			//Both Light off Dataset: 13; Both Light off Dataset: 16
			//Camera Pics Dataset: 21; Camera Pics Dataset: 24

			//Counters used to ensure standard output for processing in Matlab
			//---------------------------------------------------------------
			tempDirCounterkk = 1;
			tempDirCounterss = 3;
			//Set the datasets that you want to match
			k_start = 1;
			k_end = 4;
			if(!usingCamera){
				if(light == LIGHT_LEFT){
					lowerBound  = 5;
					upperBound  = 8;
				}
				else if(light == LIGHT_RIGHT){
					lowerBound  = 9;
					upperBound  = 12;
				}
				else if (light ==LIGHT_BOTH){
					lowerBound  = 13;
					upperBound  = 16;
				}
			}
			else{
				lowerBound  = 21;
				upperBound  = 24;
			}


			s_start = lowerBound;
			s_end = upperBound;
			//REMEMBER: CHANGE s_start, s_end IN THE KK LOOP AS WELL
			//---------------------------------------------------------------

		}
		else if(dataset==4)
		{
			//Run this twice
			//1. First do 27 and 30
			//2. Second do 28 and 29

			//Counters used to ensure standard output for processing in Matlab
			tempDirCounterkk = 1;
			tempDirCounterss = 3;

			if(!usingCamera)
			{
				lowerBound = 29;
				upperBound = 30;
				s_start = 31;
				s_end = 32;
			}
			else
			{
				lowerBound = 25;
				upperBound = 26;
				s_start = 27;
				s_end = 28;
			}
			k_start = lowerBound;
			k_end = upperBound;
		}
	}

	//Begin the matching procedure
	for(int kk=k_start;kk<=k_end;kk++)
	{
		if(dataset==1){
			//The counter must be reset for formatting purposes in Matlab
			tempDirCounterss = 3;//kk for matching; another number for non-matching
			if(isMatching)
				s_start = s_end = kk;
		}
		else if(dataset==2)
		{
			if(isMatching)
				s_start = s_end = kk;
		}
		else if(dataset==3){ //This is for the varying illumination datasets

			if(isMatching)
			{
				//Comparing datasets
				s_start = s_end = kk+step;
				//The counter must be reset for formatting purposes in Matlab
				tempDirCounterss = kk;
			}
			else
			{
				//This ensures that no datasets are repeated
				if (kk>=3){
					tempDirCounterss = 5;
					s_start = lowerBound;
					s_end = lowerBound+1;
				}
				else{
					tempDirCounterss = 3;
					s_start = upperBound - 1;
					s_end = upperBound;
				}
			}
		}
		else if (dataset==4)//This is for google street view
		{
			if(isMatching)
				s_start = s_end = kk+ step;
			else{
				if (kk==lowerBound)
					s_start = s_end = lowerBound + 3;
				else if (kk==upperBound)
					s_start = s_end = upperBound+1;
			}
		}

		for (int ss = s_start;ss<=s_end;ss++)
		{

			//Create object for dataAnalysis
			DataAnalysis dataAnalysis;

			//Create the Feature extraction object
			FeatureExtraction feature(angle, distance, usingKnnCriterion);

			//The directory where the files are stored

			std::string dir, dir1;
			dir = to_string<int>(kk);
			dir1 = to_string<int>(ss);

			std::string tempDir = to_string<int>(tempDirCounterkk);
			std::string tempDir1 =to_string<int>(tempDirCounterss);

			//Names of the two image files
			std::string name1;
			std::string name2;

			//For changing the threshold
			double testThreshold = 0;

			//Set the directory names and determine the number of images in each directory
			int jpegCounter = dataAnalysis.getNumImagesInDirectory(&dir, terminal);
			int jpegCounter1 = dataAnalysis.getNumImagesInDirectory(&dir1, terminal);

			std::cout<<"The number of images in the directory is: "<<jpegCounter<<endl;

			//Set the flag for1 setting up the BRISK lookup table for the extractor


			cv::Ptr<cv::DescriptorExtractor> descriptorExtractor;
			descriptorExtractor = feature.getExtractor(argc, feat_descriptor, hamming, descriptorExtractor);
			//*************************************
			//Get the current time for saving purposes
			//*************************************
			time_t rawtime;
			struct tm * timeinfo;
			//char filename [80];

			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			//strftime (filename,80,"../../data/Matches/matchingData_%b_%d_%H%M%S.txt",timeinfo);
			//strftime (filename,80,"../data/Matches/nonmatching_matching_Data__BRISK__BRISK_Hamming_070421012_1222.txt",timeinfo);
			//puts (filename);
			string filename = "../";
			if (dataset==1)
			{
				filename.append("data/Matches/");
				filename.append("nonmatching_matching_Data__");

			}else if (dataset==2)
			{
				filename.append("data2/Matches/");//Or data3
				filename.append("dataset2_nonmatching_matching_Data__");//Or dataset3
			}
			else if (dataset==3 && !usingCamera)
			{
				filename.append("dataLighting/Matches/");
				if(light==LIGHT_LEFT)
					filename.append("dataLighting_left_light_off_nonmatching_matching_Data__");
				else if(light==LIGHT_RIGHT)
					filename.append("dataLighting_right_light_off_nonmatching_matching_Data__");
				else if (light==LIGHT_BOTH)
					filename.append("dataLighting_both_lights_off_nonmatching_matching_Data__");
			}
			else if (dataset==3 && usingCamera){
				filename.append("dataCamera/Matches/");
				filename.append("camera_nonmatching_matching_Data__");
			}
			else if (dataset==4){

				if(usingCamera){
					filename.append("dataCamera/Matches/");
					filename.append("camera_large_hall_nonmatching_matching_Data__");
				}
				else{
					filename.append("dataStreetView/Matches/");
					filename.append("streetview_nonmatching_matching_Data__");
				}
			}
			filename.append(feat_detector);
			filename.append("_");
			filename.append(feat_descriptor);
			filename.append("_");
			if(usingKnnCriterion)
				filename.append("KNN_");
			else
				filename.append("Hamming_");
			filename.append(myDate);
			filename.append("_");
			filename.append(myTime);
			filename.append("_");
			filename.append(to_string<double>(threshold));
			filename.append("_");
			if (!usingKnnCriterion)
				filename.append(to_string<int>(hammingDistance));
			if (isMax)
				filename.append("_max");
			else
				filename.append("_consistent");
			filename.append(".txt");

			cout<<filename<<endl;
			//*************************************
			//Make sure that there are the same number of images in each frame (Not for non matches)
			if (isMatching)
			{
				if(jpegCounter>jpegCounter1)
					jpegCounter = jpegCounter1;
				else
					jpegCounter1 = jpegCounter;
			}
			//Remember that for non-matches, we can compare 1,1;2,2;3,3...etc
			//Determine matches without repetition
			for (int ii = 1;ii<=jpegCounter;ii++)
			{
				if(isMatching)
					index = ii;
				else
					index = jpegCounter1;

				for (int jj = 1; jj<index;jj++)//jpegCounter1
				{
					//Choose the images to compare
					name1 = to_string<int>(ii);
					name2 = to_string<int>(jj);

					cout<<"Image "<<ii<<", Image "<<jj<<endl;


					// names of the two images
					std::string fname1;
					std::string fname2;

					cv::Mat imgGray1Full;
					cv::Mat imgGray2Full;
					cv::Mat imgGray1;
					cv::Mat imgGray2;

					//Process the first image which would normally be stored on the robot
					//****************************************************
					while(imgGray1Full.empty()){
						fname1 = dir+"/"+name1+".jpg";
						imgGray1Full = cv::imread(fname1.c_str(), CV_LOAD_IMAGE_GRAYSCALE);

					}
					if (imgGray1Full.empty())
					{
						std::cout<<"image(s)"<<fname1<<" not found." << std::endl;
						return 2;
					}

					//Only take the image from the horizon line
					imgGray1 = imgGray1Full(cv::Rect(0, 0, imgGray1Full.cols, horizonLine));
					//****************************************************

					timespec ts, te, matchings, matchinge, detectors, detectore, extractors, extractore, verifys, verifye;
					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);

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

					//We only need the keypoints above the horizon
					imgGray2 = imgGray2Full(cv::Rect(0, 0, imgGray2Full.cols, horizonLine));
					//*****************************************************************

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &te);
					float imageProcessingTime = diff(ts,te).tv_nsec/1000000.0f;


					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectors);

					//MC: Generate a vector of keypoints
					std::vector<cv::KeyPoint> keypoints, keypoints2;
					// create the detector:
					//*****************************************************************
					cv::Ptr<cv::FeatureDetector> detector;
					//Note: Testflag==1 which means that the threshold will equal testThreshold
					detector = feature.getDetector(argc, feat_detector, detector, threshold, testThreshold,1);
					//*****************************************************************
					detector->detect(imgGray2,keypoints2);

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectore);
					float detectionTime = diff(detectors,detectore).tv_nsec/1000000.0f;

					//The descriptor stored on the robot
					//*****************************************************************
					detector->detect(imgGray1,keypoints);
					//*****************************************************************

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractors);
					// now the extractor:
					//*****************************************************************
					// get the descriptors
					cv::Mat descriptors, descriptors2;
					std::vector<cv::DMatch> indices;
					// first image. Computes the descrkeypoints2iptor for each of the keypoints.
					//Outputs a 64 bit vector describing the keypoints.
					descriptorExtractor->compute(imgGray2,keypoints2,descriptors2);

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractore);
					float extractionTime = diff(extractors,extractore).tv_nsec/1000000.0f;

					//The descriptors stored on the robot
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
					if(hamming){
						if(usingKnnCriterion)
							descriptorMatcher->knnMatch(descriptors,descriptors2,matches,2);
						else
							descriptorMatcher->radiusMatch(descriptors,descriptors2,matches,hammingDistance);
					}
					else{
						if(usingKnnCriterion)
						{
							if(descriptors2.rows>0)
								descriptorMatcher->knnMatch(descriptors,descriptors2,matches,2);
							else
								matches.clear();
						}
						else{
							//Decreasing with the maxdistance value will drastically reduce the number of matches
							if(descriptors2.rows>0)
								descriptorMatcher->radiusMatch(descriptors,descriptors2,matches,hammingDistance);
							else
								matches.clear();
						}
					}

					cv::Mat outimg;

					//Write the data to a file
					ofstream writeFile;

					//Create the filename with the current time
					writeFile.open(filename.c_str(), ios::app);//ios::app

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &verifys);

					//Perform the matching verification
					//*************************************************************************
					//					cout<<"The total number of keypoints in image 1 is: "<<keypoints.size()<<endl;
					//					cout<<"The total number of keypoints in image 2 is: "<<keypoints2.size()<<endl;
					//					cout<<"The total number of matches is: "<<matches.size()<<endl;
					feature.performMatchingValidation(imgGray1,keypoints, keypoints2, matches, hamming);

					//*************************************************************************
					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &verifye);
					float verifyTime = diff(verifys,verifye).tv_nsec/1000000.0f;

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchinge);
					float matchingTime = diff(matchings,matchinge).tv_nsec/1000000.0f;

					float overallTime = imageProcessingTime + detectionTime + extractionTime + matchingTime + verifyTime;


#if (DEBUG_MODE)
					cout<<"****************************************"<<endl;
					cout<<"The matching score for the image (condsidering all matches) is "<<feature.imageMatchingScore<<endl;
					cout<<"The matching score for the image (condsidering best match only) is "<<feature.imageMatchingScoreBest<<endl;
					cout<<"The total number of matches is "<<feature.totalNumMatches<<endl;
					cout<<"****************************************"<<endl;
#endif
#if (DEBUG_TIMES)
					std::cout<<"The times:"<<endl;
					std::cout<<"Detection Time: "<<detectionTime<<" ms"<<endl;
					std::cout<<"Extraction Time: "<<extractionTime<<" ms"<<endl;
					std::cout<<"Matching Time: "<<matchingTime<<" ms"<<endl;
					std::cout<<"Verify Matches Time: "<<verifyTime<<" ms"<<endl;
					std::cout<<"Overall Time: "<<overallTime<<" ms"<<endl;
					cv::waitKey(1000);
#endif


					//threshold = atoi(argv[3]+5);
					//Write all the information to a file
					writeFile <<tempDirCounterkk<<", "<<tempDirCounterss<<", "<<threshold<<", "<<name1<<", "<<name2<<", "<<keypoints.size()<<", "<<keypoints2.size()<<", "<<feature.imageMatchingScoreBest<<", "<<feature.imageMatchingScore<<","<<feature.totalNumMatches<<", "<<feature.totalNumValidMatches<<", "<<feature.totalNumInvalidMatches<<", "<<feature.totalNumBestMatches<<", "<<detectionTime<<", "<<extractionTime<<", "<<matchingTime<<", "<<verifyTime<<", "<<overallTime<<"\n";
					//close the file
					writeFile.close();

#if (DISPLAY)
					drawMatches(imgGray1, keypoints, imgGray2, keypoints2,matches,outimg,
							cv::Scalar(0,255,0), cv::Scalar(0,0,255),
							std::vector<std::vector<char> >(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

					cv::namedWindow("Matches");
					cv::imshow("Matches", outimg);
					//cv::imshow("keypoints", imgRGB1);
					//cv::imshow("keypoints2", imgRGB2);
					cv::waitKey();
#endif

				}//End of inner for loop (jj)
			}//end of outer for loop (ii)
			tempDirCounterss++;
		}//End of ss loop
		tempDirCounterkk++;
	}//end of kk loop

	return 0;
}
