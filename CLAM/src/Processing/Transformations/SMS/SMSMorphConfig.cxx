#include "SMSMorphConfig.hxx"

namespace CLAM
{
	SMSMorphConfig::~SMSMorphConfig()
	{
	}

	void SMSMorphConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		DefaultValues();
	}

	void SMSMorphConfig::DefaultValues()
	{
		BPF tmpBPF(2);
		tmpBPF.SetValue(0,0);
		tmpBPF.SetXValue(0,0);
		tmpBPF.SetValue(1,1);
		tmpBPF.SetXValue(1,1);
		SetHybBPF(tmpBPF);

		AddInterpolateFrame();
		UpdateData();
		SetInterpolateFrame(true);
		AddHybSinFreq();
		UpdateData();
		SetHybSinFreq(GetHybBPF());
		AddHybSinAmp();
		UpdateData();
		SetHybSinAmp(GetHybBPF());
		AddHybPitch();
		UpdateData();
		SetHybPitch(GetHybSinFreq());

		AddHybResAmp();
		UpdateData();
		SetHybResAmp(GetHybBPF());
		AddSynchronizeTime();
		UpdateData();
		BPF tmpBPF2(2);
		tmpBPF2.SetXValue(0,0);
		tmpBPF2.SetValue(0,0);
		tmpBPF2.SetXValue(1,1);
		tmpBPF2.SetValue(1,1);
		SetSynchronizeTime(tmpBPF2);

	}
}
