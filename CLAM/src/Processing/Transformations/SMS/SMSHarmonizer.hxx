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


namespace CLAM{

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

		/** 
		 *	xamat: adding residual does not improve results much and adds a lot of overhead, there should
		 *	probably be a configuration parameter to control whether we want to add residual or not, but that
		 *	would mean changing the kind of configuration. For the time being the output residual is the input.
		 */
		InControl mIgnoreResidualCtl;

		InControl mVoice0Gain; ///< Input voice gain

		//TODO define it with another InControl or like a processing with configuration?
		#define VOICES_AMOUNT 4 
		InControlArray mVoicesPitch; ///< Voices pitch
		InControlArray mVoicesGain; ///< Voices gain

	public:
		SMSHarmonizer()
			:
			mInPeaks("In SpectralPeaks", this),
			mOutPeaks("Out SpectralPeaks", this),
			mInFund("In Fundamental", this),
			mOutFund("Out Fundamental", this),
			mInSpectrum("In Spectrum", this),
			mOutSpectrum("Out Spectrum", this),

			mIgnoreResidualCtl("IgnoreResidual", this),

			mVoice0Gain("Input Voice Gain", this),
			mVoicesPitch(VOICES_AMOUNT, "Pitch", this),
			mVoicesGain(VOICES_AMOUNT, "Gain", this)
		{
			Configure(  FrameTransformationConfig() );
		}

 		~SMSHarmonizer() {}
		
		bool ConcreteConfigure(const ProcessingConfig& c)
		{
			mPitchShift.Configure( FrameTransformationConfig() );

			mIgnoreResidualCtl.SetBounds(0,1);
			//By default we ignore residual!!
			mIgnoreResidualCtl.SetDefaultValue(1);
			mIgnoreResidualCtl.DoControl(1);

			mVoice0Gain.SetBounds(-2.,2.);
			mVoice0Gain.SetDefaultValue(1.);
			mVoice0Gain.DoControl(1.);

			for (int i=0; i < mVoicesPitch.Size(); i++)
			{
				mVoicesPitch[i].SetBounds(-3.,3.);
				mVoicesPitch[i].SetDefaultValue(1.); //no pitch shift
				mVoicesPitch[i].DoControl(0.);
				mVoicesGain[i].SetBounds(-2.,2.);
				mVoicesGain[i].DoControl(0.);
			}

			return true;
		}

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
			return mPitchShift.mIgnoreResidual.DoControl(value);
		}

// FIXME
		// Configuration data
// 		DYN_ATTRIBUTE(0,protected,int,prSamplingRate);
// 		DYNAMIC_TYPE_USING_INTERFACE( SimpleConfig, 1, ProcessingConfig );
// 		DYN_ATTRIBUTE( 0, public, int, FrameSize);
// 		protected:
// 		void DefaultInit()
// 		{
// 			AddAll();
// 			UpdateData();
// 			SetFrameSize(512);
// 		};

	private:

		SMSPitchShift mPitchShift;
		SpectrumAdder2 mSpectrumAdder;
		SMSSinusoidalGain mSinusoidalGain;
	};	
	
};//namespace CLAM

#endif // _SMSHarmonizer_

