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

using namespace CLAM;


bool SMSPitchShift::Do(const SpectralPeakArray& inPeaks,const Spectrum& inRes, SpectralPeakArray& outPeaks,Spectrum& outRes)
{
	outPeaks = inPeaks; //TODO big cludge for streaming
	outRes = inRes;

	mSpectralEnvelope.SetSpectralRange(mSpectralRange);
 	
	bool haveEnvelope=false;

	//First extract spectral envelope
	if(mIsHarmonic.GetLastValue()>0)
		haveEnvelope = mSpectralEnvelopeExtract.Do(inPeaks,mSpectralEnvelope);

	if(&outPeaks!=&inPeaks)//TODO: this is already solved inPeaks new DT
		outPeaks=inPeaks;
	DataArray& iFreqArray=inPeaks.GetFreqBuffer();
	DataArray& oFreqArray=outPeaks.GetFreqBuffer();
	DataArray& iBinPosArray=inPeaks.GetBinPosBuffer();
	DataArray& oBinPosArray=outPeaks.GetBinPosBuffer();
	TSize nPeaks=inPeaks.GetnPeaks();
	TData newFreq;
	TData amount=mShiftAmount.GetLastValue();
	//Shift all peaks
	for(int i=0;i<nPeaks;i++)
	{
		newFreq=iFreqArray[i]*amount;
		if(newFreq<mSpectralRange)
		{
			oFreqArray[i]=newFreq;
			oBinPosArray[i]=iBinPosArray[i]*amount;
		}
		else
		{
			outPeaks.SetnPeaks(i);
			break;
		}
	}
	//Apply original spectral shape and comb filter the residual
	if(mIsHarmonic.GetLastValue()>0)
	{
		if(haveEnvelope) mSpectralEnvelopeApply.Do(outPeaks,mSpectralEnvelope,outPeaks);
		mFDCombFilter.mFreq.DoControl(mIsHarmonic.GetLastValue()*amount);
		mFDCombFilter.Do(inRes,outRes);
	}
	return true;
}

bool SMSPitchShift::Do(const Frame& in, Frame& out)
{
	out=in;
	TData amount=mShiftAmount.GetLastValue();
	if(amount==1)//no pitch shift
		return true;
	mSpectralRange=in.GetResidualSpec().GetSpectralRange();
	mIsHarmonic.DoControl(in.GetFundamental().GetFreq(0));
	Fundamental tmpFund=in.GetFundamental();

	for (int i=0;i<in.GetFundamental().GetnCandidates();i++)
	{
		tmpFund.SetFreq(i,in.GetFundamental().GetFreq(i)*amount);
	}
	out.SetFundamental(tmpFund);
	return Do(in.GetSpectralPeakArray(),in.GetResidualSpec(),out.GetSpectralPeakArray(),out.GetResidualSpec());

}

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

static ProcessingFactory::Registrator<CLAM::SMSPitchShift> regtSMSPitchShift( "SMSPitchShift" );
