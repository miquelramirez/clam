#include "PhaseManagementConfig.hxx"

namespace CLAM
{
	void PhaseManagementConfig::DefaultInit()
	{
		AddSamplingRate();
		AddMaxSines();
		AddType();
		UpdateData();
		DefaultValues();
	}
	
	void PhaseManagementConfig::DefaultValues()
	{
		SetSamplingRate(44100);
		SetMaxSines(100);
		SetType(EPhaseGeneration::eContinuation);
	}

}
