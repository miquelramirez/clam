#ifndef __SINETRACKSADAPTER__
#define __SINETRACKSADAPTER__

#include "Segment.hxx"
#include "SinTracksBuilder.hxx"

namespace CLAM
{
	namespace VM
	{
		class SineTracksAdapter
		{
			public:
				SineTracksAdapter();
				virtual ~SineTracksAdapter();

				SineTrackList& GetTrackList(const Segment& seg);
				SineTrackList& GetTrackList(const Array< SpectralPeakArray >& peakMtx);

			private:
				const Segment*	_segment;
				const Array< SpectralPeakArray >*	_matrix;
				Array< Partial >	_partialBuffer;
				SineTrackList	_extractedList;
				SinTrackBuilder	_trackBuilder;

				bool TranscribePeakArray( const SpectralPeakArray& peakArray );
				void TransmitPeakArrays();
		};
	}
}

#endif



