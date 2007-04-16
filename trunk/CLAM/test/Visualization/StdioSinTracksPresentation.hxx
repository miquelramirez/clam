#ifndef __STDIOSINTRACKSPRESENTATION__
#define __STDIOSINTRACKSPRESENTATION__

#include "Presentation.hxx"
#include "Array.hxx"
#include "Slotv1.hxx"
#include "Slotv2.hxx"
#include "DataTypes.hxx"
#include "SinTrackListBuilder.hxx"
#include <list>

namespace CLAMVM
{
		using SigSlot::Slotv1;
		using SigSlot::Slotv2;
		using CLAM::Array;
		using CLAM::TData;
		using CLAM::TTime;
		using CLAM::TSize;

		class SinTracksModel;

		class StdioSinTracksPresentation : public Presentation
		{
		private:
			

				class dump_to_stdout
				{
				public:
						dump_to_stdout()
								: real_track_id ( 1 )
						{
						}

						void operator()( const SinusoidalTrack& st );
				private:
						
						TIndex real_track_id;
				};

				class decimate_sine_tracks
				{
				public:
					decimate_sine_tracks( TSize min_len )
						: minimum_len( min_len )
					{
					}

					void operator() ( SineTrackList& tl )
					{
						SineTrackList::iterator i, end, aux;
						i = tl.begin();
						end = tl.end();

						while ( i != end )
						{
							if ( i->size() < minimum_len )
							{
								aux = i;
								i++;
								tl.erase( aux );
							}

							i++;
							
						}
					}

				private:

					TSize   minimum_len;
				};


		protected:
				
				typedef SineTrackList::iterator iterator;

				SineTrackList*              mSineTracks;
				TData                       mSpectralRange;
				TTime                       mBeginTime;
				TTime                       mEndTime;
				TTime                       mLen;


		protected:


				virtual void OnNewTrackList( SineTrackList& array, TSize framelen );

				virtual void OnNewRange ( TData spec_rng );
				
				virtual void OnNewDuration( TTime begin, TTime end );
		public:
				Slotv2< SineTrackList&, TSize >           SetPartials;
				Slotv1< TData >                                   SetSpectralRange;
				Slotv2< TTime, TTime >                            SetDuration;


				StdioSinTracksPresentation();

				virtual ~StdioSinTracksPresentation();

				virtual void AttachTo( SinTracksModel& );

				virtual void Detach();

				virtual void Show();

				virtual void Hide();
		};
}

#endif // StdioSinTracksPresentation.hxx

