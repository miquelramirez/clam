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

#ifndef _SynthSineSpectrum_
#define _SynthSineSpectrum_
#include <typeinfo>

#define   MAINLOBE_TABLE_SIZE 4096		// Tablesize of the mainlobe of a BH92 Window 

#include "Err.hxx"
#include "WindowGenerator.hxx"
#include "AudioMultiplier.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "Complex.hxx"
#include "Processing.hxx"
#include "SynthSineSpectrumConfig.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"

namespace CLAM{

/**
 * Configuration class for the SynthSineSpectrum Processing.
 */
	/* SynthSineSpectrumConfig moved to SynthSineSpectrumConfig.hxx */

	
							
/**
 * Processing which can synthesize a spectrum out of an 
 * array of spectral peaks.
 * It does this by convolving the spectral peaks with the 
 * significant part of the fourier transformed Blackman-Harris92 
 * window, it's main lobe. WARNING: the do method changes the type of
 * the output spectrum to complex regardless its original format. This
 * behaviour may result in inconsistencies and should be taken into account.
 */
class SynthSineSpectrum:public Processing
{

	InPort<SpectralPeakArray> mInput;
	OutPort<Spectrum> mOutput;
	
public:
	/* constructor/destructor */
	SynthSineSpectrum();
	SynthSineSpectrum(SynthSineSpectrumConfig& cfg);
	~SynthSineSpectrum();
	const char * GetClassName() const {return "SynthSineSpectrum";}

	/* public member functions */
	bool Do(const SpectralPeakArray& peakArray,Spectrum& residualSpectrumOut, double gain=1.0);
	bool Do(void);

	/** Configuration method */
	bool ConcreteConfigure(const ProcessingConfig&);

	const ProcessingConfig &GetConfig() const {return mConfig;}

private:
	/* private member functions */
	void InitSynthSpec(TSize size);

	void FillSynthSineSpectrum(const SpectralPeakArray& peakArray,double gain = 1.0);

	/* private configuration*/
	SynthSineSpectrumConfig mConfig;

	/* window table generation */
	WindowGenerator		mWndGen;
	//Spectrum			mBlackHarris92TransMainLobe;
	static double mBlackHarris92TransMainLobe[];

	/* internal buffer for synthesized sinusoidal spectrum */
	Array< Complex > mSynthSineSpectrum;
};
};// CLAM namespace
#endif

