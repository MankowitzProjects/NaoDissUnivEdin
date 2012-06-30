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
#define SINGLE_IMAGE_TEST 1
#define MULTIPLE_IMAGE_TEST 0
#define DISPLAY 0

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

	//For BRISK SURF Using radius = 0.20, threshold = 70
	//For BRISK BRISK, hammingDistance = 85, Threshold = 100
	bool hamming=true;
	std::string feat_detector = "BRISK";
	int threshold = 20;
	int hammingDistance = 20;//BRISK BRISK
	double radius = 0.20;//BRISK SURF
	std::string feat_descriptor = "BRISK";

	for (int ss=1;ss<=4; ss++)
	{
		//Create object for dataAnalysis
		DataAnalysis dataAnalysis;

		//Create the Feature extraction object
		FeatureExtraction feature;

		// Declare the extractor. Only needs to be performed once.
		//*****************************************************************
		cv::Ptr<cv::DescriptorExtractor> descriptorExtractor;
		descriptorExtractor = feature.getExtractor(argc, feat_descriptor, hamming, descriptorExtractor);
		//*****************************************************************

		//The directory where the files are stored
		std::string dir;
		dir = to_string<int>(ss);
		std::string tempDir = to_string<int>(ss);
		//Names of the two image files
		// names of the two images
		std::string fname1;
		std::string fname2;

		std::string name1;
		std::string name2;

		//Initialise the test threshold
		int testThreshold = 0;


		//Set the radius
		std::string stringRad;

		int jpegCounter = dataAnalysis.getNumImagesInDirectory(&dir);

		std::cout<<"The number of images in the directory is: "<<jpegCounter<<endl;

		//*************************************
		//Get the current time for saving purposes and set the file to save to
		//*************************************
		time_t rawtime;
		struct tm * timeinfo;
		char filename [80];
		string testThresholdString = to_string<int>(testThreshold);
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		string file = "../data/Thresholds/matchingData_threshold_26062012_BRISK_BRISK_KNN";
		//		file.append(testThresholdString.c_str());
		//		file.append("Directory_");
		//		file.append(tempDir.c_str());
		//		file.append("_");
		//		file.append(stringRad.c_str());
		file.append(".txt");
		cout<<file<<endl;
		//*************************************

//		for (int rr=1;rr<=20;rr++)//rr<20
//		{
			cout<<"Hamming Distance: "<<hammingDistance<<endl;
			//set the threshold
			testThreshold = 20;

			stringRad = to_string<double>(hammingDistance);

			//This will compute the statistics for every combination in a directory for each threshold
			for (int tt = 1; tt<=20;tt++)//20
			{
				cout<<"Threshold is: "<<testThreshold<<endl;

				for (int ii = 1; ii<jpegCounter;ii++)//jpegCounter-1
				{
					//Setting jj<ii will ensure that all combinations are computed
					for (int jj = 1; jj<ii;jj++)//jj<ii
					{

						//Choose the images to compare

						name1 = to_string<int>(ii);
						//    if(ii==jj)
						//    continue;
						name2 = to_string<int>(jj);
						//cout<<"Image 1: "<<name1<<", Image 2: "<<name2<<endl;

						//Start the timer
						timespec ts, te, matchings, matchinge, detectors, detectore, extractors, extractore,verifys,verifye;
						clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);


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
							fname2 = dir+"/"+name2+".jpg";
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

						//MC: Generate a vector of keypoints
						std::vector<cv::KeyPoint> keypoints, keypoints2;
						int threshold;

						clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectors);
						// create the detector:
						//*****************************************************************
						cv::Ptr<cv::FeatureDetector> detector;
						detector = feature.getDetector(argc, feat_detector, detector, threshold, testThreshold,2);
						//*****************************************************************

						// run the detector:
						//*****************************1************************************
						detector->detect(imgGray1,keypoints);
						detector->detect(imgGray2,keypoints2);
						//*****************************************************************
						clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectore);
						double detectionTime = diff(detectors,detectore).tv_nsec/1000000;

						clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractors);

						//*****************************************************************

						// get the descriptors
						cv::Mat descriptors, descriptors2;
						std::vector<cv::DMatch> indices;
						// first image. Computes the descriptor for each of the keypoints.
						//Outputs a 64 bit vector describing the keypoints.
						//*****************************************************************
						descriptorExtractor->compute(imgGray2,keypoints2,descriptors2);
						// and the second one
						descriptorExtractor->compute(imgGray1,keypoints,descriptors);
						//*****************************************************************
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
							descriptorMatcher->knnMatch(descriptors,descriptors2,matches,2);
						//descriptorMatcher->radiusMatch(descriptors2,descriptors,matches,hammingDistance);
						else{
							//Messing with the maxdistance value will drastically reduce the number of matches
							descriptorMatcher->radiusMatch(descriptors,descriptors2,matches,radius);
							//descriptorMatcher->knnMatch(descriptors2,descriptors,matches,3);
						}
						//For the above method, we could use KnnMatch. All values less than radius max distance are selected
						//*****************************************************************
						cv::Mat outimg;

						//Write the data to a file
						ofstream writeFile;

						//std::string filename = "../../data/Thresholds/matchingData"+name2+".txt";
						//writeFile.open(filename.c_str(), ios::app);//ios::app
						writeFile.open(file.c_str(), ios::app);//ios::app
						// drawing
						clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &verifys);

						//Perform the matching verification
						//*************************************************************************
						//						cout<<"The total number of keypoints in image 1 is: "<<keypoints.size()<<endl;
						//						cout<<"The total number of keypoints in image 2 is: "<<keypoints2.size()<<endl;
						//						cout<<"The total number of matches is: "<<matches.size()<<endl;

						feature.performMatchingValidation(imgGray1,keypoints, keypoints2, matches, hamming);

						//*************************************************************************
						clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &verifye);
						double verifyTime = diff(verifys,verifye).tv_nsec/1000;

						clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchinge);
						double matchingTime = diff(matchings,matchinge).tv_nsec/1000;

						clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &te);
						double overallTime = diff(ts,te).tv_nsec/1000000;


