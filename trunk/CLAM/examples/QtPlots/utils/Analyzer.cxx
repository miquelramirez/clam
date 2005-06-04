#include "Analyzer.hxx"
#include "SpectrumConfig.hxx"

using namespace CLAM;

void AnalyzerConfig::DefaultInit()
{
    AddAll();
    UpdateData();
    DefaultValues();
}

void AnalyzerConfig::DefaultValues()
{
    GetSpectralAnalysis().SetWindowType(EWindowType::eBlackmanHarris92);
    GetPeakDetect().SetMagThreshold(-120.0f);
    GetPeakDetect().SetMaxFreq(22050.0);
    GetFundFreqDetect().SetLowestFundFreq(TData(20.0));
    GetFundFreqDetect().SetHighestFundFreq(TData(2000.0));  
}

void AnalyzerConfig::SetWindowSize(TSize w)
{
    GetSpectralAnalysis().SetWindowSize(w);
}

TSize AnalyzerConfig::GetWindowSize()
{
    return GetSpectralAnalysis().GetWindowSize();
}

void AnalyzerConfig::SetWindowType(const EWindowType& t)
{
    GetSpectralAnalysis().SetWindowType(t);
}

void AnalyzerConfig::SetZeroPadding(int z)
{
    GetSpectralAnalysis().SetZeroPadding(z);
}

void AnalyzerConfig::SetSamplingRate(TData sr)
{
    GetSpectralAnalysis().SetSamplingRate(sr);
}

TData AnalyzerConfig::GetSamplingRate() const
{
    return GetSpectralAnalysis().GetSamplingRate();
}

void AnalyzerConfig::SetAnalysisPeakDetectMagThreshold(TData mt)
{
    GetPeakDetect().SetMagThreshold(mt);
}

TData AnalyzerConfig::GetAnalysisPeakDetectMagThreshold() const
{
    return GetPeakDetect().GetMagThreshold();
}

void AnalyzerConfig::SetHopSize(TSize h)
{
    SetprHopSize(h);
}

TSize AnalyzerConfig::GetHopSize() const
{
    return GetprHopSize();
}

Analyzer::Analyzer()
{
    AttachChildren();
    Configure(AnalyzerConfig());
}

Analyzer::Analyzer(const AnalyzerConfig& cfg)
{
    AttachChildren();
    Configure(cfg);
}

bool Analyzer::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
{
    mConfig=dynamic_cast<const AnalyzerConfig&> (c);
    ConfigureChildren();
    ConfigureData();
    return true;
}

const ProcessingConfig& Analyzer::GetConfig() const
{
    return mConfig;
}

void Analyzer::AttachChildren()
{
    mPO_SpectralAnalysis.SetParent(this);
    mPO_PeakDetect.SetParent(this);
    mPO_FundDetect.SetParent(this);
}

bool Analyzer::ConfigureChildren()
{
    mPO_SpectralAnalysis.Configure(mConfig.GetSpectralAnalysis());
    mPO_PeakDetect.Configure(mConfig.GetPeakDetect());
    mPO_FundDetect.Configure(mConfig.GetFundFreqDetect());

    return true;
}

bool Analyzer::ConcreteStart()
{
    if(mSegmentDescriptors.HasFramesD())
    {
	mSegmentDescriptors.RemoveFramesD();
	mSegmentDescriptors.UpdateData();
    }
    mSegmentDescriptors.AddFramesD();
    mSegmentDescriptors.UpdateData();

    return ProcessingComposite::ConcreteStart();
}

bool Analyzer::Do(const Audio& in,
                  Spectrum& outSpec,
		  SpectralPeakArray& outPk,
		  Fundamental& outFn)
{
    mPO_SpectralAnalysis.Do(in,mSpec);
    outSpec = mSpec;
    ComputeEnergy(mSpec);
    PitchDetection(mSpec,outPk,outFn);
    return true;
}

bool Analyzer::Do(Frame& in)
{
    InitFrame(in);
    return Do(in.GetAudioFrame(),
	      in.GetSpectrum(),
	      in.GetSpectralPeakArray(),
	      in.GetFundamental());
}

void Analyzer::PitchDetection(Spectrum& inSp,SpectralPeakArray& pkArray,Fundamental& outFn)
{
    inSp.ToDB();
    SpectralPeakArray tmpPk;
    tmpPk.SetScale( EScale::eLog);
    mPO_PeakDetect.Do(inSp,tmpPk);
    pkArray = tmpPk;
    mPO_FundDetect.Do(tmpPk,outFn);
    inSp.ToLinear();
}

void Analyzer::InitFrame(Frame& in)
{
    in.AddSpectrum();
    in.AddSpectralPeakArray();
    in.AddFundamental();
    in.UpdateData();
    
    TData specRange = in.GetAudioFrame().GetSampleRate()/2.0;
    in.GetSpectrum().SetSpectralRange(specRange);
    in.GetFundamental().SetnMaxCandidates(1);
}

void Analyzer::ConfigureData()
{
    SpectrumConfig scfg;
    scfg.SetSpectralRange(mConfig.GetSamplingRate()/2.0);
    scfg.SetSize(mConfig.GetSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
    mSpec.Configure(scfg);	
}

void Analyzer::ComputeEnergy(const Spectrum& spec)
{
    FrameDescriptors tmpFrameD;
    tmpFrameD.AddSpectrumD();
    tmpFrameD.UpdateData();
    SpectralDescriptors tmpSpecD;
    tmpSpecD.AddEnergy();
    tmpSpecD.UpdateData();
    tmpFrameD.SetSpectrumD(tmpSpecD);
    Energy energy;
    tmpFrameD.GetSpectrumD().SetEnergy(energy(spec.GetMagBuffer()));
    mSegmentDescriptors.GetFramesD().AddElem(tmpFrameD);
}

const SegmentDescriptors& Analyzer::GetDescriptors() const
{
    return mSegmentDescriptors;
}

// END




