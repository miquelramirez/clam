#include<iostream>
#include<fstream>
#include<string>


#include "Audio.hxx"
#include "AudioDescriptor.hxx"
#include "AudioDescriptorGen.hxx"
#include "HarmonicDescriptor.hxx"
#include "PeakDetect.hxx"
#include "WindowGenerator.hxx"

#include "SpectralPeakArray.hxx"
#include "FFT_rfftw.hxx"


#ifdef _MTG_USEXML_
#	include "XMLStorage.hxx"
#	include "XMLStaticAdapter.hxx"
#	include "XMLComponentAdapter.hxx"
#	include "XMLable.hxx"
#endif//_MTG_USEXML_


namespace MTG {

	class PeakDetectionSystemConfig: public ProcessingObjectConfig
	{
	public:
	  REG_NUM_ATTR_USING_INTERFACE (PeakDetectionSystemConfig, 2,ProcessingObjectConfig);
	  REGISTER (0, Name, std::string);
	  REGISTER (1, FFTSize, TSize);
	  
	  void DefaultInit();

	};


	class PeakDetectionSystem: public ProcessingObject {
	  PeakDetectionSystemConfig mConfig;
	  
	  const char *GetClassName() {return "PeakDetectionSystem";}

	  bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);
	  
	public:
	  PeakDetectionSystem();
	  
	  PeakDetectionSystem(const PeakDetectionSystemConfig &c);

	  PeakDetectionSystem::~PeakDetectionSystem(); 	  

	  const PeakDetectionSystemConfig &GetConfig() const { return mConfig;}
	  
	  bool Do(void) { 
	    throw(ErrProcessingObj("PeakDetectionSystem::Do(): Supervised mode not implemented"),this);
	    return false;
	  }
	  
	  void Start(void) throw(ErrProcessingObj);

	  bool Do(Audio& input, SpectralPeakArray& out);
	  
	  bool MayDisableExecution() const {return true;}
	  
	  void StoreOn(Storage &s) {};
	  
	private:
	  WindowGeneratorConfig*  wingenconf;
	  WindowGenerator*        wingen;

	  SpectrumConfig* sconfig;
	  Spectrum*       spectrumData;

	  PeakDetectConfig*  spconfig;
	  PeakDetect* peakproc;

	  FFTConfig* fconfig;
	  FFT_rfftw* fft;	

	};
  

  
  void PeakDetectionSystemConfig::Init()
  {
    /* the dynamic type takes care if we add an existing attr .. */
	  
    AddName();
    AddFFTSize();

    /* All Attributes are added */
    UpdateData();
    SetFFTSize(1024);
  }

  
  PeakDetectionSystem::PeakDetectionSystem() { Configure(PeakDetectionSystemConfig());}
  PeakDetectionSystem::PeakDetectionSystem(const PeakDetectionSystemConfig& c) { Configure(c);}

  PeakDetectionSystem::~PeakDetectionSystem() 
  {
    delete wingenconf;
    delete wingen;
    
    delete sconfig;
    delete spectrumData;

    delete spconfig;
    delete peakproc;
    
    delete fconfig;
    delete fft;
  }

  bool PeakDetectionSystem::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
  {
    mConfig = dynamic_cast<const PeakDetectionSystemConfig&>(c);
    int Size = mConfig.GetFFTSize();

    // Window

    wingenconf = new WindowGeneratorConfig();
    wingen = new WindowGenerator(*wingenconf);

    // Spectral Array
  
    SpecTypeFlags sflags;
    sflags.bMagPhase=false;
    sflags.bMagPhaseBPF=true;
    
    sconfig = new SpectrumConfig();
    sconfig->SetType(sflags);
    sconfig->SetSize(Size/2+1);
  
    spectrumData = new Spectrum(*sconfig);
    spectrumData->GetMagBPF().SetIntpType(EInterpolation::ePolynomial2);

    // The FFT

    fconfig = new FFTConfig;
    fconfig->SetName("FFT");
    fconfig->SetAudioSize(Size);
    fft = new FFT_rfftw(*fconfig);

    // PeakDetection
  
    spconfig = new PeakDetectConfig;
    spconfig->SetName("Peak");
    spconfig->SetNumBands(Size/2+1);
    peakproc = new PeakDetect(*spconfig);

    return true;
  }

  void PeakDetectionSystem::Start() throw(ErrProcessingObj)
  {
    fft->Start();
    peakproc->Start();

  }

  bool PeakDetectionSystem::Do(Audio& input, SpectralPeakArray& out) 
  {    
    fft->Do(input,*spectrumData);
    peakproc->Do(*spectrumData,out);
    return true;
  }

  
}



using namespace MTG;


int main()
{

  cout << "TestHarmDescriptors" << endl;

  /* An Harmonics Descriptor */  

  HarmonicDescriptorConfig harmDescConfig;
  harmDescConfig.GetType().transientMidPoint = true;
  harmDescConfig.GetType().harmonicRiseTimes = true;
  harmDescConfig.GetType().overshoots = true;
  harmDescConfig.GetType().lowHarmonicsDelay = true;
  harmDescConfig.GetType().highHarmonicsDelay = true;
  HarmonicDescriptor harmDesc(harmDescConfig);
  

  /* Do some calculation */

  int i,Size = 22050;
  TData SampleRate = 44100.;
  AudioConfig aconfig;
  aconfig.SetSize(Size);
  
  Audio audioData(aconfig);
  for (i=0;i<Size;i++) {
    audioData.GetFloatBuffer()[i]=
      0.3*sin(2.0*PI*200.0*(((float)i)/SampleRate)) + 
      0.2*sin(2.0*PI*400.0*(((float)i)/SampleRate)) +  
      0.1*sin(2.0*PI*600.0*(((float)i)/SampleRate)) +  
      0.1*sin(2.0*PI*800.0*(((float)i)/SampleRate)) +  
      0.1*sin((2.0*PI*1000.0*((float)i)/SampleRate));
    if (i < 1024) audioData.GetFloatBuffer()[i]*= ((float)i/(float)1024);
    else audioData.GetFloatBuffer()[i]*= ((float)(Size - i)/(float)Size);
    
  }

  /* Harmonic */

  Audio audioFrame;
  audioFrame.SetSize(1024);

  audioData.GetAudioChunk(0,1024,audioFrame);

  SpectralPeakArrayConfig   pconfig;
  SpectralPeakArray   peaks(pconfig);
  
    
  PeakDetectionSystemConfig pdtconf;
  pdtconf.SetName("PDT");
  PeakDetectionSystem pdt(pdtconf);
  
    
  pdt.Start();
  pdt.Do(audioFrame,peaks);

#ifdef _MTG_USEXML_

  XMLStorage storage ("XML-Document");

  harmDesc.StoreOn(storage);

  std::fstream fileout("test.xml", ios::out);

  storage.dumpOn(fileout);

#endif//_MTG_USEXML_



}
