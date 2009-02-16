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

#ifndef _SMSSynthesis_
#define _SMSSynthesis_

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

	/** This is the processing object class we are using to group several
	 *  other processing objects. 
	 */
	class SMSSynthesis : public ProcessingComposite {

	private:
		// Configuration data
		typedef SMSSynthesisConfig Config;
		Config mConfig;

		int mCurrentFrame;

		//internal Processing Objects
		/** We use a SpectralSynthesis for Sinusoidal and another one
		 * for residual synthesis. In principle we could use the same one
		 * but we may want to synthesize both components separately.
		 */
		SpectralSynthesis		mSinSpectralSynthesis;
		SpectralSynthesis		mResSpectralSynthesis;
		SpectralSynthesis		mSpectralSynthesis;
		SynthSineSpectrum		mSynthSineSpectrum;
		PhaseManagement			mPhaseMan;
		SpectrumAdder2			mSpectrumAdder;
		
		OverlapAdd				mOverlapAddSin;
		OverlapAdd				mOverlapAddRes;
		OverlapAdd				mOverlapAddGlobal;



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
		SMSSynthesis(const Config& cfg = Config());
		~SMSSynthesis();
		
		// Processing Object compliance methods.

		const char *GetClassName() const {return "SMSSynthesis";}


		const ProcessingConfig &GetConfig() const {return mConfig;}

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

		/** non-supervised Do method. Produces as output the sinusoidal spectrum, the global spectrum.
		 *	and the global audio frames. If this overload is used directly
		 *	you must set the controls for current time and current pitch from the outside.*/
		bool Do(SpectralPeakArray& inputSinusoidalPeaks, Spectrum& inputResidualSpectrum,
			Spectrum& outputSinusoidalSpectrum,	Spectrum& outputSpectrum,
			Audio& outputAudio);

			/** non-supervised Do method. SMSSynthesis produces, as side effect, also some output Spectrums. 
		 *	Use this overload if you do not care about these spectrums and just need the output audio. 
		 *	If this overload is used directly you must set the controls for current time and current pitch from the outside.
		 */
		bool Do(SpectralPeakArray& inputSinusoidal, Spectrum& inputSpectrum, 
			Audio& outputAudio, Audio& outputSinusoidal, Audio& outputResidual);
		/** non-supervised Do method: works on a CLAM::Frame */
		bool Do(Frame& in);
		/** 
		* If isSynthesizeSinusoidsAndResidual is false, this method 
		* synthesizes only the mixdown of the sinusoids and residual
		* rather than synthesizing each separately. If you don't
		* need the separate audio representations of the sinusoid 
		* and residual, use this with isSynthesizeSinusoidsAndResidual
		* set to false, because then this method only performs one IFFT
		* rather than three which saves a considerable number of CPU cycles.
		*/
		bool Do(Frame& in, bool isSynthesizeSinusoidsAndResidual);
		/** non-supervised Do method: works on a CLAM::Segment. Processes current frame in the 
		 *	segment and increments segment internal counter. */
		bool Do(Segment& in);
	protected:

		void InitFrame(Frame& in);

		/** Ports */
		InPort<SpectralPeakArray>     mInputSinSpectralPeaks;
		InPort<Spectrum>     mInputResSpectrum;
		OutPort<Spectrum>     mOutputSinSpectrum;
		OutPort<Spectrum>     mOutputSpectrum;
		AudioOutPort mOutputAudio;
		AudioOutPort mOutputResAudio;
		AudioOutPort mOutputSinAudio;

	public:
		//Controls
		FloatInControl mCurrentTimeControl;
		FloatInControl mCurrentPitch;


	};

}; //end of namespace

#endif //_SMSSynthesis_

