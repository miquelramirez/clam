#ifndef __SINETRACKSMODEL__
#define __SINETRACKSMODEL__

#include "DataTypes.hxx"
#include "Array.hxx"
#include "Partial.hxx"
#include "Signalv1.hxx"
#include "Signalv2.hxx"
#include "SineTracksDef.hxx"

namespace CLAMVM
{
	using SigSlot::Signalv1;
	using SigSlot::Signalv2;
	using CLAM::Array;
	using CLAM::TTime;
	using CLAM::TData;


	class SinTracksModel
	{
	public:
				
		SinTracksModel( );
				
		virtual ~SinTracksModel();


		// Signals
		Signalv2< SineTrackList& , int > TrackListPublished;
		Signalv1< TData >                SpectralRangePublished;
		Signalv2< TTime, TTime >         DurationPublished;

	};

}

#endif // SinTracksModel.hxx
