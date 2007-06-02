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

#ifndef __SINTRACKBUILDER__
#define __SINTRACKBUILDER__

#include <map>
#include "SineTracksDef.hxx"
#include "Partial.hxx"
#include "Array.hxx"
#include "Assert.hxx"

namespace CLAMVM
{
		using CLAM::Array;
		
		class SinTrackBuilder
		{
				// types
		protected:
				
	
				class TrackHead
				{
				private:
						TIndex             mTrackId;
						TIndex             mLastFrame;
						SinusoidalTrack*   mListPtr;
				public:
						TrackHead()
							: mTrackId( -2 )
						{
						}

						TrackHead( TIndex id, TIndex lastFrame, SinusoidalTrack* lptr )
								: mTrackId( id ), mLastFrame( lastFrame ), mListPtr( lptr )
						{
						}

						TrackHead( const TrackHead& h )
						{
								mTrackId = h.mTrackId;
								mLastFrame = h.mLastFrame;
								mListPtr = h.mListPtr;
						}

						const TrackHead& operator=( const TrackHead& h )
						{
								mTrackId = h.mTrackId;
								mLastFrame = h.mLastFrame;
								mListPtr = h.mListPtr;

								return *this;
						}

						void Update( TIndex idx, const Partial& p  )
						{
								mLastFrame = idx;
								TimeFreqMagPoint tfp;

								tfp.mMag = p.mMag;
								tfp.mFreq = p.mFreq;
								tfp.mTimeIndex = idx;

								mListPtr->push_back( tfp );
						}

						bool IsUpToDate( TIndex frame_idx )
						{
								return mLastFrame == frame_idx;
						}

						void Kill()
						{
								mListPtr = NULL;
						}

						bool IsAlive( TIndex i)
						{
								return mLastFrame == i - 1;
						}

						void AssignList( SinusoidalTrack* new_list )
						{								
								CLAM_ASSERT( new_list!=NULL, "new_list is NULL!" );
								mListPtr = new_list;
						}

						bool operator==( TIndex id )
						{
								return mTrackId==id;
						}

						bool operator==( TIndex id ) const
						{
								return mTrackId==id;
						}

						bool operator==( const TrackHead& h )
						{
								return mTrackId==h.mTrackId;
						}

						bool operator==( const TrackHead& h ) const
						{
								return mTrackId==h.mTrackId;
						}


						bool operator!=( TIndex id )
						{
								return mTrackId==id;
						}

						bool operator!=( TIndex id ) const
						{
								return mTrackId==id;
						}

						bool operator>=( TIndex id )
						{
								return mTrackId>=id;
						}

						bool operator>=( TIndex id ) const
						{
								return mTrackId>=id;
						}


				};

				class greater_or_equal_than
				{
				public:
						greater_or_equal_than( TIndex itc )
								: mIndexToCheck( itc )
						{
						}

						bool operator()( const TrackHead& st )
						{
								return st >= mIndexToCheck;
						}
				private:
						TIndex mIndexToCheck;
				};

				class check_dead_track_heads
				{
				public:
						check_dead_track_heads( TIndex i )
								: mCurrentFrame( i )
						{
						}

						void operator()( TrackHead& st )
						{
								if ( !st.IsUpToDate( mCurrentFrame ) )
										st.Kill();
						}
				private:

						TIndex mCurrentFrame;
				};


				typedef std::map< TIndex, TrackHead >   TrackingList;
				
		public:
				SinTrackBuilder( );
				
				void SetTrackList( SineTrackList& list )
				{
					mSineTrackList = &list;
				}

				void AddFrame( const Array<Partial>& , TIndex );
				
				void Flush()
				{
						mHeadsList.clear(); //aire que vol dir vent!
				}

		protected:

		private:
				TrackingList          mHeadsList;
				SineTrackList*        mSineTrackList;
		};

};

#endif // SinTrackBuilder.hxx

