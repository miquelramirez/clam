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
