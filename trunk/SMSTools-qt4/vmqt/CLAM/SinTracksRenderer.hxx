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

#ifndef __SINTRACKSRENDERER__
#define __SINTRACKSRENDERER__

#include <CLAM/QtPalette.hxx>
#include <CLAM/SinTracksDef.hxx>
#include <CLAM/Renderer.hxx>

namespace CLAM
{
	namespace VM
	{
		class SinTracksRenderer : public Renderer
		{
		public:
			SinTracksRenderer();
			~SinTracksRenderer();

			void SetPalette(QtPalette& palette);

			void SetSpanLists( SineTrackSpanEnds& heads, 
							   SineTrackSpanEnds& tails,
							   const TData& left,
							   const TData& right );
			void Render();

		private:
			SineTrackSpanEnds* mHeads;
			SineTrackSpanEnds* mTails;
			QtPalette*         mPalette;
			TData              mLeft; 
			TData              mRight;
		};
	}
}

#endif

