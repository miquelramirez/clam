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

#include "GLPulseArrayRenderer.hxx"
using namespace CLAMGUI;

GLPulseArrayRenderer::GLPulseArrayRenderer(const c3f &color,
										   unsigned size,
										   TData first,
										   TData last)
	: ArrayRenderer2D(color)
{
	InitArray( size, first, last );
}

void GLPulseArrayRenderer::ResetXValues()
{
	TData val = mFirst;

	for (unsigned k = 0; k < mSize; k++,val+=mDelta ) {
		mVertex[2*k].y = 0.0;
		mVertex[2*k].x = GLfloat(val);
		mVertex[2*k+1].x = GLfloat(val);
	}

}

void GLPulseArrayRenderer::ResizeVertexArray( unsigned int new_size )
{
	if (mVertex.AllocatedSize() < int(new_size*2))
		mVertex.Resize(new_size*4);
	mVertex.SetSize(new_size*2);
}

void GLPulseArrayRenderer::DataTransform( const DataArray& array )
{
	for ( int k = 0; k < array.Size(); k++ )
		mVertex[2*k+1].y = GLfloat( array[k] ); 
}

void GLPulseArrayRenderer::Draw(TData x_start, TData x_width)
{
	int start = int( (x_start-mFirst) / mDelta);
	int len   = int(  x_width         / mDelta)+3;

	if (start < 0)
		start = 0;
	if (start > mVertex.Size()-1)
		start = mVertex.Size()-1;

	if (len < 0)
		len = 0;
	if (start + len > mVertex.Size())
		len = mVertex.Size() - start;

	glColor3fv(&mColor[0]);
	glPointSize(3.0);
	glVertexPointer(2,GL_FLOAT,0,&mVertex[start*2]);
	glDrawArrays( GL_LINES, 0, len*2-1 );
}

void GLPulseArrayRenderer::Draw()
{
	glColor3fv(&mColor[0]);
	glPointSize(3.0);
	glVertexPointer(2,GL_FLOAT,1,&mVertex[0]);
	glDrawArrays( GL_LINES, 0, mVertex.Size() );
}

