/*********************************************************** 
 *  --- OpenSURF ---                                       *
 *  This library is distributed under the GNU GPL. Please   *
 *  use the contact form at http://www.chrisevansdev.com    *
 *  for more information.                                   *
 *                                                          *
 *  C. Evans, Research Into Robust Visual Features,         *
 *  MSc University of Bristol, 2008.                        *
 *                                                          *
 ************************************************************/

#include <cv.h>
#include <vector>
#include <iostream>
#include <iomanip>

#include "ipoint.h"

//! Populate IpPairVec with matched ipts 
void getMatches(IpVec &ipts1, IpVec &ipts2, IpPairVec &matches, int method)
{
	float dist, d1, d2;
	Ipoint *match;

	matches.clear();

	//Get the sizes of the interest point vectors
	int ip1Size = ipts1.size();
	int ip2Size = ipts2.size();

	//Declare a matrix to store the matching scores between features
	std::vector<std::vector<float> > Eij(ip1Size,vector<float>(ip2Size,0));
	std::vector<std::vector<int> > Mij(ip1Size,vector<int>(ip2Size,0));



	if(method==0)
	{
		for(unsigned int i = 0; i < ipts1.size(); i++)
		{
			d1 = d2 = FLT_MAX;

			for(unsigned int j = 0; j < ipts2.size(); j++)
			{
				dist = ipts1[i] - ipts2[j];

				if(dist<d1) // if this feature matches better than current best
				{
					d2 = d1;
					d1 = dist;
					match = &ipts2[j];
				}
				else if(dist<d2) // this feature matches better than second best
				{
					d2 = dist;
				}
			}

			// If match has a d1:d2 ratio < 0.65 ipoints are a match
			if(d1/d2 < 0.7)
			{
				// Store the change in position
				ipts1[i].dx = match->x - ipts1[i].x;
				ipts1[i].dy = match->y - ipts1[i].y;
				matches.push_back(std::make_pair(ipts1[i], *match));
			}
		}
	}else if(method==1){
		//Initialise the 0 indices to 0 as the feature vectors are assumed to be 1-indexed
		//First initialise all rows, column 1 to 0
		for (int ii=0;ii<ip1Size;ii++){
			Eij[ii][0] = 0;
			std::cout<<"Index and value: "<<ii<<", "<<Eij[ii][0]<<endl;
		}
		//Then initialise all columns, row 0 to 0
		for (int jj=0;jj<ip2Size;jj++){
			Eij[0][jj] = 0;
			std::cout<<"Index and value: "<<jj<<", "<<Eij[0][jj]<<endl;
		}

		//Maximise the matching score by considering all combinations of matches
		float maxscoreij = 0;
		float matchingScore = 0;
		int numMatches = 0;

		for (int ii = 1;ii<ip1Size;ii++)
		{
			for (int jj = 1;jj<ip2Size;jj++)
			{
				//Calculate the matching score between the current pair of feature vectors
				float scoreij = 1/(ipts1[ii] - ipts2[jj]);
				if(scoreij>maxscoreij)
					maxscoreij = scoreij;

//				cout<<"Below: "<<Eij[ii-1][jj]<<endl;
//				cout<<"Left: "<<Eij[ii][jj-1]<<endl;
//				cout<<"With Score: "<<(Eij[ii-1][jj-1] + scoreij)<<endl;

				if(Eij[ii-1][jj]>Eij[ii][jj-1] && Eij[ii-1][jj] > (Eij[ii-1][jj-1] + scoreij))
				{
					//The maximum is vertically below the current feature pairing
					Eij[ii][jj] = Eij[ii-1][jj];
					Mij[ii][jj] = 3;
				}else if(Eij[ii][jj-1]>Eij[ii-1][jj] && Eij[ii][jj-1] > (Eij[ii-1][jj-1] + scoreij))
				{
					//The maximum is horizontally to the left of the current feature pairing
					Eij[ii][jj] = Eij[ii][jj-1];
					Mij[ii][jj] = 2;
				}else if ((Eij[ii-1][jj-1] + scoreij) >Eij[ii-1][jj] &&  (Eij[ii-1][jj-1] + scoreij) > Eij[ii][jj-1])
				{
					//The maximum is diagonally away from the current feature pairing
					Eij[ii][jj] = Eij[ii-1][jj-1] + scoreij;
					Mij[ii][jj] = 1;
					//matches.push_back(std::make_pair(ipts1[ii], ipts2[jj]));
					//matchingScore = matchingScore + Eij[ii][jj];

				}
			}
		}

		cout<<"The max score is: "<<maxscoreij<<endl;

		for ( int i = 0; i < 50; i++ ) {
		for ( int j = 0; j < 50; j++ )
		cout<< setw ( 3 ) << Mij[i][j] <<' ';
		cout<<'\n';
		}

		//Now to extract the optimum feature matching sequence
		int ip1Index = ip1Size-1;
		int ip2Index = ip2Size-1;
		matchingScore = 0;
		while(ip1Index!=0 && ip2Index!=0){

			switch(Mij[ip1Index][ip2Index]){

			case 1:
				//ip1Index matches ip2Index
				matches.push_back(std::make_pair(ipts1[ip1Index], ipts2[ip2Index]));
				cout<<"The score is: "<<1/(ipts1[ip1Index] - ipts2[ip2Index])<<endl;
				ip1Index--; ip2Index--;
				matchingScore = matchingScore + 1/(ipts1[ip1Index] - ipts2[ip2Index]);
				numMatches++;
				break;
			case 2:
				//ip2Index is unmatched
				ip2Index--;
				break;
			case 3:
				//ip1Index is unmatched
				ip1Index--;
				break;
			}
		}

		cout<<"The total matching score is: "<<matchingScore<<endl;
		cout<<"The number of matches is: "<<numMatches<<endl;

	}
}

//
// This function uses homography with CV_RANSAC (OpenCV 1.1)
// Won't compile on most linux distributions
//

//-------------------------------------------------------

//! Find homography between matched points and translate src_corners to dst_corners
int translateCorners(IpPairVec &matches, const CvPoint src_corners[4], CvPoint dst_corners[4])
{
#ifndef LINUX
	double h[9];
	CvMat _h = cvMat(3, 3, CV_64F, h);
	std::vector<CvPoint2D32f> pt1, pt2;
	CvMat _pt1, _pt2;

	int n = (int)matches.size();
	if( n < 4 ) return 0;

	// Set vectors to correct size
	pt1.resize(n);
	pt2.resize(n);

	// Copy Ipoints from match vector into cvPoint vectors
	for(int i = 0; i < n; i++ )
	{
		pt1[i] = cvPoint2D32f(matches[i].second.x, matches[i].second.y);
		pt2[i] = cvPoint2D32f(matches[i].first.x, matches[i].first.y);
	}
	_pt1 = cvMat(1, n, CV_32FC2, &pt1[0] );
	_pt2 = cvMat(1, n, CV_32FC2, &pt2[0] );

	// Find the homography (transformation) between the two sets of points
	if(!cvFindHomography(&_pt1, &_pt2, &_h, CV_RANSAC, 5))  // this line requires opencv 1.1
		return 0;

	// Translate src_corners to dst_corners using homography
	for(int i = 0; i < 4; i++ )
	{
		double x = src_corners[i].x, y = src_corners[i].y;
		double Z = 1./(h[6]*x + h[7]*y + h[8]);
		double X = (h[0]*x + h[1]*y + h[2])*Z;
		double Y = (h[3]*x + h[4]*y + h[5])*Z;
		dst_corners[i] = cvPoint(cvRound(X), cvRound(Y));
	}
#endif
	return 1;
}


