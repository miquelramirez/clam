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

#include "GLLineArrayRenderer.hxx"
#include "Viewport.hxx"
#include <algorithm>
using std::max_element;
using std::min_element;
#include <cmath>
using namespace CLAMGUI;

GLLineArrayRenderer::GLLineArrayRenderer(const c3f &color,
										 unsigned size,
										 TData first,
										 TData last)
	: ArrayRenderer2D(color)
{
	InitArray(size,first,last);
}

void GLLineArrayRenderer::ResetXValues()
{
	TData val = mFirst;

	for (unsigned k = 0; k < mSize; k++,val+=mDelta )
		mVertex[k].x = GLfloat(val);
}

void GLLineArrayRenderer::ResizeVertexArray( unsigned int new_size )
{
	if (mVertex.AllocatedSize() < int(new_size))
		mVertex.Resize(new_size*2);
	mVertex.SetSize(new_size);
}

void GLLineArrayRenderer::DataTransform( const DataArray& array )
{
	int size = array.Size();
	for ( int k = 0; k < size; k++ )
		mVertex[k].y = GLfloat( array[k] ); 

}

void GLLineArrayRenderer::Draw(TData x_start, TData x_width)
{
	int start = int( (x_start-mFirst) / mDelta);
	int len   = int(  x_width         / mDelta) + 3;

	if (start < 0)
		start = 0;
	if (start > mVertex.Size()-1)
		start = mVertex.Size()-1;

	if (len < 0)
		len = 0;
	if (start + len > mVertex.Size())
		len = mVertex.Size() - start;

// 	std::cout << "Lines from "
// 			  << mVertex[start][0] << ',' << mVertex[start][1] 
// 			  << " to "
// 			  << mVertex[end][0] << ',' << mVertex[end][1] 
// 			  << std::endl;
 
	glColor3fv(&mColor[0]);
	glLineWidth(mGLsize);
	glVertexPointer(2,GL_FLOAT,0,&mVertex[start]);
	glDrawArrays( GL_LINE_STRIP, 0, len );
}

void GLLineArrayRenderer::Draw()
{
	glColor3fv(&mColor[0]);
	glVertexPointer(2,GL_FLOAT,1,&mVertex[0]);
	glDrawArrays( GL_LINE_STRIP, 0, mVertex.Size() );
}

