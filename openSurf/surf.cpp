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

#include "utils.h"

#include "surf.h"

//-------------------------------------------------------
//! SURF priors (these need not be done at runtime)
const float pi = 3.14159f;

//! lookup table for 2d gaussian (sigma = 2.5) where (0,0) is top left and (6,6) is bottom right
const double gauss25 [7][7] = {
		0.02546481,	0.02350698,	0.01849125,	0.01239505,	0.00708017,	0.00344629,	0.00142946,
		0.02350698,	0.02169968,	0.01706957,	0.01144208,	0.00653582,	0.00318132,	0.00131956,
		0.01849125,	0.01706957,	0.01342740,	0.00900066,	0.00514126,	0.00250252,	0.00103800,
		0.01239505,	0.01144208,	0.00900066,	0.00603332,	0.00344629,	0.00167749,	0.00069579,
		0.00708017,	0.00653582,	0.00514126,	0.00344629,	0.00196855,	0.00095820,	0.00039744,
		0.00344629,	0.00318132,	0.00250252,	0.00167749,	0.00095820,	0.00046640,	0.00019346,
		0.00142946,	0.00131956,	0.00103800,	0.00069579,	0.00039744,	0.00019346,	0.00008024
};

//-------------------------------------------------------

//! Constructor
Surf::Surf(IplImage *img, IpVec &ipts)
: ipts(ipts)
{
	this->img = img;
}

//-------------------------------------------------------

//! Describe all features in the supplied vector
void Surf::getDescriptors(bool upright)
{
	// Check there are Ipoints to be described
	if (!ipts.size()) return;

	// Get the size of the vector for fixed loop bounds
	int ipts_size = (int)ipts.size();

	if (upright)
	{
		// U-SURF loop just gets descriptors
		for (int i = 0; i < ipts_size; ++i)
		{
			// Set the Ipoint to be described
			index = i;

			// Extract upright (i.e. not rotation invariant) descriptors
			getDescriptor(true);
		}
	}
	else
	{
		// Main SURF-64 loop assigns orientations and gets descriptors
		//MC: This is now changed to a SURF-8 descriptor
		for (int i = 0; i < ipts_size; ++i)
		{
			// Set the Ipoint to be described
			index = i;

			// Assign Orientations and extract rotation invariant descriptors
			//getOrientation(); //MC: Not needed for the 1D SURF implementation
			getDescriptor(false);
		}
	}
}

//-------------------------------------------------------

//! Assign the supplied Ipoint an orientation
void Surf::getOrientation()
{
	Ipoint *ipt = &ipts[index];
	float gauss = 0.f, scale = ipt->scale;
	const int s = fRound(scale), r = fRound(ipt->y), c = fRound(ipt->x);
	std::vector<float> resX(109), resY(109), Ang(109);
	const int id[] = {6,5,4,3,2,1,0,1,2,3,4,5,6};

	int idx = 0;
	// calculate haar responses for points within radius of 6*scale
	for(int i = -6; i <= 6; ++i)
	{
		for(int j = -6; j <= 6; ++j)
		{
			if(i*i + j*j < 36)
			{
				gauss = static_cast<float>(gauss25[id[i+6]][id[j+6]]);  // could use abs() rather than id lookup, but this way is faster
				resX[idx] = gauss * haarX(r+j*s, c+i*s, 4*s);
				resY[idx] = gauss * haarY(r+j*s, c+i*s, 4*s);
				Ang[idx] = getAngle(resX[idx], resY[idx]);
				++idx;
			}
		}
	}

	// calculate the dominant direction
	float sumX=0.f, sumY=0.f;
	float max=0.f, orientation = 0.f;
	float ang1=0.f, ang2=0.f;

	// loop slides pi/3 window around feature point
	for(ang1 = 0; ang1 < 2*pi;  ang1+=0.15f) {
		ang2 = ( ang1+pi/3.0f > 2*pi ? ang1-5.0f*pi/3.0f : ang1+pi/3.0f);
		sumX = sumY = 0.f;
		for(unsigned int k = 0; k < Ang.size(); ++k)
		{
			// get angle from the x-axis of the sample point
			const float & ang = Ang[k];

			// determine whether the point is within the window
			if (ang1 < ang2 && ang1 < ang && ang < ang2)
			{
				sumX+=resX[k];
				sumY+=resY[k];
			}
			else if (ang2 < ang1 &&
					((ang > 0 && ang < ang2) || (ang > ang1 && ang < 2*pi) ))
			{
				sumX+=resX[k];
				sumY+=resY[k];
			}
		}

		// if the vector produced from this window is longer than all
		// previous vectors then this forms the new dominant direction
		if (sumX*sumX + sumY*sumY > max)
		{
			// store largest orientation
			max = sumX*sumX + sumY*sumY;
			orientation = getAngle(sumX, sumY);
		}
	}

	// assign orientation of the dominant response vector
	ipt->orientation = orientation;
}

