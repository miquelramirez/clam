#include "IFFTConfig.hxx"

namespace CLAM
{
	void IFFTConfig::DefaultInit(void)
	{
		AddAudioSize();
		UpdateData();
		SetAudioSize(1024);
	}	
}

