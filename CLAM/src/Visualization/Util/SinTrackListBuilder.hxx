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
