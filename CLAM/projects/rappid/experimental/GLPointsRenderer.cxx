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

#include "GLPointsRenderer.hxx"
#include "Viewport.hxx"
#include <algorithm>
using std::max_element;
using std::min_element;
#include <cmath>
using namespace CLAMGUI;

GLPointsRenderer::GLPointsRenderer(const c3f &color,
								   unsigned size,
								   TData first,
								   TData last)
	: PointsRenderer2D(color)
{
	InitArray(size,first,last);
}

void GLPointsRenderer::ResetXValues()
{
}

void GLPointsRenderer::ResizeVertexArray( unsigned int new_size )
{
	if (mVertex.AllocatedSize() < int(new_size))
		mVertex.Resize(new_size*2);
	mVertex.SetSize(new_size);
}

void GLPointsRenderer::DataTransform(const DataArray& positions,
									 const DataArray& values)
{
	CLAM_DEBUG_ASSERT(positions.Size() >= mSize &&
					  values.Size()    >= mSize,
					  "GLPointsRenderer::DataTransform(): Size mismatch");

	for ( int k = 0; k < mSize; k++ ) {
		mVertex[k].x = GLfloat( positions[k] ); 
		mVertex[k].y = GLfloat( values[k] ); 
	}

}

void GLPointsRenderer::Draw(TData x_start, TData x_width)
{
	if (!mVertex.Size())
		return;

 	int start = 0;
 	int len = mSize;

	// This asumes that vertexes are orderer in frequency which is not
	// true right now.

// 	int start = 0;
// 	int len = 1;

// 	while (1) {
// 		if (start >= mVertex.Size()-1)
// 			break;
// 		if (mVertex[start+1].x >= x_start)
// 			break;
// 		start++;
// 	}

// 	while (1) {
// 		if (start+len >= mVertex.Size())
// 			break;
// 		if (mVertex[start+len-1].x - x_start >= x_width)
// 			break;
// 		len++;
// 	}

// 	std::cout << "Lines from "
// 			  << mVertex[start][0] << ',' << mVertex[start][1] 
// 			  << " to "
// 			  << mVertex[end][0] << ',' << mVertex[end][1] 
// 			  << std::endl;
 
	glColor3fv(&mColor[0]);
	glPointSize(mGLsize);
	glVertexPointer(2,GL_FLOAT,0,&mVertex[start]);
	glDrawArrays( GL_POINTS , 0, len );
}

void GLPointsRenderer::Draw()
{
	glColor3fv(&mColor[0]);
	glVertexPointer(2,GL_FLOAT,1,&mVertex[0]);
	glDrawArrays( GL_LINE_STRIP, 0, mVertex.Size() );
}

