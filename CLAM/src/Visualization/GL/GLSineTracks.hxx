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

#ifndef __GLSINETRACKS__
#define __GLSINETRACKS__

#include "GLRenderer.hxx"
#include "SineTracksDef.hxx"

namespace CLAMVM
{
		class Fl_Palette;

		class GLSineTracks
				: public GLRenderer
		{
		public:
				GLSineTracks();
				virtual ~GLSineTracks();

				void SetPalette( Fl_Palette& pal )
				{
					mPalette = &pal;
				}

				void SetSpanLists( SineTrackSpanEnds& heads, SineTrackSpanEnds& tails )
				{
						mHeads = &heads;
						mTails = &tails;
				}

				virtual void ExecuteGLCommands();

		private:

				SineTrackSpanEnds* mHeads;
				SineTrackSpanEnds* mTails;
				Fl_Palette*        mPalette;
		};
}

#endif // GLSineTracks.hxx
