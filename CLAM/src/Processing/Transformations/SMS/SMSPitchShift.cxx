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

#include "SMSPitchShift.hxx"
#include "Factory.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{


bool SMSPitchShift::Do(const SpectralPeakArray& inPeaks,
		const Fundamental& inFund, 
		const Spectrum& inRes, 
		SpectralPeakArray& outPeaks,
		Fundamental& outFund,
		Spectrum& outRes)
{
	bool ignoreResidual = mIgnoreResidual.GetLastValue()>0.01;
	if (!mConfig.GetPreserveOuts()) //TODO big cludge for streaming
	{
		outPeaks = inPeaks; 
		outFund = inFund;
		if (!ignoreResidual)
			outRes = inRes;
	}

	TData amount=mAmount.GetLastValue();
	if(amount==1)//no pitch shift
		return true;
	
	TData spectralRange = inRes.GetSpectralRange();
	TData fundamental = inFund.GetFreq(0);
	bool isHarmonic = fundamental > .5;
	mIsHarmonic.DoControl( isHarmonic );

	for (int i=0;i<inFund.GetnCandidates();i++)
		outFund.SetFreq(i,inFund.GetFreq(i)*amount);

	mSpectralEnvelope.SetSpectralRange(spectralRange);
 	
	bool haveEnvelope=false;

	//First extract spectral envelope
	if(isHarmonic)
		haveEnvelope = mSpectralEnvelopeExtract.Do(inPeaks,mSpectralEnvelope);

	if(&outPeaks!=&inPeaks)//TODO: this is already solved inPeaks new DT
		outPeaks=inPeaks;
	DataArray& iFreqArray=inPeaks.GetFreqBuffer();
	DataArray& oFreqArray=outPeaks.GetFreqBuffer();
	DataArray& iBinPosArray=inPeaks.GetBinPosBuffer();
	DataArray& oBinPosArray=outPeaks.GetBinPosBuffer();
	TSize nPeaks=inPeaks.GetnPeaks();
	
	//Shift all peaks
	for(int i=0;i<nPeaks;i++)
	{
		TData newFreq=iFreqArray[i]*amount;
		if (newFreq>=spectralRange)
		{
			outPeaks.SetnPeaks(i);
			break;
		}
		oFreqArray[i]=newFreq;
		oBinPosArray[i]=iBinPosArray[i]*amount;
	}
	//Apply original spectral shape and comb filter the residual
	if (isHarmonic)
	{
		if (haveEnvelope)
			mSpectralEnvelopeApply.Do(outPeaks,mSpectralEnvelope,outPeaks);
		if (false && !ignoreResidual)
		{
			mFDCombFilter.mFreq.DoControl(fundamental*amount);
			mFDCombFilter.Do(inRes,outRes);
		}
	}
	return true;
}

bool SMSPitchShift::Do(const Frame& in, Frame& out)
{
	return Do( in.GetSpectralPeakArray(),
			in.GetFundamental(), 
			in.GetResidualSpec(), 
			out.GetSpectralPeakArray(),
			out.GetFundamental(), 
			out.GetResidualSpec());
}

namespace detail
{
	static ProcessingFactory::Registrator<SMSPitchShift> regtSMSPitchShift( "SMSPitchShift" );
}

}

