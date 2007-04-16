/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "SMSAnalysisConfig.hxx"

namespace CLAM
{

void SMSAnalysisConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	DefaultValues();	
}

void SMSAnalysisConfig::DefaultValues()
{

	SetprSamplingRate(44100);

	GetSinSpectralAnalysis().SetWindowType(EWindowType::eHamming);
	GetResSpectralAnalysis().SetWindowType(EWindowType::eBlackmanHarris92);

	GetPeakDetect().SetMagThreshold(-60);

	//TODO PA  remove these values (aiming to analy-synt streaming) ?
	SetSinWindowSize(2049);
	SetHopSize(512);
	SetSinZeroPadding(0);
	SetResWindowSize(1025);
	
}


void SMSAnalysisConfig::SetSinWindowSize(TSize w)
{
	GetSinSpectralAnalysis().SetWindowSize(w);
}

TSize SMSAnalysisConfig::GetSinWindowSize() const
{
	return GetSinSpectralAnalysis().GetWindowSize();
}

/** Analysis Window type*/
void SMSAnalysisConfig::SetSinWindowType(const EWindowType& t)
{
	GetSinSpectralAnalysis().SetWindowType(t);
}

const EWindowType& SMSAnalysisConfig::GetSinWindowType() const
{
	return GetSinSpectralAnalysis().GetWindowType();
}

/** Zero padding factor*/
void SMSAnalysisConfig::SetSinZeroPadding(int z)
{
	GetSinSpectralAnalysis().SetZeroPadding(z);
}

int SMSAnalysisConfig::GetSinZeroPadding() const
{
	return GetSinSpectralAnalysis().GetZeroPadding();
}

void SMSAnalysisConfig::SetHopSize(TSize h)
{
	GetSinSpectralAnalysis().SetHopSize(h);
	GetResSpectralAnalysis().SetHopSize(h);
}

TSize SMSAnalysisConfig::GetHopSize() const
{
	return GetSinSpectralAnalysis().GetHopSize();
}

/*****Configuration for residual component analysis*****/

void SMSAnalysisConfig::SetResWindowSize(TSize w)
{
	GetResSpectralAnalysis().SetWindowSize(w);
	GetSynthSineSpectrum().SetSpectrumSize(GetResSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
}

TSize SMSAnalysisConfig::GetResWindowSize() const
{
	return GetResSpectralAnalysis().GetWindowSize();
}

/** Analysis Window type*/
void SMSAnalysisConfig::SetResWindowType(const EWindowType& t)
{
	GetResSpectralAnalysis().SetWindowType(t);
}

const EWindowType& SMSAnalysisConfig::GetResWindowType() const
{
	return GetResSpectralAnalysis().GetWindowType();
}

/** Zero padding factor*/
void SMSAnalysisConfig::SetResZeroPadding(int z)
{
	GetResSpectralAnalysis().SetZeroPadding(z);
}

int SMSAnalysisConfig::GetResZeroPadding() const
{
	return GetResSpectralAnalysis().GetZeroPadding();
}

/** Sampling rate of the input audio*/
void SMSAnalysisConfig::SetSamplingRate(TData sr)
{
	SetprSamplingRate( int(sr) );
	GetSinSpectralAnalysis().SetSamplingRate(sr);
	GetResSpectralAnalysis().SetSamplingRate(sr);
	GetSynthSineSpectrum().SetSamplingRate(sr);
}

TData SMSAnalysisConfig::GetSamplingRate() const
{
	return TData(GetprSamplingRate());
}


TInt32 SMSAnalysisConfig::PowerOfTwo(TInt32 size)
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

TSize SMSAnalysisConfig::GetInitialOffset() const
{
	TSize largerWindowSize;
	if(GetSinWindowSize()>GetResWindowSize()) largerWindowSize=GetSinWindowSize();
	else largerWindowSize=GetResWindowSize();
			
	return -(largerWindowSize-1)/2+GetHopSize();
}

TSize SMSAnalysisConfig::GetHopsInBiggerWindow() const
{
	if(GetSinWindowSize()>GetResWindowSize()) return (GetSinWindowSize()-1)/GetHopSize();
	else return (GetResWindowSize()-1)/GetHopSize();
}

	
} // namespace CLAM

