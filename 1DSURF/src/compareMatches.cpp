

#include "surflib.h"

#include <cv.h>
#include <highgui.h>

#include <limits>
#include <ctime>
#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <fstream>
#include "DataAnalysis.h"
#include <ctime>
#include "config_file.h"

#define DEBUG_TIMES 0
using namespace std;



template <class T>
inline std::string to_string (const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

struct TestData{
	bool match;
	float score;
	float tpr;
	float fpr;

	// needed for sorting
	bool operator<(TestData other) const{
		return score < other.score;
	}

};



//! Round float to nearest integer
inline int fRound(float flt)
{
	return (int) floor(flt+0.5f);
}


//! Draw a matched feature on the matching curve
void drawMatch(IplImage *img, std::pair<Ipoint, Ipoint> &match, int width, bool visual)
{
	float s = 5;
	int r1, c1;

	r1 = fRound(match.first.x*SUBSAMPLE);
	c1 = fRound(match.second.x*SUBSAMPLE);

	cvCircle(img, cvPoint(r1,c1), fRound(s), cvScalar(255,0,0), -1);
	cvCircle(img, cvPoint(r1,c1), fRound(s+1), cvScalar(255,255,0), 2);

}

//-------------------------------------------------------


float visualStaticMatch(TestImage image1, TestImage image2, bool visual, IplImage* &half)
{
	// Get matches
	IpPairVec matches;
	//	double time = (double)cvGetTickCount();
	Score result = getMatchesRANSAC(image1.ipts,image2.ipts,matches);
	float	match_score = result.score;
	//	double ttmatch = (double)cvGetTickCount() - time;

	if(visual){

		IplImage* img1 = image1.img;
		IplImage* img2 = image2.img;
		// Display the horizon lines on the images
		int integral_height = WIDTH;

		//Draws the horizon lines
		cvLine(img1, cvPoint(0, image1.left+WIDTH/2),cvPoint(img1->width, image1.right+WIDTH/2), cvScalar(0, 0, 255),1);
		cvLine(img1, cvPoint(0, image1.left-WIDTH/2),cvPoint(img1->width, image1.right-WIDTH/2), cvScalar(0, 0, 255),1);
		cvLine(img2, cvPoint(0, image2.left+WIDTH/2),cvPoint(img2->width, image2.right+WIDTH/2), cvScalar(0, 0, 255),1);
		cvLine(img2, cvPoint(0, image2.left-WIDTH/2),cvPoint(img2->width, image2.right-WIDTH/2), cvScalar(0, 0, 255),1);

		// Copy the original images to the output
		IplImage* visual = cvCreateImage( cvSize(img1->width + img2->height + integral_height,
				img2->width + img1->height + integral_height), img1->depth, img1->nChannels );
		cvZero(visual);
		cvSetImageROI( visual, cvRect( img2->height + integral_height, img2->width+ integral_height, img1->width, img1->height ) );
		cvCopy( img1, visual );
		cvSetImageROI( visual, cvRect( 0, 0, img2->height, img2->width ) );

		cvFlip(img2, NULL, 0);
		//cvFlip(img2, NULL, 1);
		cvTranspose(img2, visual);
		//cvFlip(img2, NULL, 1);
		cvFlip(img2, NULL, 0);

		// Display the integral image above each image
		std::vector<int> y_coords;
		IplImage *int_img1 = getGrayHorizon(img1, image1.left, image1.right, y_coords, WIDTH, 1, true);
		IplImage *im8 = cvCreateImage(cvSize(int_img1->width, int_img1->height), visual->depth, int_img1->nChannels);
		cvConvertScale(int_img1, im8, 255.);
		IplImage *colint_img1 = cvCreateImage( cvSize(int_img1->width, int_img1->height), visual->depth, visual->nChannels );
		cvCvtColor( im8, colint_img1, CV_GRAY2BGR );
		cvSetImageROI( visual, cvRect( img2->height + integral_height, img2->width, int_img1->width, integral_height ) );
		cvResize(colint_img1, visual);
		cvReleaseImage(&int_img1);
		cvReleaseImage(&im8);
		cvReleaseImage(&colint_img1);

		IplImage *int_img2 = getGrayHorizon(img2, image2.left, image2.right, y_coords, WIDTH, 1, true);
		im8 = cvCreateImage(cvSize(int_img2->width, int_img2->height), visual->depth, int_img2->nChannels);
		cvConvertScale(int_img2, im8, 255.);
		IplImage *colint_img2 = cvCreateImage( cvSize(int_img2->width, int_img2->height), visual->depth, visual->nChannels );
		cvCvtColor( im8, colint_img2, CV_GRAY2BGR );
		cvSetImageROI( visual, cvRect( img2->height , 0, integral_height, int_img2->width  ) );

		IplImage *transpose = cvCreateImage( cvSize(colint_img2->height, colint_img2->width), colint_img2->depth, colint_img2->nChannels );
		cvFlip(colint_img2, NULL, 0);
		//cvFlip(colint_img2, NULL, 1);
		cvTranspose(colint_img2, transpose);
		cvResize(transpose, visual);

		cvReleaseImage(&transpose);
		cvReleaseImage(&int_img2);
		cvReleaseImage(&im8);
		cvReleaseImage(&colint_img2);

		// Draw match points
		cvSetImageROI( visual, cvRect( img2->height+integral_height, 0, img1->width, img2->width ) );
		cvSet(visual,cvScalar(255, 255, 255));
		for (unsigned int i = 0; i < matches.size(); ++i){
			drawMatch(visual, matches[i], img2->width, visual);
		}

		// Display text
		cvSetImageROI( visual, cvRect( 0 , img2->width, img2->height + integral_height, img2->height + integral_height  ) );
		char text[200];
		CvFont font;
		cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX, 0.9,0.9,0,2);
		sprintf(text,"Features: %d/%d",(int)image2.ipts.size(),(int)image1.ipts.size());
		cvPutText (visual, text, cvPoint(20,50), &font, cvScalar(255,255,255));
		sprintf(text,"Valid matches: %d",(int)matches.size());
		cvPutText (visual, text, cvPoint(20,100), &font, cvScalar(255,255,255));
		sprintf(text,"Recognition score: %.2f",match_score);
		cvPutText (visual, text, cvPoint(20,150), &font, cvScalar(255,255,255));
		sprintf(text,"Extraction time: %.2fms",Ipoint::extractionTime);//image2.tt_extraction, image1.tt_extraction);
		cvPutText (visual, text, cvPoint(20,200), &font, cvScalar(255,255,255));
		sprintf(text,"Matching time: %.2fms",Ipoint::detectionTime);
		cvPutText (visual, text, cvPoint(20,250), &font, cvScalar(255,255,255));

		// Resize and Show output
		cvResetImageROI( visual );
		half = cvCreateImage( cvSize(visual->width/2, visual->height/2), visual->depth, visual->nChannels );
		cvResize(visual, half);
		cvReleaseImage(&visual);

	}
	return match_score;
}


