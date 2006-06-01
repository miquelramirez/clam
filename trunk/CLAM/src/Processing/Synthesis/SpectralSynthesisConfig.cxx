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
	SetAnalWindowSize(1025);
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

void SpectralSynthesisConfig::Sync()
{
  int aws = GetprAnalWindowSize();
  int hs = GetprHopSize();
  
  SetSamplingRate(GetprSamplingRate());

  SetAnalWindowSize(aws);
  SetAnalWindowType(GetprAnalWindowType());

  SetHopSize(hs);

  GetSynthWindowGenerator().SetSize(GetHopSize()*2+1);
}


void SpectralSynthesisConfig::SetAnalWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	SetprAnalWindowSize(w);
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
	SetprAnalWindowType(t);
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
	SetprHopSize(h);
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

