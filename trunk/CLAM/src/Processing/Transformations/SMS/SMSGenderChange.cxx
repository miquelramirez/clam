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

#include "SMSGenderChange.hxx"
#include "Factory.hxx"

namespace CLAM
{

bool SMSGenderChange::Do(const SpectralPeakArray& inPeaks, 
				const Fundamental& inFund,
				const Spectrum& inSpectrum, 
				SpectralPeakArray& outPeaks,
				Fundamental& outFund,
				Spectrum& outSpectrum)
{

	if(inFund.GetFreq(0))//we only transform voiced frames
	{	
		TData minPitch = 100;
		TData maxPitch = 800;

		//Maximum spectral shift
		TData maxSss = 200;

		//amount for spectral shape shift
		TData sssAmount;

		TData pitch = inFund.GetFreq(0);

		if(pitch<minPitch) sssAmount = 0;
		else if (pitch>maxPitch) sssAmount = maxSss;
		else sssAmount = (pitch-minPitch) / ( (maxPitch-minPitch)/maxSss);

		
		//if amount is zero it means from male to female, else from female to male
		TData amount=mAmount.GetLastValue();

		TData pitchTransposition;

		if(amount) //female to male
		{
			sssAmount=-sssAmount;
			pitchTransposition=0.5;
		}
		else //male to female
		{
			pitchTransposition=2;
		}

		mSpectralShapeShift.GetInControl("Amount").DoControl(sssAmount);

		mPitchShift.GetInControl("Amount").DoControl(pitchTransposition);

		SpectralPeakArray tmpSpectralPeaks;
		mSpectralShapeShift.Do(inPeaks,tmpSpectralPeaks);

		mPitchShift.Do( tmpSpectralPeaks, inFund, inSpectrum, 
				outPeaks, outFund, outSpectrum);
	}
	return true;
}

bool SMSGenderChange::Do(const Frame& in, Frame& out)
{
	out=in;	// TODO most likely this copy is not necessary

	return Do( in.GetSpectralPeakArray(),
				in.GetFundamental(), 
				in.GetResidualSpec(), 
				out.GetSpectralPeakArray(),
				out.GetFundamental(), 
				out.GetResidualSpec() 
				);

}

typedef Factory<Processing> ProcessingFactory;
static ProcessingFactory::Registrator<SMSGenderChange> regtSMSGenderChange( "SMSGenderChange" );

}
