#ifndef __AUDIOMODEL__
#define __AUDIOMODEL__

#include "Signalv1.hxx"
#include "Signalv4.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"

namespace CLAMVM
{
	using SigSlot::Signalv1;
	using SigSlot::Signalv4;
	using CLAM::TData;
	using CLAM::TTime;
	using CLAM::DataArray;
	class AudioAdapter;

	class AudioModel
	{
	public:				
		AudioModel(  );

		virtual ~AudioModel();

		//signals
		Signalv1< const DataArray& >                          SamplesPublished;
		Signalv1< TTime >                                     DurationPublished;
		Signalv1< TTime >                                     StartTimePublished;
		Signalv1< TData >                                     SampleRatePublished;
		Signalv4< const DataArray&, TTime, TTime, TData >     ObjectPublished;

	};

}


#endif // AudioModel.hxx
