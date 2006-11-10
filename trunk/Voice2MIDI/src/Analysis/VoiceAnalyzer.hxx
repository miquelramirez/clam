/*
 * VoiceAnalyzer.hxx
 * Interface for VoiceAnalyzer class
 *
 * This class implements the analyzer used for extract the characteristics
 * needed to obtain the melody (energy and fundamental frequency).
 * An object of the class MyAnalyzer is used as a member to perform the
 * spectral analysis over the signal that will be analyzed.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _VoiceAnalyzer_
#define _VoiceAnalyzer_

#include "MyAnalyzer.hxx"
#include <CLAM/SpectralPeakDetectConfig.hxx>
#include <CLAM/SpectralPeakDetect.hxx>
#include <CLAM/FundFreqDetect.hxx>
#include <CLAM/SegmentDescriptors.hxx>

using CLAM::SpectralPeakDetectConfig;
using CLAM::SpectralPeakDetect;
using CLAM::FundFreqDetectConfig;
using CLAM::FundFreqDetect;
using CLAM::SegmentDescriptors;
using CLAM::SpectralPeakArray;
using CLAM::Fundamental;

//////////////////// VoiceAnalyzerConfig ////////////////////

class VoiceAnalyzerConfig : public CLAM::ProcessingConfig
{
    DYNAMIC_TYPE_USING_INTERFACE (VoiceAnalyzerConfig,6,CLAM::ProcessingConfig);
    DYN_ATTRIBUTE(0,public,std::string,Name);
    DYN_ATTRIBUTE(1,public,MyAnalyzerConfig,SpectralAnalysis);
    DYN_ATTRIBUTE(2,public,SpectralPeakDetectConfig,PeakDetect);
    DYN_ATTRIBUTE(3,public,FundFreqDetectConfig,FundFreqDetect);
    DYN_ATTRIBUTE(4,protected,TData,prSamplingRate);
    DYN_ATTRIBUTE(5,protected,TSize,prHopSize);

public:
    void SetWindowSize(TSize w);
    TSize GetWindowSize();
    void SetWindowType(const EWindowType& t);
    void SetZeroPadding(int z);
    void SetSamplingRate(TData sr);
    TData GetSamplingRate() const;
    void SetAnalysisPeakDetectMagThreshold(TData mt);
    TData GetAnalysisPeakDetectMagThreshold() const;
    void SetHopSize(TSize h);
    TSize GetHopSize() const;

private:
    void DefaultInit();
    void DefaultValues();
};

//////////////////// VoiceAnalyzer ////////////////////

class VoiceAnalyzer : public ProcessingComposite
{

public:
    VoiceAnalyzer();
    VoiceAnalyzer(const VoiceAnalyzerConfig& cfg);

    const ProcessingConfig& GetConfig() const;

    bool Do(){return false;}
    bool Do(const Audio& in,
	    Spectrum& outSpec,
	    SpectralPeakArray& outPk,
	    Fundamental& outFn);

    bool Do(Frame& in);

    SegmentDescriptors GetDescriptors();

private:
    VoiceAnalyzerConfig mConfig;

    /* child process MyAnalyzer for spectral analysis */
    MyAnalyzer mPO_SpectralAnalysis;

    /* child process for spectral peak detection */
    SpectralPeakDetect mPO_PeakDetect;
    
    /* child process for pitch estimation */
    FundFreqDetect mPO_FundDetect;

    Spectrum mSpec;

    /* descriptors to store the energy */
    SegmentDescriptors mSegmentDescriptors;
	
    bool ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast);
  
    void AttachChildren();
    bool ConfigureChildren();
    void ConfigureData();
    bool ConcreteStart();
    void InitFrame(Frame& in);
    void PitchDetection(Spectrum& inSp,SpectralPeakArray& pkArray,Fundamental& outFn);
    void ComputeEnergy(const Spectrum& spec);
};

#endif


