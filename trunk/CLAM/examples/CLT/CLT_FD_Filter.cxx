
#include "InControl.hxx"
#include "ErrProcessingObj.hxx"
#include <stdlib.h>
#include "CLT_FD_Filter.hxx"
#include "iostream"
#include"XMLStorage.hxx"

unsigned long CLT_FD_Filter::sBufferAudioSize = 2048;

void CLT_FD_Filter::AdoptChildren()
{
		mFFT.SetParent(this);
		mIFFT.SetParent(this);
		mFilterR.SetParent(this);
		mFilterL.SetParent(this);
}

CLT_FD_Filter::CLT_FD_Filter()
{
		Configure(CLT_PluginConfig());
		AdoptChildren();
}

bool CLT_FD_Filter::ConfigureChildren()
{
		static const char *filterR_name = "Filter Generator Right";
		static const char *filterL_name = "Filter Generator Left";
		static const char *fft_name = "Input FFT";
		static const char *ifft_name = "Output IFFT";
		static const char *filter_name = "Filter product";

		FDFilterGenConfig cfg_fd;
		cfg_fd.SetType(EFDFilterType(EFDFilterType::eLowPass));
		cfg_fd.SetSpectralRange(22050);
		cfg_fd.SetLowCutOff(1000);
		cfg_fd.SetStopBandSlope(3);
		
		cfg_fd.SetName(filterR_name);
		mFilterR.Configure(cfg_fd);

		cfg_fd.SetName(filterL_name);
		mFilterL.Configure(cfg_fd);

		CLAM::SpecTypeFlags fflags; // flags for all spectral data
		fflags.bMagPhase = true;
		fflags.bComplex = true;

		SpectrumConfig spconf;
		spconf.SetType(fflags);
		mSpecR.Configure(spconf);
		mSpecL.Configure(spconf);

		SpecProductConfig cfg_prod;
		IFFTConfig cfg_ifft;
		FFTConfig cfg_fft;

		cfg_fft.SetAudioSize(2048);
		cfg_fft.SetName(fft_name);
		
		cfg_prod.SetName(filter_name);

		cfg_ifft.SetAudioSize(2048);
		cfg_ifft.SetName(ifft_name);

		CLAM::SpecTypeFlags sflags; // flags for all spectral data
		sflags.bMagPhase = false;
		sflags.bComplex = false;
		sflags.bMagPhaseBPF = true;

		SpectrumConfig     specconf; // Configuration for all spectrums
		specconf.SetType( sflags );
		specconf.SetScale( CLAM::EScale::eLog );
		mSpecFilterR.Configure( specconf );
		mSpecFilterL.Configure( specconf );

		sflags.bMagPhaseBPF = false;
		sflags.bMagPhase = true;
		specconf.SetType( sflags );
		specconf.SetScale( CLAM::EScale::eLinear );
		mSpecRout.Configure( specconf );
		mSpecRout.Configure( specconf );

		mFFT.Configure(cfg_fft);
		mProduct.Configure(cfg_prod);
		mIFFT.Configure(cfg_ifft);

		return true;
}

bool CLT_FD_Filter::ConfigureData(int size)
{
		mSpecR.SetSize(size+1);
		mSpecL.SetSize(size+1);
		mSpecRout.SetSize(size+1);
		mSpecLout.SetSize(size+1);
		mSpecFilterR.SetSize(size+1);
		mSpecFilterL.SetSize(size+1);

		return true;
}

bool CLT_FD_Filter::ConcreteConfigure(const ProcessingConfig &c)
{
		CopyAsConcreteConfig(mConfig, c);
		ConfigureChildren();
		ConfigureData(1024);
		return true;
}

