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
#include "ProcessingData.hxx"
#include "ProcessingDataConfig.hxx"
#include "Processing.hxx"
#include "AudioMultiplier.hxx"
#include "WindowGenerator.hxx"
#include "OverlapAdd.hxx"
#include "AudioCircularBuffer.hxx"
#include "CircularShift.hxx"



#include "SpectralAnalysis.hxx"

#include <stdlib.h>

namespace CLAM {




class SpectralSynthesisConfig:public ProcessingConfig
{

friend class SpectralSynthesis;

public:
	
	DYNAMIC_TYPE_USING_INTERFACE(SpectralSynthesisConfig,8,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
/** Configuration for children Processing Objects*/
	DYN_ATTRIBUTE(1,public,WindowGeneratorConfig,AnalWindowGenerator);
	DYN_ATTRIBUTE(2,public,WindowGeneratorConfig,SynthWindowGenerator);
	DYN_ATTRIBUTE(3,public,CircularShiftConfig,CircularShift);
	DYN_ATTRIBUTE(4,public,IFFTConfig,IFFT);
	DYN_ATTRIBUTE(5,protected,int, prZeroPadding);
	DYN_ATTRIBUTE(6,protected,int,prSamplingRate);
	DYN_ATTRIBUTE(7,public,bool,Residual);


//Config shortcuts

public:
	~SpectralSynthesisConfig(){};

/** Setter for Analysis Window size in number of samples. It should match the size used in the
 *	SpectralAnalysis Processing. An assertion fault is raised if size is not odd. This
 *	method changes size of AnalysisWindowGenerator, IFFT and CircularShift accordingly.
 */	
	void SetAnalWindowSize(TSize w);
/** Getter for Analysis Window Size
 *	@see SetAnalWindowSize
 */
	TSize GetAnalWindowSize() const;

/** Setter for Analysis Window type
 *	@see EWindowType
 */
	void SetAnalWindowType(const EWindowType& t);
/** Getter for Analysis Window type
 *	@see SetAnalWindowType
 */
	const EWindowType& GetAnalWindowType() const;

/** Sets Zero padding factor.
 *	A factor of zero means that no zero padding has been applied in the analysis.
 *	@see SpectralAnalysis::SetZeroPadding
 */
	void SetZeroPadding(int z);
/** Getter for Zero padding factor 
 *	@see SetZeroPadding*/
	int GetZeroPadding() const;
/** Setter for Synthesis window size in number of samples. Just affects the Synthesis
 *	Window Generator Size
 */
	void SetSynthWindowSize(TSize w);
/**	Getter for Synthesis Window Size
 *	@see SetSynthWindowSize
 */
	TSize GetSynthWindowSize()const;

/** Setter for Synthesis Hop size in number of samples.
 *	Changes size of the Synthesis WindowGenerator, and the Hop Size and Buffer Size in the
 *	OverlapAdd Processing
 */	
	void SetHopSize(TSize h);
/** Getter for Synthesis Hop size
 *	@see  SetHopSize
 */
	TSize GetHopSize() const;
/** Setter for global Sampling rate. It synchronizes all existing sizes. */
	void SetSamplingRate(TData sr);
/** Getter for global Sampling rate */
	TData GetSamplingRate() const;
/** Setter for Spectrum Size **/
	void SetSpectrumSize(TSize specSize);
	TSize GetSpectrumSize() const;

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
		Audio                     mAudio0,mAudio1, mAudio2,mAudio3;
		//Spectrum                  Spec;
		Audio                    mSynthWindow;


		//internal Processing Objects
		WindowGenerator			mPO_AnalWindowGen;
		WindowGenerator			mPO_SynthWindowGen;
		IFFT_rfftw              mPO_IFFT;
		AudioMultiplier            mPO_AudioProduct;
		CircularShift			mPO_CircularShift;
			
		// And the interfaces with the outside world.

		
		// Internal convenience methods.

		void AttachChildren();
		bool ConfigureChildren();
		void ConfigureData();

		
		/** Configuration method */
		bool ConcreteConfigure(const ProcessingConfig&);

		TInt32 CalculatePowerOfTwo(TInt32 size);

	public:
		SpectralSynthesis(const SpectralSynthesisConfig& cfg);
		SpectralSynthesis();
		~SpectralSynthesis();

		void Attach(Spectrum& in, Audio &out);
		
// Processing Object compliance methods.
		const char *GetClassName() const {return "SpectralSynthesis";}


		const ProcessingConfig &GetConfig() const {return mConfig;}

		/** Supervised mode execution */
		bool Do(void);

		/** Unsupervised mode execution */
		bool Do(Spectrum& in, Audio &out);
		
		bool Do(Frame& in);
		bool Do(Segment& in);


		/** Ports */
		InPortTmpl<Spectrum>     mInput;
		OutPortTmpl<Audio> mOutput;


	};

}; //end of namespace

#endif //_SpectralSynthesis_
