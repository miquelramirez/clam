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

#ifndef _SpectralSynthesis_
#define _SpectralSynthesis_

#include "ProcessingComposite.hxx"

#include "Array.hxx"
#include "Audio.hxx"
#include "DataTypes.hxx"
#include "DynamicType.hxx"
#include "Err.hxx"
#include "ErrProcessingObj.hxx"
#include "IFFT.hxx"
#include "IFFT_rfftw.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "InControl.hxx"
#include "Spectrum.hxx"
#include "Port.hxx"
#include "ProcessingData.hxx"
#include "ProcessingDataConfig.hxx"
#include "Processing.hxx"
#include "AudioMultiplier.hxx"
#include "WindowGenerator.hxx"
#include "OverlapAdd.hxx"
#include "AudioCircularBuffer.hxx"
#include "CircularShift.hxx"

#include "SnapshotGenerator.hxx"

#include "SpectralAnalysis.hxx"

#include <stdlib.h>

namespace CLAM {


/**
* Flags that are used to decide which Snapshots will be shown
*/
class SpectralSynthesisSnapshotsFlags : public Flags<7> {
public:
	static tFlagValue sFlagValues[];
	virtual CLAM::Component * Species() const {
		return new SpectralSynthesisSnapshotsFlags();
	}
	typedef enum {
		eInputFrameSpectrum=0,
		eIFFTOutput=1,
		eAudioFrameNoCircularShift=2,
		eAudioFrameNoZeroPadding=3,
		eWindowedAudioFrame=4,
		eOutputAudioFrame=5,
		eSynthesisWindow=6
	} tFlag;
	SpectralSynthesisSnapshotsFlags () :
		CLAM::Flags<7>(sFlagValues),
		bInputFrameSpectrum(operator[](eInputFrameSpectrum)),
		bIFFTOutput(operator[](eIFFTOutput)),
		bAudioFrameNoCircularShift(operator[](eAudioFrameNoCircularShift)),
		bAudioFrameNoZeroPadding(operator[](eAudioFrameNoZeroPadding)),
		bWindowedAudioFrame(operator[](eWindowedAudioFrame)),
		bOutputAudioFrame(operator[](eOutputAudioFrame)),
		bSynthesisWindow(operator[](eSynthesisWindow))
	{};
	template <class T> SpectralSynthesisSnapshotsFlags(const T &t) : 
		CLAM::Flags<7>(sFlagValues,t),
		bInputFrameSpectrum(operator[](eInputFrameSpectrum)),
		bIFFTOutput(operator[](eIFFTOutput)),
		bAudioFrameNoCircularShift(operator[](eAudioFrameNoCircularShift)),
		bAudioFrameNoZeroPadding(operator[](eAudioFrameNoZeroPadding)),
		bWindowedAudioFrame(operator[](eWindowedAudioFrame)),
		bOutputAudioFrame(operator[](eOutputAudioFrame)),
		bSynthesisWindow(operator[](eSynthesisWindow))
	{};
	template <class T1, class T2> SpectralSynthesisSnapshotsFlags(const T1 &t1, const T2 &t2) : 
		CLAM::Flags<7>(sFlagValues,t1,t2),
		bInputFrameSpectrum(operator[](eInputFrameSpectrum)),
		bIFFTOutput(operator[](eIFFTOutput)),
		bAudioFrameNoCircularShift(operator[](eAudioFrameNoCircularShift)),
		bAudioFrameNoZeroPadding(operator[](eAudioFrameNoZeroPadding)),
		bWindowedAudioFrame(operator[](eWindowedAudioFrame)),
		bOutputAudioFrame(operator[](eOutputAudioFrame)),
		bSynthesisWindow(operator[](eSynthesisWindow))
	{};

	reference bInputFrameSpectrum;
	reference bIFFTOutput;
	reference bAudioFrameNoCircularShift;
	reference bAudioFrameNoZeroPadding;
	reference bWindowedAudioFrame;
	reference bOutputAudioFrame;
	reference bSynthesisWindow;
	

};



class SpectralSynthesisConfig:public ProcessingConfig
{

friend class SpectralSynthesis;

public:
	
