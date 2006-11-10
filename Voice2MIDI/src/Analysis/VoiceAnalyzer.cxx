/*
 * VoiceAnalyzer.cxx
 * Implementation for VoiceAnalyzer class.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include "VoiceAnalyzer.hxx"
#include <CLAM/SpectrumConfig.hxx>

using namespace CLAM;

//////////////////// VoiceAnalyzerConfig ////////////////////

void VoiceAnalyzerConfig::DefaultInit()
{
    AddAll();
    UpdateData();
    DefaultValues();
}

void VoiceAnalyzerConfig::DefaultValues()
{
    GetSpectralAnalysis().SetWindowType(EWindowType::eHamming);
    GetPeakDetect().SetMagThreshold(-80.0f);
    GetPeakDetect().SetMaxFreq(5500.0);
    GetFundFreqDetect().SetLowestFundFreq(TData(20.0));
    GetFundFreqDetect().SetHighestFundFreq(TData(1000.0));  
}

void VoiceAnalyzerConfig::SetWindowSize(TSize w)
{
    GetSpectralAnalysis().SetWindowSize(w);
}

TSize VoiceAnalyzerConfig::GetWindowSize()
{
    return GetSpectralAnalysis().GetWindowSize();
}

void VoiceAnalyzerConfig::SetWindowType(const EWindowType& t)
{
    GetSpectralAnalysis().SetWindowType(t);
}

void VoiceAnalyzerConfig::SetZeroPadding(int z)
{
    GetSpectralAnalysis().SetZeroPadding(z);
}

void VoiceAnalyzerConfig::SetSamplingRate(TData sr)
{
    GetSpectralAnalysis().SetSamplingRate(sr);
}

TData VoiceAnalyzerConfig::GetSamplingRate() const
{
    return GetSpectralAnalysis().GetSamplingRate();
}

void VoiceAnalyzerConfig::SetAnalysisPeakDetectMagThreshold(TData mt)
{
    GetPeakDetect().SetMagThreshold(mt);
}

TData VoiceAnalyzerConfig::GetAnalysisPeakDetectMagThreshold() const
{
    return GetPeakDetect().GetMagThreshold();
}

void VoiceAnalyzerConfig::SetHopSize(TSize h)
{
    SetprHopSize(h);
}

TSize VoiceAnalyzerConfig::GetHopSize() const
{
    return GetprHopSize();
}

//////////////////// VoiceAnalyzer ////////////////////

VoiceAnalyzer::VoiceAnalyzer()
{
    AttachChildren();
    Configure(VoiceAnalyzerConfig());
}

VoiceAnalyzer::VoiceAnalyzer(const VoiceAnalyzerConfig& cfg)
{
    AttachChildren();
    Configure(cfg);
}

bool VoiceAnalyzer::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
{
    mConfig=dynamic_cast<const VoiceAnalyzerConfig&> (c);
    ConfigureChildren();
    ConfigureData();
    return true;
}

const ProcessingConfig& VoiceAnalyzer::GetConfig() const
{
    return mConfig;
}

void VoiceAnalyzer::AttachChildren()
{
    mPO_SpectralAnalysis.SetParent(this);
    mPO_PeakDetect.SetParent(this);
    mPO_FundDetect.SetParent(this);
}

bool VoiceAnalyzer::ConfigureChildren()
{
    mPO_SpectralAnalysis.Configure(mConfig.GetSpectralAnalysis());
    mPO_PeakDetect.Configure(mConfig.GetPeakDetect());
    mPO_FundDetect.Configure(mConfig.GetFundFreqDetect());

    return true;
}

bool VoiceAnalyzer::ConcreteStart()
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

bool VoiceAnalyzer::Do(const Audio& in,
                       Spectrum& outSpec,
                       SpectralPeakArray& outPk,
		       Fundamental& outFn)
{
    /* spectral analysis */
    mPO_SpectralAnalysis.Do(in,mSpec);
    /* global spectrum */
    outSpec = mSpec;
    /* compute energy of the frame */
    ComputeEnergy(mSpec);
    /* pitch detection */
    PitchDetection(mSpec,outPk,outFn);
    return true;
}

bool VoiceAnalyzer::Do(Frame& in)
{
    InitFrame(in);
    return Do(in.GetAudioFrame(),
	      in.GetSpectrum(),
	      in.GetSpectralPeakArray(),
	      in.GetFundamental());
}

void VoiceAnalyzer::PitchDetection(Spectrum& inSp,SpectralPeakArray& pkArray,Fundamental& outFn)
{
    // Convert spectrum to dB
    inSp.ToDB();
    SpectralPeakArray tmpPk;
    tmpPk.SetScale( EScale::eLog);
    mPO_PeakDetect.Do(inSp,tmpPk);
    pkArray = tmpPk;
    mPO_FundDetect.Do(tmpPk,outFn);
    // Convert to linear
    inSp.ToLinear();
}

void VoiceAnalyzer::InitFrame(Frame& in)
{
    // add attributes needed to the frame
    in.AddSpectrum();
    in.AddSpectralPeakArray();
    in.AddFundamental();
    in.UpdateData();
    /* configuration */
    TData specRange = in.GetAudioFrame().GetSampleRate()/2.0;
    in.GetSpectrum().SetSpectralRange(specRange);
    in.GetFundamental().SetnMaxCandidates(1);
}

void VoiceAnalyzer::ConfigureData()
{
    /* configure the aux spectrum */
    SpectrumConfig scfg;
    scfg.SetSpectralRange(mConfig.GetSamplingRate()/2.0);
    scfg.SetSize(mConfig.GetSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
    mSpec.Configure(scfg);	
}

void VoiceAnalyzer::ComputeEnergy(const Spectrum& spec)
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

SegmentDescriptors VoiceAnalyzer::GetDescriptors()
{
    return mSegmentDescriptors;
}

// END




