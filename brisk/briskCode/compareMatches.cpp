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



#define DEBUG_MODE 0
#define DEBUG_TIMES 0
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
	bool hamming=false;
	std::string feat_detector = "BRISK";
	std::string feat_descriptor = "SURF";
	double hammingDistance = 0.14;
	//int threshold = 45;
	int threshold = 33.75;

	//For SBRISK SBRISK, hammingDistance = 85, Threshold = 100
//	bool hamming=true;
//	std::string feat_detector = "BRISK";
//	std::string feat_descriptor = "BRISK";
//	int threshold = 30;//46.25;//46.25 KNN
//	int threshold = 95;//Hamming
//	int hammingDistance = 93.75;//Hamming

	//For BRISK4 (4 octaves)
//	bool hamming=true;
//	std::string feat_detector = "BRISK";
//	std::string feat_descriptor = "BRISK";
//	//int threshold = 51.25; //KNN
//	int threshold = 88.75;//Hamming
//	int hammingDistance = 116.25;//Hamming

	//For 1D SURF



	//For 2D SURF



	for(int kk=1;kk<=2;kk++)
	{

		for (int ss = 3;ss<=4;ss++)
		{

			//Create object for dataAnalysis
			DataAnalysis dataAnalysis;

			//Create the Feature extraction object
			FeatureExtraction feature;

			//The directory where the files are stored

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
			int jpegCounter = dataAnalysis.getNumImagesInDirectory(&dir);
			int jpegCounter1 = dataAnalysis.getNumImagesInDirectory(&dir1);

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
			string filename = "../data/Matches/nonmatching_matching_Data__SBRISK__SURF2D_Hamming_070521012_0250_33";
			//		file.append(testThresholdString.c_str());
			//		file.append("Directory_");
			//		file.append(tempDir.c_str());
			//		file.append("_");
			//		file.append(stringRad.c_str());
			filename.append(".txt");
			cout<<filename<<endl;
			//*************************************
			//Make sure that there are the same number of images in each frame
			if(jpegCounter>jpegCounter1)
				jpegCounter = jpegCounter1;
			else
				jpegCounter1 = jpegCounter;

			//Remember that for non-matches, we can compare 1,1;2,2;3,3...etc
			//Determine matches without repetition
			for (int ii = 1;ii<=jpegCounter;ii++)
			{

				for (int jj = 1; jj<ii;jj++)
				{
					//Choose the images to compare
					name1 = to_string<int>(ii);
					//if(ii==jj)
					//	continue;images

					name2 = to_string<int>(jj);

					cout<<"Image "<<ii<<", Image "<<jj<<endl;


					// names of the two images
					std::string fname1;
					std::string fname2;
//					cv::Mat imgRGB1;
//					cv::Mat imgRGB2;
//					cv::Mat imgRGB3;

					cv::Mat imgGray1Full;
					cv::Mat imgGray2Full;
					cv::Mat imgGray1;
					cv::Mat imgGray2;
					bool do_rot=false;
					// standard file extensions
					std::vector<std::string> fextensions;
					fextensions.push_back(".bmp");
					fextensions.push_back(".jpeg");
					fextensions.push_back(".jpg");
					fextensions.push_back(".jpe");
					fextensions.push_back(".jp2");
					fextensions.push_back(".png");
					fextensions.push_back(".pgm");
					fextensions.push_back(".ppm");
					fextensions.push_back(".sr");
					fextensions.push_back(".ras");
					fextensions.push_back(".tiff");
					fextensions.push_back(".tif");

					//
					//clock_t start = clock();
					timespec ts, te, matchings, matchinge, detectors, detectore, extractors, extractore, verifys, verifye;
					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);

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

					//MC: Generate a vector of keypoints
					std::vector<cv::KeyPoint> keypoints, keypoints2;
					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectors);

					// create the detector:
					//*****************************************************************
					cv::Ptr<cv::FeatureDetector> detector;
					//Note: Testflag==1 which means that the threshold will equal testThreshold
					detector = feature.getDetector(argc, feat_detector, detector, threshold, testThreshold,1);
					//*****************************************************************


					// run the detector:
					//*****************************************************************
					detector->detect(imgGray1,keypoints);
					detector->detect(imgGray2,keypoints2);
					//*****************************************************************
					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectore);
					double detectionTime = diff(detectors,detectore).tv_nsec/1000000;
					//std::cout<<"Detection Time: "<<diff(detectors,detectore).tv_nsec/1000000<<" ms"<<endl;


					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractors);
					// now the extractor:
					//*****************************************************************
					// get the descriptors
					cv::Mat descriptors, descriptors2;
					std::vector<cv::DMatch> indices;
					// first image. Computes the descrkeypoints2iptor for each of the keypoints.
					//Outputs a 64 bit vector describing the keypoints.

					descriptorExtractor->compute(imgGray2,keypoints2,descriptors2);
					// and the second one
					descriptorExtractor->compute(imgGray1,keypoints,descriptors);

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractore);
					double extractionTime = diff(extractors,extractore).tv_nsec/1000000;
					//std::cout<<"Extraction Time: "<<diff(extractors,extractore).tv_nsec/1000000<<" ms"<<endl;
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
					descriptorMatcher->knnMatch(descriptors,descriptors2,matches,2);
					else{
//						if(descriptors2.rows>0)
//						descriptorMatcher->knnMatch(descriptors,descriptors2,matches,2);
//						else
//							matches.clear();
						//Decreasing with the maxdistance value will drastically reduce the number of matches
						if(descriptors2.rows>0)
						descriptorMatcher->radiusMatch(descriptors,descriptors2,matches,hammingDistance);
						else
							matches.clear();
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
					double verifyTime = diff(verifys,verifye).tv_nsec/1000;

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchinge);
					double matchingTime = diff(matchings,matchinge).tv_nsec/1000;

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &te);
					double overallTime = diff(ts,te).tv_nsec/1000000;

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
					std::cout<<"Matching Time: "<<matchingTime<<" us"<<endl;
					std::cout<<"Verify Matches Time: "<<verifyTime<<" us"<<endl;
					std::cout<<"Overall Time: "<<overallTime<<" ms"<<endl;
					cv::waitKey(1000);
#endif


					//threshold = atoi(argv[3]+5);
					//Write all the information to a file
					writeFile <<tempDir<<", "<<tempDir1<<", "<<threshold<<", "<<name1<<", "<<name2<<", "<<keypoints.size()<<", "<<keypoints2.size()<<", "<<feature.imageMatchingScoreBest<<", "<<feature.imageMatchingScore<<","<<feature.totalNumMatches<<", "<<feature.totalNumValidMatches<<", "<<feature.totalNumInvalidMatches<<", "<<feature.totalNumBestMatches<<", "<<detectionTime<<", "<<extractionTime<<", "<<matchingTime<<", "<<verifyTime<<", "<<overallTime<<"\n";
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
					cv::waitKey(1000);
#endif

				}//End of inner for loop (jj)
			}//end of outer for loop (ii)
		}//End of ss loop
	}//end of kk loop

	return 0;
}
