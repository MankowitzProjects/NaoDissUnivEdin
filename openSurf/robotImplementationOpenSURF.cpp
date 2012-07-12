#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <list>
#include <iomanip>
#include "surflib.h"
#include "utils.h"
#include "kmeans.h"
#include <ctime>
#include "OSFeatureExtraction.h"
//#include "include/DataAnalysis.h"
//#include "include/FeatureExtraction.h"


#include <string>
#include <sstream>

#define DEBUG_MODE 0
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

//*********************************************

/* Currently the time issue is with building the response maps. This is in the fastHessian.cpp file
 * In addition, there are too many interest points being detected as positive.
 * Does RANSAC Work for these points. What are the bd, bs parameters
 */

//*********************************************



int main(int argc, char ** argv) {


	//Set the arguments
	//std::string feat_detector = "SURF";
	//int threshold = 1000
	bool hamming=false;
	std::string feat_detector = "SURF";
	std::string feat_descriptor = "SURF";

	//Create the Feature extraction
	OSFeatureExtraction feature(10,200);

	//Create data analysis object
	//DataAnalysis dataAnalysis;


	//The directory where the files are stored TESTING
	std::string dir = "../brisk/images/PicsOG/Matching_Pics_Right_Overlapping";
	std::string dir1 = "../brisk/images/PicsMG/Matching_Pics_Right_Overlapping";//PicsOG/Matching_Images_OG_Left
	//Names of the two image files
	std::string name1 = "1";
	std::string name2 = "24";

	//Threshold
	float threshold = 0.005f;

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

	//Declare the two images
	IplImage *imgYUV1, *imgYUV2;

	//Determine the region of interest
	int horizonStart = 200;
	int horizonLine = 240;

	//******************************************************************
	//Compute the image that will be stored in the image bank
	//The Grayscale image
	IplImage *imgGray1;

	fname1 =dir + "/"+ name1+".jpg";
	imgYUV1 = cvLoadImage(fname1.c_str());
	cout<<"Size of the image is: "<<imgYUV1->height<<", "<<imgYUV1->width<<endl;
	//Create an image from the YUV image
	imgGray1 = cvCreateImage(cvSize(imgYUV1->width,imgYUV1->height),IPL_DEPTH_8U,1);

	//Convert the image to grayscale
	cvCvtColor(imgYUV1,imgGray1,CV_RGB2GRAY);

	//Create images that are of width 640 and height 1
	IplImage *imgFinal1 = cvCreateImage(cvSize(imgYUV1->width,1),IPL_DEPTH_8U,1);

	//Get a row of pixels by sub-sampling every 4th pixel and taking the mean
	imgFinal1 = getRowOfPixels(imgGray1, horizonStart, horizonLine);

	//Generate a vector of interest points
	IpVec interestPoints1;

	//Detect the interest points
	surfDet(imgFinal1,interestPoints1, 4, 3, 2, threshold);

	//Compute the SURF descriptor
	surfDes(imgFinal1,interestPoints1, false);

	//******************************************************************

	//Read in the current image
	//*****************************************************************
	fname2 = dir1+ "/"+ name2+".jpg";
	imgYUV2 = cvLoadImage(fname2.c_str());

	//*****************************************************************
	//Set the regions of interest in which we want to work
	//cvSetImageROI(imgYUV1, cvRect(0,0,imgYUV1->width,horizonLine));
	//cvSetImageROI(imgYUV2, cvRect(0,0,imgYUV2->width,horizonLine));

	//Note that the image dimensions do not change for the image
	//Create Grayscale images from the YUV images
	IplImage *imgGray2;
	imgGray2 = cvCreateImage(cvSize(imgYUV2->width,imgYUV2->height),IPL_DEPTH_8U,1);

	//cvSetImageROI(imgGray1, cvRect(0,0,imgYUV1->width,horizonLine));
	//cvSetImageROI(imgGray2, cvRect(0,0,imgYUV2->width,horizonLine));

#if (DEBUG_MODE)
	cout<<"The image WIDTH is: "<<imgYUV1->width<<endl;
	cout<<"The image HEIGHT is: "<<imgYUV1->height<<endl;
	cout<<"The image depth is: "<<imgYUV1->depth<<endl;
	cout<<"The number of channels is: "<<imgYUV1->nChannels<<endl;
	cout<<"The image data order is: "<<imgYUV1->dataOrder<<endl;
#endif

	//cvAddS(imgYUV1, cvScalar(100), imgYUV1);

	timespec ts, te, matchings, matchinge, detectors, detectore, extractors, extractore;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
	//Convert the images to grayscale
	cvCvtColor(imgYUV2,imgGray2,CV_RGB2GRAY);

	//Create images that are of width 640 and height 1
	IplImage *imgFinal2 = cvCreateImage(cvSize(imgYUV1->width,1),IPL_DEPTH_8U,1);

	//Compute a vertical average of the row of pixels above the horizon
	imgFinal2 = getRowOfPixels(imgGray2, horizonStart, horizonLine);

#if (DEBUG_MODE)
	cvNamedWindow("1", CV_WINDOW_AUTOSIZE );
	cvNamedWindow("2", CV_WINDOW_AUTOSIZE );
	cvShowImage("1", imgFinal1);
	cvShowImage("2",imgFinal2);
	// Save the frame into a file
	cvSaveImage("Imgs/image1Row.jpg" ,imgFinal1);
	// Save the frame into a file
	cvSaveImage("Imgs/image2Row.jpg" ,imgFinal2);
	cvWaitKey(200);
#endif

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectors);
	//MC: Generate a vector of Interest Points
	//*****************************************************************
	IpVec interestPoints2;
	//*****************************************************************
	// create the OpenSURF detector:
	//*****************************************************************
	surfDet(imgFinal2,interestPoints2, 4, 3, 2, threshold);
	//*****************************************************************

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectore);
	float detectionTime = diff(detectors,detectore).tv_nsec/1000000.0f;


	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractors);

	// get the OpenSURF descriptors
	// first image. Computes the descriptor for each of the keypoints.
	//Outputs a 64 bit vector describing the keypoints.
	//*****************************************************************
	surfDes(imgFinal2,interestPoints2, false);//False means we do not wish to use upright SURF (U-SURF)
	//*****************************************************************
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractore);
	float extractionTime = diff(extractors,extractore).tv_nsec/1000000.0f;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchings);

	//OpenSURF matching
	//*****************************************************************
	cout<<"The number of interestPoints 1 is: "<<interestPoints1.size()<<endl;
	cout<<"The number of interestPoints 2 is: "<<interestPoints2.size()<<endl;
	IpPairVec matches;
	getMatches(interestPoints1, interestPoints2, matches, 0);
	//*****************************************************************

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchinge);
	float matchingTime = diff(matchings,matchinge).tv_nsec/1000000.0f;



	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &te);
	float overallTime = diff(ts,te).tv_nsec/1000000.0f;

	//Find the number of valid matches
	feature.performRansac(matches);

	//Get the matching Statistics
	feature.calculateMatchingScore(*imgFinal2,interestPoints1, interestPoints2, matches);