void CLT_FD_Filter::ConnectPort(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
{
		CLAM_ASSERT(sizeof(TData) == sizeof(float), "TData must be float.");

		switch (Port)
		{
		case R_CUTOFF:
				((CLT_FD_Filter *)Instance)->mCutoffR = (TData*)DataLocation;
				break;
		case L_CUTOFF:
				((CLT_FD_Filter *)Instance)->mCutoffL = (TData*)DataLocation;
				break;
		case R_INPUT:
				((CLT_FD_Filter *)Instance)->sIBufferR.GetBuffer().SetPtr((TData*)DataLocation,2048);
				break;
		case L_INPUT:
				((CLT_FD_Filter *)Instance)->sIBufferL.GetBuffer().SetPtr((TData*)DataLocation,2048);
				break;
		case R_OUTPUT:
				((CLT_FD_Filter *)Instance)->sOBufferR.GetBuffer().SetPtr((TData*)DataLocation,2048);
				break;
		case L_OUTPUT:
				((CLT_FD_Filter *)Instance)->sOBufferL.GetBuffer().SetPtr((TData*)DataLocation,2048);
				break;
		}
}

LADSPA_Handle CLT_FD_Filter::Instantiate
( const struct _LADSPA_Descriptor* Descriptor, 
  unsigned long SampleRate )
{
		LADSPA_Handle ret = new CLT_FD_Filter;
		return ret;
}

void CLT_FD_Filter::ChangeFFT(unsigned long SampleCount)
{
		if (GetExecState() == Running)
		{
				Stop();
		}

		FFTConfig cfg_fft;
		cfg_fft.SetAudioSize((TData)SampleCount);
		mFFT.Configure(cfg_fft);
		IFFTConfig cfg_ifft;
		cfg_ifft.SetAudioSize((TData)SampleCount);
		mIFFT.Configure(cfg_ifft);
	
		Start();
}

void CLT_FD_Filter::RunSelf(unsigned long SampleCount)
{

		 sIBufferR.GetBuffer().SetSize(SampleCount);
		 sIBufferL.GetBuffer().SetSize(SampleCount);
		 sOBufferR.GetBuffer().SetSize(SampleCount);
		 sOBufferL.GetBuffer().SetSize(SampleCount);

		 if (sBufferAudioSize != SampleCount)
		 {
				 sBufferAudioSize = SampleCount;
				 ChangeFFT(SampleCount);
		 }
		 Convert(sIBufferR);
		 Convert(sIBufferL);
						
		 Do(sIBufferL, sIBufferR, sOBufferL,sOBufferR);

		 Deconvert(sOBufferR);
		 Deconvert(sOBufferL);
}


bool CLT_FD_Filter::Do(Audio &inL, Audio &inR, Audio &outL,Audio &outR)
{

		InControl &coffR = mFilterR.LowCutOff;
		InControl &coffL = mFilterL.LowCutOff;

		mFFT.Do(inR,mSpecR);
		mFFT.Do(inL,mSpecL);

		coffR.DoControl(*mCutoffR);
		coffL.DoControl(*mCutoffL);


		mFilterR.Do(mSpecFilterR);
		mFilterL.Do(mSpecFilterL);


		mProduct.Do(mSpecR,mSpecFilterR,mSpecRout);
		mProduct.Do(mSpecL,mSpecFilterL,mSpecLout);

		mIFFT.Do(mSpecRout,outR);
		mIFFT.Do(mSpecLout,outL);

		return true;

}

void initialise_CLT_FD_filter()
{

		
		FDF_Descriptor = new CLT_Descriptor
				(1103,
				 "CLT_fdf",
				 "CLAM Frequency Domain Filter",
				 "Xavi Rubio (MTG)",
				 "None",
				 6);

		FDF_Descriptor->SetProperties(0,0,1);


		CLT_Port * cutoffR = 
				new CLT_Port(1,0,0,1,"Cutoff Frequency Right(Hz)");
		cutoffR->CreateHint(1,1,0,1,1,0);
		cutoffR->SetLowerBound(0);

		CLT_Port * cutoffL = 
				new CLT_Port(1,0,0,1,"Cutoff Frequency Left(Hz)");
		cutoffL->CreateHint(1,1,0,1,1,0);
		cutoffL->SetLowerBound(0);

		CLT_Port * inputR = new CLT_Port(1,0,1,0,"Input Right");
		CLT_Port * inputL = new CLT_Port(1,0,1,0,"Input Left");
		CLT_Port * outputR = new CLT_Port(0,1,1,0,"Output Right");
		CLT_Port * outputL = new CLT_Port(0,1,1,0,"Output Left");


		FDF_Descriptor->ConnectPort(cutoffR);
		FDF_Descriptor->ConnectPort(cutoffL);
		FDF_Descriptor->ConnectPort(inputR);
		FDF_Descriptor->ConnectPort(inputL);
		FDF_Descriptor->ConnectPort(outputR);
		FDF_Descriptor->ConnectPort(outputL);

		CONNECT( FDF_Descriptor , CLT_FD_Filter );

		registerNewPluginDescriptor(FDF_Descriptor);
}

void finalise_CLT_FD_filter()
{
		delete FDF_Descriptor;
}

