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

#ifndef _SMSSynthesis_
#define _SMSSynthesis_

#include "ProcessingComposite.hxx"

#include "Array.hxx"
#include "Audio.hxx"
#include "DataTypes.hxx"
#include "DynamicType.hxx"
#include "Err.hxx"
#include "ErrProcessingObj.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "InControl.hxx"
#include "Spectrum.hxx"
#include "ProcessingData.hxx"
#include "ProcessingDataConfig.hxx"
#include "Processing.hxx"
#include "SynthSineSpectrum.hxx"
#include "PhaseManagement.hxx"
#include "SpectralSynthesis.hxx"
#include "SpectrumAdder2.hxx"
#include "SMSAnalysis.hxx"

#include <stdlib.h>

namespace CLAM {


class SMSSynthesisConfig:public ProcessingConfig
{

friend class SMSSynthesis;

public:
	
	DYNAMIC_TYPE_USING_INTERFACE(SMSSynthesisConfig,8,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
/** Configuration for children Processing Objects*/
	DYN_ATTRIBUTE(1,public, SynthSineSpectrumConfig,SynthSineSpectrum);
	DYN_ATTRIBUTE(2,public, PhaseManagementConfig,PhaseMan);
	DYN_ATTRIBUTE(3,public, SpectralSynthesisConfig,SpectralSynth);

	DYN_ATTRIBUTE(4,public,OverlapAddConfig,OverlapAddSin);
	DYN_ATTRIBUTE(5,public,OverlapAddConfig,OverlapAddRes);
	DYN_ATTRIBUTE(6,public,OverlapAddConfig,OverlapAddGlobal);

	DYN_ATTRIBUTE(7,protected,int,prSamplingRate);


//Config shortcuts

public:
/** Analysis Window size in miliseconds. In num. of samples WindowSize/SR is forced to be odd*/	
	void SetAnalWindowSize(TSize w);
	TSize GetAnalWindowSize() const;
/** Analysis Window type*/
	void SetAnalWindowType(const EWindowType& t);
	const EWindowType& GetAnalWindowType() const;
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

	~SMSSynthesisConfig(){};

private:

	void DefaultInit();
	void DefaultValues();

	/** Ad-hoc routine for finding FFT Size*/
	TInt32 PowerOfTwo(TInt32 size);
};

	/** This is the processing object class we are using to group several
	 *  other processing objects. 
	 */
	class SMSSynthesis : public ProcessingComposite {

private:
		// Configuration data
		SMSSynthesisConfig mConfig;

		//internal Processing Objects
		/** We use a SpectralSynthesis for Sinusoidal and another one
		 * for residual synthesis. In principle we could use the same one
		 * but we may want to synthesize both components separately.
		 */
		SpectralSynthesis		mPO_SinSpectralSynthesis;
		SpectralSynthesis		mPO_ResSpectralSynthesis;
		SpectralSynthesis		mPO_SpectralSynthesis;
		SynthSineSpectrum		mPO_SynthSineSpectrum;
		PhaseManagement			mPO_PhaseMan;
		SpectrumAdder2			mPO_SpectrumAdder;
		
		OverlapAdd				mPO_OverlapAddSin;
		OverlapAdd				mPO_OverlapAddRes;
		OverlapAdd				mPO_OverlapAddGlobal;



		/** internal data members used for convenience */
		Audio mAudioFrame;

		// Internal convenience methods.

		void AttachChildren();
		bool ConfigureChildren();
		void ConfigureData();

		
		/** Configuration method */
		bool ConcreteConfigure(const ProcessingConfig&);

		TInt32 CalculatePowerOfTwo(TInt32 size);

	public:
		SMSSynthesis(const SMSSynthesisConfig& cfg);
		SMSSynthesis();
		~SMSSynthesis();
		
		// Processing Object compliance methods.

		const char *GetClassName() const {return "SMSSynthesis";}


		const ProcessingConfig &GetConfig() const {return mConfig;}

		/** Method used to attach a Processing Data to input and output ports */
		void Attach(SpectralPeakArray& inputSinusoidalPeaks, Spectrum& inputResidualSpectrum,
			Spectrum& outputSinusoidalSpectrum,	Spectrum& outputSpectrum,
			Audio& outputAudio, Audio& outputSinusoidalAudio, Audio& outputResidualAudio);
		
		/** Supervised mode execution */
		bool Do(void);

		/** Sinusoidal synthesis, gives also the output spectrum */
		bool SinusoidalSynthesis(const SpectralPeakArray& in,Spectrum& outSpec,Audio& outAudio);
		/** Sinusoidal synthesis */
		bool SinusoidalSynthesis(const SpectralPeakArray& in,Audio& out);

		/** non-supervised Do method. Produces as output the sinusoidal spectrum, the global spectrum.
		 *	and the sinusoidal, residual and globar audio frames. If this overload is used directly
		 *	you must set the controls for current time and current pitch from the outside.*/
		bool Do(SpectralPeakArray& inputSinusoidalPeaks, Spectrum& inputResidualSpectrum,
			Spectrum& outputSinusoidalSpectrum,	Spectrum& outputSpectrum,
			Audio& outputAudio, Audio& outputSinusoidalAudio, Audio& outputResidualAudio);


		/** non-supervised Do method. SMSSynthesis produces, as side effect, also some output Spectrums. 
		 *	Use this overload if you do not care about these spectrums and just need the output audio. 
		 *	If this overload is used directly you must set the controls for current time and current pitch from the outside.
		 */
		bool Do(SpectralPeakArray& inputSinusoidal, Spectrum& inputSpectrum, 
			Audio& outputAudio, Audio& outputSinusoidal, Audio& outputResidual);
		/** non-supervised Do method: works on a CLAM::Frame */
		bool Do(Frame& in);
		/** non-supervised Do method: works on a CLAM::Segment. Processes current frame in the 
		 *	segment and increments segment internal counter. */
		bool Do(Segment& in);
	protected:

		void InitFrame(Frame& in);

		/** Ports */
		InPortTmpl<SpectralPeakArray>     mInputSinSpectralPeaks;
		InPortTmpl<Spectrum>     mInputResSpectrum;
		OutPortTmpl<Spectrum>     mOutputSinSpectrum;
		OutPortTmpl<Spectrum>     mOutputSpectrum;
		OutPortTmpl<Audio> mOutputAudio;
		OutPortTmpl<Audio> mOutputResAudio;
		OutPortTmpl<Audio> mOutputSinAudio;

	public:
		//Controls
		InControlTmpl<SMSSynthesis> mCurrentTime;
		InControlTmpl<SMSSynthesis> mCurrentPitch;


	};

}; //end of namespace

#endif //_SMSSynthesis_
