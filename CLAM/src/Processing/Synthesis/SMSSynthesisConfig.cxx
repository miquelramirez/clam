
#include "SMSSynthesisConfig.hxx"

namespace CLAM
{

void SMSSynthesisConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	DefaultValues();	
}

void SMSSynthesisConfig::DefaultValues()
{
	SetSamplingRate(44100);

	/** Default analysis window size corresponds to 512*/
//	GetSpectralSynth().SetAnalWindowSize(513);
//	TODO PA remove (hack for anal-synth in streaming)
//	GetSpectralSynth().SetAnalWindowSize(1025);

	
	GetSpectralSynth().SetAnalWindowType(EWindowType::eBlackmanHarris92);
	GetSpectralSynth().GetAnalWindowGenerator().SetInvert(true);

	
	/** WindowSize/2*/
//	SetHopSize((GetAnalWindowSize()i-1)/2);
//	TODO PA remove hack...
	SetAnalWindowSize(1025);	
	SetHopSize(256);
	SetFrameSize(256);

	/* Default frame size is 256*/
//	SetFrameSize(GetHopSize());


}

void SMSSynthesisConfig::SetSpectrumSize(TSize specSize)
{
	GetSynthSineSpectrum().SetSpectrumSize(specSize);
}

TSize SMSSynthesisConfig::GetSpectrumSize() const
{
	return GetSynthSineSpectrum().GetSpectrumSize();
}

/** Synthesis Window size in miliseconds. In num. of samples WindowSize/SR is forced to be odd*/	
void SMSSynthesisConfig::SetAnalWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	GetSpectralSynth().SetAnalWindowSize(w);
	
	GetSynthSineSpectrum().SetSpectrumSize(GetSpectralSynth().GetIFFT().GetAudioSize()/2+1);
/*	TODO:This condition should be checked!!
	if(w<2*GetHopSize()+1)
		SetHopSize((w-1)/2);*/
}

TSize SMSSynthesisConfig::GetAnalWindowSize() const
{
	return GetSpectralSynth().GetAnalWindowSize();
}

/** Analysis Window type*/
void SMSSynthesisConfig::SetAnalWindowType(const EWindowType& t)
{
	GetSpectralSynth().SetAnalWindowType(t);
}

const EWindowType& SMSSynthesisConfig::GetAnalWindowType() const
{
	return GetSpectralSynth().GetAnalWindowType();
}


void SMSSynthesisConfig::SetSynthWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	GetSpectralSynth().SetSynthWindowSize(w);
}

TSize SMSSynthesisConfig::GetSynthWindowSize() const
{
	return GetSpectralSynth().GetSynthWindowSize();
}

/** Synthesis Hop size in miliseconds. Must be < (WindowSize-(1/SR))/2*/	
void SMSSynthesisConfig::SetHopSize(TSize h)
{

	//CLAM_ASSERT(GetSynthWindowSize()>=2*h, "SMSSynthesisConfig::SetHopSize: Hop Size is too large compared to window size");
	GetSpectralSynth().SetHopSize(h);
	GetOverlapAddSin().SetHopSize(h);
	GetOverlapAddRes().SetHopSize(h);
	GetOverlapAddGlobal().SetHopSize(h);
}

void SMSSynthesisConfig::SetFrameSize(TSize f)
{
	GetOverlapAddSin().SetFrameSize(f);
	GetOverlapAddRes().SetFrameSize(f);
	GetOverlapAddGlobal().SetFrameSize(f);
}

TSize SMSSynthesisConfig::GetFrameSize()
{
	return GetOverlapAddSin().GetFrameSize();
}


TSize SMSSynthesisConfig::GetHopSize() const
{
	return GetSpectralSynth().GetHopSize();
}

/** Sampling rate of the input audio*/
void SMSSynthesisConfig::SetSamplingRate(TData sr)
{
	GetSynthSineSpectrum().SetSamplingRate(sr);
	GetPhaseMan().SetSamplingRate(sr);
	GetSpectralSynth().SetSamplingRate(sr);
}

TData SMSSynthesisConfig::GetSamplingRate() const
{
	return GetSynthSineSpectrum().GetSamplingRate();
}


TInt32 SMSSynthesisConfig::PowerOfTwo(TInt32 size)
{
	int tmp = size;
	int outputSize = 1;
	while (tmp) 
	{
	 	outputSize=outputSize << 1;
	 	tmp=tmp >> 1;
	}
	if(outputSize == size << 1)
		outputSize = outputSize >> 1;
	return outputSize;
}




} // namespace CLAM
