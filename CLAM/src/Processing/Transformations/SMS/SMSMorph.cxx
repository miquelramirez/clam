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

#include "SMSMorph.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, SMSMorph> regSMSMorph("SMSMorph");
}

bool SMSMorph::Do(const Frame& in1, const Frame& in2, Frame& out)
{
	return Do( in1.GetSpectralPeakArray(),
		   in1.GetFundamental(),
		   in1.GetResidualSpec(),

		   in2.GetSpectralPeakArray(),
		   in2.GetFundamental(),
		   in2.GetResidualSpec(),

		   out.GetSpectralPeakArray(),
		   out.GetFundamental(), 
		   out.GetResidualSpec() 
		 );
}

bool SMSMorph::Do( const SpectralPeakArray& inPeaks1, 
		   const Fundamental& inFund1,
		   const Spectrum& inSpectrum1,

		   const SpectralPeakArray& inPeaks2,
		   const Fundamental& inFund2,
		   const Spectrum& inSpectrum2,

		   SpectralPeakArray& outPeaks,
		   Fundamental& outFund,
		   Spectrum& outSpectrum
		 )
{


	bool Harmonic = false;
	if (inFund1.GetFreq()!=0&&inFund2.GetFreq()!=0&&mConfig.GetIsHarmonic1()&&mConfig.GetIsHarmonic2())
		Harmonic = true;

	TData alpha = mInterpolationFactor.GetLastValue();
	TData newPitch = (1. - alpha)*inFund1.GetFreq() + alpha*inFund2.GetFreq();

	if( Harmonic ) newPitch=0;

	//Sets new fund freq
	if (outFund.GetnCandidates()==0)
		outFund.AddElem(newPitch,0);
	else
		outFund.SetFreq(0,newPitch);
	outFund.SetnCandidates(1);

	mPeaksInterpolator.Do(inPeaks1, inPeaks2, outPeaks);

	if (!mIgnoreResidual)
// 		TODO ask about 'clam' style... (mSpectrumInterpolator.Do(inSpectrum1, inSpectrum2, outSpectrum);)
// 		TODO fix (and check SpectrumInterpolator bug...
		mSpectrumInterpolator.Do(const_cast<Spectrum&>(inSpectrum1), const_cast<Spectrum&>(inSpectrum2), outSpectrum);

	return true;
}

bool SMSMorph::ConcreteConfigure(const ProcessingConfig& config)
{
	CopyAsConcreteConfig( mConfig, config );

	mIgnoreResidual = mConfig.GetIgnoreResidual();

	mInterpolationFactor.SetBounds(0.,1.);
	mInterpolationFactor.SetDefaultValue(0.5);

	return true;
}

}
