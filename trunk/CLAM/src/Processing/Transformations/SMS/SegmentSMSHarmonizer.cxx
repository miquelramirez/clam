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

#include "SegmentSMSHarmonizer.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "SegmentSMSHarmonizer",
// 		"category", "SMS Transformations",
		"description", "SegmentSMSHarmonizer",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SegmentSMSHarmonizer> reg = metadata;
}

bool SegmentSMSHarmonizer::Do(const Frame& in, Frame& out)
{
	BPF& voices=mConfig.GetBPF();
	TSize nVoices=voices.Size();
	
	for(int i=0;i<nVoices;i++)
	{
		TData amount=voices.GetValueFromIndex(i);
		TData gain=voices.GetXValue(i);
		mPitchShift.GetInControl("PitchSteps").DoControl(amount);
		mPitchShift.Do(in,mTmpFrame);
		Gain(mTmpFrame,gain);
		AddFrame(mTmpFrame,out,out);
	}
	
	//We set fundamental value of input to zero as it is inharmonic
	out.SetFundamental(mTmpFund);
	return true;
}

void SegmentSMSHarmonizer::AddFrame(const Frame& in1, const Frame& in2, Frame& out)
{
	if(mIgnoreResidualCtl.GetLastValue()<0.01) // is 0
	{
		mSpectrumAdder.Start();
		mSpectrumAdder.Do(in1.GetResidualSpec(),in2.GetResidualSpec(),out.GetResidualSpec());
		mSpectrumAdder.Stop();
	}
	out.SetSpectralPeakArray(in1.GetSpectralPeakArray()+in2.GetSpectralPeakArray());
}
	
void SegmentSMSHarmonizer::Gain(Frame& inputFrame, TData gain)
{
	SpectralPeakArray& peaks=inputFrame.GetSpectralPeakArray();
	Spectrum& residual=inputFrame.GetResidualSpec();
	DataArray& peakMag=peaks.GetMagBuffer();
	int nPeaks=peaks.GetnPeaks();
	int specSize=residual.GetSize();
//TODO check whether spectrum is in dB or not
	TData linGain=Lin(gain);

	for(int i=0;i<nPeaks;i++)
	{
		peakMag[i]=std::min(peakMag[i]+gain,TData(0));
	}
	if(mIgnoreResidualCtl.GetLastValue()<0.01) // is 0
	{
		DataArray& resMag = residual.GetMagBuffer();
		for(int i=0;i<specSize;i++)
		{
			resMag[i] = resMag[i]*linGain;
		}
	}
}


}

