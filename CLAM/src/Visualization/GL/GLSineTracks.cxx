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

#include "GLSineTracks.hxx"
#include "CLAMGL.hxx"
#include "Assert.hxx"
#include "Fl_Palette.hxx"
#include <iostream>

namespace CLAMVM
{
	GLSineTracks::GLSineTracks()
		: mHeads(NULL), mTails(NULL), mPalette( NULL )
	{
	}

	GLSineTracks::~GLSineTracks()
	{
				
	}

	void GLSineTracks::ExecuteGLCommands()
	{
		CLAM_ASSERT( mPalette!=NULL, "No palette was given!" );
		unsigned char colors[3];

		glClear( GL_COLOR_BUFFER_BIT );
		glMatrixMode( GL_MODELVIEW );
		glShadeModel( GL_SMOOTH );
		glLoadIdentity();

		SineTrackSpanEnds::iterator currStart, currEnd;
				
		currStart = mHeads->begin();
		currEnd = mTails->begin();

		while ( currStart != mHeads->end() && currEnd !=mTails->end() )
		{
			SinusoidalTrack::iterator trackBegin, trackEnd;
			trackBegin = *currStart;
			trackEnd = *currEnd;

			glBegin( GL_LINE_STRIP );
			while ( trackBegin != trackEnd )
			{
				if ( ((*trackBegin).mColorIndex < 64) || ((*trackBegin).mColorIndex >= 128 ) )
				{
					std::cerr << (*trackBegin).mMag << std::endl;
					std::cerr << (*trackBegin).mFreq << std::endl;
					std::cerr << (*trackBegin).mTimeIndex << std::endl;
					std::cerr << (*trackBegin).mColorIndex << std::endl;

					CLAM_ASSERT( false, "Out of range" );
				}


				mPalette->GetRGBFromIndex( (*trackBegin).mColorIndex, colors[0], colors[1], colors[2]);
				glColor3ubv( colors );
				glVertex2f( (*trackBegin).mTimeIndex, (*trackBegin).mFreq );
				trackBegin++;
			}
			glEnd();
			currStart++;
			currEnd++;
		}
		glShadeModel( GL_FLAT );
	}
}
