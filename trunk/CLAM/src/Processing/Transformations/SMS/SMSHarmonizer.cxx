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

#include "SMSHarmonizer.hxx"
#include "Factory.hxx"

using namespace CLAM;



bool SMSHarmonizer::Do(const Frame& in, Frame& out)
{
	int i;
	BPF voices=mConfig.GetBPFAmount();
	TSize nVoices=voices.Size();
	Frame tmpInFrame(in),tmpOutFrame(in);
	tmpOutFrame.AddResidualSpec();
	tmpOutFrame.AddSpectralPeakArray();
	tmpOutFrame.UpdateData();
	
	for(i=0;i<nVoices;i++)
	{
		TData amount=voices.GetValueFromIndex(i);
		TData gain=voices.GetXValue(i);
		mPO_PitchShift.mAmountCtrl.DoControl(amount);
		mPO_PitchShift.Do(in,tmpInFrame);
		Gain(tmpInFrame,gain);
		AddFrame(tmpInFrame,tmpOutFrame,tmpOutFrame);
	}
	out.SetResidualSpec(tmpOutFrame.GetResidualSpec());
	out.SetSpectralPeakArray(tmpOutFrame.GetSpectralPeakArray());
	//We set fundamental value of input to zero as it is inharmonic
	Fundamental tmpFund;
	tmpFund.AddElem();
	out.SetFundamental(tmpFund);
	return true;
}

void SMSHarmonizer::AddFrame(const Frame& in1, const Frame& in2, Frame& out)
{
	mPO_SpectrumAdder.Start();
	mPO_SpectrumAdder.Do(in1.GetResidualSpec(),in2.GetResidualSpec(),out.GetResidualSpec());
	mPO_SpectrumAdder.Stop();
	out.SetSpectralPeakArray(in1.GetSpectralPeakArray()+in2.GetSpectralPeakArray());
}
	
void SMSHarmonizer::Gain(Frame& inputFrame, TData gain)
{
	SpectralPeakArray& peaks=inputFrame.GetSpectralPeakArray();
	Spectrum& residual=inputFrame.GetResidualSpec();
	DataArray& peakMag=peaks.GetMagBuffer();
	int i;
	int nPeaks=peaks.GetnPeaks();
	int specSize=residual.GetSize();
//TODO check whether spectrum is in dB or not
	TData linGain=Lin(gain);

	for(i=0;i<nPeaks;i++)
	{
		peakMag[i]=std::min(peakMag[i]+gain,TData(0));
	}
	for(i=0;i<specSize;i++)
	{
		residual.SetMag(i,residual.GetMag(i)*linGain);
	}
}

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
static ProcessingFactory::Registrator<CLAM::SMSHarmonizer> regtSMSHarmonizer( "SMSHarmonizer" );
