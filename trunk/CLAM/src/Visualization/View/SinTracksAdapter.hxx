#ifndef __SINTRACKSADAPTER__
#define __SINTRACKSADAPTER__

#include "SpectralPeakArrayAdapter.hxx"
#include "SinTracksModel.hxx"
#include "SineTracksDef.hxx"
#include "SinTrackListBuilder.hxx"
#include "DataTypes.hxx"

namespace CLAM
{
	template <typename T> class Array;
	class Segment;
	class SpectralPeakArray;
};


namespace CLAMVM
{
	using CLAM::Segment;
	using CLAM::ProcessingData;
	using CLAM::Array;

	class SinTracksAdapter 
		: public SpectralPeakArrayAdapter, public SinTracksModel
	{
		// attributes
	private:
		const Segment*                  mObserved;
		const CLAM::Array< CLAM::SpectralPeakArray >* mMatrixObserved;
		CLAM::TData                     mMatrixSampleRate;
		SineTrackList                   mExtractedList;
		SinTrackBuilder                 mTrackBuilder;

		// implementation details
	protected:
				
		virtual void TransmitPeakArrays();

		// class interface
	public:

		SinTracksAdapter();

		virtual ~SinTracksAdapter();

		virtual const char* GetClassName() const
		{
			return "SinTracksAdapter";
		}


		virtual bool Publish();
			
		virtual bool BindTo( const ProcessingData& procDataObj );

		virtual bool BindTo( const CLAM::Array< CLAM::SpectralPeakArray >& peakMatrix, CLAM::TData srate );
	};

}
#endif // SinTrackView.hxx
