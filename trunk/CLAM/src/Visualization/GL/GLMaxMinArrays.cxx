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

#include "GLMaxMinArrays.hxx"
#include "Assert.hxx"
#include "CLAMGL.hxx"
#include <cstdlib>

namespace CLAMVM
{
		GLMaxMinArrays::GLMaxMinArrays()
				: mMaxsPtr( NULL ), mMinsPtr( NULL ), mNElems( 0 ), mMaxY( 1.0 ), mMinY( -1.0 )
		{
		}

		GLMaxMinArrays::~GLMaxMinArrays()
		{
		}

		void GLMaxMinArrays::ExecuteGLCommands()
		{
				CLAM_ASSERT( (mMaxsPtr!=NULL && mNElems>0), "Invalid pointer!" );
				CLAM_ASSERT( (mMinsPtr!=NULL && mNElems>0), "Invalid pointer!" );

				float fnelems = float(mNElems);

				glMatrixMode( GL_PROJECTION );
				glPushMatrix();
				glLoadIdentity();
				glOrtho( 0, fnelems, mMinY, mMaxY, -1, 1 );
				glMatrixMode( GL_MODELVIEW );
				glClear( GL_COLOR_BUFFER_BIT );
				glLoadIdentity();

				glColor3f( 0, 200, 0 );

				glBegin( GL_LINE_STRIP );

				for ( float x = 0.0f; x < fnelems; x+=1.0f )
				{
/*						if ( mMaxsPtr == mMinsPtr )
						{
							glVertex2f( x-1.0, *mMaxsPtr++);
							glVertex2f( x, *mMinsPtr++ );
						}
						else
						{*/
							glVertex2f( x, *mMaxsPtr++ );
							glVertex2f( x, *mMinsPtr++ );
						//}
				}

				glEnd();
				glMatrixMode( GL_PROJECTION );
				glPopMatrix();
				glMatrixMode( GL_MODELVIEW );
		}
}
