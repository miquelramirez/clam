#include "SMSTimeStretchConfig.hxx"

namespace CLAM
{
	SMSTimeStretchConfig::~SMSTimeStretchConfig()
	{
	}

	void SMSTimeStretchConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		DefaultValues();
	}

	void SMSTimeStretchConfig::DefaultValues()
	{
		SetSamplingRate(44100);
		SetHopSize(512);
	}
}