#if (DEBUG_MODE)
						std::cout<<"The times:"<<endl;
						std::cout<<"Detection Time: "<<detectionTime<<" ms"<<endl;
						std::cout<<"Extraction Time: "<<extractionTime<<" ms"<<endl;
						std::cout<<"Matching Time: "<<matchingTime<<" us"<<endl;
						std::cout<<"Overall Time: "<<overallTime<<" ms"<<endl;
#endif


#if (DEBUG_MODE)
						cout<<"****************************************"<<endl;
						cout<<"The matching score for the image (condsidering all matches) is "<<feature.imageMatchingScore<<endl;
						cout<<"The matching score for the image (condsidering best match only) is "<<feature.imageMatchingScoreBest<<endl;
						cout<<"The total number of matches is "<<feature.totalNumMatches<<endl;
						cout<<"The total number of valid matches is "<<feature.totalNumValidMatches<<endl;
						cout<<"****************************************"<<endl;
#endif

						//Write all the information to a file
						//Swap hamming distance with radius
						writeFile <<tempDir<<", "<<hammingDistance<<", "<<testThreshold<<", "<<name1<<", "<<name2<<", "<<keypoints.size()<<", "<<keypoints2.size()<<", "<<feature.imageMatchingScoreBest<<", "<<feature.imageMatchingScore<<","<<feature.totalNumMatches<<", "<<feature.totalNumValidMatches<<", "<<feature.totalNumBestMatches<<", "<<detectionTime<<", "<<extractionTime<<", "<<matchingTime<<", "<<verifyTime<<", "<<overallTime<<"\n";

						//close the file
						writeFile.close();

#if (DISPLAY)
						//Draw the matches and keypoints
						drawMatches(imgGray1, keypoints, imgGray2, keypoints2,matches,outimg,
								cv::Scalar(0,255,0), cv::Scalar(0,0,255),
								std::vector<std::vector<char> >(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
						int colourChanger=0;

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
						//cv::imshow("keypoints", imgRGB1);
						//cv::imshow("keypoints2", imgRGB2);
						//cv::waitKey();
#endif


					}//End of inner for loop (jj)

				}//End of outer loop (ii)
				//Change the test threshold
				testThreshold = testThreshold + 5;

			}//End of the tt threshold

			//Increment the radius
			//radius = radius+ 0.01;//For BRISK_SURF
			hammingDistance = hammingDistance + 5;//For BRISK BRISK
		//}//End of rr loop for radius

	}//End of main ss loop
	return 0;
}