//-------------------------------------------------------

//! Get the modified descriptor. See Agrawal ECCV 08
//! Modified descriptor contributed by Pablo Fernandez
void Surf::getDescriptor(bool bUpright)
{
	int y, x, sample_x, sample_y, count=0;
	int i = 0, ix = 0, j = 0, jx = 0, xs = 0, ys = 0;
	float scale, *desc, dx, mdx, dy, mdy, co, si; //MC: Not needed as we are only using the row of pixels
	float gauss_s1 = 0.f, gauss_s2 = 0.f;
	float rx = 0.f, ry = 0.f, rrx = 0.f, rry = 0.f, len = 0.f;
	float cx = -0.5f, cy = 0.f; //Subregion centers for the 4x4 gaussian weighting

	Ipoint *ipt = &ipts[index];
	scale = ipt->scale;

	//Interest point coordinates
	x = fRound(ipt->x);
	y = fRound(ipt->y);
	desc = ipt->descriptor;

	if (bUpright)
	{
		//co = 1;
		//si = 0;
	}
	else
	{
		//MC: I assume this orientation is to calculate the box to be aligned with the orientation
		//of the interest point
		//co = cos(ipt->orientation);
		//si = sin(ipt->orientation);
	}

	//i = -8;
	//MC: 1D SURF implementation
	//Row index
	i=1;


	//Calculate descriptor for this interest point
	//Indicates that all row subregions have been traversed
	//while(i < 12)
	//{
	//Column index
	//By setting j to minus 8, we are placing
	j = -8;
	//i = i-4;

	//Updates the Gaussian center to indicates that we have moved to a new row
	cx += 1.f;
	cy = -0.5f;
	//Indicates when all column regions have been traversed for a particular row

	while(j < 7)//12 creates 4 sub-regions, 7 creates 3 sub-regions
	{
		dx=dy=mdx=mdy=0.f;
		//Update the Gaussian center to indicate that we have moved to a new column
		cy += 1.f;

		//Move along the current column
		j = j - 4;

		//
		//ix = i + 5;
		//MC: 1D SURF
		jx = j + 5;

		//This rotates the points by the angle theta. This aligns the points with the
		//direction of orientation of the keypoint and selects them (rounded off) based on
		//the scale and angle theta.
		//xs = fRound(x + ( -jx*scale*si + ix*scale*co));//Note that the minus is for -sin(theta)
		//ys = fRound(y + ( jx*scale*co + ix*scale*si));

		//Convert the current x value to the correct scale and orientation
		xs = fRound(x + jx*scale);
		ys=1;

		// for (int k = i; k < i + 9; ++k)
		//{

		//MC: 1D SURF Implementation
		for (int l = j; l < j + 9; ++l)
		{
			//Get coords of sample point on the rotated axis
			//Rotate the axes and get the coordinates of the sample point
			//          sample_x = fRound(x + (-l*scale*si + k*scale*co));
			//          sample_y = fRound(y + ( l*scale*co + k*scale*si));

			//MC: 1D SURF implementation
			//Choose 25 uniform samples from each region
			sample_x = fRound(x + l*scale);
			sample_y = 1;

			//Get the gaussian weighted x and y responses. The Gaussian is centered at the interest point
			//gauss_s1 = gaussian(xs-sample_x,ys-sample_y,2.5f*scale);
			gauss_s1 = gaussian1D(xs-sample_x, 2.5f*scale);
			rx = haarX1D(sample_y, sample_x, 2*fRound(scale));
			//ry = haarY(sample_y, sample_x, 2*fRound(scale));

			//Get the gaussian weighted x and y responses on rotated axis
			//rrx = gauss_s1*(-rx*si + ry*co);
			//rry = gauss_s1*(rx*co + ry*si);

			//MC: was 1D SURF but now i decided to remove it
			//rrx = gauss_s1*(rx);

			//dx += rrx;
			//dy += rry;

			//MC: 1D SURF
			dx +=rx;
			//mdx += fabs(rrx);
			//mdy += fabs(rry);
			//MC: 1D SURF
			mdx +=fabs(rx);

		}
		// }

		//Add the values to the descriptor vector. This is the used to
		//reweight the descriptors
		//gauss_s2 = gaussian(cx-2.0f,cy-2.0f,1.5f);
		//MC: 1D SURF implementation
		gauss_s2 = gaussian1D(cx-2.0f, 1.5f);
		desc[count++] = dx*gauss_s2;
		//desc[count++] = dy*gauss_s2;
		desc[count++] = mdx*gauss_s2;
		//desc[count++] = mdy*gauss_s2;

		//len += (dx*dx + dy*dy + mdx*mdx + mdy*mdy) * gauss_s2*gauss_s2;

		//MC: 1D SURF Implementation
		len += (dx*dx + mdx*mdx) * gauss_s2*gauss_s2;

		//Moving along to the next column subregion
		j += 9;
	}
	//Moving to the next row sub-region
	//i += 9;
	//}

	//Convert to Unit Vector
	len = sqrt(len);
	for(int i = 0; i < 6; ++i)//64
		desc[i] /= len;

}


