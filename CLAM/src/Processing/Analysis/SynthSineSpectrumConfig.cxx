#include "SynthSineSpectrumConfig.hxx"

namespace CLAM
{

/******************************************************************/
/************* SynthSineSpectrum CONFIGURATION ********************/


void SynthSineSpectrumConfig::DefaultInit()
{
	AddSpectrumSize();
	AddSamplingRate();
	AddZeroPadding();
	UpdateData();

	DefaultValues();
}

void SynthSineSpectrumConfig::DefaultValues()
{
	SetSpectrumSize(1024);
	SetSamplingRate(44100);
	SetZeroPadding(0);
}


}
