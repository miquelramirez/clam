#include<iostream>
#include<fstream>
#include<string>

#define WITH_GUI

#include "AudioProduct.hxx"
#include "CircularShift.hxx"
#include "FFT.hxx"
#include "SpectrumJTransform.hxx"
#include "SpectrumMovingAverage.hxx"
#include "SpectrumSubtract.hxx"
#include "SpectrumClip.hxx"
#ifdef WITH_GUI
#include "FLTKBuilder.hxx" 
#endif

using namespace MTG;

  /*
   * gnuplot format (GPF) output routine
   * within gnuplot use > plot "name1", "name2"
   */

GUIObject* gui;

void DumpArray(DataArray& a,char* name)
{
#ifdef WITH_GUI
  gui->ShowSnapshot(a,name);
#else
  int i;
  int size = a.Size();

  ofstream f(name);

  for (i=0;i<size;i++)
    f << a[i]<< endl;
#endif
}


int main(int argc,char** argv)
{

#ifdef WITH_GUI
  gui = Builder::GetInstance("FLTK")->GetGUI();
#endif
  try {
  WindowGeneratorConfig wconf;
  wconf.SetSize(512);
  WindowGenerator wingen(wconf);
  

  /*
   *                      Processing Data
   */ 

  int i,Size = 2048;
  TData SampleRate = 44100.;
  AudioConfig aconfig;
  aconfig.SetSize(Size);
  
  Audio audioData(aconfig);

  aconfig.SetSize(Size);
  Audio audiowin(aconfig);


  SpecTypeFlags sflags;
  sflags.bMagPhase=true;
  sflags.bMagPhaseBPF=false;
  sflags.bComplex=false;


  SpectrumConfig sconfig;
  sconfig.SetType(sflags);
  sconfig.SetSize(Size/2+1);

  Spectrum  spec(sconfig);
  Spectrum  avgspec(sconfig);

  /* Excurs:   Fill the Audio Data with live with live */

  for (i=0;i<Size;i++) {
#if 0
    audioData.GetFloatBuffer()[i]=1.0;
#endif
#if 1
    #define RAND  (float)rand()/6553600000.
    audioData.GetFloatBuffer()[i]=
      RAND + 0.3*sin(2.0*PI*2000.0*(((float)i)/SampleRate)) + 
      0.2*sin(2.0*PI*4000.0*(((float)i)/SampleRate)) +  
      0.1*sin(2.0*PI*6000.0*(((float)i)/SampleRate)) +  
      0.1*sin(2.0*PI*8000.0*(((float)i)/SampleRate)) +  
      0.1*sin((2.0*PI*10000.0*((float)i)/SampleRate));
    if (i < 1024) audioData.GetFloatBuffer()[i]*= ((float)i/(float)1024);
    else audioData.GetFloatBuffer()[i]*= ((float)(Size - i)/(float)Size);
#endif
  }


  /*
   *                      Processing Objects
   */ 


  AudioProductConfig apconfig;
  AudioProduct aprod(apconfig);

  CircularShiftConfig shiftconfig;
  shiftconfig.SetAmount(-256);
  CircularShift       shift(shiftconfig);


  FFTConfig   fftconf;
  fftconf.SetAudioSize(Size);
  FFT        fft(fftconf);

  SpectrumJTransformConfig spjconf;
  SpectrumJTransform spjtrans(spjconf);

  SpectrumMovingAverageConfig smaconf;
  SpectrumMovingAverage sma(smaconf);

  SpectrumClipConfig clipcfg;
  clipcfg.AddLower();
  clipcfg.UpdateData();
  clipcfg.SetLower(0.0);
  SpectrumClip  clip(clipcfg);

  /* 
   *     Start the system 
   */


  wingen.Start();
  aprod.Start();
  shift.Start();
  fft.Start();
  spjtrans.Start();
  sma.Start();
  clip.Start();

  /* 
   *      Run the system 
   */

  wingen.Do(audiowin);
  aprod.Do(audioData,audiowin,audioData);
  shift.Do(audioData,audioData);
  fft.Do(audioData,spec);
  DumpArray(spec.GetMagBuffer(),"pure");


  sma.Do(spec,avgspec);
  DumpArray(avgspec.GetMagBuffer(),"avg");

  spjtrans.Do(spec,spec);
  DumpArray(spec.GetMagBuffer(),"jtrans");

  clip.Do(spec,avgspec,spec);
  
  DumpArray(spec.GetMagBuffer(),"equi");


  } 
  catch(Err error)
    {
      error.Print();
      std::cerr << "Abnormal Program Termination" << std::endl;
    }
  catch (std::exception e)
    {
      std::cout << e.what() << std::endl;
    }

  return 0;
}
