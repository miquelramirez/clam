/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "GLStraightLineArray.hxx"
#include "CLAMGL.hxx"
#include <cstdlib>

namespace CLAMVM
{
	GLStraightLineArray::GLStraightLineArray()
		: mElemsToDraw( 0 ), mValues( NULL ), mXStart( 0 ), mXHopSize( 1 )
	{
		mLineColor[0] = 0;
		mLineColor[1] = 255;
		mLineColor[2] = 0;
	}
	
	
	GLStraightLineArray::~GLStraightLineArray()
	{
	}
	
	void GLStraightLineArray::ExecuteGLCommands()
	{
		//glClear( GL_COLOR_BUFFER_BIT );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		TData xvalue = mXStart;
		
		glColor3ubv( mLineColor );
		glBegin( GL_LINE_STRIP );
		
		for ( unsigned i = 0; i < mElemsToDraw; i++ )
		{
			glVertex2f( xvalue, mValues[i] );
			xvalue+=mXHopSize;
		}
		
		glEnd();
	}
}

