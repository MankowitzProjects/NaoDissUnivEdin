

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


using namespace std;


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
	double time = (double)cvGetTickCount();
	Score result = getMatchesRANSAC(image1.ipts,image2.ipts,matches);
	float	match_score = result.score;
	double ttmatch = (double)cvGetTickCount() - time;

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
		sprintf(text,"Extraction time: %.2fms/%.2fms",image2.tt_extraction, image1.tt_extraction);
		cvPutText (visual, text, cvPoint(20,200), &font, cvScalar(255,255,255));	
		sprintf(text,"Matching time: %.2fms",ttmatch/(cvGetTickFrequency()*1000.));
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
	bool visible = false;
	bool printROC = true;
	if (argc == 2){
		string arg(argv[1]);
		if (arg == "-visual") visible = true;
		if (arg == "-ROC") printROC = true;
	}

	//Read in the images
	string line;
	ifstream myfile ("SURFtest/dataset1/surf.data");

	//Create a vector of the test images
	vector<TestImage> test_images;
	double total_extraction = 0;
	double total_mapping = 0;
	double total_integral = 0;
	double total_interestpoints = 0;
	double total_descriptors = 0;
	double num_features = 0;


	if (myfile.is_open())
	{

		string filename;
		int left,right,direction;
		int counter = 0;
		//Read in the left right and direction parameters from the data file
		while ( myfile.good() )
		{
			myfile >> filename;
			myfile >> left >> right >> direction;
			//Initialise the image with the directions
			TestImage image = TestImage(cvLoadImage( ("SURFtest/dataset1/"+filename).c_str() ),left,right,direction,filename);

			//			cout<<"Index: "<<counter<<endl;
			//			cout<<"Image: "<<image.filename<<endl;
			//Start the timer
			double time = (double)cvGetTickCount();
			//Run the 1D SURF feature extraction algorithm
			surfDetDes(image);


			double maptime = (double)cvGetTickCount();
			double extraction = ((double)cvGetTickCount() - time)/(cvGetTickFrequency()*1000.f);
			double mapping = ((double)cvGetTickCount() - maptime)/(cvGetTickFrequency()*1000.f);

			if(image.ipts.size() == 0){
				cout << filename << " found "<< image.ipts.size()<< " interest points" << endl;
			}
			//Set the time
			image.tt_extraction = extraction;
			total_extraction += extraction;
			total_mapping += mapping;
			total_integral += image.tt_integral;
			total_interestpoints += image.tt_interestpoints;
			total_descriptors += image.tt_descriptors;
			num_features += image.ipts.size();
			if(counter==108) break; //To prevent an extra image being added
			test_images.push_back(image);

			counter = counter+1;
		}//End while

	} else {
		cout << "Unable to open file" << endl;
	}
	cout << endl << "Loaded " << test_images.size() << " test images" << endl;

	vector<TestData> classification;
	int pos = 0;
	double time = (double)cvGetTickCount();

	if(visible){
		cvNamedWindow("1D SURF", CV_WINDOW_AUTOSIZE );
		cvMoveWindow("1D SURF", 0, 0);
	}

	cout<<"Test Images size: "<<test_images.size()<<endl;
	int counter = 0;
	int tpCounter = 0;
	int fpCounter = 0;
	for(int i=1; i<=(int)test_images.size(); i++){
		for(int j=1; j<=(int)test_images.size(); j++){
			if(i>j) {

				//Dataset 1: To prevent LMG and RMG being compared and LOG and ROG being compared
								if((i>=27 && i<=54) && (j>=1 && j<=26)) continue;
								if((i>=86 && i<=108) && (j>=55 && j<=85)) continue;
				counter ++;

				TestImage test_i = test_images.at(i-1);
				TestImage test_j = test_images.at(j-1);
				int diff = 	min(test_i.direction - test_j.direction, test_j.direction - (test_i.direction-360));
				IplImage* display;
				if(diff <= 20){

					float result = visualStaticMatch(test_images.at(i-1), test_images.at(j-1), visible, display);
					//Indicates a true match if the angle difference is less than 20 degrees
					TestData data = {true, result, 0, 0};

						//printf("True: %d\t%d\t%.2f\n",i,j,result);

					tpCounter++;
					classification.push_back(data);
					if(visible){
						cvShowImage("1D SURF", display);
						cvWaitKey(0);

						cvReleaseImage(&display);
					}
					pos++;
				} else if (diff > 80){
					//if (i-1), (j-1) then we get dataset 6-->5 but we want dataset 5-->6
					float result = visualStaticMatch(test_images.at(j-1), test_images.at(i-1), visible, display);
					//Indicates a false match if the angle distance is more than 20 degrees
					//data = [match, score, tp rate, fp rate]
						//printf("False: %d\t%d\t%.2f\n",j,i,result);

					TestData data = {false, result, 0, 0};
					classification.push_back(data);
					fpCounter++;
					if(visible){
						cvShowImage("1D SURF", display);
						cvWaitKey(0);
						cvReleaseImage(&display);
					}
				}
			}
		}
	}
	cout<<"The number of matches are: "<<counter<<endl;
	cout<<"TP Matches: "<<tpCounter<<endl;
	cout<<"FP Matches: "<<fpCounter<<endl;

	if(visible) cvDestroyWindow("1D SURF");


	cout<<"Interest point time: "<<total_interestpoints<<endl;
	cout<<"Descriptor point time: "<<total_descriptors<<endl;
	cout<<"Test image size: "<<test_images.size()<<endl;

	double tt_matching = ((double)cvGetTickCount() - time)/(cvGetTickFrequency()*1000.);
	int neg = (int)classification.size()-pos;
	printf("Tested %d matching images and %d unmatched images\n",pos, neg);
	printf("Average extraction time %.3fms\n", total_extraction/test_images.size());
	printf("\tconstructing integral time %.3fms\n", total_integral/test_images.size());
	printf("\tfinding interest points time %.3fms\n", total_interestpoints/test_images.size());
	printf("\tcalculating descriptors time %.3fms\n", total_descriptors/test_images.size());
	printf("Average matching time %.3fms\n",tt_matching/classification.size());
	printf("Average features extracted: %.1f\n",num_features/test_images.size());

	//Sort the matches in reverse order since we want to create the ROC curve
	sort(classification.begin(), classification.end());
	reverse(classification.begin(), classification.end());


	double prev_tp = 0;
	double tp = 0;
	double prev_fp = 0;
	double fp = 0;
	double AUC = 0;
	if(printROC) printf("FPR\tTPR\tThreshold\n");
	for(int i=0; i<(int)classification.size(); i++){
		TestData data = classification.at(i);
		if(data.match) tp++;
		else fp++;
		if(i%20==0){
			if(printROC) printf("%.2f\t%.2f\t%.2f\n",fp/neg,tp/pos,data.score);
		}
		if(true){
			AUC += (fp/neg-prev_fp/neg)*(tp/pos+prev_tp/pos);
			prev_tp = tp;
			prev_fp = fp;
		}
	}
	cout<<"FP: "<<fp<<endl;
	cout<<"TP: "<<tp<<endl;
	AUC = AUC/2;
	printf("AUC = %f\n\n",AUC);
	return 0;
}
