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

bool SMSHarmonizer::Do( const SpectralPeakArray& inPeaks, 
			const Fundamental& inFund,
			const Spectrum& inSpectrum, 
			SpectralPeakArray& outPeaks,
			Fundamental& outFund,
			Spectrum& outSpectrum
		      )
{
	//Voice 0 (input voice)
	outPeaks = inPeaks;
	outFund = inFund;
	outSpectrum = inSpectrum;

	//TODO - skip if gain<0.01, check if outputs arrive clean or not
	TData gain0 = mVoice0Gain.GetLastValue();
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

		TData amount = mVoicesPitch[i].GetLastValue();
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
		if (mIgnoreResidualCtl.GetLastValue()<0.01) // is 0
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
