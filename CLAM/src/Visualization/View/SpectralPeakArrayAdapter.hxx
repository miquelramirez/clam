#ifndef __SPECTRALPEAKARRAYADAPTER__
#define __SPECTRALPEAKARRAYADAPTER__

#include "ProcessingDataAdapter.hxx"
#include "SpectralPeaksModel.hxx"

namespace CLAM
{
		class SpectralPeakArray;
		class ProcessingData;
}

namespace CLAMVM
{
		using CLAM::SpectralPeakArray;
		using CLAM::ProcessingData;
		using CLAM::Array;

		class SpectralPeakArrayAdapter 
				: public ProcessingDataAdapter, public SpectralPeaksModel
		{
				// attributes
		private:
				const SpectralPeakArray*        mObserved;

		protected:
				Array< Partial >  mPartialBuffer;

				// implementation details
		protected:

				virtual bool TranscribePeakArray( const SpectralPeakArray& );
				
				//class interface
		public:
				SpectralPeakArrayAdapter();

				virtual ~SpectralPeakArrayAdapter();

				virtual const char* GetClassName() const
				{
						return "SpectralPeakArrayAdapter";
				}
				
				virtual bool Publish();

				virtual bool BindTo( const ProcessingData& procDataObj );
		};
}

#endif // SpectralPeakArrayAdapter.hxx
