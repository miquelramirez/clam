/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "GLWaveRenderer.hxx"
#include "DataTypes.hxx"
#include "CLAM_Math.hxx"

using CLAM::TData;
using namespace CLAMGUI;

GLWaveRenderer::GLWaveRenderer( unsigned char red, unsigned char green, unsigned char blu )
		: GLArrayRenderer( red, green, blu ), mPixelColor(0), mPixelColorSize(0)
{
}

GLWaveRenderer::~GLWaveRenderer()
{
}

void GLWaveRenderer::InitArray( unsigned int n_elems )
{
	GLfloat color[3] = { 0.0f, 0.0f, 0.0f };
	color[0] = GLfloat(r)/255.0f; color[1] = GLfloat(g)/255.0f; color[2] = GLfloat(b)/255.0f;

	ArrangeXScale();

	for (unsigned int k = 0; k < n_elems; k++ )
	{
		mIntertwined[k].r = color[0]; mIntertwined[k].g = color[1]; mIntertwined[k].b = color[2];

		mIntertwined[k].z = 0.0f;             // We make zero the z-values
	}
}

void GLWaveRenderer::DrawVerticalLine(float x,float top,float bottom)
{
	if (bottom < mCullingData.bottomY)
		return;
	if (top > mCullingData.topY)
		return;
	top = std::max(mCullingData.bottomY + 0.1f/Y2Pixel,top);
	bottom = std::max(mCullingData.bottomY + 0.1f/Y2Pixel,bottom);
	int nPixels = (bottom - top) * Y2Pixel + 1;
	glRasterPos2f(x,top);
	glDrawPixels(1,nPixels,GL_GREEN,GL_FLOAT,mPixelColor);
}

void GLWaveRenderer::ArrangeXScale()
{
	// we update here the x-axis coordinates

	CLAM_ASSERT( mSampRate!=0, "Sampling Rate was not set" );

	float invNFs = GLfloat( 1.0f / mSampRate );

	for ( unsigned int k = 0; k < mIntertwined.size(); k++ )
		mIntertwined[k].x = GLfloat( k * invNFs );
	
	mXConversionFactor = mSampRate;
}

void GLWaveRenderer::SetSamplingRate( TData value )
{
	mSampRate = value;

	ArrangeXScale();
}

void GLWaveRenderer::XaxisTransform( TData left, TData right, TData& transleft, TData& transright, bool& integer )
{
	transleft = left;

	transright = ( mSampRate == 0 ? right : right*(1.0f/mSampRate) );
	integer = false;
}

void GLWaveRenderer::YaxisTransform( TData top, TData bottom, TData& transtop, TData& transbottom, bool& integer )
{

	if ( fabs( top - bottom ) < 0.01 )
	{
		top += TData(0.01);
		bottom -= TData(0.01);
	}


	transtop = top  + 0.001f;
	transbottom = bottom  - 0.001f;
	integer = false;
}

void GLWaveRenderer::CacheData( const DataArray& array )
{
	mDataArray = array;
	int newSize = ((int)(( mDataArray.Size()/10)+1));
	mDataArrayTop.Resize( newSize );
	mDataArrayTop.SetSize(newSize);
	mDataArrayBottom.Resize( newSize );
	mDataArrayBottom.SetSize( newSize );
	for(int i=0; i<mDataArrayTop.Size(); i++)
	{
		int begin = (i-0.5)*10;
		int end = (i+0.5)*10;
		begin = std::max(begin , 0);
		begin = std::min(begin , mDataArray.Size()-2);
		end = std::min(end , mDataArray.Size());
		float top = mDataArray[begin];
		float bottom = mDataArray[begin];
		for(int k=begin+1; k<end; k++)
		{
			if (mDataArray[k] > top)
				top = mDataArray[k];
			if (mDataArray[k] < bottom)
				bottom = mDataArray[k];
		}
		mDataArrayTop[i] = top;
		mDataArrayBottom[i] = bottom;
	}
	newSize = ((int)(( mDataArrayTop.Size()/10)+1));
	mDataArrayTop2.Resize( newSize );
	mDataArrayTop2.SetSize(newSize);
	mDataArrayBottom2.Resize( newSize );
	mDataArrayBottom2.SetSize( newSize );
	for(int i=0; i<mDataArrayTop2.Size(); i++)
	{
		int begin = (i-0.5)*10;
		int end = (i+0.5)*10;
		begin = std::max(begin , 0);
		begin = std::min(begin , mDataArrayTop.Size()-2);
		end = std::min(end , mDataArrayTop.Size());
		float top = mDataArrayTop[begin];
		float bottom = mDataArrayBottom[begin];
		for(int k=begin+1; k<end; k++)
		{
			if (mDataArrayTop[k] > top)
				top = mDataArrayTop[k];
			if (mDataArrayBottom[k] < bottom)
				bottom = mDataArrayBottom[k];
		}
		mDataArrayTop2[i] = top;
		mDataArrayBottom2[i] = bottom;
	}

	/*unsigned int nbins = array.Size();

	if ( nbins != mIntertwined.size() )
		ResizeArray( nbins ); // Valarray resizing to accomodate the new CLAM Array

	DataTransform( array );
	mDataChanged = true;
	if(nbins>mMinPointsToOptimize)
		FindMaxMin();*/

}

