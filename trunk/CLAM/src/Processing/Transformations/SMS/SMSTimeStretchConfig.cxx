#include "SMSTimeStretchConfig.hxx"

namespace CLAM
{
	void SMSTimeStretchConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		DefaultValues();
	}

	void SMSTimeStretchConfig::DefaultValues()
	{
		SetSamplingRate(44100);
		SetHopSize(256);
		SetUseBPF(true);
	}
}
