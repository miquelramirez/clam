#include "SMSAnalysisSynthesisConfig.hxx"

using namespace CLAM;

void SMSAnalysisSynthesisConfig::DefaultInit()
{
	//It is not really clear wether some attributes should not be added by default
	AddAll();
	UpdateData();

	DefaultValues();

}

void SMSAnalysisSynthesisConfig::DefaultValues()
{

/** General	**/
	SetInputSoundFile("");
	SetOutputSoundFile("");
	SetInputAnalysisFile("");
	SetOutputAnalysisFile("");
	SetSamplingRate(44100);
/** Analysis **/
	SetAnalysisWindowSize(513);
	/* by default (if -1) will take (WindowSize-1)/2*/
	SetAnalysisHopSize(-1);
	SetAnalysisWindowType(EWindowType::eHamming);
	
	SetResAnalysisWindowSize(513);
	SetResAnalysisWindowType(EWindowType::eBlackmanHarris92);
	
	SetAnalysisZeroPaddingFactor(0);
	SetAnalysisPeakDetectMagThreshold(-60);//in dB
	SetAnalysisMaxSines(50);
	SetAnalysisSinTrackingFreqDeviation(20);//in percetage
	SetAnalysisReferenceFundFreq(1000); 
	SetAnalysisLowestFundFreq(40); 
	SetAnalysisHighestFundFreq(6000); 
	SetAnalysisMaxFundFreqError(10); 				 
	SetAnalysisMaxFundCandidates(50);
	SetAnalysisHarmonic(false);
	SetDoCleanTracks(false);
/** Synthesis **/
	SetSynthesisFrameSize(256);
	SetSynthesisWindowType(EWindowType::eTriangular);
	/* by default (if -1) will take (WindowSize-1)/2*/
	SetSynthesisHopSize(-1);
	SetSynthesisZeroPaddingFactor(0);
	SetSynthesisPhaseManagementType(EPhaseGeneration::eContinuation);

}
