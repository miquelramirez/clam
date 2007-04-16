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

#include "SinTrackListBuilder.hxx"
#include <algorithm>

namespace CLAMVM
{
		SinTrackBuilder::SinTrackBuilder(  )
			: mSineTrackList( NULL )
		{
		}

		void SinTrackBuilder::AddFrame( const Array<Partial>& a, TIndex frame_idx )
		{
			CLAM_ASSERT( mSineTrackList!=NULL, "No assigned track list" );
			SineTrackList& trackList = *mSineTrackList;
	
			for ( int i = 0; i < a.Size(); i++ )
			{
					const Partial& p = a[i];

					if ( p.mTrackId >= 0 )
					{
							
							TrackingList::iterator i = mHeadsList.find(  p.mTrackId  );

							if ( i == mHeadsList.end() ) // new track at the end
							{
									trackList.push_back( SinusoidalTrack() );								
									mHeadsList.insert( std::make_pair( p.mTrackId, TrackHead( p.mTrackId, frame_idx, &trackList.back() ) ) );
									
									TrackHead& h = mHeadsList[p.mTrackId];
									h.Update( frame_idx, p );
							}
							else  // exact match
							{
									// liveness check
									TrackHead& h = i->second;
									
									if ( h.IsAlive( frame_idx ) )
									{
											h.Update( frame_idx, p );
									}
									else  // it was dead, so we have to "resurrect" it
									{
											trackList.push_back( SinusoidalTrack() );
											h.AssignList( &trackList.back() );
											h.Update( frame_idx, p );
									}
							}
					}
			}

		}
		
}

