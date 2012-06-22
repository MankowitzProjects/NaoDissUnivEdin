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
	//int threshold = 1000
	bool hamming=false;
	std::string feat_detector = "BRISK";
	int threshold = 100;
	double radius = 0.20;
	std::string feat_descriptor = "SURF";

	//Create the Feature extraction
	OSFeatureExtraction feature;

	//Create data analysis object
	//DataAnalysis dataAnalysis;


	//The directory where the files are stored TESTING
	std::string dir = "../brisk/images/PicsMG/Matching_Pics_Right_Overlapping";
	std::string dir1 = "../brisk/images/PicsOG/Matching_Pics_Right_Overlapping";//PicsOG/Matching_Images_OG_Left
	//Names of the two image files
	std::string name1 = "1";
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

	//Declare the two images
	IplImage *imgYUV1, *imgYUV2;

	//Determine the region of interest
	int horizonStart = 320;
	int horizonLine = 350;



	//Read in images
	//*****************************************************************
	fname1 =dir + "/"+ name1+".jpg";
	fname2 = dir1+ "/"+ name2+".jpg";
	imgYUV1 = cvLoadImage(fname1.c_str());
	imgYUV2 = cvLoadImage(fname2.c_str());
	//*****************************************************************
	//Set the regions of interest in which we want to work
	//cvSetImageROI(imgYUV1, cvRect(0,0,imgYUV1->width,horizonLine));
	//cvSetImageROI(imgYUV2, cvRect(0,0,imgYUV2->width,horizonLine));

	//Note that the image dimensions do not change for the image
	//Create Grayscale images from the YUV images
	IplImage *imgGray1, *imgGray2;

	imgGray1 = cvCreateImage(cvSize(imgYUV1->width,imgYUV1->height),IPL_DEPTH_8U,1);
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
	cvCvtColor(imgYUV1,imgGray1,CV_RGB2GRAY);
	cvCvtColor(imgYUV2,imgGray2,CV_RGB2GRAY);

	//Create images that are of width 640 and height 1
	IplImage *imgFinal1 = cvCreateImage(cvSize(imgYUV1->width,1),IPL_DEPTH_8U,1);
	IplImage *imgFinal2 = cvCreateImage(cvSize(imgYUV1->width,1),IPL_DEPTH_8U,1);

	//Compute a vertical average of the row of pixels above the horizon
	imgFinal1 = getRowOfPixels(imgGray1, horizonStart, horizonLine);
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
	//MC: Generate a vector of Interest Points
	//*****************************************************************
	IpVec interestPoints1, interestPoints2;
	//*****************************************************************

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectors);

	// create the OpenSURF detector:
	//*****************************************************************
	surfDet(imgFinal1,interestPoints1, 4, 3, 2, 0.00005f);
	surfDet(imgFinal2,interestPoints2, 4, 3, 2, 0.00005f);
	//*****************************************************************

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectore);
	double detectionTime = diff(detectors,detectore).tv_nsec/1000000;


	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractors);

	// get the OpenSURF descriptors

	// first image. Computes the descriptor for each of the keypoints.
	//Outputs a 64 bit vector describing the keypoints.
	//*****************************************************************
	surfDes(imgYUV1,interestPoints1, false);
	surfDes(imgYUV2,interestPoints2, false);//False means we do not wish to use upright SURF (U-SURF)
	//*****************************************************************
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractore);
	double extractionTime = diff(extractors,extractore).tv_nsec/1000000;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchings);

	//OpenSURF matching
	//*****************************************************************
	cout<<"The number of interestPoints 1 is: "<<interestPoints1.size()<<endl;
	cout<<"The number of interestPoints 2 is: "<<interestPoints2.size()<<endl;
	IpPairVec matches;
	getMatches(interestPoints1, interestPoints2, matches, 0);
	//*****************************************************************

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchinge);
	double matchingTime = diff(matchings,matchinge).tv_nsec/1000;



	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &te);
	double overallTime = diff(ts,te).tv_nsec/1000000;



	//Output the matches
	cout<<"First Match coords x,y: "<<matches[1].first.x<<", "<<matches[1].first.y<<endl;
	cout<<"Second Match coords x,y: "<<matches[1].second.x<<", "<<matches[1].second.y<<endl;
	cout<<"The distance between interest points is: "<<matches[1].first-matches[1].second<<endl;
	//cv::Mat outimg;


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
	std::vector<Ipoint> leftPoints;
	std::vector<Ipoint> rightPoints;
#endif

#if (DEBUG_MODE)
	//cout<<"The total number of keypoints in image 1 is: "<<keypoints.size()<<endl;
	//cout<<"The total number of keypoints in image 2 is: "<<keypoints2.size()<<endl;
