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
	static FactoryRegistrator<ProcessingFactory, SMSHarmonizer> regSMSHarmonizer("SMSHarmonizer");
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
	mPitchShift.GetInControl("IgnoreResidual").DoControl(mIgnoreResidual);

	mInputVoiceGain.SetBounds(-2.,2.);
	mInputVoiceGain.SetDefaultValue(0.);
	mInputVoiceGain.DoControl(0.);

	int n_voices = mConfig.GetNumberOfVoices();
	//max amount of voices: adding many voices is translated into many performance issues
	if (n_voices>6) n_voices = 6;

	mVoicesPitch.Resize(n_voices, "Pitch", this);
	mVoicesGain.Resize(n_voices, "Gain", this);
	for (int i=0; i < mVoicesPitch.Size(); i++)
	{
		mVoicesPitch[i].SetBounds(-24.,24.);
		mVoicesPitch[i].SetDefaultValue(0.); //no pitch shift
		mVoicesPitch[i].DoControl(0.);
		mVoicesGain[i].SetBounds(0.,2.);
		mVoicesGain[i].SetDefaultValue(0.);
		mVoicesGain[i].DoControl(0.);
	}

	return true;
}

bool SMSHarmonizer::Do( const SpectralPeakArray& inPeaks, 
			const Fundamental& inFund,
			const Spectrum& inSpectrum, 
			SpectralPeakArray& outPeaks,
			Fundamental& outFund,
			Spectrum& outSpectrum
		      )
{
	outPeaks = inPeaks;
	outFund = inFund;
	outSpectrum = inSpectrum;

	TData gain0 = mInputVoiceGain.GetLastValue();
	mSinusoidalGain.GetInControl("Gain").DoControl(gain0);
	mSinusoidalGain.Do(outPeaks,outPeaks);

	SpectralPeakArray mtmpPeaks;
	Fundamental mtmpFund;
	Spectrum mtmpSpectrum;

	for (int i=0; i < mVoicesPitch.Size(); i++)
	{
		TData gain = mVoicesGain[i].GetLastValue();
		if (gain<0.01) //means voice OFF
			continue;

		TData amount = CLAM_pow( 2., mVoicesPitch[i].GetLastValue()/12. ); //value adjust to equal-tempered scale semitones
		mPitchShift.GetInControl("PitchSteps").DoControl(amount);
		mPitchShift.Do( inPeaks,
				inFund, 
				inSpectrum,
				mtmpPeaks, 
				mtmpFund,
				mtmpSpectrum);
	
		mSinusoidalGain.GetInControl("Gain").DoControl(gain);
		mSinusoidalGain.Do(mtmpPeaks,mtmpPeaks);

		outPeaks = outPeaks + mtmpPeaks;
		if (!mIgnoreResidual)
			mSpectrumAdder.Do(outSpectrum, mtmpSpectrum, outSpectrum);
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

}
