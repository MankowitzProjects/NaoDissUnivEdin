

#ifndef IPOINT_H
#define IPOINT_H

#include <vector>
#include <math.h>
#include <limits>
#include <iostream>
#include <iomanip>
#include <cv.h>
#include <highgui.h>


#define DESCRIPTOR_LENGTH 6  
#define PIXEL_ERROR_MARGIN 10.f
#define ITERATIONS 20


//-------------------------------------------------------

struct Score{
	float score;
	float m;
	float b;
};


class Ipoint; // Pre-declaration
typedef std::vector<Ipoint> IpVec;
typedef std::vector<std::pair<Ipoint, Ipoint> > IpPairVec;

//-------------------------------------------------------

//! Ipoint operations
Score getMatchesRANSAC(IpVec &ipts1, IpVec &ipts2, IpPairVec &matches);

//-------------------------------------------------------

class Ipoint {

   //! Print an interest point descriptor
   friend  std::ostream& operator << (std::ostream& os, const Ipoint &p){
      os << p.laplacian << "\t";
      os << setiosflags(std::ios::fixed) << std::setprecision(3);
      for(int i=0; i < DESCRIPTOR_LENGTH; i++){
         os << p.descriptor[i] << "\t";
		}
      return os;
   };

public:

   //! Destructor
   ~Ipoint() {};

   //! Constructor
   Ipoint() {};

   //! Gets the distance in descriptor space between Ipoints
   float operator-(const Ipoint &rhs)
   {
      if(this->laplacian-rhs.laplacian !=0)return std::numeric_limits<float>::max(); // can't be match
      float sum=0.f;
      for(int i=0; i < DESCRIPTOR_LENGTH; ++i){
         sum += (this->descriptor[i] - rhs.descriptor[i])*(this->descriptor[i] - rhs.descriptor[i]);
		}
      return sqrt(sum);
   };


	//! Enables Ipoints to be sorted by location on x axis
   bool operator<(const Ipoint &rhs) const
   {
		if (this->x < rhs.x) return true;
		else return false;
   };
   //The timing function for the interest points
   inline static timespec diffSurf(timespec start, timespec end)
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

   //! Coordinates of the detected interest point
   float x, y;

   //! Detected scale
   float scale;

   //! Sign of laplacian for fast matching purposes (1 or 0)
   int laplacian;

   //! Vector of descriptor components
   float descriptor[DESCRIPTOR_LENGTH];

   //!The total number of matches before RANSAC
   static int totalNumMatches;

   //!The total number of valid matches after RANSAC
   static int numValidMatches;

   //!The time variables for each of the interest points
   static float detectionTime;
   static float extractionTime;
   static float verificationTime;
   static float matchingTime;
   static float overallTime;


};

//-------------------------------------------------------


#endif
