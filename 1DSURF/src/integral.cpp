

#include "integral.h"



// Convert horizon of image to single channel 32F
IplImage *getGrayHorizon(const IplImage *img, int left_horizon, int right_horizon, std::vector<int> &y_coords, int horizon_width, int subsample, bool divide)
{
   assert (left_horizon >= 0 && left_horizon < img->height);
	assert (right_horizon >= 0 && right_horizon < img->height);

   IplImage* result = cvCreateImage( cvSize(img->width / subsample,1), IPL_DEPTH_32F, 1 );  

	// This is Bresenham's line algorithm

   //Set y0 as the horizon coordinate on the left side of the image
   //Set y1 as the horizon coordinate on the right side of the image
   int x0 = 0;
   int y0 = left_horizon;
	int x1 = img->width-1;
   int y1 = right_horizon;

   //Find the difference between each of the coordinates for the gradient
   int dx = abs(x1-x0);
   int dy = abs(y1-y0); 

   int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
   int err = dx-dy; 

   while (1){
		//In my case, row pixel 300 is placed here
		y_coords.push_back(y0); // remember y coords for later
		int channels = 3;	
		unsigned char *data = (unsigned char *) img->imageData;
		//For moving down rows correctly
		int step = img->widthStep/(sizeof(unsigned char));

		// Get pixel average if at correct subsample frequency
      if(x0 % subsample == 0){
		   float average = 0.0;
		   int n=0;
		   //Takes the center of the horizon and 15 pixels above and below
		   for (int j=-horizon_width/2; j<=horizon_width/2; j++){	//-(horizon_width/2)
			   //Keep adding j to increase the row position
			   if( (y0 + j) >=0 && (y0 + j) < img->height){

				   //Get the new row position that depends on j and the number of channels
				   int position = (y0+j)*step + x0*channels;
				   //I assume this computes the BGR average for a single pixel along all three colour channels
				   //to get the Luma channel (Y). This will create a set of grayscale pixels
				   average += 	(0.114f*data[position] + 0.587f*data[position+1] + 0.299f*data[position+2]);	
				   n++;
			   }
		   }
      if (divide)  ((float*)result->imageData)[x0/subsample] = average / (n*255.f) ; //For scaling by 255
      else  ((float*)result->imageData)[x0/subsample] = average; 
      }
      if (x0 == x1) return result; // && y0 == y1 Not Needed since the rows are the same size
      int e2 = 2*err;
      if (e2 > -dy){ 
         err -= dy;
         x0 += sx;
      }
      if (e2 < dx){ 
    	   err += dx;
         y0 += sy; 
      }
   }
}


//! Computes the 1d integral image of the specified horizon line 
//! in image img.  Assumes source image to be a 
//! 32-bit floating point.  Returns IplImage in 32-bit float form.
IplImage *Integral(IplImage *source, int left_horizon, int right_horizon, std::vector<int> &y_coords, int horizon_width)
{
   // convert the image to single channel 32f

   IplImage *img = getGrayHorizon(source, left_horizon, right_horizon, y_coords, horizon_width, SUBSAMPLE, false);
   IplImage *int_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);
   // set up variables for data access
   int width = img->width;
   float *data   = (float *) img->imageData;  
   float *i_data = (float *) int_img->imageData;  

   // one row only
   float rs = 0.0f;
   for(int j=0; j<width; j++) 
   {
      rs += data[j]; 
      i_data[j] = rs;
   }
	
   // release the gray image
   cvReleaseImage(&img);

   // return the integral image
   return int_img;
}




