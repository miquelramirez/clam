#include "CLAM_Math.hxx"
#include "SpectralSynthesisConfig.hxx"

namespace CLAM
{

void SpectralSynthesisConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	DefaultValues();	
}

void SpectralSynthesisConfig::DefaultValues()
{
/** FFTSize will be next power of two of the window size*/
	SetprZeroPadding(0);
	SetSamplingRate(44100);

	/** Default analysis window size corresponds to 512*/
	SetAnalWindowSize(513);
	SetAnalWindowType(EWindowType::eBlackmanHarris92);
	GetAnalWindowGenerator().SetInvert(true);

	/** WindowSize/2*/
	SetHopSize((GetAnalWindowSize()-1)/2);

	/** Synthesis Window Type is set to triangular*/
	GetSynthWindowGenerator().SetType(EWindowType::eTriangular);
	GetSynthWindowGenerator().SetNormalize(EWindowNormalize::eNone);
	GetSynthWindowGenerator().SetSize(GetHopSize()*2+1);

	SetResidual(false);
	

}

void SpectralSynthesisConfig::SetAnalWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	GetAnalWindowGenerator().SetSize(w);
	TData audioSize=TData(PowerOfTwo((w-1)*int(pow(TData(2.0),TData(GetZeroPadding())))));
	GetIFFT().SetAudioSize(int(audioSize));
	GetCircularShift().SetAmount(TData(w/2)); 
}

TSize SpectralSynthesisConfig::GetAnalWindowSize() const
{
	return GetAnalWindowGenerator().GetSize();
}

void SpectralSynthesisConfig::SetAnalWindowType(const EWindowType& t)
{
	GetAnalWindowGenerator().SetType(t);
}

const EWindowType& SpectralSynthesisConfig::GetAnalWindowType() const
{
	return GetAnalWindowGenerator().GetType();
}


void SpectralSynthesisConfig::SetSynthWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	GetSynthWindowGenerator().SetSize(w);
}

TSize SpectralSynthesisConfig::GetSynthWindowSize() const
{
	return GetSynthWindowGenerator().GetSize();
}

void SpectralSynthesisConfig::SetZeroPadding(int z)
{
	SetprZeroPadding(z);
	TData audioSize=TData(PowerOfTwo((GetAnalWindowSize()-1)*int(pow(TData(2),TData(GetZeroPadding())))));
	GetIFFT().SetAudioSize(int(audioSize));
}

int SpectralSynthesisConfig::GetZeroPadding() const
{
	return GetprZeroPadding();
}

void SpectralSynthesisConfig::SetHopSize(TSize h)
{
	GetSynthWindowGenerator().SetSize(2*h+1);
	//GetOverlapAdd().SetHopSize(h);
	//GetOverlapAdd().SetBufferSize(GetFrameSize()+h);
}

TSize SpectralSynthesisConfig::GetHopSize() const
{
	return (GetSynthWindowGenerator().GetSize()-1)>>1;
}

void SpectralSynthesisConfig::SetSamplingRate(TData sr)
{
	SetprSamplingRate(int(sr));

	/** for sychronizing existing config*/
	SetAnalWindowSize(GetAnalWindowSize());
}

TData SpectralSynthesisConfig::GetSamplingRate() const
{
	return TData(GetprSamplingRate());
}


TInt32 SpectralSynthesisConfig::PowerOfTwo(TInt32 size)
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

