

#include "integral.h"
#include "ipoint.h"

#include <vector>

#include "responselayer.h"
#include "fasthessian.h"


using namespace std;

#define IMAGE_WIDTH 640

//-------------------------------------------------------


//! Constructor with image and y_coord vector (for 1D SURF)
FastHessian::FastHessian(std::vector<int> &y_coords, IplImage *img, std::vector<Ipoint> &ipts, 
                         const int octaves, const int intervals, const int init_sample, 
                         const float thresh) 
                         : ipts(ipts), i_width(0)
{
   // Save parameter set
   saveParameters(octaves, intervals, init_sample, thresh);
	this->y_coords = y_coords;
	
   // Set the current image
   this->img = img;
   i_width = img->width;

}

//-------------------------------------------------------


FastHessian::~FastHessian()
{
   for (unsigned int i = 0; i < responseMap.size(); ++i)
   {
      delete responseMap[i];
   }
}

//-------------------------------------------------------

//! Save the parameters
void FastHessian::saveParameters(const int octaves, const int intervals, 
                                 const int init_sample, const float thresh)
{
   // Initialise variables with bounds-checked values
   this->octaves = 
      (octaves > 0 && octaves <= 4 ? octaves : OCTAVES);
   this->intervals = 
      (intervals > 0 && intervals <= 4 ? intervals : INTERVALS);
   this->init_sample = 
      (init_sample > 0 && init_sample <= 6 ? init_sample : INIT_SAMPLE);
   this->thresh = (thresh >= 0 ? thresh : THRES);
}


//-------------------------------------------------------

//! Find the image features and write into vector of features
void FastHessian::getIpoints()
{
   // filter index map
   static const int filter_map [OCTAVES][INTERVALS] = {{0,1,2,3}, {1,3,4,5}, {3,5,6,7}, {5,7,8,9}, {7,9,10,11}};

   // Clear the vector of exisiting ipts
   ipts.clear();

   // Build the response map
	buildResponseMap();

   // Get the response layers
   ResponseLayer *b, *m, *t;
   for (int o = 0; o < octaves; ++o) for (int i = 0; i <= 1; ++i)
   {
      b = responseMap.at(filter_map[o][i]);
      m = responseMap.at(filter_map[o][i+1]);
      t = responseMap.at(filter_map[o][i+2]);

      // loop over middle response layer at density of the most 
      // sparse layer (always top), to find maxima across scale and space
      for (int c = 0; c < t->width; ++c)
      {
         if (isExtremum(c, t, m, b))
         {
            saveExtremum(c, t, m, b);
         }
      }
   }
}

//-------------------------------------------------------

//! Build map of DoH responses
void FastHessian::buildResponseMap()
{
   // Calculate responses for the first 4 octaves:
   // Oct1: 9,  15, 21, 27
   // Oct2: 15, 27, 39, 51
   // Oct3: 27, 51, 75, 99
   // Oct4: 51, 99, 147,195
   // Oct5: 99, 195,291,387

   // Deallocate memory and clear any existing response layers
   for(unsigned int i = 0; i < responseMap.size(); ++i)  
      delete responseMap[i];
   responseMap.clear();

   // Get image attributes
   int w = (i_width / init_sample);
   int s = (init_sample);

   // Calculate approximated determinant of hessian values
   if (octaves >= 1)
   {
      responseMap.push_back(new ResponseLayer(w,   s,   9));
      responseMap.push_back(new ResponseLayer(w,   s,   15));
      responseMap.push_back(new ResponseLayer(w,   s,   21));
      responseMap.push_back(new ResponseLayer(w,   s,   27));
   }
 
   if (octaves >= 2)
   {
      responseMap.push_back(new ResponseLayer(w/2, s*2, 39));
      responseMap.push_back(new ResponseLayer(w/2, s*2, 51));
   }

   if (octaves >= 3)
   {
      responseMap.push_back(new ResponseLayer(w/4, s*4, 75));
      responseMap.push_back(new ResponseLayer(w/4, s*4, 99));
   }

   if (octaves >= 4)
   {
      responseMap.push_back(new ResponseLayer(w/8, s*8, 147));
      responseMap.push_back(new ResponseLayer(w/8, s*8, 195));
   }

   if (octaves >= 5)
   {
      responseMap.push_back(new ResponseLayer(w/16, s*16, 291));
      responseMap.push_back(new ResponseLayer(w/16, s*16, 387));
   }

   // Extract responses from the image
   for (unsigned int i = 0; i < responseMap.size(); ++i)
   {
      buildHorizonResponseLayer(responseMap[i]);
   }
}

//-------------------------------------------------------


//! Calculate DoH responses for supplied hoirzlayer
void FastHessian::buildHorizonResponseLayer(ResponseLayer *rl)
{
   float *responses = rl->responses;        // response storage
   unsigned char *laplacian = rl->laplacian; // laplacian sign storage
   int step = rl->step;                      // step size for this filter
   int b = (rl->filter - 1)/2 + 1;         	// border for this filter
   int l = rl->filter / 3;                   // lobe for this filter (filter size / 3)
   int w = rl->filter;                       // filter size
   float inverse_area = 1.f/w;           		// normalisation factor
   float Dxx;

	int c, index = 0;

   for(int ac = 0; ac < rl->width; ++ac, index++) 
   {
      // get the image coordinates
      c = ac * step; 

      // Compute response components
		Dxx = BoxIntegral(img, c - b, w)
         - BoxIntegral(img, c - l / 2, l)*3;

      // Normalise the filter responses with respect to their size
      Dxx *= inverse_area;
   
      // Get the determinant of hessian response & laplacian sign
      responses[index] = (Dxx * Dxx);
      laplacian[index] = (Dxx >= 0 ? 1 : 0);

   }

}


//-------------------------------------------------------

//! Non Maximal Suppression function
int FastHessian::isExtremum(int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b)
{

   // bounds check
   int layerBorder = (t->filter + 1) / (2 * t->step);
	if(c <= layerBorder || c >= t->width - layerBorder){
    	return 0;
   } 

   // check the candidate point in the middle layer is above thresh 
   float candidate = m->getResponse(c, t);
   if (candidate < thresh){ 
      return 0; 
   }
	
	if (m->getResponse(c-1, t)  >= candidate) return 0;
	if (m->getResponse(c+1, t)  >= candidate) return 0;
   return 1;

}

//-------------------------------------------------------

void FastHessian::saveExtremum(int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b)
{
   // get the step distance between filters
   // check the middle filter is mid way between top and bottom
   int filterStep = (m->filter - b->filter);
   assert(filterStep > 0 && t->filter - m->filter == m->filter - b->filter);
 	
   // Get the offsets to the actual location of the extremum
   double xi = 0, xr = 0, xc = 0;

	// If point is sufficiently close to the actual extremum 
	if( fabs( xi ) < 0.5f  &&  fabs( xr ) < 0.5f  &&  fabs( xc ) < 0.5f )
	{
	   Ipoint ipt;
	   ipt.x = static_cast<float>((c + xc) * t->step);
		ipt.y = y_coords.at(ipt.x); 
		ipt.scale = static_cast<float>((0.1333f) * (m->filter + xi * filterStep));
	   ipt.laplacian = static_cast<int>(m->getLaplacian(c,t));
	   ipts.push_back(ipt);
		return;
	}
	return;
}


