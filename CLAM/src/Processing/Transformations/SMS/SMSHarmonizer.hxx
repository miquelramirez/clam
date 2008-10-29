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


#ifndef _SMSHarmonizer_
#define _SMSHarmonizer_

#include "SMSHarmonizerConfig.hxx"
#include "Frame.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "InControlArray.hxx"
#include "FrameTransformation.hxx"
#include "SpectralPeakArray.hxx"
#include "SMSPitchShift.hxx"
#include "SpectrumAdder2.hxx"
#include "SMSSinusoidalGain.hxx"
#include "TokenDelay.hxx"
#include "SpectralPeakArray.hxx"

namespace CLAM{

	/**	\brief Many voices harmonizer using the SMS model
	 *
	 *	Harmonizes many voices adding pitch-shifted gained versions of the original voice (with the same timbre)
	 */
	class SMSHarmonizer: public FrameTransformation
	{	

		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const { return "SMSHarmonizer"; }
		
		InPort<SpectralPeakArray> mInPeaks;
		OutPort<SpectralPeakArray> mOutPeaks;
		InPort<Fundamental> mInFund;
		OutPort<Fundamental> mOutFund;
		InPort<Spectrum> mInSpectrum;
		OutPort<Spectrum> mOutSpectrum;

		FloatInControl mInputVoiceGain; ///< Input 'clean' voice gain

		//(fixed) max amount of voices: adding many voices is translated into many performance issues
		#define MAX_AMOUNT_OF_VOICES 6

		InControlArray mVoicesPitch; ///< Voices pitch (in semitones)
		InControlArray mVoicesGain; ///< Voices gain
		InControlArray mVoicesDetuningAmount; ///< amount of randomness in voice detuning (one semitone max)
		InControlArray mVoicesDelay; ///< delay amount of each voice

		// FIXME move ore remove this?
		#define frand() ( float( rand() ) / float(RAND_MAX) )

		/** 
		 *	xamat: adding residual does not improve results much and adds a lot of overhead.
		 *	Ignoring by default (check SMSHarmonizerConfig), so the output residual is the input.
		 */
		bool mIgnoreResidual;
	public:
		SMSHarmonizer()
			:
			mInPeaks("In SpectralPeaks", this),
			mOutPeaks("Out SpectralPeaks", this),
			mInFund("In Fundamental", this),
			mOutFund("Out Fundamental", this),
			mInSpectrum("In Spectrum", this),
			mOutSpectrum("Out Spectrum", this),

			mInputVoiceGain("Input Voice Gain", this),
			mVoicesPitch(0, "Pitch", this),
			mVoicesGain(0, "Gain", this),
			mVoicesDetuningAmount(0, "Voice Detuning", this),
			mVoicesDelay(0, "Voice Delay", this)
		{
			Configure( mConfig );
		}

 		~SMSHarmonizer() {}

		bool Do()
		{
			bool result = Do(mInPeaks.GetData(), 
					 mInFund.GetData(), 
					 mInSpectrum.GetData(), 
					 mOutPeaks.GetData(), 
					 mOutFund.GetData(),
					 mOutSpectrum.GetData() 
					 );

			mInPeaks.Consume(); 
			mInFund.Consume(); 
			mInSpectrum.Consume(); 
			mOutPeaks.Produce(); 
			mOutFund.Produce();
			mOutSpectrum.Produce();

			return result;
		}
		
		bool Do(const Frame& in, Frame& out);

		bool Do(const SpectralPeakArray& inPeaks,
			const Fundamental& inFund,
			const Spectrum& inSpectrum,
			SpectralPeakArray& outPeaks,
			Fundamental& outFund,
			Spectrum& outSpectrum
			);

		int IgnoreResidual(TControlData value)
		{
			mPitchShift.mIgnoreResidual.DoControl(value);
			return 0;
		}

		typedef SMSHarmonizerConfig Config;

		const ProcessingConfig & GetConfig() const
		{
			return mConfig;
		}

	protected:
		bool ConcreteConfigure(const ProcessingConfig& config);

	private:
		Config mConfig;


		/** Child processings **/
		SMSPitchShift mPitchShift;
		SpectrumAdder2 mSpectrumAdder;
		SMSSinusoidalGain mSinusoidalGain;

		TokenDelay<SpectralPeakArray> mPeaksDelay;

	};	
	
};//namespace CLAM

#endif // _SMSHarmonizer_

