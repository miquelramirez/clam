#include "FundFreqDetectConfig.hxx"

namespace CLAM
{
	/* The Configuration object has at least to have a name */

	void FundFreqDetectConfig::DefaultInit()
	{
		AddAll();
		/* All Attributes are added */
		UpdateData();
		/* Set default values */
		DefaultValues();
	}
	
	void FundFreqDetectConfig::DefaultValues()
	{		
		SetReferenceFundFreq(1000);
		SetLowestFundFreq(35);
		SetHighestFundFreq(2000);
		SetMaxCandMagDiff(30);
		SetMaxFundFreqError(10); 
		SetNInt(5);
		SetPMp(0.5);
		SetPMq(TData(1.4));
		SetPMr(TData(0.5));
		SetMPp(0.5);
		SetMPq(TData(1.4));
		SetMPr(0.5);
		SetPMnPeaks(10);
		SetMPnPeaks(10);
		SetPMCont(1.0);
		SetMPCont(1.0);
		SetNMaxCandidates(60);
	}


}
