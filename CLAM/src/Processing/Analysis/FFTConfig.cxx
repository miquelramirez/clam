#include "FFTConfig.hxx"

namespace CLAM
{
	void FFTConfig::DefaultInit()
	{
		AddAudioSize();
		UpdateData();
		SetAudioSize(1024);
	}
		
}

