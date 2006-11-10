/*
 *  MyAnalyzer.cxx
 *  Implementation for MyAnalyzer class.
 *
 *  Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include "MyAnalyzer.hxx"

//////////////////// MyAnalizerConfig ////////////////////

void MyAnalyzerConfig::DefaultInit()
{
    AddAll();
    UpdateData();
    DefaultValues();
}

void MyAnalyzerConfig::DefaultValues()
{
    SetprZeroPadding(0);  
    SetWindowSize(513);  
    SetWindowType(EWindowType::eHamming);  
    GetCircularShift().SetAmount(-256);
}     

void MyAnalyzerConfig::SetWindowSize(TSize w)
{
    CLAM_ASSERT(w%2==1,"Window size must be odd");
    GetWindowGenerator().SetSize(w);
    /*
     * compute FFT size from window size 
     * and the zero padding value
    */
    SetprFFTSize(ComputeFFTSize(GetWindowSize(),GetZeroPadding()));
    GetCircularShift().SetAmount(-((w-1)/TData(2)));
    /* set FFT size */
    GetFFT().SetAudioSize(GetprFFTSize());
}

TSize MyAnalyzerConfig::GetWindowSize() const
{
    return GetWindowGenerator().GetSize();
}

void MyAnalyzerConfig::SetWindowType(const EWindowType& t)
{
    GetWindowGenerator().SetType(t);
}

const EWindowType& MyAnalyzerConfig::GetWindowType() const
{
    return GetWindowGenerator().GetType();
}

void MyAnalyzerConfig::SetZeroPadding(int z)
{
    SetprZeroPadding(z);
    SetprFFTSize(ComputeFFTSize(GetWindowSize(),GetZeroPadding()));
    GetFFT().SetAudioSize(GetprFFTSize());
}

int MyAnalyzerConfig::GetZeroPadding() const
{
    return GetprZeroPadding();
}

void MyAnalyzerConfig::SetSamplingRate(TData sr)
{
    SetprSamplingRate(sr);
}

TData MyAnalyzerConfig::GetSamplingRate() const
{
    return GetprSamplingRate();
}

int MyAnalyzerConfig::ComputeFFTSize(int wSize,int zpFact)
{
    double tmp = log(double(wSize))/log(2.0); // compute log2
    double aux = tmp-int(tmp);
    aux = (aux >= 0.5) ? 1 : 0; // round
    int pot = int(tmp)+int(aux)+zpFact; // exponent

    return int(pow(2.0,pot));  // compute and return the result
}

//////////////////// MyAnalyzer ////////////////////

MyAnalyzer::MyAnalyzer()
{
    AttachChildren();
    Configure(MyAnalyzerConfig()); 
}

MyAnalyzer::MyAnalyzer(const MyAnalyzerConfig& cfg)
{
    AttachChildren();
    Configure(cfg);
}

bool MyAnalyzer::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
{
    mConfig=dynamic_cast<const MyAnalyzerConfig&> (c);
    ConfigureChildren();
    ConfigureData();
    return true;
}  

const ProcessingConfig& MyAnalyzer::GetConfig() const
{
    return mConfig;
}

bool MyAnalyzer::Do(const Audio& in,Spectrum& out)
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

bool MyAnalyzer::Do(Frame& in)
{
    ConfigureFrame(in);
    return Do(in.GetAudioFrame(),in.GetSpectrum());
}

void MyAnalyzer::AttachChildren()
{
    mPO_FFT.SetParent(this);
    mPO_WinGen.SetParent(this);
    mPO_AProduct.SetParent(this);
    mPO_CShift.SetParent(this);
}

bool MyAnalyzer::ConfigureChildren()
{
    mPO_FFT.Configure(mConfig.GetFFT());
    mPO_WinGen.Configure(mConfig.GetWindowGenerator());
    mPO_CShift.Configure(mConfig.GetCircularShift());
    return true;
}

void MyAnalyzer::ConfigureData()
{
    /* sample rate to audio multiplier */
    mAudioFrame.SetSampleRate(mConfig.GetSamplingRate());

    mWindow.SetSize(mConfig.GetWindowSize());

    /* generate window */
    mPO_WinGen.Do(mWindow);

    mWindow.SetSize(mConfig.GetFFT().GetAudioSize());  // add zero padding
}

void MyAnalyzer::ConfigureFrame(Frame& in)
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