//	ofstream writeFile;
//	std::string filename = "../../data/implementation/matchingData.txt";
//	writeFile.open(filename.c_str(), ios::app);//ios::app
//	//Output the matches
//	for (int i=1;i<matches.size();i++)
//	{
//		cout<<"First Match coords x,y: "<<matches[i].first.x<<", "<<matches[i].first.y<<endl;
//		cout<<"Second Match coords x,y: "<<matches[i].second.x<<", "<<matches[i].second.y<<endl;
//		cout<<"The distance between interest points is: "<<matches[i].first-matches[i].second<<endl;
//
//		writeFile <<matches[i].first.x<<", "<<matches[i].second.x<<", "<<matches[i].first-matches[i].second<<"\n";
//
//	}
//
//	//close the file
//	writeFile.close();
	//cv::Mat outimg;


	//Write the data to a file
	//    ofstream writeFile;
	//
	//    std::string filename = "../../data/implementation/matchingData.txt";
	//    writeFile.open(filename.c_str(), ios::app);//ios::app


	//To store the incorrect matches
#if(DISPLAY || DEBUG_MODE)
	std::vector<Ipoint> leftPoints;
	std::vector<Ipoint> rightPoints;
#endif

#if (DEBUG_MODE)
	//cout<<"The total number of keypoints in image 1 is: "<<keypoints.size()<<endl;
	//cout<<"The total number of keypoints in image 2 is: "<<keypoints2.size()<<endl;
#endif


#if 1// (DEBUG_MODE)
	cout<<"****************************************"<<endl;
	cout<<"The matching score for the image (condsidering all matches) is "<<feature.imageMatchingScore<<endl;
	cout<<"The total number of matches is "<<feature.totalNumMatches<<endl;
	cout<<"The total number of valid matches is "<<feature.totalNumValidMatches<<endl;
	cout<<"The total number of invalid Matches is: "<<feature.totalNumMatches -feature.totalNumValidMatches <<endl;
	cout<<"****************************************"<<endl;
