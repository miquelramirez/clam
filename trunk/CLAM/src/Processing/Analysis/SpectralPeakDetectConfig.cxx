#include "SpectralPeakDetectConfig.hxx"

namespace CLAM
{
	/* The  Configuration object has at least to have a name */

	void SpectralPeakDetectConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		DefaultValues();
	}

	
	void SpectralPeakDetectConfig::DefaultValues()
	{
		SetMagThreshold(-80.f);
		SetMaxPeaks(1000);
		SetMaxFreq(10000.f);
	}

}
