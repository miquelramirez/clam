#ifndef _Analyzer_
#define _Analyzer_

#include "SpectralAnalyzer.hxx"
#include "SpectralPeakDetectConfig.hxx"
#include "SpectralPeakDetect.hxx"
#include "FundFreqDetect.hxx"
#include "SegmentDescriptors.hxx"

using CLAM::SpectralPeakDetectConfig;
using CLAM::SpectralPeakDetect;
using CLAM::FundFreqDetectConfig;
using CLAM::FundFreqDetect;
using CLAM::SegmentDescriptors;
using CLAM::SpectralPeakArray;
using CLAM::Fundamental;

class AnalyzerConfig : public CLAM::ProcessingConfig
{
    DYNAMIC_TYPE_USING_INTERFACE (AnalyzerConfig,6,CLAM::ProcessingConfig);
    DYN_ATTRIBUTE(0,public,std::string,Name);
    DYN_ATTRIBUTE(1,public,SpectralAnalyzerConfig,SpectralAnalysis);
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

class Analyzer : public ProcessingComposite
{

public:
    Analyzer();
    Analyzer(const AnalyzerConfig& cfg);

    const ProcessingConfig& GetConfig() const;

    bool Do(){return false;}
    bool Do(const Audio& in,
	    Spectrum& outSpec,
	    SpectralPeakArray& outPk,
	    Fundamental& outFn);

    bool Do(Frame& in);

    const SegmentDescriptors& GetDescriptors() const;

private:
    AnalyzerConfig mConfig;
    SpectralAnalyzer mPO_SpectralAnalysis;
    SpectralPeakDetect mPO_PeakDetect;
    FundFreqDetect mPO_FundDetect;

    Spectrum mSpec;

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


