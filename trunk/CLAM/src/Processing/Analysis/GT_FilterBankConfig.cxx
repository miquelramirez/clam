#include "GT_FilterBankConfig.hxx"

namespace CLAM
{
	void GT_FilterBankConfig::DefaultInit()
	{
		AddLowFreq();
		AddHighFreq();
		AddnChannels();
		AddSampleRate();
		UpdateData();
		
		SetLowFreq(100);
		SetHighFreq(44100/4);
		SetnChannels(10);
		
	}

}
