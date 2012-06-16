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

	
	
	for (int ss=1;ss<=4; ss++)
	{
	//Create object for dataAnalysis
	DataAnalysis dataAnalysis;

	//Create the Feature extraction object
	FeatureExtraction feature;


	bool hamming=true;
	// Declare the extractor. Only needs to be performed once.
	//*****************************************************************
	cv::Ptr<cv::DescriptorExtractor> descriptorExtractor;
    descriptorExtractor = feature.getExtractor(argc, argv, hamming, descriptorExtractor);
    //*****************************************************************

	//The directory where the files are stored
	std::string dir;
	dir = to_string<int>(ss);
	std::string tempDir = to_string<int>(ss);
	//Names of the two image files
	std::string name1;
    std::string name2;

    //For changing the threshold
    int testThreshold = 10;
    //Set the radius
    std::string stringRad;
    //double radius = 0.05;
    double hammingDistance = 10;
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
	string file = "../../data/Thresholds/matchingData_threshold_14062012_BRISK_BRISK";
//		file.append(testThresholdString.c_str());
//		file.append("Directory_");
//		file.append(tempDir.c_str());
//		file.append("_");
//		file.append(stringRad.c_str());
	file.append(".txt");
	cout<<file<<endl;
    //*************************************
    
    for (int rr=1;rr<20;rr++)
    {

	//set the threshold
	testThreshold = 20;
	
	stringRad = to_string<double>(hammingDistance);

	//This will compute the statistics for every combination in a directory for each threshold
    for (int tt = 1; tt<20;tt++)
    {

    	

		//strftime (filename,80,file.append("_%b_%d_%H%M%S.txt"),timeinfo);
		//puts (filename);
		//cout<<filename<<endl;
		//*************************************

	for (int ii = 1; ii<=10;ii++)//jpegCounter-1
	{

        for (int jj = 1; jj<ii;jj++)//jpegCounter-1
        {




    //Choose the images to compare

    name1 = to_string<int>(ii);
//    if(ii==jj)
//    continue;
    name2 = to_string<int>(jj);
    cout<<"Image 1: "<<name1<<", Image 2: "<<name2<<endl;

	// names of the two images
	std::string fname1;
	std::string fname2;
	cv::Mat imgRGB1;
	cv::Mat imgRGB2;
	cv::Mat imgRGB3;
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
	
	//Start the timer
	//clock_t start = clock();
	timespec ts, te, matchings, matchinge, detectors, detectore, extractors, extractore;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
	

	// if no arguments are passed:
	if(argc==1){
		int i=0;
		int fextensions_size=fextensions.size();

//		string name1;
//        string name2;
//        std::cout<<"Enter image 1"<<endl;
//        std::cin>>name1;
//        std::cout<<"Enter image 2"<<endl;
//        std::cin>>name2;
//        cout<<name1<<" and "<<name2<<endl;

		while(imgRGB1.empty()||imgRGB2.empty()){
		    fname1 = dir+name1+fextensions[i];
			fname2 = dir+name2+fextensions[i];
			imgRGB1 = cv::imread(fname1);
			imgRGB2 = cv::imread(fname2);
			i++;
			if(i>=fextensions_size) break;
		}
		if (imgRGB2.empty()||imgRGB2.empty())
		{
			std::cout<<"image(s) "<<fname1<<", "<<fname2<<" not found." << std::endl;
			return 2;
		}
	}
	else{
		if(strncmp("rot-", argv[1], 4)==0){
			do_rot=true;
			int i=0;
			int fextensions_size=fextensions.size();
			while(imgRGB1.empty()){
				fname1 = std::string(argv[1]+4)+"/img1"+fextensions[i];
				imgRGB1 = cv::imread(fname1);
				i++;
				if(i>=fextensions_size) break;
			}
			if (imgRGB2.empty())
			{
				std::cout<<"image not found." << std::endl;
				return 2;
			}
		}
		else{

			int i=0;
			int fextensions_size=fextensions.size();
			while(imgRGB1.empty()||imgRGB2.empty()){
				fname1 = dir+"/"+name1+".jpg";
				fname2 = dir+"/"+name2+".jpg";
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
		}
		//unsigned int N=atoi(argv[3]);
		if (imgRGB1.empty())
		{
			fname1 = std::string(argv[1]+4)+"/img1.pgm";
			imgRGB1 = cv::imread(fname1);
			if (imgRGB1.empty()){
				std::cout<<"image not found at " << fname1 << std::endl;
				return 2;
			}
		}
	}

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
	int threshold;
	
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectors);
	// create the detector:
	//*****************************************************************
	cv::Ptr<cv::FeatureDetector> detector;
    detector = feature.getDetector(argc, argv, detector, threshold, testThreshold,0);
    //*****************************************************************

	// run the detector:
    //*****************************************************************
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
		descriptorMatcher->radiusMatch(descriptors2,descriptors,matches,hammingDistance);
	else{
	    //Messing with the maxdistance value will drastically reduce the number of matches
		//descriptorMatcher->radiusMatch(descriptors2,descriptors,matches,radius);
		//descriptorMatcher->knnMatch(descriptors2,descriptors,matches,3);
	}
		//For the above method, we could use KnnMatch. All values less than radius max distance are selected
    //*****************************************************************
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchinge);
	double matchingTime = diff(matchings,matchinge).tv_nsec/1000;



	
	
	cv::Mat outimg;


    //Write the data to a file
    ofstream writeFile;

    //std::string filename = "../../data/Thresholds/matchingData"+name2+".txt";
    //writeFile.open(filename.c_str(), ios::app);//ios::app
    writeFile.open(file.c_str(), ios::app);//ios::app
	// drawing
    float imageMatchingScore = 0;
    float imageMatchingScoreBest = 0;
    int totalNumMatches = 0;
    int totalNumBestMatches = 0;
    int totalNumValidMatches = 0;
    int totalNumInvalidMatches = 0;

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
        //cout<<"For interest point :"<<i<<" the number of matches are "<<matches[i].size()<<endl;

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


        for( size_t j = 0; j < matches[i].size(); j++ )
        {

            //Corresponds to the first image
            int i1 = matches[i][j].trainIdx;
            //Corresponds to the second image
            int i2 = matches[i][j].queryIdx;



            float distanceMatch = matches[i][j].distance;
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
    cout<<"The matching score for the image (condsidering all matches) is "<<imageMatchingScore<<endl;
    cout<<"The matching score for the image (condsidering best match only) is "<<imageMatchingScoreBest<<endl;
    cout<<"The total number of matches is "<<totalNumMatches<<endl;
    cout<<"The total number of valid matches is "<<totalNumValidMatches<<endl;
    cout<<"****************************************"<<endl;
#endif
    
    //Write all the information to a file
    writeFile <<tempDir<<", "<<hammingDistance<<", "<<testThreshold<<", "<<name1<<", "<<name2<<", "<<keypoints.size()<<", "<<keypoints2.size()<<", "<<imageMatchingScoreBest<<", "<<imageMatchingScore<<","<<totalNumMatches<<", "<<totalNumValidMatches<<", "<<totalNumBestMatches<<", "<<detectionTime<<", "<<extractionTime<<", "<<matchingTime<<", "<<overallTime<<"\n";
    
    //close the file
    writeFile.close();

#if (DISPLAY)
    //Draw the matches and keypoints
	drawMatches(imgRGB2, keypoints2, imgRGB1, keypoints,matches,outimg,
			 cv::Scalar(0,255,0), cv::Scalar(0,0,255),
			std::vector<std::vector<char> >(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

	cv::namedWindow("Matches");
	cv::imshow("Matches", outimg);
	//cv::imshow("keypoints", imgRGB1);
	//cv::imshow("keypoints2", imgRGB2);
	cv::waitKey();
#endif


					}//End of inner for loop (jj)

				}//End of outer loop (ii)
				//Change the test threshold
				testThreshold = testThreshold + 5;
	
			}//End of the tt threshold
    
		//Increment the radius
		//radius = radius+ 0.01;//For BRISK_SURF
    hammingDistance = hammingDistance + 5;//For BRISK BRISK
		}//End of rr loop for radius
    
	}//End of main ss loop
	return 0;
}
