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

#include "GLArrayRenderer.hxx"
#include "Viewport.hxx"
#include <algorithm>
#include "Assert.hxx"
using std::max_element;
using std::min_element;
#include "CLAM_Math.hxx"
#include <iostream>
using namespace CLAMGUI;

GLArrayRenderer::GLArrayRenderer( unsigned char red, unsigned char gree, unsigned char blu)
	: mElemIdxBuffer(0,100), mLastIndex(0), mFirstIndex(0), r( red ), g( gree ), b( blu ), mDataChanged( false ), 
	  mCullingRequested(false), mMustUpdateBounds( true )
{
	ResizeArray( 1024 );
	/*This limit has been experimentally found to be correct but it depends on the
	  system and the amount of simultaneous views.*/
	mMinPointsToOptimize=500000;
}


void GLArrayRenderer::InitArray( unsigned int nelems )
{
	GLfloat color[3] = { 0.0f, 0.0f, 0.0f };
	color[0] = GLfloat(r)/255.0f; color[1] = GLfloat(g)/255.0f; color[2] = GLfloat(b)/255.0f;

	for (unsigned int k = 0; k < nelems; k++ )
	{
		mIntertwined[k].r = color[0]; mIntertwined[k].g = color[1]; mIntertwined[k].b = color[2];
		mIntertwined[k].x = GLfloat( k );     // We update here the x-coordinates
		mIntertwined[k].z = 0.0f;             // We make zero the z-values
	}

}

void GLArrayRenderer::ResizeArray( unsigned int new_size )
{
	mIntertwined.resize( new_size );
	InitArray( new_size );
	
	mLastIndex = 0;
	mFirstIndex = 0;
}

void GLArrayRenderer::CacheData( const DataArray& array )
{

	unsigned int nbins = array.Size();

	if ( nbins != mIntertwined.size() )
		ResizeArray( nbins ); // Valarray resizing to accomodate the new CLAM Array

	DataTransform( array );
	mDataChanged = true;
	if(nbins>mMinPointsToOptimize)
		FindMaxMin();
}

void GLArrayRenderer::DataTransform( const DataArray& array )
{
	for ( int k = 0; k < array.Size(); k++ )
	{
		mIntertwined[k].y = GLfloat( array[k] ); 
	}

}

void GLArrayRenderer::Draw()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	if ( mDataChanged )
	{
		// caches the data on the video card ( if possible, otherwise it remains in 
		// processor memory - bad luck boy)
		glInterleavedArrays (GL_C3F_V3F, 0, &mIntertwined[0]);
		mDataChanged = false;
		
	}
	if(mCullingRequested && mCullingData.right-mCullingData.left>mMinPointsToOptimize)
	{
		if ( mMustUpdateBounds)
		{
			UpdateBounds();
			mMustUpdateBounds= false;
		}
		
		glDrawElements( GL_LINE_STRIP, (mLastIndex-mFirstIndex), GL_UNSIGNED_INT, mElemIdxBuffer.GetPtr()+mFirstIndex );
	}
	else
	{
		glDrawArrays( GL_LINE_STRIP, 0, mIntertwined.size() );
	}
	glFlush();
}

void GLArrayRenderer::DefineViewport( const DataArray& array, Viewport& view_specs )
{


	TData top ;
	TData bottom ;	
	
	// First of all we check if the caller knows something about the data to be rendered
	
	if ( view_specs.top_es ) // Estimation provided
		top = view_specs.top;
	else
		top    = *( max_element( array.GetPtr(), array.GetPtr()+array.Size() - 1 ) );
	
	if ( view_specs.bottom_es) // Estimation provided
		bottom = view_specs.bottom;
	else
		bottom = *( min_element( array.GetPtr(), array.GetPtr()+array.Size() - 1 ) );
	
	// MRJ:
	// we should avoid that top and bottom match, since it seems to cause an undetermined
	// 'memory straying' somewhere. The possible causes could range from a faulty OpenGL
	// ICD to a hidden bug in FLTK. However I haven't got a clue what happens when you
	// specify a projection with the same "top" and "bottom", I should revise the projective
	// math, since I fear that some numerical unstability problem manifestates 
	
	// :KLUDGE: let's pad the top and bottom when they are equal
	
	if ( fabs( top - bottom ) < 0.01 )
	{
		top += TData(1.0);
	}

	TData left   = 0;
	TData right  = (TData) array.Size();
	
	XaxisTransform( left, right, view_specs.left, view_specs.right, view_specs.isIntX );
	YaxisTransform( top,  bottom, view_specs.top, view_specs.bottom, view_specs.isIntY );
}

void GLArrayRenderer::XaxisTransform( TData left, TData right, TData& transleft, TData& transright, bool& integer )
{
	transleft = left;
	transright = right;
	integer = true;
}

