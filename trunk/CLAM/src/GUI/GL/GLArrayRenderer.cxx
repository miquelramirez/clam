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
using std::max_element;
using std::min_element;
#include "CLAM_Math.hxx"
using namespace CLAMGUI;

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
}

void GLArrayRenderer::CacheData( const DataArray& array )
{

	unsigned int nbins = array.Size();

	if ( nbins != mIntertwined.size() )
		ResizeArray( nbins ); // Valarray resizing to accomodate the new CLAM Array

	DataTransform( array );
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
	glInterleavedArrays (GL_C3F_V3F, 0, &mIntertwined[0]);
	glDrawArrays( GL_LINE_STRIP, 0, mIntertwined.size() );
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
