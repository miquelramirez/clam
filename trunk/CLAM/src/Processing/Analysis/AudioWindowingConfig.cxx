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

#include "AudioWindowingConfig.hxx"
#include "CLAM_Math.hxx"

#include <iostream>

namespace CLAM
{

void AudioWindowingConfig::DefaultInit()
{
  
  AddAll();
  UpdateData();
  DefaultValues();
}

void AudioWindowingConfig::DefaultValues()
{

	SetprSamplingRate(44100);
/** FFTSize will be next power of two of the window size*/
	SetprZeroPadding(0);

	SetprHopSize(0);//for preventing reading uninitialized memory

	/** Default window size */
	SetWindowSize(1025);
	SetWindowType(EWindowType::eBlackmanHarris92);
	
	/** WindowSize/2*/
	SetHopSize((GetWindowSize()-1)/2);

	GetCircularShift().SetAmount(-(GetWindowSize()-1)/2);

}


void AudioWindowingConfig::SetWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	SetprWindowSize(w);
	GetWindowGenerator().SetSize(w);
	SetprFFTSize(nextPowerOfTwo( int( (w-1)*CLAM_pow(TData(2),TData(GetZeroPadding())) ) ) );
	GetCircularShift().SetAmount(-((w-1)/TData(2))); 
	if(w<2*GetHopSize()+1)
		SetHopSize((w-1)/2);

}

TSize AudioWindowingConfig::GetWindowSize() const
{
	return GetWindowGenerator().GetSize();
}

/** Analysis Window type*/
void AudioWindowingConfig::SetWindowType(const EWindowType& t)
{
        SetprWindowType(t);
        GetWindowGenerator().SetType(t);
}

const EWindowType& AudioWindowingConfig::GetWindowType() const
{
	return GetWindowGenerator().GetType();
}

/** Zero padding factor*/
void AudioWindowingConfig::SetZeroPadding(int z)
{
	SetprZeroPadding(z);
	SetprFFTSize(nextPowerOfTwo( int( (GetWindowSize()-1) * CLAM_pow(TData(2),TData(GetZeroPadding())) ) ) );
}

int AudioWindowingConfig::GetZeroPadding() const
{
	return GetprZeroPadding();
}



void AudioWindowingConfig::SetHopSize(TSize h)
{
	SetprHopSize(h);
}

TSize AudioWindowingConfig::GetHopSize() const
{
	return GetprHopSize();
}

/** Sampling rate of the input audio*/
void AudioWindowingConfig::SetSamplingRate(TData sr)
{
	SetprSamplingRate(int(sr));
	/** for sychronizing existing config*/
	SetWindowSize(GetWindowSize());
}

TData AudioWindowingConfig::GetSamplingRate() const
{
	return GetprSamplingRate();
}


void  AudioWindowingConfig::Sync()
{
  
	int zp = GetprZeroPadding();
	int ws = GetprWindowSize();
	SetSamplingRate(GetprSamplingRate());

	SetZeroPadding(zp);

	SetWindowSize(ws);
	SetWindowType(GetprWindowType());  

	SetHopSize(GetprHopSize());
}

} // namespace CLAM

