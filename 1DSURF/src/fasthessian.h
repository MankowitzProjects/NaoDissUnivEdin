

#ifndef FASTHESSIAN_H
#define FASTHESSIAN_H

#include <cv.h>
#include "ipoint.h"

#include <vector>


//-------------------------------------------------------


class ResponseLayer;
static const int WIDTH = 30;
static const int OCTAVES = 5;
static const int INTERVALS = 4;
static float THRES = 325.125f;
static const int INIT_SAMPLE = 1;


class FastHessian {
  
  public:

		//! Constructor with image and y_coords for 1D
    FastHessian(std::vector<int> &y_coords,
								IplImage *img, 
                std::vector<Ipoint> &ipts,                 
								const int octaves = OCTAVES, 
                const int intervals = INTERVALS, 
                const int init_sample = INIT_SAMPLE, 
                const float thres = THRES);

    //! Destructor
    ~FastHessian();

    //! Save the parameters
    void saveParameters(const int octaves, 
                        const int intervals,
                        const int init_sample, 
                        const float thres);

    //! Find the image features and write into vector of features
    void getIpoints();
    
  private:

    //---------------- Private Functions -----------------//

    //! Build map of DoH responses
    void buildResponseMap();

    //! Calculate DoH responses for supplied Horizon layer
    void buildHorizonResponseLayer(ResponseLayer *r);

    //! 3x3 Extrema test
    int isExtremum(int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b);    
    
    //! Save extremum point
    void saveExtremum(int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b);
    

    //---------------- Private Variables -----------------//

	 //! Reference to vector of features passed from outside 
    std::vector<Ipoint> &ipts;

    //! Pointer to the integral Image, and its attributes 
    IplImage *img;
    int i_width;

    //! Response stack of determinant of hessian values
    std::vector<ResponseLayer *> responseMap;

    //! Number of Octaves
    int octaves;

    //! Number of Intervals per octave
    int intervals;

    //! Initial sampling step for Ipoint detection
    int init_sample;

    //! Threshold value for blob resonses
    float thresh;

	 //! Y Coordinates of features (for when doing on 1D pixels)
	 std::vector<int> y_coords;
};


#endif
