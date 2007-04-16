#include "StdioSinTracksPresentation.hxx"
#include "SinTracksModel.hxx"
//#include "SinTrackSegment.hxx"
#include <algorithm>
#include <iostream>

namespace CLAMVM
{
		void StdioSinTracksPresentation::dump_to_stdout::operator()( const SinusoidalTrack& st )
		{

				if ( st.empty() )
				{
						std::cout << "(WW) Track #" << real_track_id << " was empty!" << std::endl;

						return;
				}
				
				SinusoidalTrack::const_iterator i, end;
				i = st.begin();
				end = st.end();

				std::cout << "Track #" << real_track_id << std::endl;
				
				while ( i != end )
				{
						if ( i->isValid() )
						{
								std::cout << "(" << i->mFreq << "," << i->mTimeIndex << "," << i->mMag << ") ";
						}
						
						i++;
				}
				std::cout << "   " << std::endl;
				
				real_track_id++;

		}

		
		StdioSinTracksPresentation::StdioSinTracksPresentation()
			: mSineTracks( NULL )
		{
				SetPartials.Wrap( this, &StdioSinTracksPresentation::OnNewTrackList );
				SetSpectralRange.Wrap( this, &StdioSinTracksPresentation::OnNewRange );
				SetDuration.Wrap( this, &StdioSinTracksPresentation::OnNewDuration );
		}

		StdioSinTracksPresentation::~StdioSinTracksPresentation()
		{
		}

		void StdioSinTracksPresentation::AttachTo( SinTracksModel& model )
		{
				
				model.TrackListPublished.Connect( SetPartials );
				model.SpectralRangePublished.Connect( SetSpectralRange );
				model.DurationPublished.Connect( SetDuration  );
		}

		void StdioSinTracksPresentation::Detach()
		{
			SetPartials.Unbind();
			SetSpectralRange.Unbind();
			SetDuration.Unbind();
		}

		void StdioSinTracksPresentation::Show()
		{

				// we will dump the present tracks as well as the valid segments present in each track
			decimate_sine_tracks        Decimate( 3 );

			std::cout << "DECIMATION... ";
	
			Decimate( *mSineTracks );

			std::cout << "DONE!" << std::endl;
	
			std::for_each( mSineTracks->begin(), mSineTracks->end(), dump_to_stdout() );
				
		}

		void StdioSinTracksPresentation::Hide()
		{
		}
		
		void StdioSinTracksPresentation::OnNewRange( TData spec_rng )
		{
				mSpectralRange = spec_rng;
		}

		void StdioSinTracksPresentation::OnNewDuration( TTime begin, TTime end )
		{
				mBeginTime = begin;
				mEndTime = end;
				mLen = end - begin;
		}

		void StdioSinTracksPresentation::OnNewTrackList( SineTrackList& list, TSize framelen )
		{
				// ufff

				mSineTracks = &list;
				
		}
		
		
}

