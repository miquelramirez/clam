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
#include "Assert.hxx"
#include "CLAMGL.hxx"
#include "SinTracksRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
		SinTracksRenderer::SinTracksRenderer()
			: _heads(NULL), _tails(NULL), _left(TData(0.0)), _right(TData(1.0))
		{
		}

		SinTracksRenderer::~SinTracksRenderer()
		{
		}

		void SinTracksRenderer::SetPalette(QtPalette& palette)
		{
			_palette = &palette;
		}

		void SinTracksRenderer::SetSpanLists( SineTrackSpanEnds& heads, 
												SineTrackSpanEnds& tails,
												const TData& left,
												const TData& right )
		{
			_heads = &heads;
			_tails = &tails;
			_left = left;
			_right = right;
		}

		void SinTracksRenderer::Render()
		{
			CLAM_ASSERT( _palette!=NULL, "No palette was given!" );

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(_left,_right,_bottom,_top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glClear(GL_COLOR_BUFFER_BIT);

			SineTrackSpanEnds::iterator currStart, currEnd;
					
			currStart = _heads->begin();
			currEnd = _tails->begin();
		
			while ( currStart != _heads->end() && currEnd !=_tails->end() )
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


					_palette->GetRGBFromIndex( (*trackBegin).mColorIndex, _color.r, _color.g, _color.b);
					glColor3ub(GLubyte(_color.r),GLubyte(_color.g),GLubyte(_color.b));
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
		}
	}
}

// END