//-------------------------------------------------------

int main(int argc, char *argv[])
{

	//Determine whether or not you should show the matches and print the ROC curve
	bool visible = true;
	if (argc == 2){
		string arg(argv[1]);
		if (arg == "-visual") visible = true;
	}

	//Create a vector of the test images
	vector<TestImage> test_images;

	string filename;
	int left = 120;
	int right = 120;
	int direction = 0;


	vector<TestData> classification;


	if(visible){
		cvNamedWindow("1D SURF", CV_WINDOW_AUTOSIZE );
		cvMoveWindow("1D SURF", 0, 0);
	}

	//At this point, all of the images have been loaded. They now need to be broken into
	//the various datasets


	//*****************MATCHING*********************************
	//The number of images in each dataset
	int jpegCounter = 0;
	int jpegCounter1 = 0;

	//Use the terminal or not
	bool terminal = true;

	//Set the date and time
	string myDate = "10082012";
	string myTime = "1028";


	//Matching parameters
	int k_start =0;
	int k_end = 0;
	int s_start = 0;
	int s_end = 0;

	int index = 0;

	//NB *************************************
	//CHOOSE THE DATASET TO USE
	//Counters used to ensure standard output for processing in Matlab
	int tempDirCounterkk = 0;
	int tempDirCounterss = 0;
	datasetType Type  = MAIN_ROBOCUP_TESTING_DATASETS;
	//Set if you are matching or not matching
	bool isMatching = true;
	//	MAIN_ROBOCUP_TESTING_DATASETS
	//	OFFICE_ENVIRONMENT_DATASETS
	//	LARGE_HALL_ENVIRONMENT_DATASETS
	if(isMatching)
	{

		if (Type==MAIN_ROBOCUP_TESTING_DATASETS)//The original dataset
		{
			//Counters used to ensure standard output for processing in Matlab
			tempDirCounterkk = 1;
			tempDirCounterss = 1;

			k_start = 1;
			k_end = 4;
		}
		else if (Type==OFFICE_ENVIRONMENT_DATASETS)//Additional datasets
		{
			//Counters used to ensure standard output for processing in Matlab
			tempDirCounterkk = 5;
			tempDirCounterss = 5;

			k_start = 17;
			k_end = 18;
		}
		else if(Type==LARGE_HALL_ENVIRONMENT_DATASETS)
		{
			//Counters used to ensure standard output for processing in Matlab
			tempDirCounterkk = 5;
			tempDirCounterss = 5;

			k_start = 19;
			k_end = 20;
		}

	}
	else
	{
		if (Type==MAIN_ROBOCUP_TESTING_DATASETS)
		{
			//Counters used to ensure standard output for processing in Matlab
			tempDirCounterkk = 1;
			tempDirCounterss = 3;

			k_start = 1;
			k_end = 2;
			s_start = 3;
			s_end = 4;
		}
		else if (Type==OFFICE_ENVIRONMENT_DATASETS)
		{
			//Counters used to ensure standard output for processing in Matlab
			tempDirCounterkk = 5;
			tempDirCounterss = 6;

			k_start = 17;
			k_end = 17;
			s_start = 18;
			s_end = 18;
		}
		else if (Type == LARGE_HALL_ENVIRONMENT_DATASETS){

			//Counters used to ensure standard output for processing in Matlab
			tempDirCounterkk = 5;
			tempDirCounterss = 6;

			k_start = 19;
			k_end = 19;
			s_start = 20;
			s_end = 20;

		}
	}


	//Create object for dataAnalysis
	DataAnalysis dataAnalysis;



	//Set the various directories for matching and non-matching respectively
	for(int kk=k_start;kk<=k_end;kk++)
	{

		if(isMatching){
			//The counter must be reset for formatting purposes in Matlab

			if(isMatching){
				s_start = s_end = kk;
				tempDirCounterss = kk;//kk for matching
			}
			else{
				tempDirCounterss = 3;// 3 for non-matching
			}
		}

		for (int ss = s_start;ss<=s_end;ss++)
		{
			//The directory where the files are stored
			string dir, dir1;
			dir = to_string<int>(kk);
			dir1 = to_string<int>(ss);

			string fname1;
			string fname2;

			string name1;
			string name2;

			std::string tempDir = to_string<int>(tempDirCounterkk);
			std::string tempDir1 =to_string<int>(tempDirCounterss);

			//Set the directory names and determine the number of images in each directory
			jpegCounter = dataAnalysis.getNumImagesInDirectory(&dir, terminal);
			jpegCounter1 = dataAnalysis.getNumImagesInDirectory(&dir1, terminal);

			cout<<"The directory is: "<<dir<<endl;
			cout<<"The second directory is: "<<dir1<<endl;

			cout<<"The number of images in the directory 1,2 is: "<<jpegCounter<<", "<<jpegCounter1<<endl;

			//	MAIN_ROBOCUP_TESTING_DATASETS
			//	OFFICE_ENVIRONMENT_DATASETS
			//	LARGE_HALL_ENVIRONMENT_DATASETS
			//The file to store the matching data
			string filename = "data/Matches/";
			if (Type==MAIN_ROBOCUP_TESTING_DATASETS)
				filename.append("nonmatching_matching_Data__");
			else if (Type==OFFICE_ENVIRONMENT_DATASETS)
				filename.append("dataset2_nonmatching_matching_Data__");
			else if (Type==LARGE_HALL_ENVIRONMENT_DATASETS)
				filename.append("dataset3_nonmatching_matching_Data__");
			filename.append("SURF1D");
			filename.append("_Euclidean_");
			filename.append(myDate);
			filename.append("_");
			filename.append(myTime);
			filename.append("_");
			filename.append(to_string<double>(THRES));
			filename.append("_");
			filename.append("_given");
			filename.append(".txt");
			cout<<filename<<endl;

			//Remember that for non-matches, we can compare 1,1;2,2;3,3...etc
			//Determine matches without repetition
			for (int ii = 1;ii<=jpegCounter;ii++)
			{
				if(isMatching)
					index = ii;
				else
					index = jpegCounter1;

				for (int jj = 1; jj<index;jj++)
				{

					cout<<"Image "<<ii<<", image "<<jj<<endl;

					//The Processing the first image
					name1 = to_string<int>(ii);

					fname1 =dir + "/"+ name1+".jpg";

					//cout<<"Directory: "<<fname1<<endl;
					TestImage image1 = TestImage(cvLoadImage(fname1.c_str() ),left,right,direction,dir);
					//Compute the descriptors for image 1
					surfDetDes(image1);

					timespec times, timee;
					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &times);

					//Start processing the second image. This image will be timed.
					name2 = to_string<int>(jj);

					//Load the images from the dataset
					fname2 =dir1 + "/"+ name2+".jpg";
					//Assign the test images
					TestImage image2 = TestImage(cvLoadImage(fname2.c_str() ),left,right,direction,dir1);


					//Run the 1D SURF feature extraction algorithm
					//Compute the descriptors for image 2
					surfDetDes(image2);

					float result =0;
					IplImage* display;

					//Calculate the matches
					result = visualStaticMatch(image1, image2, visible, display);

					//cout<<result<<endl;

					clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timee);
					Ipoint::overallTime = Ipoint::diffSurf(times, timee).tv_nsec/1000000.0f;

					if(visible){
						cvShowImage("1D SURF", display);
						cvWaitKey(0);
						cv::imwrite("SURFtest/myImages/nonmatching.jpg",display);
						cvReleaseImage(&display);
					}
					//Write the data to a file
					ofstream writeFile;
					writeFile.open(filename.c_str(), ios::app);
#if (DEBUG_TIMES)
					cout<<"Total Matches: "<<Ipoint::totalNumMatches<<endl;
					cout<<"Num valid matches: "<<Ipoint::numValidMatches<<endl;

					cout<<"Detection Time: "<<Ipoint::detectionTime<<endl;
					cout<<"Extraction Time: "<<Ipoint::extractionTime<<endl;
					cout<<"Matching Time: "<<Ipoint::matchingTime<<endl;
					cout<<"Verification Time: "<<Ipoint::verificationTime<<endl;
					cout<<"Overall Time: "<<Ipoint::overallTime<<endl;
#endif

					writeFile <<tempDir<<", "<<tempDir1<<", "<<325<<", "<<name1<<", "<<name2<<", "<<image1.ipts.size()<<", "<<image2.ipts.size()<<", "<<result<<", "<<0<<", "<<Ipoint::totalNumMatches<<", "<<Ipoint::numValidMatches<<","<<Ipoint::totalNumMatches -Ipoint::numValidMatches<<", "<<0<<", "<<Ipoint::detectionTime<<", "<<Ipoint::extractionTime<<", "<<Ipoint::matchingTime<<", "<<Ipoint::verificationTime<<", "<<Ipoint::overallTime<<"\n";


					//close the file
					writeFile.close();
					writeFile.clear();

					cvReleaseImage(&(image1.img));
					cvReleaseImage(&(image2.img));
				}//End of jj
			}//end of ii
			tempDirCounterss++;
		}//End of ss
		tempDirCounterkk++;
	}//End of kk

	if(visible) cvDestroyWindow("1D SURF");

	return 0;
}
