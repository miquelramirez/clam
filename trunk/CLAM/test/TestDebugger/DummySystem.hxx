/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __DUMMYSYSTEM__
#define __DUMMYSYSTEM__


#include "AudioFileIn.hxx"
#include "AudioFileOut.hxx"
#include "Controller.hxx"  //processing object that is controlled by the GUI (or Midi in the future)
#include "FFT_rfftw.hxx"
#include "IFFT_rfftw.hxx"
#include "SpectrumAdder.hxx"
#include "SpectrumProduct.hxx"
#include "FDFilterGen.hxx"
#include "TokenDelay.hxx"

namespace CLAM
{

class SpectralDelaySystem
{

public:
	enum {AudioFrameSize = 1024, SpectralRange = 20000};


	CLAM::AudioFileIn          mAudioFileIn;
	CLAM::AudioFileOut         mAudioFileOut;
	CLAM::Controller           mDelayContr;
	CLAM::FFT_rfftw            mFFT;
	CLAM::IFFT_rfftw           mIFFT;
	CLAM::SpectrumAdder        mAdder;
	CLAM::SpectrumProduct      mProduct;
	CLAM::FDFilterGen          mFilter;
	CLAM::TokenDelay<Spectrum> mDelayLow, mDelayMid, mDelayHigh;

	SpectralDelaySystem() {};
	~SpectralDelaySystem() {}; //TODO: remove all dynamic objects.

	/** Initialization of all Processing Objects and GUI */
	void InitProcessings();

	void StoreOn (CLAM::Storage & store)
	{

	}

};

}

#endif // DummySystem.hxx
