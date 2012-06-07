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
#include "../include/brisk/brisk.h"
#include <fstream>
#include <iostream>
#include <list>
#include <iomanip>
#include "../include/brisk/DataAnalysis.h"
#include "../include/brisk/FeatureExtraction.h"


#include <string>
#include <sstream>

#define DEBUG_MODE 0
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



//standard configuration for the case of no file given
const int n=12;
const float r=2.5; // found 8-9-11, r=3.6, exponent 1.5


int main(int argc, char ** argv) {

	//Create object for dataAnalysis
	DataAnalysis dataAnalysis;

	//Create the Feature extraction object
	FeatureExtraction feature;

	//The directory where the files are stored
	std::string dir;

	//Names of the two image files
	std::string name1;
    std::string name2;

    //For changing the threshold
    int testThreshold = 10;

    int jpegCounter = dataAnalysis.getNumImagesInDirectory(&dir);

    std::cout<<"The number of images in the directory is: "<<jpegCounter<<endl;
    //*************************************

    bool multipleTesting = true;

    for (int ii = 1;ii<jpegCounter;ii++)
    {
        for (int jj = 1; jj<jpegCounter-1;jj++)
        {

    //Choose the images to compare
#if (MULTIPLE_IMAGE_TEST)
    name1 = to_string<int>(ii);
    if(ii==jj)
    continue;

    name2 = to_string<int>(jj);

    cout<<"Image "<<ii<<", Image "<<jj<<endl;
#else

    name1 = "1";
    if(1==jj)
    continue;
    name2 = to_string<int>(jj);
    cout<<"Threshold "<<testThreshold<<", Image "<<jj<<endl;
#endif

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

	// if no arguments are passed:
	if(argc==1){
		int i=0;
		int fextensions_size=fextensions.size();

		string name1;
        string name2;
        std::cout<<"Enter image 1"<<endl;
        std::cin>>name1;
        std::cout<<"Enter image 2"<<endl;
        std::cin>>name2;
        cout<<name1<<" and "<<name2<<endl;

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


            /*std::cout<<"Enter image 1"<<endl;
            std::cin>>name1;
            std::cout<<"Enter image 2"<<endl;
            std::cin>>name2;
            cout<<name1<<" and "<<name2<<endl;*/
			int i=0;
			int fextensions_size=fextensions.size();
			while(imgRGB1.empty()||imgRGB2.empty()){
//				fname1 = std::string(argv[1])+"/"+name1+fextensions[i];
//				fname2 = std::string(argv[1])+"/"+name2+std::string(argv[2])+fextensions[i];
				//fname1 = std::string(argv[1])+"/"+name1+".jpg";
				//fname2 = std::string(argv[1])+"/"+name2+".jpg";
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
	// create the detector:
	//*****************************************************************
	cv::Ptr<cv::FeatureDetector> detector;
    detector = feature.getDetector(argc, argv, detector, threshold, testThreshold,1);
    //*****************************************************************

	// run the detector:
    //*****************************************************************
		detector->detect(imgGray1,keypoints);
		detector->detect(imgGray2,keypoints2);
    //*****************************************************************



	// now the extractor:
	bool hamming=false;
	cv::Ptr<cv::DescriptorExtractor> descriptorExtractor;
	// now the extractor:
	//*****************************************************************
    descriptorExtractor = feature.getExtractor(argc, argv, hamming, descriptorExtractor);
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

	// matching
	//*****************************************************************
	std::vector<std::vector<cv::DMatch> > matches;
	cv::Ptr<cv::DescriptorMatcher> descriptorMatcher;
	if(hamming)
		descriptorMatcher = new cv::BruteForceMatcher<cv::HammingSse>();
	else
		descriptorMatcher = new cv::BruteForceMatcher<cv::L2<float> >();
	if(hamming)
		descriptorMatcher->radiusMatch(descriptors2,descriptors,matches,100.0);
	else{
	    //Messing with the maxdistance value will drastically reduce the number of matches
		descriptorMatcher->radiusMatch(descriptors2,descriptors,matches,0.15);
		//descriptorMatcher->knnMatch(descriptors2,descriptors,matches,3);
	}
		//For the above method, we could use KnnMatch. All values less than 0.21 max distance are selected
    //*****************************************************************


	cv::Mat outimg;


    //Write the data to a file
    ofstream writeFile;

    std::string filename = "../../data/matchingDataDemo.txt";
    writeFile.open(filename.c_str(), ios::app);//ios::app
	// drawing
    float imageMatchingScore = 0;
    float imageMatchingScoreBest = 0;
    int totalNumMatches = 0;
    int totalNumBestMatches = 0;

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
        for( size_t j = 0; j < matches[i].size(); j++ )
        {

            //Corresponds to the first image
            int i1 = matches[i][j].trainIdx;
            //Corresponds to the second image
            int i2 = matches[i][j].queryIdx;



            float distanceMatch = matches[i][j].distance;
            float matchingScore = 1/distanceMatch;
           // if( matchesMask.empty() || matchesMask[i][j] )
            //{
                //const KeyPoint &kp1 = keypoints1[i1], &kp2 = keypoints2[i2];
                //_drawMatch( outImg, outImg1, outImg2, kp1, kp2, matchColor, flags );

            #if (DEBUG_MODE)
            std::cout<<"The match corresponds to keypoint locations ("<<keypoints[i1].pt.x<<", "<<keypoints[i1].pt.y<<") and second ("<<keypoints2[i2].pt.x<<", "<<keypoints2[i2].pt.y<<")"<<endl;
            dataAnalysis.displayOutput(keypoints, keypoints2, matchingScore, i1, i2);

            cout<<"The distance between "<<i1<<" and "<<i2<<" is "<<distanceMatch<<endl;
            cout<<"The matching score between "<<i1<<" and "<<i2<<" is "<<matchingScore<<endl;
            #endif
            //This only considers the best match
            if(j==0){
            totalNumBestMatches = totalNumBestMatches + 1;
            imageMatchingScoreBest = imageMatchingScoreBest + matchingScore;

            }

            imageMatchingScore = imageMatchingScore + matchingScore;

            //Compute the total num of matches


          //  }
        }
        totalNumMatches = totalNumMatches + matches[i].size();
    }
    #if (DEBUG_MODE)
    cout<<"****************************************"<<endl;
    cout<<"The matching score for the image (condsidering all matches) is "<<imageMatchingScore<<endl;
    cout<<"The matching score for the image (condsidering best match only) is "<<imageMatchingScoreBest<<endl;
    cout<<"The total number of matches is "<<totalNumMatches<<endl;
    cout<<"****************************************"<<endl;
    #endif
    writeFile <<threshold<<", "<<name1<<", "<<name2<<", "<<keypoints.size()<<", "<<keypoints2.size()<<", "<<imageMatchingScoreBest<<", "<<imageMatchingScore<<","<<totalNumMatches<<", "<<totalNumBestMatches<<"\n";
    //close the file
    writeFile.close();

#if (DISPLAY)
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
    }//end of outer for loop (ii)
	return 0;
}
