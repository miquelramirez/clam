/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

using namespace CLAM;


bool SMSPitchShift::Do(const SpectralPeakArray& inPeaks,const Spectrum& inRes, SpectralPeakArray& outPeaks,Spectrum& outRes)
{
	int i;
	mSpectralEnvelope.SetSpectralRange(mSpectralRange);
 	
	bool haveEnvelope=false;

	//First extract spectral envelope
	if(mIsHarmonic.GetLastValue()>0)
		haveEnvelope = mPO_SpectralEnvelopeExtract.Do(inPeaks,mSpectralEnvelope);

	if(&outPeaks!=&inPeaks)//TODO: this is already solved inPeaks new DT
		outPeaks=inPeaks;
	DataArray& iFreqArray=inPeaks.GetFreqBuffer();
	DataArray& oFreqArray=outPeaks.GetFreqBuffer();
	DataArray& iBinPosArray=inPeaks.GetBinPosBuffer();
	DataArray& oBinPosArray=outPeaks.GetBinPosBuffer();
	TSize nPeaks=inPeaks.GetnPeaks();
	TData newFreq;
	TData amount=mAmountCtrl.GetLastValue();
	//Shift all peaks
	for(i=0;i<nPeaks;i++)
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
		if(haveEnvelope) mPO_SpectralEnvelopeApply.Do(outPeaks,mSpectralEnvelope,outPeaks);
		mPO_FDCombFilter.mFreq.DoControl(mIsHarmonic.GetLastValue()*amount);
		mPO_FDCombFilter.Do(inRes,outRes);
	}
	return true;
}

bool SMSPitchShift::Do(const Frame& in, Frame& out)
{
	mSpectralRange=in.GetResidualSpec().GetSpectralRange();
	mIsHarmonic.DoControl(in.GetFundamental().GetFreq(0));
	Fundamental tmpFund=in.GetFundamental();
	int i;
	for (i=0;i<in.GetFundamental().GetnCandidates();i++)
	{
		tmpFund.SetFreq(i,in.GetFundamental().GetFreq(i)*mAmountCtrl.GetLastValue());
	}
	out.SetFundamental(tmpFund);
	return Do(in.GetSpectralPeakArray(),in.GetResidualSpec(),out.GetSpectralPeakArray(),out.GetResidualSpec());
}
