#ifndef __SPECTRALPEAKARRAYASPECT__
#define __SPECTRALPEAKARRAYASPECT__

#include "DataTypes.hxx"
#include "Array.hxx"
#include "Partial.hxx"
#include "Signalv1.hxx"

namespace CLAMVM
{
	using SigSlot::Signalv1;
	using CLAM::Array;

		class SpectralPeakArrayAdapter;

		class SpectralPeakArrayAspect
		{
		public:
			   
				SpectralPeakArrayAspect( SpectralPeakArrayAdapter& );
				
				virtual ~SpectralPeakArrayAspect();

				virtual void ForceViewRefresh();

				// Signals
				Signalv1< const Array<Partial>& >       AcquirePartials;

		private:
				SpectralPeakArrayAdapter&       mAdapter;
		};

}

#endif // SpectralPeakArrayAspect.hxx