void GLArrayRenderer::YaxisTransform( TData top, TData bottom, TData& transtop, TData& transbottom, bool& integer )
{
	transtop = top;
	transbottom = bottom;
	integer = true;
}

void GLArrayRenderer::PerformCulling( float left, float right, unsigned pixel_width )
{
	mCullingData.left = (unsigned)(left*GetXConversionFactor());
	mCullingData.right = (unsigned)(right*GetXConversionFactor());
	mCullingData.pixel_width = pixel_width;
	mCullingRequested = true;
	mMustUpdateBounds= true;	

}

void GLArrayRenderer::UpdateBounds()
{
	GLuint start, end;
	start = mCullingData.left;
	end = mCullingData.right;
	CLAM_ASSERT( start<end, "Start and End indexes cannot be equal!!!!" );

	unsigned int k;
	
	bool found=false;

	if(mCullingData.left>mElemIdxBuffer[mFirstIndex])
	{
		for(k=mFirstIndex;k<mnMaxMin;k++)
		{
			if(mElemIdxBuffer[k]>mCullingData.left)
			{
				mFirstIndex=k;
				found=true;
				break;
			}
		}
		if(!found) mFirstIndex=mnMaxMin-1;
	}
	else if(mCullingData.left<mElemIdxBuffer[mFirstIndex])
	{
		for(k=mFirstIndex;k>0;k--)
		{
			if(mElemIdxBuffer[k]<mCullingData.left)
			{
				mFirstIndex=k+1;
				found=true;
				break;
			}
		}
		if(!found) mFirstIndex=0;
	}
	
	found=false;

	if(mCullingData.right>mElemIdxBuffer[mLastIndex])
	{
		for(k=mLastIndex;k<mnMaxMin;k++)
		{
			if(mElemIdxBuffer[k]>mCullingData.right)
			{
				mLastIndex=k-1;
				found=true;
				break;
			}
		}
		if(!found) mLastIndex=mnMaxMin-1;
	}
	else if(mCullingData.right<mElemIdxBuffer[mLastIndex])
	{
		for(k=mLastIndex;k>0;k--)
		{
			if(mElemIdxBuffer[k]<mCullingData.right)
			{
				mLastIndex=k;
				found=true;
				break;
			}
		}
		if(!found) mLastIndex=0;
	}

}



void GLArrayRenderer::FindMaxMin()
{
	//Minimum value that will be acknowledged as maximum or minimum
	float minY=0.00001;
	float threshold=0.0015;

	unsigned int nElems=mIntertwined.size();
	
	float leftY,middleY,rightY;
	// detection loop 
	unsigned int i;
	
	//We always add a start point at zero
	mElemIdxBuffer.AddElem(0);
	mIntertwined[0].y=0;

	mnMaxMin=1;
	
	i=1;
	bool firstZero=true;
	while (i<nElems-2) 
	{
		//constant area with low amplitude values (noise)
		while(i<nElems-2&&mIntertwined[i].y<minY && mIntertwined[i].y > -minY)
		{
			if(firstZero)
			{
				mElemIdxBuffer.AddElem(i);
				mnMaxMin++;
				firstZero=false;
				mIntertwined[i].y=0;
			}				
			i++;
		}
		if(!firstZero)
		{
			mElemIdxBuffer.AddElem(i-1);
			mnMaxMin++;
			mIntertwined[i-1].y=0;
			firstZero=true;
		}
				
		leftY 	= mIntertwined[i].y;
		middleY	= mIntertwined[i+1].y;
		rightY 	= mIntertwined[i+2].y;

		
		// local Minimum detected 
		if ((leftY-middleY > threshold) && (rightY-middleY> threshold)) 
		{
			float interpolatedPosition;
			
			int pointPosition = i+1; 	// middleY has index i+1
			// quadratic interpolation
			float diffFromMin =  TData(0.5) * ((leftY-rightY) / (leftY- 2*middleY + rightY));
			interpolatedPosition = pointPosition+diffFromMin;
			mElemIdxBuffer.AddElem((unsigned)(interpolatedPosition));
							
			mnMaxMin++;
		
		}

			
		// local maximum Detected ! 
		if ((middleY-leftY > threshold) && (middleY-rightY > threshold)) 
		{	
			//if(middleY < 2*minY) continue;
			float diffFromMax;
			float interpolatedPosition;
			int pointPosition = i+1; 	// middleY has index i+1
			// quadratic interpolation
			diffFromMax =  TData(0.5) * ((leftY-rightY) / (leftY- 2*middleY + rightY));
			interpolatedPosition = pointPosition+diffFromMax;
			mElemIdxBuffer.AddElem((unsigned)(interpolatedPosition));
							
			mnMaxMin++;
		}
		i++;
	}

	//We always add an end point at zero
	mElemIdxBuffer.AddElem(nElems-1);
	mIntertwined[nElems-1].y=0;
	mnMaxMin++;
	
}








