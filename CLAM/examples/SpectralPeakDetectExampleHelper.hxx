#ifndef __SPECTRAL_PEAK_DETECT_EXAMPLE_HELPER__
#define __SPECTRAL_PEAK_DETECT_EXAMPLE_HELPER__

#include "Filename.hxx"
#include "SpectralPeakDetectConfig.hxx"
#include "SpectralPeakArray.hxx"
#include <list>
#include "ProcessingData.hxx"

namespace CLAMExamples
{

class SpectralPeaksSet
	: public CLAM::ProcessingData
{
	DYNAMIC_TYPE_USING_INTERFACE( SpectralPeaksSet, 3,        CLAM::ProcessingData );
	DYN_ATTRIBUTE( 0, public, CLAM::Filename,                 SpectralAnalysis     );
	DYN_ATTRIBUTE( 1, public, CLAM::SpectralPeakDetectConfig, PeakDetectionSettings );
	DYN_CONTAINER_ATTRIBUTE( 2, public, std::list< CLAM::SpectralPeakArray >, DetectedPeaks, PeaksInFrame );

protected:
	void DefaultInit();

public:

	typedef std::list< CLAM::SpectralPeakArray >  SpectralPeaksList;
};
}



#endif // SpectralPeakDetectExampleHelper.hxx
