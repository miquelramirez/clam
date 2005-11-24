/*
 * MyAnalyzer.hxx
 * Interface for MyAnalizer class
 * Used to perform the spectral analysis over the audio signal.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _MYANALYZER_
#define _MYANALYZER_

#include <CLAM/ProcessingComposite.hxx>
#include <CLAM/FFT.hxx>
#include <CLAM/Frame.hxx>
#include <CLAM/WindowGeneratorConfig.hxx>
#include <CLAM/WindowGenerator.hxx>
#include <CLAM/AudioMultiplier.hxx>
#include <CLAM/CircularShiftConfig.hxx>
#include <CLAM/CircularShift.hxx>

using CLAM::ProcessingComposite;
using CLAM::ProcessingConfig;
using CLAM::FFT;
using CLAM::FFTConfig;
using CLAM::Audio;
using CLAM::Frame;
using CLAM::Spectrum;
using CLAM::WindowGenerator;
using CLAM::WindowGeneratorConfig;
using CLAM::AudioMultiplier;
using CLAM::CircularShiftConfig;
using CLAM::CircularShift;
using CLAM::TData;
using CLAM::TSize;
using CLAM::EWindowType;

class MyAnalyzerConfig: public ProcessingConfig
{
    DYNAMIC_TYPE_USING_INTERFACE(MyAnalyzerConfig,7,ProcessingConfig);

    DYN_ATTRIBUTE(0,public,std::string,Name);
    DYN_ATTRIBUTE(1,public,FFTConfig,FFT);
    DYN_ATTRIBUTE(2,public,WindowGeneratorConfig,WindowGenerator);
    DYN_ATTRIBUTE(3,public,CircularShiftConfig,CircularShift);
    DYN_ATTRIBUTE(4,protected,int, prFFTSize);
    DYN_ATTRIBUTE(5,protected,int,prZeroPadding);
    DYN_ATTRIBUTE(6,protected,TData,prSamplingRate);
    
public:
    void SetWindowSize(TSize w);
    TSize GetWindowSize() const;
    void SetWindowType(const EWindowType& t);
    const EWindowType& GetWindowType() const;
    void SetZeroPadding(int z);
    int GetZeroPadding() const;
    void SetSamplingRate(TData sr);
    TData GetSamplingRate() const;
    
private:
    void DefaultInit();
    void DefaultValues();
    int ComputeFFTSize(int wSize,int zpFact);
};

class MyAnalyzer : public ProcessingComposite
{
  
public:
    MyAnalyzer();
    MyAnalyzer(const MyAnalyzerConfig& cfg);
  
    const ProcessingConfig& GetConfig() const;

    bool Do(){return false;}

    bool Do(const Audio& in,Spectrum& out);
    bool Do(Frame& in);
  
private:
    MyAnalyzerConfig mConfig;

    FFT mPO_FFT;
    WindowGenerator mPO_WinGen;
    CircularShift mPO_CShift;
    AudioMultiplier mPO_AProduct;

    
    Audio mWindow;
    Audio mAudioFrame;

    bool ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast);

    void AttachChildren();
    bool ConfigureChildren();
    void ConfigureData();
    void ConfigureFrame(Frame& in);
};

#endif





