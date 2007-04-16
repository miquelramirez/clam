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

#ifndef _SpectralSynthesis_
#define _SpectralSynthesis_

//uncomment this if you want to use Ooura fft instead of fftw 
//#define CLAM_OOURA

#include "ProcessingComposite.hxx"

#include "Array.hxx"
#include "Audio.hxx"
#include "DataTypes.hxx"
#include "DynamicType.hxx"
#include "Err.hxx"
#include "IFFT.hxx"

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

#include "SpectralSynthesisConfig.hxx"

#include "AudioOutPort.hxx"
#include "InPort.hxx"

namespace CLAM {




	/* SpectralSynthesisConfig moved to SpectralSynthesisConfig.hxx */


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
		IFFT				mPO_IFFT;
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
		InPort<Spectrum>     mInput;
		AudioOutPort mOutput;


	};

} //end of namespace

#endif //_SpectralSynthesis_

