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
#include "Port.hxx"
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
	
	DYNAMIC_TYPE_USING_INTERFACE(SMSSynthesisConfig,5,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
/** Configuration for children Processing Objects*/
	DYN_ATTRIBUTE(1,public, SynthSineSpectrumConfig,SynthSineSpectrum);
	DYN_ATTRIBUTE(2,public, PhaseManagementConfig,PhaseMan);
	DYN_ATTRIBUTE(3,public, SpectralSynthesisConfig,SpectralSynth);

	DYN_ATTRIBUTE(4,protected,int,prSamplingRate);


//Config shortcuts

public:
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


		// Internal convenience methods.

		void AttachChildren();
		bool ConfigureChildren();
		void ConfigureData();

		
		/** Configuration method */
		bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

		TInt32 CalculatePowerOfTwo(TInt32 size);

	public:
		SMSSynthesis(const SMSSynthesisConfig& cfg);
		SMSSynthesis();
		~SMSSynthesis();
		
		// Processing Object compliance methods.

		const char *GetClassName() {return "SMSSynthesis";}


		const ProcessingConfig &GetConfig() const {return mConfig;}

		/** Supervised mode execution */
		bool Do(void);

		/** Sinusoidal synthesis, gives also the output spectrum */
		bool Do(SpectralPeakArray& in,Spectrum& outSpec,Audio& outAudio);
		/** Sinusoidal synthesis */
		bool Do(SpectralPeakArray& in,Audio& out);
		
		bool Do(Frame& in);

		bool Do(Segment& in);


	};

}; //end of namespace

#endif //_SMSSynthesis_
