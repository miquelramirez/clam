#include "SpectralAnalyzer.hxx"

void SpectralAnalyzerConfig::DefaultInit()
{
    AddAll();
    UpdateData();
    DefaultValues();
}

void SpectralAnalyzerConfig::DefaultValues()
{
    SetprZeroPadding(0);  
    SetWindowSize(513);  
    //SetWindowType(EWindowType::eBlackmanHarris92);  
    SetWindowType(EWindowType::eHamming);
}     

void SpectralAnalyzerConfig::SetWindowSize(TSize w)
{
    CLAM_ASSERT(w%2==1,"Window size must be odd");
    GetWindowGenerator().SetSize(w);
    /*
     * compute FFT size from window size 
     * and the zero padding value
    */
    SetprFFTSize(ComputeFFTSize(GetWindowSize(),GetZeroPadding()));
    GetCircularShift().SetAmount( -((w-1)/2) );
    /* set FFT size */
    GetFFT().SetAudioSize(GetprFFTSize());
}

TSize SpectralAnalyzerConfig::GetWindowSize() const
{
    return GetWindowGenerator().GetSize();
}

void SpectralAnalyzerConfig::SetWindowType(const EWindowType& t)
{
    GetWindowGenerator().SetType(t);
}

const EWindowType& SpectralAnalyzerConfig::GetWindowType() const
{
    return GetWindowGenerator().GetType();
}

void SpectralAnalyzerConfig::SetZeroPadding(int z)
{
    SetprZeroPadding(z);
    SetprFFTSize(ComputeFFTSize(GetWindowSize(),GetZeroPadding()));
    GetFFT().SetAudioSize(GetprFFTSize());
}

int SpectralAnalyzerConfig::GetZeroPadding() const
{
    return GetprZeroPadding();
}

void SpectralAnalyzerConfig::SetSamplingRate(TData sr)
{
    SetprSamplingRate(sr);
}

TData SpectralAnalyzerConfig::GetSamplingRate() const
{
    return GetprSamplingRate();
}

int SpectralAnalyzerConfig::ComputeFFTSize(int wSize,int zpFact)
{
    double tmp = log(double(wSize))/log(2.0); // compute log2
    double aux = tmp-int(tmp);
    aux = (aux >= 0.5) ? 1 : 0; // round
    int pot = int(tmp)+int(aux)+zpFact; // exponent

    return int(pow(2.0,pot));  // compute and return the result
}

SpectralAnalyzer::SpectralAnalyzer()
{
    AttachChildren();
    Configure(SpectralAnalyzerConfig()); 
}

SpectralAnalyzer::SpectralAnalyzer(const SpectralAnalyzerConfig& cfg)
{
    AttachChildren();
    Configure(cfg);
}

bool SpectralAnalyzer::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
{
    mConfig=dynamic_cast<const SpectralAnalyzerConfig&> (c);
    ConfigureChildren();
    ConfigureData();
    return true;
}  

const ProcessingConfig& SpectralAnalyzer::GetConfig() const
{
    return mConfig;
}

bool SpectralAnalyzer::Do(const Audio& in,Spectrum& out)
{
    in.GetAudioChunk(0,in.GetSize()-1,mAudioFrame,true);
    mAudioFrame.SetSize(mConfig.GetFFT().GetAudioSize());
  
    /* windowing */
    mPO_AProduct.Do(mAudioFrame,mWindow,mAudioFrame);
 
    /* circular shift */
    mPO_CShift.Do(mAudioFrame,mAudioFrame);
  
    /* apply FFT to get the spectrum */
    mPO_FFT.Do(mAudioFrame,out);

    return true;
}

bool SpectralAnalyzer::Do(Frame& in)
{
    ConfigureFrame(in);
    return Do(in.GetAudioFrame(),in.GetSpectrum());
}

void SpectralAnalyzer::AttachChildren()
{
    mPO_FFT.SetParent(this);
    mPO_WinGen.SetParent(this);
    mPO_AProduct.SetParent(this);
    mPO_CShift.SetParent(this);
}

bool SpectralAnalyzer::ConfigureChildren()
{
    mPO_FFT.Configure(mConfig.GetFFT());
    mPO_WinGen.Configure(mConfig.GetWindowGenerator());
    mPO_CShift.Configure(mConfig.GetCircularShift());
    return true;
}

void SpectralAnalyzer::ConfigureData()
{
    /* sample rate to audio multiplier */
    mAudioFrame.SetSampleRate(mConfig.GetSamplingRate());

    mWindow.SetSize(mConfig.GetWindowSize());

    /* generate window */
    mPO_WinGen.Do(mWindow);

    mWindow.SetSize(mConfig.GetFFT().GetAudioSize());  // add zero padding
}

void SpectralAnalyzer::ConfigureFrame(Frame& in)
{
    in.AddSpectrum();
    in.UpdateData();
    // configure spectrum
    CLAM::SpecTypeFlags specFlags;
    specFlags.bMagPhase = 1;
    in.GetSpectrum().SetType(specFlags);
    in.GetSpectrum().SetSize(mConfig.GetFFT().GetAudioSize()/2+1);  
}

// END

