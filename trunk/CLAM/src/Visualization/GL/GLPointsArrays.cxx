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

#include "GLPointsArrays.hxx"
#include "Assert.hxx"
#include "CLAMGL.hxx"
#include <cstdlib>

namespace CLAMVM
{
	GLPointsArrays::GLPointsArrays()
		: mXDataArray( NULL ), mYDataArray( NULL ), mXDataLen( 0 ), mYDataLen( 0 ), mMinY( 0 )
	{
	}

	GLPointsArrays::~GLPointsArrays()
	{
	}

	void GLPointsArrays::ExecuteGLCommands()
	{
		if ( mXDataArray == NULL ) return;
		if ( mYDataArray == NULL ) return;

		CLAM_DEBUG_ASSERT( mXDataLen == mYDataLen, "X's and Y's arrays lenghts differ!!" );
				
		glMatrixMode( GL_MODELVIEW );
				
		glLineWidth( 1.5 );
		for ( int i = 0; i < mXDataLen; i++ )
		{
			glColor3ubv( mLineColor );

			glBegin( GL_LINES );
			{
				glVertex2f( mXDataArray[i], mMinY );
				glVertex2f( mXDataArray[i], mYDataArray[i] );
			}
			glEnd();
			// box at the tip of the peak ( leftbottom - rightbottom - righttop - lefttop )
			glColor3ub( 255, 0, 0 );
			glBegin( GL_LINE_STRIP );
			{
				glVertex2f( mXDataArray[i]-0.05f, mYDataArray[i]-0.05f );
				glVertex2f( mXDataArray[i]+0.05f, mYDataArray[i]-0.05f );
				glVertex2f( mXDataArray[i]+0.05f, mYDataArray[i]+0.05f );
				glVertex2f( mXDataArray[i]-0.05f, mYDataArray[i]+0.05f );
				glVertex2f( mXDataArray[i]-0.05f, mYDataArray[i]-0.05f );
			}
			glEnd();
		}
		glLineWidth( 1.0 );

	}
		
}
