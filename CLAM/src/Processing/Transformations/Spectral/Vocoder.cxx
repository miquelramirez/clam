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

#include "Vocoder.hxx"
#include "Factory.hxx"

namespace CLAM
{


bool Vocoder::Do(const Spectrum& in, Spectrum& out)
{
	if ( !mConfig.GetPreserveOuts() )
	{
		out = in; //TODO big cludge for streaming
	}
	
	int i;
	
	int nBands = mNumBandsCtl.GetLastValue(); 
	int nGainBands = 5; //it is one more than the actual gains
	int firstBand = 5;
	TData fFirstBand = 100;
	
	DataArray gains(4);
	gains[0] = log2lin(mBand0GainCtl.GetLastValue());
	gains[1] = log2lin(mBand1GainCtl.GetLastValue());
	gains[2] = log2lin(mBand2GainCtl.GetLastValue());
	gains[3] = log2lin(mBand3GainCtl.GetLastValue());
	
	int spectrumSize = in.GetSize();
	TData spectralRange = in.GetSpectralRange();
	TData spectralResolution = spectrumSize/spectralRange;
	
	mMagBuffer.Resize(0);// make sure all bins are set to zero
	mMagBuffer.Resize(spectrumSize);
	mMagBuffer.SetSize(spectrumSize);
	
	TData bandFactor = pow(TData(spectrumSize),1./nBands);
	TData fBandFactor = pow(TData(spectralRange),1./nBands);
	
	TData fCurrentBandLimit = fFirstBand;
	int currentBandLimit = Round(fFirstBand*spectralResolution);
	int lastBandLimit = 0;
	TData fLastBandLimit = 0.;
	
	int freqShift = Round(mFreqShiftCtl.GetLastValue()*spectralResolution); 
	
	int currentOscBin = int(spectrumSize/nBands) + freqShift;
	TData fCurrentOsc = spectralRange/nBands;
	
	int currentGainIndex = 0;
	TData fCurrentGainBandLimit = fCurrentBandLimit*2;
	int currentGainBandLimit = Round(fCurrentGainBandLimit*spectralResolution);
	
	TData gainBandFactor = pow(TData(spectrumSize/currentBandLimit),1./nGainBands);
	TData fGainBandFactor = pow(TData(spectralRange/fCurrentGainBandLimit),1./nGainBands);
	
	TData bandEnergy =0;
	DataArray& inMag = in.GetMagBuffer();
	int currentBand = 0;
	for(int i = 0; i<spectrumSize; i++)
	{
		if(i>currentBandLimit)
		{
			bandEnergy = sqrt(bandEnergy);
			mMagBuffer[currentOscBin] = bandEnergy*gains[currentGainIndex];
			lastBandLimit = currentBandLimit;
			fCurrentBandLimit*=fBandFactor;
			currentBandLimit = Round(fCurrentBandLimit*spectralResolution);
			if(currentBandLimit>spectrumSize) currentBandLimit = spectrumSize;
			currentOscBin =  int ((currentBandLimit-lastBandLimit)*0.5 + lastBandLimit) + freqShift;
			bandEnergy = 0;
			currentBand++;
			if(currentBand>nBands) break;
		}
		if (i>currentGainBandLimit)
		{ 
			if(currentGainIndex<3)
				currentGainIndex++;
			fCurrentGainBandLimit*=fGainBandFactor;
			currentGainBandLimit = Round(fCurrentGainBandLimit*spectralResolution);
		}
		bandEnergy += inMag[i];
		
	}
	out.SetMagBuffer(mMagBuffer);
	return true;
}

typedef Factory<Processing> ProcessingFactory;

static ProcessingFactory::Registrator<Vocoder> regtVocoder( "Vocoder" );

}