//-------------------------------------------------------

//! Calculate the value of the 2d gaussian at x,y
inline float Surf::gaussian(int x, int y, float sig)
{
	return (1.0f/(2.0f*pi*sig*sig)) * exp( -(x*x+y*y)/(2.0f*sig*sig));
}

//-------------------------------------------------------

//! Calculate the value of the 2d gaussian at x,y
inline float Surf::gaussian(float x, float y, float sig)
{
	return 1.0f/(2.0f*pi*sig*sig) * exp( -(x*x+y*y)/(2.0f*sig*sig));
}

//! Calculate the value of the 2d gaussian at x,y
inline float Surf::gaussian1D(float x, float sig)
{
	return 1.0f/(2.0f*pi*sig*sig) * exp( -(x*x)/(2.0f*sig*sig));
}
//! Calculate the value of the 2d gaussian at x,y
inline float Surf::gaussian1D(int x, float sig)
{
	return 1.0f/(2.0f*pi*sig*sig) * exp( -(x*x)/(2.0f*sig*sig));
}

//-------------------------------------------------------

//! Calculate Haar wavelet responses in x direction
inline float Surf::haarX(int row, int column, int s)
{
	return BoxIntegral(img, row-s/2, column, s, s/2)
    		-1 * BoxIntegral(img, row-s/2, column-s/2, s, s/2);
}

//! Calculate Haar wavelet responses in x direction
inline float Surf::haarX1D(int row, int column, int s)
{
	return BoxIntegral(img, 1, column, 0, s/2)
    		-1 * BoxIntegral(img, 1, column-s/2, 0, s/2);
}

//-------------------------------------------------------

//! Calculate Haar wavelet responses in y direction
inline float Surf::haarY(int row, int column, int s)
{
	return BoxIntegral(img, row, column-s/2, s/2, s)
    		-1 * BoxIntegral(img, row-s/2, column-s/2, s/2, s);
}

//-------------------------------------------------------

//! Get the angle from the +ve x-axis of the vector given by (X Y)
float Surf::getAngle(float X, float Y)
{
	if(X > 0 && Y >= 0)
		return atan(Y/X);

	if(X < 0 && Y >= 0)
		return pi - atan(-Y/X);

	if(X < 0 && Y < 0)
		return pi + atan(Y/X);

	if(X > 0 && Y < 0)
		return 2*pi - atan(-Y/X);

	return 0;
}
