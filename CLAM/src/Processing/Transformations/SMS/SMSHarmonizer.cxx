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

#include "SMSHarmonizer.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "SMSHarmonizer",
		"category", "SMS Transformations",
		"description", "SMSHarmonizer",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SMSHarmonizer> reg = metadata;
}

bool SMSHarmonizer::ConcreteConfigure(const ProcessingConfig& config)
{
	CopyAsConcreteConfig( mConfig, config );

	if ( mConfig.GetNumberOfVoices()==0 )
	{
		AddConfigErrorMessage("The provided config object lacked NumberOfVoices value");
		return false;
	}

	mIgnoreResidual = mConfig.GetIgnoreResidual();

	mPitchShift.Configure( FrameTransformationConfig() );
	SendFloatToInControl(mPitchShift,"IgnoreResidual",mIgnoreResidual);

	mInputVoiceGain.SetBounds(-2.,2.);
	mInputVoiceGain.SetDefaultValue(0.);
	mInputVoiceGain.DoControl(0.);

	int n_voices = mConfig.GetNumberOfVoices();
	
	if (n_voices>MAX_AMOUNT_OF_VOICES) n_voices = MAX_AMOUNT_OF_VOICES;

	mVoicesPitch.Resize(n_voices, "Pitch", this);
	mVoicesGain.Resize(n_voices, "Gain", this);
	mVoicesDetuningAmount.Resize(n_voices, "Voice Detuning", this);
	mVoicesDelay.Resize(n_voices, "Voice Delay", this);
	for (int i=0; i < mVoicesPitch.Size(); i++)
	{
		mVoicesGain[i].SetBounds(0.,2.);
		mVoicesGain[i].SetDefaultValue(0.);
		mVoicesGain[i].DoControl(0.);

		mVoicesPitch[i].SetBounds(-24.,24.);
		mVoicesPitch[i].SetDefaultValue(0.); //no pitch shift
		mVoicesPitch[i].DoControl(0.);

		mVoicesDetuningAmount[i].SetBounds(0.,1.);
		mVoicesDetuningAmount[i].SetDefaultValue(0.);
		mVoicesDetuningAmount[i].DoControl(0.);

		mVoicesDelay[i].SetBounds(0.,1.);
		mVoicesDelay[i].SetDefaultValue(0.);
		mVoicesDelay[i].DoControl(0.);
	}

	return true;
}

bool SMSHarmonizer::Do(const Frame& in, Frame& out)
{
	return Do( in.GetSpectralPeakArray(),
		   in.GetFundamental(),
		   in.GetResidualSpec(),

		   out.GetSpectralPeakArray(),
		   out.GetFundamental(), 
		   out.GetResidualSpec() 
		 );
}

bool SMSHarmonizer::Do(
			const SpectralPeakArray& inPeaks, 
			const Fundamental& inFund,
			const Spectrum& inSpectrum, 
			SpectralPeakArray& outPeaks,
			Fundamental& outFund,
			Spectrum& outSpectrum
		)
{
	outPeaks = inPeaks;
	outFund = inFund;
	outSpectrum = inSpectrum; //residual processing is ignored by default, so the output residual is the input one

	TData gain0 = mInputVoiceGain.GetLastValue();
	SendFloatToInControl(mSinusoidalGain,"Gain",gain0);
	mSinusoidalGain.Do(outPeaks,outPeaks);
	//TODO add detuning and delay for input voice?

	SpectralPeakArray mtmpPeaks;
	Fundamental mtmpFund;
	Spectrum mtmpSpectrum;

	for (int i=0; i < mVoicesPitch.Size(); i++)
	{
		TData gain = mVoicesGain[i].GetLastValue();
		if (gain<0.01) //means voice OFF
			continue;
		
		TData amount = mVoicesPitch[i].GetLastValue() + frand()*mVoicesDetuningAmount[i].GetLastValue(); //detuning
		amount = CLAM_pow( 2., amount/12. ); //adjust to equal-tempered scale semitones

		SendFloatToInControl(mPitchShift,"PitchSteps",amount);
		mPitchShift.Do( inPeaks,
				inFund, 
				inSpectrum,
				mtmpPeaks, 
				mtmpFund,
				mtmpSpectrum);
				
		SendFloatToInControl(mSinusoidalGain,"Gain",gain);
		mSinusoidalGain.Do(mtmpPeaks,mtmpPeaks);

		TData delay = mVoicesDelay[i].GetLastValue();
		if (delay>0.)
		{
			SendFloatToInControl(mPeaksDelay,"Delay Control",delay);
			mPeaksDelay.Do(mtmpPeaks, mtmpPeaks);
		}

		outPeaks = outPeaks + mtmpPeaks;
		
		if (!mIgnoreResidual)
			mSpectrumAdder.Do(outSpectrum, mtmpSpectrum, outSpectrum);
	}
	return true;
}

}
