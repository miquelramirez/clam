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

#ifndef _SMSAnalysis_
#define _SMSAnalysis_

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
#include "CircularStreamImpl.hxx"
#include "StreamBuffer.hxx"

namespace CLAM {

class Frame;
class Segment;
class Fundamental;
class ProcessingConfig;
class WriteStreamRegion;
class ReadStreamRegion;
class SpectralPeakArray;

/** Class that encapsulates all the building blocks of the SegmentAnalysis example */
class SMSAnalysis:public ProcessingComposite
{

public:
	
// Processing Object compliance methods.

	const char *GetClassName() const {return "SMSAnalysis";}


	SMSAnalysis(SMSAnalysisConfig& config);
	SMSAnalysis();
	~SMSAnalysis();
	
			
	const ProcessingConfig &GetConfig() const {return mConfig;}

	/** Method used to attach a Processing Data to input and output ports */
	void Attach(Audio& in, Spectrum& outSpectrum,SpectralPeakArray& outPk,
		Fundamental& outFn,Spectrum& outResSpec,Spectrum& outSinSpec);
	
	/** Supervised mode execution */
	bool Do(void);

	void Start();
	
	/** Processes a single frame in a Segment, calling the Do(Frame&) overload. It adds the
	 *	frame to the segment if the analysis has produced some data and increments the internal
	 *	frame counter in the Segment. Note that the audio to be analyzed is taken from the
	 *	segment's Audio attribute.
	 *	@return: false if end of input audio has been reached and the analysis process has thus
	 *	finished. 
	 */
	bool Do(Segment& in);
	
	/** Processes a Frame and writes result of the analysis into the Frame's attributes.
	 *	This method adds the necessary attributes to the Frame and maybe allocating memory.
	 *	@return: false if no data has been produced in the Analysis. This condition is usually
	 *	produced when not enough samples have been written onto the stream buffer.
	 */
	bool Do(Frame& in);
	
	/** This overload is actually where all processing is accomplished. It computes residual and
	 *	sinusoidal components and writes them into the arguments of the call.
	 *	@return: false if the member stream buffer has not enough written samples so as to do
	 *	the processing.
	 *	@param in: input audio frame (size should be hop size as these are the new samples that
	 *	are actually written into stream buffer.
	 *	@param outSpectrum: output of the FFT, it is taken as the one coming from the residual
	 *	branch.
	 *	@param outPk: output peakArray detected from the sinusoidal component
	 *	@param outFn: output Fundamental Frequency computed from the sinusoidal peaks
	 *	@param outResSpec: residual spectrum computed from substracting the synthesized sinusoidal
	 *	spectrum from the original spectrum.
	 */
	bool Do(Audio& in, Spectrum& outSpectrum,Spectrum& sinGlobalSpec,SpectralPeakArray& outPk,Fundamental& outFn,
		Spectrum& outResSpec);

	/**	Auxiliary method to perform the analysis on the sinusoidal component. It computes the
	 *	spectral peaks, the fundamental frequency and then performs sinusoidal tracking on the
	 *	spectral peaks. Note: to do this operations, the method converts the input spectrum to
	 *	dB's and leaves it so. If you need linear magnitude afterwards, you will need to convert
	 *	it back.
	 *	@param inSpectrum: input sinusoidal spectrum to compute the peaks and fundamental.
	 *	@param outpkArray: output spectral peak array computed from the spectrum and after the
	 *	sinusoidal tracking has been applied.
	 *	@param outFn: output fundamental frequency.
	 *	@return: false if no fundamental frequency has been found and this is supposed to be an 
	 *	unvoiced segment. */
	bool SinusoidalAnalysis(Spectrum& inSpectrum, SpectralPeakArray& outPkArray,Fundamental& outFn);

private:

/** Configuration data */
	SMSAnalysisConfig mConfig;

// The internal Processing Objects

/** Child processing object: spetral analysis for the sinusoidal component.
 *	@see SpectralAnalysis */
	SpectralAnalysis mPO_SinSpectralAnalysis;
/** Child processing object: spetral analysis for the residual component
 *	@see SpectralAnalysis */
	SpectralAnalysis mPO_ResSpectralAnalysis;
/** Child processing object: spetral peak detection for the sinusoidal component
 *	@see SpectralPeakDetect */
	SpectralPeakDetect		mPO_PeakDetect;
/** Child processing object: fundamental detection for the sinusoidal component
 *	@see FundFreqDetect */
	FundFreqDetect  mPO_FundDetect;
/** Child processing object: sinusoidal peak tracking for the sinusoidal component
 *	@see SinTracking */
	SinTracking		mPO_SinTracking;
/** Child processing object: sinusoidal synthesis to compute a sinsoidal spectrum from
	the extracted peaks. 
 *	@see SynthSineSpectrum*/
	SynthSineSpectrum		mPO_SynthSineSpectrum;
/** Child processing object: spectrum substracter to compute the residual spectrum from
	the original and the sinusoidal one.
 *	@see SpectrumSubstracter2 */
	SpectrumSubstracter2		mPO_SpecSubstract;

	//Internal DataObjects


/** member stream buffer*/
	AudioStreamBuffer<CircularStreamImpl<TData> > *mStreamBuffer;
/** member writer into stream buffer */
	WriteStreamRegion* mWriter;
/** member sinusoidal reader from stream buffer */
	ReadStreamRegion* mSinReader;
/** member residual reader from stream buffer */
	ReadStreamRegion* mResReader;

/** Internal audio objects used for convenience */
	Audio mSinAudioFrame;
	Audio mResAudioFrame;

/** internal spectrum objects used for convinience */	
	Spectrum mResSpec;
	Spectrum mSinSpec;
	
/** Index that indicates how many audio frames have been processed until now*/
	TSize mAudioFrameIndex;
	

// Internal convenience methods.

/**	This method notifies child processing objets that "this" is their parent*/
	void AttachChildren();
/** Configures child processing using configuration data in mConfig. */
	bool ConfigureChildren();
/** Configures internal data objects and stream buffer. */
	void ConfigureData();

	
	/** Configuration method */
	bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

	/** Internal convenience method used for initializing frames */
	void InitFrame(Frame& in);

	/** Ports */
	InPortTmpl<Audio>				mInputAudio;
	OutPortTmpl<Spectrum>			mOutputSpectrum;
	OutPortTmpl<SpectralPeakArray>  mOutputSpectralPeaks;
	OutPortTmpl<Fundamental>			mOutputFundamental;
	OutPortTmpl<Spectrum>			mOutputResSpectrum;
	OutPortTmpl<Spectrum>			mOutputSinSpectrum;
};

};//namespace

#endif
