#include "RappidTypes.hxx"


void RappidAppConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
	SetFrameSize(512);
	SetSampleRate(48000);
	SetVerbose(false);
	SetADAT(false);
}

void RappidAppConfig::Dump(void)
{
	std::cout << "      Sample rate:         ";
	std::cout << GetSampleRate();
	std::cout << " (For real time operation)";
	std::cout << std::endl;
	std::cout << "      Frame size:          ";
	std::cout << GetFrameSize();
	std::cout << std::endl;
	std::cout << "      ADAT interface:      ";
	std::cout << GetADAT();
	std::cout << std::endl;
}

