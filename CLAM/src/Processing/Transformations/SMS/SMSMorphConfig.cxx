#include "SMSMorphConfig.hxx"

namespace CLAM
{
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

		SetInterpolateFrame(true);
		SetHybSinFreq(GetHybBPF());
		SetHybSinAmp(GetHybBPF());
		SetHybPitch(GetHybSinFreq());

		SetHybResAmp(GetHybBPF());

		SetSynchronizeTime(tmpBPF);

		BPF defaultWeights;
		defaultWeights.Insert( 0.0, 0.5 );
		defaultWeights.Insert( 1.0, 0.5 );

		SetHybSinShapeW1( defaultWeights );
		SetHybSinShapeW2( defaultWeights );
		
		SetHybResShapeW1( defaultWeights );
		SetHybResShapeW2( defaultWeights );

		BPF defaultShapeWeights;
		defaultShapeWeights.Insert( 0.0, 1.0 );
		defaultShapeWeights.Insert( 1.0, 1.0 );


		SetHybSinSpectralShape( defaultShapeWeights );
		SetHybResSpectralShape( defaultShapeWeights );
	}
}
