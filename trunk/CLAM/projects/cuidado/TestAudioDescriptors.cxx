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

using namespace MTG;


int main()
{

  cout << "TestDescriptors" << endl;

  /* An Audio Descriptor */

  AudioDescriptorConfig audioDescConfig;

  audioDescConfig.GetType().mean=true;
  audioDescConfig.GetType().variance=true;
  audioDescConfig.GetType().temporalCentroid=true;
  audioDescConfig.GetType().attack=true;
  audioDescConfig.GetType().decay=true;

  audioDescConfig.GetType().sustain=true;
  audioDescConfig.GetType().release=true;
  audioDescConfig.GetType().logAttackTime=true;
  audioDescConfig.GetType().energy=true;

  audioDescConfig.GetType().zeroCrossingRate=true;
  audioDescConfig.GetType().riseTime=true;

  AudioDescriptor audioDesc(audioDescConfig);
  
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

  AudioDescriptorGenConfig  agenconf;
  AudioDescriptorGen        agen(agenconf);
  
  agen.Do(audioData,audioDesc);



#ifdef _MTG_USEXML_

  XMLStorage storage ("XML-Document");

  audioDesc.StoreOn(storage);

  std::fstream fileout("test.xml", ios::out);

  storage.dumpOn(fileout);

#endif//_MTG_USEXML_



}
