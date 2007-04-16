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

#ifndef __SpectralAnalysis__
#define __SpectralAnalysis__

//Uncomment this line if you want to use Ooura fft instead of fftw
//#define CLAM_OOURA

#include "WindowGenerator.hxx"
#include "CircularShift.hxx"
#include "SpectralAnalysisConfig.hxx"
#include "Audio.hxx"
#include "AudioMultiplier.hxx"
#include "ProcessingComposite.hxx"

#include "FFT.hxx"

#include "Spectrum.hxx"
#include "AudioInPort.hxx"
#include "OutPort.hxx"

namespace CLAM {

class Frame;
class Segment;
class ProcessingConfig;

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
	bool ConcreteConfigure(const ProcessingConfig&);

	/** Ports */
	AudioInPort     mInput;
	OutPort<Spectrum> mOutput;

};

} //namespace

#endif

