#include<iostream>
#include<fstream>
#include<string>

#include "AudioProduct.hxx"
#include "CircularShift.hxx"
#include "WindowGenerator.hxx"


using namespace MTG;


void DumpAudio(Audio& a)
{
  int i;
  int size = a.GetSize();

  for (i=0;i<size;i++)
    cout << a.GetFloatBuffer()[i]<< endl;

}



int main(int argc,char** argv)
{

  WindowGeneratorConfig wconf;
  wconf.SetSize(1024);
  wconf.SetNormalize(EWindowNormalize::eNone);
  WindowGenerator wingen(wconf);
  

  /* Audio Data Objects */

  int i,Size = 1024;
  AudioConfig aconfig;
  aconfig.SetSize(Size);
  
  Audio audioData(aconfig);

  aconfig.SetSize(1024);
  Audio audiowin(aconfig);

  /* Fill them with live */

  for (i=0;i<Size;i++) {
#if 0
    audioData.GetFloatBuffer()[i]=1.0;
#endif
#if 0
    audioData.GetFloatBuffer()[i]=
      0.3*sin(2.0*PI*200.0*(((float)i)/SampleRate)) + 
      0.2*sin(2.0*PI*400.0*(((float)i)/SampleRate)) +  
      0.1*sin(2.0*PI*600.0*(((float)i)/SampleRate)) +  
      0.1*sin(2.0*PI*800.0*(((float)i)/SampleRate)) +  
      0.1*sin((2.0*PI*1000.0*((float)i)/SampleRate));
    if (i < 1024) audioData.GetFloatBuffer()[i]*= ((float)i/(float)1024);
    else audioData.GetFloatBuffer()[i]*= ((float)(Size - i)/(float)Size);
#endif
  }

  AudioProductConfig apconfig;
  AudioProduct aprod(apconfig);

  CircularShiftConfig sconfig;
  sconfig.SetAmount(-256);
  CircularShift       shift(sconfig);

  wingen.Start();
  aprod.Start();
  shift.Start();

  /* Fill the audio with a window */
  wingen.SetSize(4);
  wingen.Do(audiowin);
  aprod.Do(audioData,audiowin,audioData);
  shift.Do(audioData,audioData);

  DumpAudio(audiowin);

  return 0;
}
