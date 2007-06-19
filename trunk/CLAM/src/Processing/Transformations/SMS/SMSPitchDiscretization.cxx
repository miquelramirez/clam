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

#include "SMSPitchDiscretization.hxx"
#include "ProcessingFactory.hxx"



namespace CLAM
{


namespace detail
{	
	static FactoryRegistrator<ProcessingFactory, SMSPitchDiscretization> regSMSPitchDiscretization("SMSPitchDiscretization");
}

bool SMSPitchDiscretization::Do(const Frame& in, Frame& out)
{
	return Do( in.GetSpectralPeakArray(), 
		in.GetFundamental(), 
		in.GetSpectrum(), 
		
		out.GetSpectralPeakArray(), 
		out.GetFundamental(), 
		out.GetSpectrum() 
	);
}

bool SMSPitchDiscretization::Do( 	const SpectralPeakArray& inPeaks, 
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
	
	TData pitch = inFund.GetFreq();

	if (pitch>0)
	{
		TData log2=0.69314718f;

		int nst = Round(12*log(pitch/55)/log2);
		TData pow2_1_12=1.0594630f;
		TData discPitch=55*(CLAM_pow(pow2_1_12,nst));
		
		TData amount=discPitch/pitch;

		mPitchShift.GetInControl("Amount").DoControl(amount);
		mPitchShift.Do( inPeaks, 
		                inFund, 
		                inSpectrum, 
		                outPeaks, 
		                outFund, 
		                outSpectrum);
		
		Fundamental tmpFund;
		tmpFund.AddElem(discPitch);
		outFund = tmpFund;
	}
	return true;

}


}

