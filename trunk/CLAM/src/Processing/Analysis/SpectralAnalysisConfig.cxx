
#include "SpectralAnalysisConfig.hxx"
#include "CLAM_Math.hxx"

#include <iostream>

namespace CLAM
{

void SpectralAnalysisConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	DefaultValues();	
}

void SpectralAnalysisConfig::DefaultValues()
{

	SetprSamplingRate(44100);
/** FFTSize will be next power of two of the window size*/
	SetprZeroPadding(0);

	SetprHopSize(0);//for preventing reading uninitialized memory

	/** Default window size */
	SetWindowSize(513);
	SetWindowType(EWindowType::eHamming);
	
	/** WindowSize/2*/
	SetHopSize((GetWindowSize()-1)/2);

	GetCircularShift().SetAmount(-256);

}


void SpectralAnalysisConfig::SetWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	GetWindowGenerator().SetSize(w);
	SetprFFTSize(nextPowerOfTwo( int( (w-1)*pow(TData(2),TData(GetZeroPadding())) ) ) );
	GetCircularShift().SetAmount(-((w-1)/TData(2))); 
	GetFFT().SetAudioSize(GetprFFTSize());
	if(w<2*GetHopSize()+1)
		SetHopSize((w-1)/2);

}

TSize SpectralAnalysisConfig::GetWindowSize() const
{
	return GetWindowGenerator().GetSize();
}

/** Analysis Window type*/
void SpectralAnalysisConfig::SetWindowType(const EWindowType& t)
{
	GetWindowGenerator().SetType(t);
}

const EWindowType& SpectralAnalysisConfig::GetWindowType() const
{
	return GetWindowGenerator().GetType();
}

/** Zero padding factor*/
void SpectralAnalysisConfig::SetZeroPadding(int z)
{
	SetprZeroPadding(z);
	SetprFFTSize(nextPowerOfTwo( int( (GetWindowSize()-1) * pow(TData(2),TData(GetZeroPadding())) ) ) );
	GetFFT().SetAudioSize(GetprFFTSize());
}

int SpectralAnalysisConfig::GetZeroPadding() const
{
	return GetprZeroPadding();
}



void SpectralAnalysisConfig::SetHopSize(TSize h)
{
	SetprHopSize(h);
}

TSize SpectralAnalysisConfig::GetHopSize() const
{
		return GetprHopSize();
}

/** Sampling rate of the input audio*/
void SpectralAnalysisConfig::SetSamplingRate(TData sr)
{
	SetprSamplingRate(int(sr));
	/** for sychronizing existing config*/
	SetWindowSize(GetWindowSize());
}

TData SpectralAnalysisConfig::GetSamplingRate() const
{
	return GetprSamplingRate();
}




} // namespace CLAM

