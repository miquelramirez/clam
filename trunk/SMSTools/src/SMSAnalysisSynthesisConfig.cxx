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
	SetOutputSoundFile("outputSound.wav");
	SetInputAnalysisFile("");
	SetOutputAnalysisFile("outputAnalysis.sdif");
/** Analysis **/
	SetAnalysisWindowSize(2049);
	/* by default (if -1) will take (WindowSize-1)/2*/
	SetAnalysisHopSize(256);
	SetAnalysisWindowType(EWindowType::eBlackmanHarris92);
	
	SetResAnalysisWindowSize(1025);
	SetResAnalysisWindowType(EWindowType::eBlackmanHarris92);
	
	SetAnalysisZeroPaddingFactor(2);
	SetAnalysisPeakDetectMagThreshold(-150);//in dB
	SetAnalysisPeakDetectMaxFreq(10000);
	SetAnalysisSinTrackingFreqDeviation(20);//in percetage
	SetAnalysisReferenceFundFreq(1000); 
	SetAnalysisLowestFundFreq(40); 
	SetAnalysisHighestFundFreq(6000); 
	SetAnalysisHarmonic(true);
	SetDoCleanTracks(false);
	SetCleanTracksMaxDropOut( 8 );
	SetCleanTracksMinLength( 4 );
/** Synthesis **/
	SetSynthesisFrameSize(256);
	SetSynthesisWindowType(EWindowType::eTriangular);
/** Morph **/
	SetMorphSoundFile("");

}

