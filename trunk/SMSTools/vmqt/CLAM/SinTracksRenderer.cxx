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

#include <iostream>
#include <CLAM/Assert.hxx>
#include <CLAM/CLAMGL.hxx>
#include <CLAM/SinTracksRenderer.hxx>

namespace CLAM
{
	namespace VM
	{
		SinTracksRenderer::SinTracksRenderer()
			: mHeads(0)
			, mTails(0)
			, mLeft(TData(0.0))
			, mRight(TData(0.0))
		{
		}

		SinTracksRenderer::~SinTracksRenderer()
		{
		}

		void SinTracksRenderer::SetPalette(QtPalette& palette)
		{
			mPalette = &palette;
		}

		void SinTracksRenderer::SetSpanLists( SineTrackSpanEnds& heads, 
											  SineTrackSpanEnds& tails,
											  const TData& left,
											  const TData& right )
		{
			mHeads = &heads;
			mTails = &tails;
			mLeft = left;
			mRight = right;
		}

		void SinTracksRenderer::Render()
		{
			CLAM_ASSERT( mPalette!=NULL, "SinTracksRenderer::Render(): No palette was given!" );

			if(!mReadPixels)
			{
				DrawPixels();
				return;
			}
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(mLeft,mRight,BottomBound(),TopBound(),-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
		 
			Color c;

			SineTrackSpanEnds::iterator currStart, currEnd;
					
			currStart = mHeads->begin();
			currEnd = mTails->begin();
		
			while ( currStart != mHeads->end() && currEnd != mTails->end() )
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


					mPalette->GetRGBFromIndex( (*trackBegin).mColorIndex, c.r, c.g, c.b);
					glColor3ub(GLubyte(c.r),GLubyte(c.g),GLubyte(c.b));
					glVertex3f(GLfloat((*trackBegin).mTimeIndex),GLfloat((*trackBegin).mFreq),-1.0f);
					trackBegin++;
				}
				glEnd();
				currStart++;
				currEnd++;
			}

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glShadeModel( GL_FLAT );

			ReadPixels();
		}
	}
}

// END

