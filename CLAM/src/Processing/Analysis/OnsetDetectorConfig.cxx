#include "OnsetDetectorConfig.hxx"

namespace CLAM
{
	/////////////////////////////////////////////////////////////////////////////////
	void OnsetDetectorConfig::DefaultInit()
	{
		AddName();
		AddnBands();
		AddLowFreq();
		AddHighFreq();
		AddSmoothFiltSize();
		AddBandThreshold();
		AddMinPeakDist();
		AddGlobalThreshold();
		AddNoiseThreshold();
		AddComputeOffsets();
		AddOffsetThreshold();
		UpdateData();

		/* Set default values */
		SetnBands(20);
		SetLowFreq(100);
		SetHighFreq(10000);
		SetSmoothFiltSize(10);		//in samples (40ms at 245 Hz, the sampling rate used in the algorithm)
		SetBandThreshold(0.25);
		SetMinPeakDist(18);//12);			//in samples (50ms at 245 Hz, the sampling rate used in the algorithm)
		SetGlobalThreshold(25);
		SetNoiseThreshold(0.01);
		SetComputeOffsets(true);
		SetOffsetThreshold(0.08);

	}

}
