#include<iostream>
#include<fstream>
#include<string>

//#define WITH_GUI

#include "AudioProduct.hxx"
#include "CircularShift.hxx"
#include "FFT.hxx"
#include "SpectrumJTransform.hxx"
#include "SpectrumMovingAverage.hxx"
#include "SpectrumSubtract.hxx"
#include "SpectrumClip.hxx"
#include "SpectrumProduct.hxx"
#include "PeakLikelihood.hxx"
#include "LikelihoodComposite.hxx"
#ifdef WITH_GUI
#include "FLTKBuilder.hxx" 
#endif


using namespace MTG;

  /*
   * gnuplot format (GPF) output routine
   * within gnuplot use > plot "name1", "name2"
   */
#ifdef WITH_GUI
GUIObject* gui;
#endif

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

    int i,Size = 2048;
    TData SampleRate = 44100.;
    
    
    /*
     *                      Processing Data
     */ 
    
    
    SpecTypeFlags sflags;
    sflags.bMagPhase=true;
    sflags.bMagPhaseBPF=false;
    sflags.bComplex=false;
    
    SpectrumConfig sconfig;
    sconfig.SetType(sflags);
    sconfig.SetSize(Size/2+1);
    
    Spectrum*  spec[20];
    for (i=0;i<20;i++) {
      spec[i] = new Spectrum(sconfig);
    }
    
    Spectrum  input(sconfig);
    
    
    for (i=0;i<Size/2+1;i++) {
#define RAND  (float)rand()/6553600000.
      input.GetMagBuffer()[i]= RAND;
    }
    
    input.GetMagBuffer()[30]= 0.1;
    input.GetMagBuffer()[60]= 0.02;
    input.GetMagBuffer()[90]= 0.05;
    input.GetMagBuffer()[120]= 0.01;
    
    
    /*    
     *                      Processing Objects
     */ 

    TData vals[] = {0,20,10,25,22,30,37,40,57,60,87,100,-1};

    LikelihoodCompositeConfig likeliconf;
    likeliconf.SetSize(Size);
    likeliconf.AddBandDefinitions();
    likeliconf.UpdateData();
    likeliconf.SetBandDefinitions(vals);
    LikelihoodComposite   likeli(likeliconf);

    /* 
     *     Start the system 
     */
    
    
    likeli.Start();
    
    /* 
     *      Run the system 
     */
    
    
    likeli.Do(input,spec);


    for (i=0;i<20;i++) {
      char name[40];
      sprintf(name,"filter%d",i+1);
      DumpArray(spec[i]->GetMagBuffer(),name);
    }


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