#endif

	for( size_t i = 0; i < matches.size(); i++ )
	{

		if (matches.size()>0){
#if (DEBUG_MODE)
			cout<<"The number of matches is : "<<matches.size()<<endl;
			cout<<"****************************************"<<endl;
#endif
		}
#if 0// (DEBUG_MODE)
		for(size_t kk =0; kk<matches.size();kk++)
		{
			cout<<"Keypoint index : "<<i<<endl;
			cout<<"****************************"<<endl;
			cout<<"Keypoint Image 1  row,col : "<<matches[kk].first.x<<", "<<matches[kk].first.y<<endl;
			cout<<"Keypoint Image 2  row,col : "<<matches[kk].second.x<<", "<<matches[kk].second.y<<endl;
			cout<<"****************************"<<endl;
		}
#endif

		int allMatches = matches.size();
		int counter = 0;
		int matchSize = matches.size();
		bool isTrueMatchFound = false;

		Ipoint ip1 = matches[i].first;
		Ipoint ip2 = matches[i].second;
		//Determine the distance between matches
		float distanceMatch = ip1 - ip2;

		//Give a constant reward for being under a certain threshold
		float matchingScore = 0;
		if (distanceMatch==0)
			matchingScore=100;
		else
			matchingScore = 1/distanceMatch;

		cout<<"Distance match is: "<<distanceMatch<<endl;
#if 0//(DEBUG_MODE)
		cout<<"Keypoint index : "<<i<<endl;
		cout<<"****************************"<<endl;
		cout<<"Keypoint Image 1  row,col : "<<ip1.x<<", "<<ip1.y<<endl;
		cout<<"Keypoint Image 2  row,col : "<<ip2.y<<", "<<ip2.x<<endl;
		cout<<"****************************"<<endl;
#endif

		//	#if (OUTPUT)
		//				dataAnalysis.displayOutput(keypoints2, keypoints, matchingScore, i1, i2);
		//	#endif
		//
		//Verify whether the match is correct or not
		//****************************************************
		bool correctMatch = feature.verifyMatch(*imgYUV1, ip1, ip2);
#if (DEBUG_MODE)
		cout<<"CorrectMatch: "<<correctMatch<<endl;
#endif
		//****************************************************

		//If the match is incorrect, remove the invalid match
		if (correctMatch==false)
		{
#if (DEBUG_MODE)
			cout<<"Entered Here"<<endl;
			cout<<"Keypoint Left to be erased row,col : "<<ip1.y<<", "<<ip2.x<<endl;
			cout<<"Keypoint Right to be erased row,col : "<<ip1.y<<", "<<ip2.x<<endl;
#endif

#if(DEBUG_MODE)
			cout<<"The number of matches before removal is : "<<matches.size()<<endl;
			cout<<"-----------------------------------------"<<endl;
#endif
			//Erase the corresponding match
			std::vector<std::pair<Ipoint, Ipoint> >::iterator IptIt;
			IptIt = matches.begin();

			matches[i].first.x = 0;
			matches[i].first.y = 0;
			matches[i].second.x = 0;
			matches[i].second.y = 0;

			//matches.erase(IptIt + i);

			matchSize = matches.size();
#if (DEBUG_MODE)
			cout<<"The number of matches after removal is : "<<matchSize<<endl;
			cout<<"counter is : "<<counter<<endl;
			cout<<"----------------------------------------"<<endl;
#endif

			totalNumInvalidMatches = totalNumInvalidMatches + 1;
#if (DEBUG_MODE)
			leftPoints.push_back(ip1);
			rightPoints.push_back(ip2);
#endif
		}
		else
		{
			isTrueMatchFound = true;
			counter++;
		}
		//cv::waitKey(500);

		if(matches.size()==0)
			break;


		cout<<"The counter is: "<<counter<<endl;

		//This only considers the best correct match.
		if(isTrueMatchFound && counter ==1){
			totalNumBestMatches = totalNumBestMatches + 1;
			totalNumValidMatches = totalNumValidMatches + 1;

			imageMatchingScoreBest = imageMatchingScoreBest + matchingScore;
			isTrueMatchFound = false;
		}

		imageMatchingScore = imageMatchingScore + matchingScore;


		totalNumMatches = totalNumMatches + 1;
	}
#if (DEBUG_MODE)
	cout<<"****************************************"<<endl;
	cout<<"The matching score for the image (condsidering all matches) is "<<imageMatchingScore<<endl;
	cout<<"The matching score for the image (condsidering best match only) is "<<imageMatchingScoreBest<<endl;
	cout<<"The total number of matches is "<<totalNumMatches<<endl;
	cout<<"The total number of valid matches is "<<totalNumValidMatches<<endl;
	cout<<"The total number of invalid Matches is: "<<totalNumInvalidMatches<<endl;
	cout<<"****************************************"<<endl;
#endif
#if (DEBUG_MODE)
	std::cout<<"The times:"<<endl;
	std::cout<<"Detection Time: "<<detectionTime<<" ms"<<endl;
	std::cout<<"Extraction Time: "<<extractionTime<<" ms"<<endl;
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

	std::cout<< "Matches: " << matches.size()<<endl;

	//Image created for drawing
	for (unsigned int i = 0; i < matches.size(); ++i)
	{
		matches[i].first.clusterIndex = i;
		matches[i].second.clusterIndex = i;
		drawPoint(imgYUV1,matches[i].first);
		drawPoint(imgYUV2,matches[i].second);

		const int & w = imgYUV1->width;
		cvLine(imgYUV1,cvPoint(matches[i].first.x,matches[i].first.y),cvPoint(matches[i].second.x+w,matches[i].second.y), cvScalar(0,128,0),1.5);
		cvLine(imgYUV2,cvPoint(matches[i].first.x-w,matches[i].first.y),cvPoint(matches[i].second.x,matches[i].second.y), cvScalar(0,128,0),1.5);
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
