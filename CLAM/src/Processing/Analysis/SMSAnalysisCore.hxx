/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __SMSAnalysisCore_hxx__
#define __SMSAnalysisCore_hxx__

#include "DataTypes.hxx"
#include "SpectralPeakDetect.hxx"
#include "FundFreqDetect.hxx"
#include "SinTracking.hxx"
#include "SpectrumSubstracter2.hxx"
#include "SpectralAnalysis.hxx"
#include "SMSAnalysisConfig.hxx"
#include "Audio.hxx"
#include "ProcessingComposite.hxx"
#include "Spectrum.hxx"
#include "SynthSineSpectrum.hxx"

#include "OutPortPublisher.hxx"
#include "AudioInPortPublisher.hxx"

namespace CLAM 
{

class Segment;
class Fundamental;
class ProcessingConfig;
class SpectralPeakArray;

/** Class that encapsulates all the building blocks of the SegmentAnalysis example */
class SMSAnalysisCore : public ProcessingComposite
{

public:
	
// Processing Object compliance methods.

	const char *GetClassName() const {return "SMSAnalysisCore";}

	SMSAnalysisCore(SMSAnalysisConfig& config);
	SMSAnalysisCore();
	~SMSAnalysisCore();
	
			
	const ProcessingConfig &GetConfig() const {return mConfig;}

	/** Supervised mode execution */
	bool Do(void);

	bool ConcreteStart();
private:

/** Configuration data */
	SMSAnalysisConfig mConfig;

// The internal Processing Objects

/** Child processing object: spetral analysis for the sinusoidal component.
 *	@see SpectralAnalysis */
	SpectralAnalysis mSinSpectralAnalysis;
/** Child processing object: spetral analysis for the residual component
 *	@see SpectralAnalysis */
	SpectralAnalysis mResSpectralAnalysis;
/** Child processing object: spetral peak detection for the sinusoidal component
 *	@see SpectralPeakDetect */
	SpectralPeakDetect		mPeakDetect;
/** Child processing object: fundamental detection for the sinusoidal component
 *	@see FundFreqDetect */
	FundFreqDetect  mFundDetect;
/** Child processing object: sinusoidal peak tracking for the sinusoidal component
 *	@see SinTracking */
	SinTracking		mSinTracking;
/** Child processing object: sinusoidal synthesis to compute a sinsoidal spectrum from
	the extracted peaks. 
 *	@see SynthSineSpectrum*/
	SynthSineSpectrum		mSynthSineSpectrum;
/** Child processing object: spectrum substracter to compute the residual spectrum from
	the original and the sinusoidal one.
 *	@see SpectrumSubstracter2 */
	SpectrumSubstracter2		mSpecSubstracter;

// Internal convenience methods.

/**	This method notifies child processing objets that "this" is their parent*/
	void AttachChildren();
/** Configures child processing using configuration data in mConfig. */
	bool ConfigureChildren();
/** Configures internal data objects and stream buffer. */
	void ConfigureData();

	
	/** Configuration method */
	bool ConcreteConfigure(const ProcessingConfig&);

	/** Method to connect and publish children's ports **/
	void ConnectAndPublishPorts();

	/** Ports */
	AudioInPortPublisher			mInputAudio; // audio input
	OutPortPublisher<Spectrum> 		mOutputResSpectrum; // output of resSpectralAnalysis
	OutPortPublisher<Spectrum>		mOutputSinSpectrum; // output of sinSpectralAnalysis
	OutPortPublisher<SpectralPeakArray>  	mOutputSpectralPeaks; // output of sintracking
	OutPortPublisher<Fundamental>		mOutputFundamental; // output of fundamental
	OutPortPublisher<Spectrum>		mOutputSubstractedSpectrum; // output of spectrumSubstracter
};

} //namespace CLAM

#endif // __SMSAnalysisCore_hxx__

