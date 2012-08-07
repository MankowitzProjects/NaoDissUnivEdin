

#include <cv.h>
#include <vector>

#include <iostream>
#include "ipoint.h"
#include "math.h"

#define RUNSWIFT 0
int Ipoint::totalNumMatches = 0;
int Ipoint::numValidMatches = 0;
float Ipoint::extractionTime = 0;
float Ipoint::detectionTime = 0;
float Ipoint::verificationTime = 0;
float Ipoint::matchingTime = 0;
float Ipoint::overallTime = 0;

//! Populate IpPairVec with matched ipts using nearest neighbour and RANSAC
Score getMatchesRANSAC(IpVec &ipts1, IpVec &ipts2, IpPairVec &matches)
{
#if RUNSWIFT
#else
	timespec matchings, matchinge, verifys, verifye;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchings);
#endif
	float dist, d1, d2;
	Ipoint *match;
	float matching_score = 0;

	matches.clear();

	for(unsigned int i = 0; i < ipts1.size(); i++)
	{
		ipts1[i].x = ipts1[i].x;
		d1 = d2 = FLT_MAX;
		match = &ipts2[0]; // to avoid unitialized warning

		for(unsigned int j = 0; j < ipts2.size(); j++)
		{
			ipts2[j].x = ipts2[j].x;
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

		// If match has a d1:d2 ratio < 0.75 ipoints are a match
		if(d1/d2 < 0.75)
		{
			// Store the match
			matches.push_back(std::make_pair(ipts1[i], *match));
			//Increment the matching score
			matching_score += 1/d1;
		}
	}

	float best_score = matching_score;
	float best_b = -1;
	float best_m = -1;
#if RUNSWIFT
#else
	Ipoint::totalNumMatches = matches.size();
	//At this point we have the total matches before the final number of matches
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &verifys);
#endif

	if(matches.size()>1){

		best_score = 0;

		for(int i=0; i<ITERATIONS; i++){
			//Choose random matches
			int pos1 = rand() % (int)matches.size();
			int pos2 = rand() % (int)matches.size();
			while(pos1 == pos2) {
				//Make sure that both matches are different
				pos2 = rand() % (int)matches.size();
			}
			//Should generate a positive value
			float m = (matches.at(pos2).second.x - matches.at(pos1).second.x)/(matches.at(pos2).first.x - matches.at(pos1).first.x);
			//If a gradient is discarded
			if (m <= 0){
				continue;
			}
			//Calculate the translation component
			float b = matches.at(pos2).second.x - m*matches.at(pos2).first.x;
			float score = 0;
			for(int j=0; j<(int)matches.size(); j++){
				//Calculate the function x_stored,i = b_s * x_test,i + b_d
				if( fabs(matches.at(j).second.x - (m*matches.at(j).first.x + b)) < PIXEL_ERROR_MARGIN)
					score += 1/fabs(matches.at(j).first - matches.at(j).second);
			}
			if (score > best_score){
				best_score = score;
				best_b = b;
				best_m = m;
			}	
		}
	}

	// Now remove all matches who are not within this pixel error margin
	//if(best_m > 0){
	for(int j=0; j<(int)matches.size(); j++){
		if( fabs(matches.at(j).second.x - (best_m*matches.at(j).first.x + best_b)) >= PIXEL_ERROR_MARGIN) {
			matches.erase(matches.begin() + j);
			j--;
		}
	}
	//}

	Score score = {best_score, best_m, best_b};
#if RUNSWIFT
#else
	Ipoint::numValidMatches = matches.size();

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &verifye);
	Ipoint::verificationTime = Ipoint::diffSurf(verifys,verifye).tv_nsec/1000000.0f;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchinge);
	Ipoint::matchingTime = Ipoint::diffSurf(matchings,matchinge).tv_nsec/1000000.0f;
#endif
	return score;
}