void GLWaveRenderer::Draw()
{
	glClear( GL_COLOR_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	float leftX = mCullingData.leftX;
	float rightX= mCullingData.rightX;
	float topY = mCullingData.topY;
	float bottomY= mCullingData.bottomY;
	Index2Pixel = mCullingData.pixel_width / (float)(mCullingData.right-mCullingData.left);
	X2Pixel = mCullingData.pixel_width / (float)(rightX-leftX);
	Y2Pixel = mCullingData.pixel_height / (float)(topY-bottomY);

	float indexes_per_pixel = (mCullingData.right - mCullingData.left) / mCullingData.pixel_width;

	//mMustUpdateBounds
	float lastTop,lastBottom;
	for(int i=0; i<mCullingData.pixel_width; i++)
	{
		float topValue,bottomValue;
		int nIndex = mCullingData.left + i / Index2Pixel;
		int nNextIndex = mCullingData.left + (i+1) / Index2Pixel;
		float x = leftX + i / X2Pixel * 1.0001;
		if (indexes_per_pixel > 200)
		{
			nIndex /= 100;
			nNextIndex /= 100;
			nIndex = std::min(nIndex,mDataArrayTop2.Size()-1);
			nNextIndex = std::min(nNextIndex,mDataArrayTop2.Size()-1);
			topValue = mDataArrayTop2[nIndex];
			bottomValue = mDataArrayBottom2[nIndex];
			if (nNextIndex > nIndex)
				for(int k=nIndex+1; k<nNextIndex; k++)
				{
					if (mDataArrayTop2[k] > topValue)
						topValue = mDataArrayTop2[k];
					if (mDataArrayBottom2[k] < bottomValue)
						bottomValue = mDataArrayBottom2[k];
				}
		}
		else
		if (indexes_per_pixel > 20)
		{
			nIndex /= 10;
			nNextIndex /= 10;
			nIndex =std::min(nIndex,mDataArrayTop.Size()-1);
			nNextIndex = std::min(nNextIndex,mDataArrayTop.Size()-1);
			topValue = mDataArrayTop[nIndex];
			bottomValue = mDataArrayBottom[nIndex];
			if (nNextIndex > nIndex)
				for(int k=nIndex+1; k<nNextIndex; k++)
				{
					if (mDataArrayTop[k] > topValue)
						topValue = mDataArrayTop[k];
					if (mDataArrayBottom[k] < bottomValue)
						bottomValue = mDataArrayBottom[k];
				}
		}
		else
		{
			nIndex =std::min(nIndex,mDataArray.Size()-1);
			nNextIndex = std::min(nNextIndex,mDataArray.Size()-1);
			topValue = mDataArray[nIndex];
			bottomValue = mDataArray[nIndex];
			if (nNextIndex > nIndex)
				for(int k=nIndex+1; k<nNextIndex; k++)
				{
					if (mDataArray[k] > topValue)
						topValue = mDataArray[k];
					if (mDataArray[k] < bottomValue)
						bottomValue = mDataArray[k];
				}
		}
		if (i > 0)
			if (bottomValue > lastTop)
				DrawVerticalLine(x,lastTop,bottomValue);
			else
			if (topValue < lastBottom)
				DrawVerticalLine(x,topValue,lastBottom);
		if (bottomValue == topValue)
		{
			glRasterPos2f(x,topValue);
			glDrawPixels(1,1,GL_GREEN,GL_FLOAT,mPixelColor);
		}
		else
			DrawVerticalLine(x,bottomValue,topValue);
		lastTop = topValue;
		lastBottom = bottomValue;
	}
 	glFlush();
}

void GLWaveRenderer::PerformCulling( float left, float right, unsigned pixel_width, unsigned extraparams )
{
		// MRJ: pixel_height es l'alçada del viewport en pixels
	float *pextraparams = (float*)extraparams;
	mCullingData.leftX = left;
	mCullingData.rightX = right;
	mCullingData.left = (unsigned)(left*GetXConversionFactor());
	mCullingData.right = (unsigned)(right*GetXConversionFactor());
	mCullingData.topY = pextraparams[0];
	mCullingData.bottomY = pextraparams[1];
	mCullingData.pixel_width = pixel_width;
	mCullingData.pixel_height = (unsigned int)(pextraparams[2] + 0.5);
	mCullingRequested = true;
	mMustUpdateBounds= true;	
	if (mCullingData.pixel_height > mPixelColorSize)
	{
		delete [] mPixelColor;
		mPixelColorSize = ((int)(mCullingData.pixel_height / 100) + 1 )*100;
		mPixelColor = new float[mPixelColorSize];
		for(int m=0; m<mPixelColorSize; m++)
			mPixelColor[m]=1.0;
	}
}