	DYNAMIC_TYPE_USING_INTERFACE(SpectralSynthesisConfig,9,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
/** Configuration for children Processing Objects*/
	DYN_ATTRIBUTE(1,public,WindowGeneratorConfig,AnalWindowGenerator);
	DYN_ATTRIBUTE(2,public,WindowGeneratorConfig,SynthWindowGenerator);
	DYN_ATTRIBUTE(3,public,CircularShiftConfig,CircularShift);
	DYN_ATTRIBUTE(4,public,IFFTConfig,IFFT);
	DYN_ATTRIBUTE(5,public,OverlapAddConfig,OverlapAdd);
	DYN_ATTRIBUTE(6,protected,int, prZeroPadding);
	DYN_ATTRIBUTE(7,protected,int,prSamplingRate);

/** Flags that are used in order to decide what debug display is shown TODO?*/
	DYN_ATTRIBUTE(8,public, SpectralSynthesisSnapshotsFlags,DisplayFlags);

//Config shortcuts

public:
	~SpectralSynthesisConfig(){};

/** Analysis Window size in miliseconds. In num. of samples WindowSize/SR is forced to be odd*/	
	void SetAnalWindowSize(TSize w);
	TSize GetAnalWindowSize() const;
/** Analysis Window type*/
	void SetAnalWindowType(const EWindowType& t);
	const EWindowType& GetAnalWindowType() const;
/** Analysis Zero padding factor*/
	void SetZeroPadding(int z);
	int GetZeroPadding() const;
/** Synthesis window size*/
	void SetSynthWindowSize(TSize w);
	TSize GetSynthWindowSize()const;

/** Synthesis Hop size in miliseconds. Must be < (WindowSize-(1/SR))/2*/	
	void SetHopSize(TSize h);
	TSize GetHopSize() const;
/** Sampling rate of the output audio*/
	void SetSamplingRate(TData sr);
	TData GetSamplingRate() const;
/** Spectrum Size **/
	void SetSpectrumSize(TSize specSize);
	TSize GetSpectrumSize() const;
/** Frame Size **/
	void SetFrameSize(TSize f);
	TSize GetFrameSize();

private:

	void DefaultInit();
	void DefaultValues();

	/** Ad-hoc routine for finding FFT Size*/
	TInt32 PowerOfTwo(TInt32 size);
};

	/** This is the processing object class we are using to group several
	 *  other processing objects. 
	 */
	class SpectralSynthesis : public ProcessingComposite {

private:
		// Configuration data
		SpectralSynthesisConfig mConfig;


		//internal Processing Data
		Audio                     mAudio1, mAudio2,mAudio3;
		//Spectrum                  Spec;
		Audio                    mSynthWindow;


		//internal Processing Objects
		WindowGenerator			mPO_AnalWindowGen;
		WindowGenerator			mPO_SynthWindowGen;
		IFFT_rfftw              mPO_IFFT;
		AudioMultiplier            mPO_AudioProduct;
		CircularShift			mPO_CircularShift;
		OverlapAdd				mPO_OverlapAdd;
		

		// And the interfaces with the outside world.

		
		// Internal convenience methods.

		void AttachChildren();
		bool ConfigureChildren();
		void ConfigureData();

		
		/** Configuration method */
		bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

		TInt32 CalculatePowerOfTwo(TInt32 size);

	public:
		SpectralSynthesis(const SpectralSynthesisConfig& cfg);
		SpectralSynthesis();
		~SpectralSynthesis();
		
		// Processing Object compliance methods.

		const char *GetClassName() {return "SpectralSynthesis";}


		const ProcessingConfig &GetConfig() const {return mConfig;}

		/** Supervised mode execution */
		bool Do(void);

		/** Unsupervised mode execution */
		bool Do(Spectrum& in, Audio &out);
		
		bool Do(Frame& in, bool residual=false);
		bool Do(Segment& in, bool residual=false);


	};

}; //end of namespace

#endif //_SpectralSynthesis_
