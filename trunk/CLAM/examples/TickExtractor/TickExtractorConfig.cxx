#include "TickExtractorConfig.hxx"

namespace CLAM
{
	OnsetDetectMethod::OnsetDetectMethod()
		: Enum( sEnumValues, sDefault )
	{
	}

	OnsetDetectMethod::OnsetDetectMethod( tValue v )
		: Enum( sEnumValues, v )
	{
	}

	OnsetDetectMethod::OnsetDetectMethod( std::string s )
		: Enum( sEnumValues, s )
	{
	}

	Enum::tEnumValue OnsetDetectMethod::sEnumValues[] = {
		{ eMTG, "MTG" },
		{ eQMUL_Energy, "QMUL_Energy"},
		{ eQMUL_SpectralDifference, "QMUL_SpectralDifference"},
		{ eQMUL_HFC, "QMUL_HFC"},
		{ eQMUL_ComplexDomain, "QMUL_ComplexDomain"},
		{ eQMUL_Phase, "QMUL_Phase"},
		{ 0,           NULL }
	};
	
	Enum::tValue OnsetDetectMethod::sDefault = OnsetDetectMethod::eMTG;

	Component* OnsetDetectMethod::Species() const
	{
		return new OnsetDetectMethod( "MTG" );
	}

	void TickExtractorConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetFromAudio(true);
		SetComputeBeats(true);
		SetSaveIOIHistData(false);
		SetDownsamplingFactor_IOIHist(100.0);
		SetThreshold_IOIHistPeaks(0.0);//in percents
		SetTempoLimInf(50);
		SetTempoLimSup(220);
		SetTickLimInf(0.6);
		SetTickLimSup(0.09);
		SetDeviationPenalty(1);
		SetOverSubdivisionPenalty(5); // bigger --> favor large ticks
		SetGaussianWindowSize(0.2);
		SetScope(0.10); //in seconds
		SetAdjustWithOnsets(true);
		SetNTrans(500);
		SetTransHop(100);
	}

}
