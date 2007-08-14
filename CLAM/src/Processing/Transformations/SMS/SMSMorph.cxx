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
	static const char * metadata[] = {
		"key", "SMSMorph",
		"category", "SMS Transformations",
		"description", "SMSMorph",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, SMSMorph> regSMSMorph("SMSMorph");
	static FactoryRegistrator<ProcessingFactory, SMSMorph> reg = metadata;
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
	if ( inFund1.GetFreq()!=0 && inFund2.GetFreq()!=0 && mConfig.GetIsHarmonic1() && mConfig.GetIsHarmonic2 () )
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

	mPeaksInterpolator.GetInControl("MagInterpolationFactor").DoControl(alpha);
// 	mPeaksInterpolator.GetInControl("FreqInterpolationFactor").DoControl(alpha);
// 	mPeaksInterpolator.GetInControl("PitchInterpolationFactor").DoControl(alpha);
	mPeaksInterpolator.Do(inPeaks1, inPeaks2, outPeaks);


//TODO separate alpha/interpolation value for peaks and residual????

	outSpectrum = inSpectrum1; //FIXME asserts without this...
	CLAM_DEBUG_ASSERT( inSpectrum1.GetSize()==inSpectrum2.GetSize(), "Expected two spectrums of the same size");
	mSpectrumInterpolator.GetInControl("InterpolationFactor").DoControl(alpha);

// 	TODO fix (and check SpectrumInterpolator bug... (add/fix const inputs) mSpectrumInterpolator.Do(inSpectrum1, inSpectrum2, outSpectrum);)
	mSpectrumInterpolator.Do(const_cast<Spectrum&>(inSpectrum1), const_cast<Spectrum&>(inSpectrum2), outSpectrum);

	return true;
}

bool SMSMorph::ConcreteConfigure(const ProcessingConfig& config)
{
	CopyAsConcreteConfig( mConfig, config );

	mInterpolationFactor.SetBounds(0.,1.);
	mInterpolationFactor.SetDefaultValue(0.5);

	return true;
}

}
