/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <cmath>
#include "FourierTransform.hxx"
#include <iostream>

#define SWAP(a,b) do {double swaptemp=(a); (a)=(b); (b)=swaptemp;} while(false)


//  constructor
// inpointer points to the input data. n is twice the frame size
// complex = 0 -> augmentation with 0s is necessary
FourierTransform::FourierTransform(unsigned long int framesize, double datanorm, bool isComplex)
	: mFrameSize(framesize)
	, mIsComplex(isComplex)
{
	datah.resize(2*mFrameSize);
}

FourierTransform::~FourierTransform()
{
}

//----------------------------------------------------------------------------------------
// FourierTransform transform code adapted from: Chapter 12, Fast Fourier Transform from Numerical
// Recipes in C: The Art of Scientific Computing. Numerical Recipes Software (1988-1992)
// Cambridge University Press, Cambridge, accessed 25.08.04,
// available at http://www.library.cornell.edu/nr/cbookcpdf.html

void doFourierTransform(double * data, unsigned frameSize)
{
	// Matlab indeces, data[0] is out of bounds  TOREMOVE?
	const unsigned long n=frameSize<<1; //n is the data array size
	for (unsigned long i=1, j=1; i<n; i+=2) {
		if (j>i) {
			SWAP(data[j], data[i]);
			SWAP(data[j+1], data[i+1]);
		}
		unsigned long m=frameSize;
		while (m >= 2 && j>m) {
			j-= m;
			m >>= 1;
		}
		j += (m);
	}
	//now Danielson-Lanczos
	unsigned long istep;
	for (unsigned long mmax = 2; mmax<n; mmax = istep)
	{
		istep = mmax << 1;
		const double theta =(6.28318530717959/mmax); // multiply by -1 here for ifft
		double wtemp = sin(0.5*theta);
		double wpr = -2.0*wtemp*wtemp;
		double wpi = sin(theta);
		double wr=1.0;
		double wi=0.0;
		for(unsigned long m=1; m<mmax; m+=2) {
			for (unsigned long i=m; i<=n; i+=istep) {
				unsigned long j = i+mmax;
				double tempr = float (wr*data[j] - wi*data[j+1]);
				double tempi = float (wr*data[j+1] + wi*data[j]);
				data[j] = data[i] - tempr;
				data[j+1] = data[i+1] - tempi;
				data[i] += tempr;
				data[i+1] += tempi;
			}
			wtemp=wr;
			wr = wtemp*wpr - wi*wpi + wr; //trigonometric recurrence
			//wr = (wtemp=wr)*wpr - wi*wpi + wr; //trigonometric recurrence
			wi = wi*wpr + wtemp*wpi + wi;
		}
	}
}

void FourierTransform::doIt(const float * inpointer)
{
	//fill array "data" with data
	if (mIsComplex)
	{
		for (unsigned long k=0; k<mFrameSize*2; k++) {
			datah[k] = inpointer[k];
		}
	}
	else {
		for (unsigned long k=0; k<mFrameSize; k++) {
			datah[k<<1] = inpointer[k];
			datah[(k<<1)+1] = 0.0;
		}
	}

	double * data = &(datah[0])-1; // Hack to use Matlab indeces TOREMOVE?
	doFourierTransform(data, mFrameSize);
}

void FourierTransform::doIt(const double * inpointer)
{
	//fill array "data" with data
	if (mIsComplex)
	{
		for (unsigned long k=0; k<mFrameSize*2; k++) {
			datah[k] = inpointer[k];
		}
	}
	else {
		for (unsigned long k=0; k<mFrameSize; k++) {
			datah[k<<1] = inpointer[k];
			datah[(k<<1)+1] = 0.0;
		}
	}
	double * data = &(datah[0])-1; // Hack to use Matlab indeces TOREMOVE?
	doFourierTransform(data, mFrameSize);
}


//-------------------------------------------------------------------------

