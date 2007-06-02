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

#ifndef __SINTRACKHORCLIPPER__
#define __SINTRACKHORCLIPPER__

#include "SineTracksDef.hxx"
#include <vector>
#include "DataTypes.hxx"

namespace CLAMVM
{
		
		using CLAM::TIndex;
		using CLAM::TSize;

		class SinTrackHorClipper
		{
				typedef SineTrackSpanEnds::iterator peak_iterator;
		public:
				
				SinTrackHorClipper( )
						: mList( NULL)
				{
				}

				void SetTrackList( SineTrackList& list )
				{
					mList = &list;
				}

				void Cull( TIndex start_frame, TIndex end_frame, 
						   SineTrackSpanEnds& line_starts,
						   SineTrackSpanEnds& line_ends );

		protected:
				
				void DetermineSpanPeaks( TIndex frame0, TIndex frame1, SineTrackList::iterator itrack,
										 peak_iterator& start, peak_iterator& end );

		private:
				
				SineTrackList*   mList;
		};
}


#endif // CullSineTracks.hxx

