

#include "surf.h"


//-------------------------------------------------------
//! SURF priors (these need not be done at runtime)
const float pi = 3.14159f;


//-------------------------------------------------------

//! Constructor
Surf::Surf(IplImage *img, IpVec &ipts)
: ipts(ipts)
{
   this->img = img;
}

//-------------------------------------------------------


//! Describe all features in the supplied vector
void Surf::getHorizonDescriptors()
{
   // Check there are Ipoints to be described
   if (!ipts.size()) return;

   // Get the size of the vector for fixed loop bounds
   int ipts_size = (int)ipts.size();

   for (int i = 0; i < ipts_size; ++i)
   {
      // Set the Ipoint to be described
      index = i;
      getHorizonDescriptor();
   }

}

//-------------------------------------------------------


//! Our modified descriptor.
inline void Surf::getHorizonDescriptor()
{
   int x, sample_x, count=0;
   float scale, *desc, dx, mdx;
   float rx = 0.f, len = 0.f;

   Ipoint *ipt = &ipts[index];
   scale = ipt->scale;
   x = fRound(ipt->x); 
   desc = ipt->descriptor;


	for(int subregion = -1; subregion<2; subregion++){

		dx=mdx=0.f;

		for(int sample = 0; sample < SUBSAMPLES; sample++){
			sample_x = fRound(x + (subregion*SUBSAMPLES*scale + (sample-(SUBSAMPLES/2))*scale) );
			//Get the haar wavelet responses 
			rx = haar(sample_x, 2*fRound(scale));
         dx += rx;
         mdx += fabs(rx);
		}
		desc[count++] = dx;
      desc[count++] = mdx;
	
      len += (dx*dx + mdx*mdx);
	}

   //Convert to Unit Vector
   len = sqrt(len);
   for(int i = 0; i < DESCRIPTOR_LENGTH; ++i){
      desc[i] /= len;
	}
}



//-------------------------------------------------------


//! Calculate Haar wavelet responses in x direction (1D only)
inline float Surf::haar(int column, int s)
{
   return BoxIntegral(img, column, s/2) 
      -1.f * BoxIntegral(img, column-s/2, s/2);
}

//-------------------------------------------------------

