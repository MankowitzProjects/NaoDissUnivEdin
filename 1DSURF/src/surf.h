

#ifndef SURF_H
#define SURF_H

#include <cv.h>
#include <vector>
#include "ipoint.h"
#include "integral.h"

#define SUBSAMPLES 5


using namespace std;

class Surf {
  
  public:
    
    //! Standard Constructor (img is an integral image)
    Surf(IplImage *img, std::vector<Ipoint> &ipts);

	 //! Describe all features in the supplied vector
    void getHorizonDescriptors();
  
  private:
    
    //---------------- Private Functions -----------------//
   
    //! Our modified descriptor
    void getHorizonDescriptor();

    //! Calculate Haar wavelet response
	 inline float haar(int column, int size);

	 //! Round float to nearest integer
	 inline int fRound(float flt)
	 {
		 return (int) floor(flt+0.5f);
	 }


    //---------------- Private Variables -----------------//

    //! Integral image where Ipoints have been detected
    IplImage *img;

    //! Ipoints vector
    IpVec &ipts;

    //! Index of current Ipoint in the vector
    int index;
};


#endif