#endif
#if 1//(DEBUG_MODE)
	std::cout<<"The times:"<<endl;
	std::cout<<"Detection Time: "<<detectionTime<<" us"<<endl;
	std::cout<<"Extraction Time: "<<extractionTime<<" us"<<endl;
	std::cout<<"Matching Time: "<<matchingTime<<" us"<<endl;
	std::cout<<"Overall Time: "<<overallTime<<" ms"<<endl;
#endif
	//	threshold = atoi(argv[3]+5);
	//	//    writeFile <<threshold<<", "<<name1<<", "<<name2<<", "<<keypoints.size()<<", "<<keypoints2.size()<<", "<<imageMatchingScoreBest<<", "<<imageMatchingScore<<","<<totalNumMatches<<", "<<totalNumBestMatches<<"\n";
	//	//    //close the file
	//	//    writeFile.close();
	//#if (DEBUG_MODE)
	//	cout<<"The total number of keypoints in image 1 is: "<<keypoints.size()<<endl;
	//	cout<<"The total number of keypoints in image 2 is: "<<keypoints2.size()<<endl;
	//#endif
	//
	//
	//#if (DISPLAY)
	//	drawMatches(imgYUV2, keypoints2, imgYUV1, keypoints,matches,outimg,
	//			cv::Scalar(0,255,0), cv::Scalar(0,0,255),
	//			std::vector<std::vector<char> >(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
	////NOT_DRAW_SINGLE_POINTS
	//
	//	int colourChanger = 0;
	//	for (int k = 0; k<leftPoints.size(); k++)
	//	{
	//
	//		circle(imgYUV2,cv::Point(leftPoints[k].x, leftPoints[k].y), 5, cv::Scalar(colourChanger, 100, 255), 4, 8, 0);
	//
	//#if(DEBUG_MODE)
	//		cout<<"Incorrect coord Left row,col : "<<leftPoints[k].y<<", "<<leftPoints[k].x<<endl;
	//#endif
	//		colourChanger = colourChanger+30;
	//	}
	//	colourChanger = 0;
	//	for (int k = 0; k<rightPoints.size(); k++)
	//	{
	//		circle(imgYUV1,cv::Point(rightPoints[k].x, rightPoints[k].y), 5, cv::Scalar(colourChanger, 100, 255), 4, 8, 0);
	//#if(DEBUG_MODE)
	//		cout<<"Incorrect coord Right row,col : "<<rightPoints[k].y<<", "<<rightPoints[k].x<<endl;
	//#endif
	//		colourChanger = colourChanger+30;
	//	}
	//
	//
	//
	//	cv::namedWindow("Matches");
	//	cv::imshow("Matches", outimg);
	//	//imgYUV1 is right. imgYUV2 is left
	//#if(DEBUG_MODE)
	//	cv::imshow("keypoints", imgYUV1);
	//	cv::imshow("keypoints2", imgYUV2);
	//#endif
	//
	//	cv::waitKey();
	//#endif

	std::cout<< "Matches: " << feature.totalNumValidMatches<<endl;

	//Image created for drawing
	for (unsigned int i = 0; i < matches.size(); ++i)
	{
		matches[i].first.clusterIndex = i;
		matches[i].second.clusterIndex = i;

		if(matches[i].first.x!=-1){
			drawPoint(imgYUV1,matches[i].first);
			drawPoint(imgYUV2,matches[i].second);

			const int & w = imgYUV1->width;
			cvLine(imgYUV1,cvPoint(matches[i].first.x,matches[i].first.y),cvPoint(matches[i].second.x+w,matches[i].second.y), cvScalar(0,128,0),1.5);
			cvLine(imgYUV2,cvPoint(matches[i].first.x-w,matches[i].first.y),cvPoint(matches[i].second.x,matches[i].second.y), cvScalar(0,128,0),1.5);
		}
	}


	cvNamedWindow("1", CV_WINDOW_AUTOSIZE );
	cvNamedWindow("2", CV_WINDOW_AUTOSIZE );
	cvShowImage("1", imgYUV1);
	cvShowImage("2",imgYUV2);
	// Save the frame into a file
	cvSaveImage("Imgs/image1.jpg" ,imgYUV1);
	// Save the frame into a file
	cvSaveImage("Imgs/image2.jpg" ,imgYUV2);
	cvWaitKey(0);

	return 0;
}
