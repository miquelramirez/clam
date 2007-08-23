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

#ifndef _SinusoidalSynthesis_
#define _SinusoidalSynthesis_

#include "SMSSynthesisConfig.hxx"
#include "ProcessingComposite.hxx"

#include "Array.hxx"
#include "Audio.hxx"
#include "DataTypes.hxx"
#include "DynamicType.hxx"
#include "Err.hxx"
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

#include "InPort.hxx"
#include "OutPort.hxx"
#include "AudioOutPort.hxx"

#include <stdlib.h>

namespace CLAM {

	/** This is a processing object class that performs a sinusoidal resynthesis
	 *  given a set of spectral peaks. Because it performs only one IFFT rather 
	 *  than the three performed by SMSSynthesis and it does not have process
	 *  the residual, it requires approximately half the CPU cycles needed
	 *  by SMSSynthesis, and it uses a lot less memory.
	 *
	 *  \author greg kellum [gkellum@iua.upf.edu] 08/02/2007
	 *  \since CLAM v1.2
	 */
	class SinusoidalSynthesis : public ProcessingComposite {

	private:
		// Configuration data
		SMSSynthesisConfig mConfig;

		int mCurrentFrame;

		//internal Processing Objects
		SpectralSynthesis		mSinSpectralSynthesis;
		SynthSineSpectrum		mSynthSineSpectrum;
		PhaseManagement			mPhaseMan;
		SpectrumAdder2			mSpectrumAdder;
		
		OverlapAdd				mOverlapAddSin;



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
		SinusoidalSynthesis(const SMSSynthesisConfig& cfg);
		SinusoidalSynthesis();
		~SinusoidalSynthesis();
		
		// Processing Object compliance methods.

		const char *GetClassName() const {return "SinusoidalSynthesis";}


		const ProcessingConfig &GetConfig() const {return mConfig;}

		/** Supervised mode execution */
		bool Do(void);

		/** non-supervised Do method. Produces as output the sinusoidal spectrum, the global spectrum.
		 *	and the sinusoidal, residual and globar audio frames. If this overload is used directly
		 *	you must set the controls for current time and current pitch from the outside.*/
		bool Do(SpectralPeakArray& inputSinusoidalPeaks,
				Spectrum& outputSpectrum,
				Audio& outputAudio);


		/** non-supervised Do method. SinusoidalSynthesis produces, as side effect, also some output Spectrums. 
		 *	Use this overload if you do not care about these spectrums and just need the output audio. 
		 *	If this overload is used directly you must set the controls for current time and current pitch from the outside.
		 */
		bool Do(SpectralPeakArray& inputSinusoidal, Audio& outputAudio);
		/** non-supervised Do method: works on a CLAM::Frame */
		bool Do(Frame& in);
		/** non-supervised Do method: works on a CLAM::Segment. Processes current frame in the 
		 *	segment and increments segment internal counter. */
		bool Do(Segment& in);
	protected:

		void InitFrame(Frame& in);

		/** Ports */
		InPort<SpectralPeakArray>     mInputSinSpectralPeaks;
		OutPort<Spectrum>     mOutputSinSpectrum;
		OutPort<Spectrum>     mOutputSpectrum;
		AudioOutPort mOutputAudio;

	public:
		//Controls
		InControlTmpl<SinusoidalSynthesis> mCurrentTimeControl;
		InControlTmpl<SinusoidalSynthesis> mCurrentPitch;


	};

}; //end of namespace

#endif //_SinusoidalSynthesis_

