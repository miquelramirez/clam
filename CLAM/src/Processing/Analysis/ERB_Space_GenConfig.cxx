#include "ERB_Space_GenConfig.hxx"

namespace CLAM
{
	
	void ERB_SpaceGenConfig::DefaultInit()
	{
		AddNumFilter();
		AddLowFreq();
		AddHighFreq();
		UpdateData();
		//Default Values
		SetNumFilter(10);
		SetLowFreq(100);
		SetHighFreq(44100/4);
	}

}
