#ifndef _CLT_FD_FILTER
#define _CLT_FD_FILTER

#include "CLT_StereoPlugin.hxx"
#include "SpectrumProduct.hxx"
#include "FDFilterGen.hxx"
#include "Spectrum.hxx"
#include "FFT_rfftw.hxx"
#include "IFFT_rfftw.hxx"

#define R_CUTOFF 0
#define L_CUTOFF 1
#define R_INPUT 2
#define L_INPUT 3
#define R_OUTPUT 4
#define L_OUTPUT 5

using namespace CLAM;

class CLT_FD_Filter : public CLT_StereoPlugin
{
public:
		TData* mCutoffR;
		TData* mCutoffL;

private:
		static unsigned long sBufferAudioSize;
		Spectrum                 mSpecFilterR;
		Spectrum                 mSpecFilterL
;		Spectrum                       mSpecR;
		Spectrum                       mSpecL;
		Spectrum                       mSpecRout;
		Spectrum                       mSpecLout;
		SpectrumProduct              mProduct; //PO to multiply signals
		FDFilterGen                  mFilterR;
		FDFilterGen                  mFilterL;
		FFT_rfftw                 mFFT;
		IFFT_rfftw                      mIFFT;

		void ChangeFFT(unsigned long SampleCount);

public:
		CLT_FD_Filter();

		virtual ~CLT_FD_Filter(){}

		inline bool Do(Audio &inL, Audio &inR, Audio &outL, Audio &outR);
		bool ConfigureChildren();
		bool ConfigureData(int size);
		void AdoptChildren();
		bool ConcreteConfigure(const ProcessingConfig &c);

		static LADSPA_Handle Instantiate
		( const struct _LADSPA_Descriptor* Descriptor, 
		unsigned long SampleRate );
		static void ConnectPort (LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation);
		static void Run(LADSPA_Handle Instance, unsigned long SampleCount)
		{
				CLT_FD_Filter* filter;
				filter = (CLT_FD_Filter*) Instance;
				filter->RunSelf(SampleCount);
		}
		void RunSelf(unsigned long SampleCount);
};

static CLT_Descriptor * FDF_Descriptor=NULL;
void initialise_CLT_FD_filter();
void finalise_CLT_FD_filter();

#endif

