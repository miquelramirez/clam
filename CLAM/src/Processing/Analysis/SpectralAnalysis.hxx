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

#ifndef _SpectralAnalysis_
#define _SpectralAnalysis_


#include "WindowGenerator.hxx"
#include "CircularShift.hxx"
#include "FFT_rfftw.hxx"
#include "ProcessingComposite.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "AudioCircularBuffer.hxx"
#include "AudioMultiplier.hxx"
#include "Flags.hxx"

namespace CLAM{



/** Configuration class for Spectral Analysis class
 *  It includes specific parameters: HopSize, 
 *  ZeroPadding, Sampling Rate and FFTSize. Further 
 *  setter/getters are also offered for WindowSize and WindowType.
 *  It also includes complete configurations for child processings: 
 *  WindowGenerator, Circular Shift and FFT. These configurations
 *  should only be modified by advanced users
 */
class SpectralAnalysisConfig:public ProcessingConfig
{

	friend class SpectralAnalysis;

	DYNAMIC_TYPE_USING_INTERFACE (SpectralAnalysisConfig,8,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
/** Configuration for children Processing Objects*/
	DYN_ATTRIBUTE(1,public,WindowGeneratorConfig, WindowGenerator);
	DYN_ATTRIBUTE(2,public,CircularShiftConfig,CircularShift);
	DYN_ATTRIBUTE(3,public,FFTConfig, FFT);
	DYN_ATTRIBUTE(4,protected,int, prHopSize);
	DYN_ATTRIBUTE(5,protected,int, prZeroPadding);
	DYN_ATTRIBUTE(6,protected,int, prSamplingRate);
	DYN_ATTRIBUTE(7,protected,int, prFFTSize);

public:
	~SpectralAnalysisConfig(){};

//Config shortcuts

/** Sets Analysis Window size in num. of samples. 
 *  This setter actually changes window size in the window generator,
 *  size of the FFT, and amount of circular shift. If new window size
 *  is smaller than twice the hop size plus one, hop size is also 
 *  modified to that value.
 *  Note1: for best results, window size should be set to odd number.
 *  Note2: this setter should be called after the hop size has already
 *  been set. 
 */	
	void SetWindowSize(TSize w);

/** Getter for window size. Actually returns size of the window generator. */
	TSize GetWindowSize() const;

/** Sets Analysis Window type, modifying configuration of child Window Generator
 *  configuration.
 *  @see EWindowType
 */
	void SetWindowType(const EWindowType& t);

/** Getter for window type, returns window type in Window Generator */
	const EWindowType& GetWindowType() const;

/** Sets Zero padding factor. It changes FFTSize and Circular shift accordingly.
 *  If factor is 0, FFTSize is set to WindowSize-1 next power of two (assumes window
 *  size is odd!)
 *  Assuming a WindowSize of 513, see following table to understand how the factor
 *  works:
 *  ZeroPaddingFactor		FFTSize
 *		 -1					  256
 *        0					  512
 *		  1					 1024
 *		  2					 2048
 *  @see PowerOfTwo
 */
	void SetZeroPadding(int z);

/** returns ZeroPaddingFactor being used.*/
	int GetZeroPadding() const;

/** Sets Analysis Hop size in number of samples. This setter only changes attribute but
 *  does not change configurations accordingly! VIP: It assumes that a SetWindowSize is
 *  called afterwards.
 */	
	void SetHopSize(TSize h);

/** returns HopSize in number of samples.*/
	TSize GetHopSize() const;

/** Sets Sampling rate of the input audio. It calls SetWindowSize for synchronizing existing
	configurations.*/
	void SetSamplingRate(TData sr);

/** returns SamplingRate */
	TData GetSamplingRate() const;

private:

/** Default Init adds all Dynamic Attributes */	
	void DefaultInit();

/** Sets default values: SamplingRate=44100, ZeroPaddingFactor=0, WindowSize=513,
 *  WindowType=Hamming, HopSize=256, and CircularShift's amount= 256*/
	void DefaultValues();

/** Ad-hoc routine for finding FFT Size*/
	TInt32 PowerOfTwo(TInt32 size);
};


/** Processing class for performing a STFT. Its main functionality is to obtain
 *  a spectrum out of an incoming audio signal, applying windowing, circular shift
 *  and zero padding. Its different 
 */
class SpectralAnalysis:public ProcessingComposite
{

public:
	
// Processing Object compliance methods.

	const char *GetClassName() const {return "SpectralAnalysis";}


	SpectralAnalysis(SpectralAnalysisConfig& config);
	SpectralAnalysis();
	~SpectralAnalysis();
	
			
	const ProcessingConfig &GetConfig() const {return mConfig;}

/** Supervised mode execution */
	bool Do(void);

/** Basic unsupervised Do method. It returns an output spectrum using an input audio frame*/
	bool Do(const Audio& in,Spectrum& outSp);

/** Unsupervised Do method that uses a CLAM Frame as input and output. AudioFrame attribute of
 *  the frame is used as input and Spectrum as output.
 *  @see Frame */
	bool Do(Frame& in);

/** Unsupervised Do method that uses an incoming CLAM Segment as input and
 *  output. It processes the frame that is indicated by the current frame index
 *  in the segment and increments that index.
 *  @see Segment*/
	bool Do(Segment& in);


	virtual void Attach(Audio& in, Spectrum &out);


private:

/**	Internal Configuration data */
	SpectralAnalysisConfig mConfig;

// The internal Processing Objects

/** Child Window Generator Processing 
 *  @see WindowGenerator 
 */
	WindowGenerator mPO_WinGen;

/** Child Audio Multiplier Processing for doing the windowing 
 *  @see AudioMultiplier
 */
	AudioMultiplier mPO_AProduct;

/** Child Circular Shift Processing 
 *  @see CircularShift
 */
	CircularShift   mPO_CShift;

/** Child FFT Processing 
 *  @see FFT
 */
	FFT		mPO_FFT;

/** Internal Circular Buffer Processing for overlap 
 *  @see AudioCircularBuffer  */
//	AudioCircularBuffer mCircularBuffer;

//Internal DataObjects

/** Audio internal object that will be processed through the FFT*/	
	Audio mAudioFrame;
/** Window internal object that will be multiplied with incoming audio frame*/
	Audio mWindow; 
/** spectrum object only used for initializing frames */	
	Spectrum mSpec;

	// Internal convenience methods.

	void AttachChildren();
	bool ConfigureChildren();
	void ConfigureData();

	
	/** Configuration method */
	bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

	/** Ports */
	InPortTmpl<Audio>     mInput;
	OutPortTmpl<Spectrum> mOutput;

};

};//namespace

#endif
