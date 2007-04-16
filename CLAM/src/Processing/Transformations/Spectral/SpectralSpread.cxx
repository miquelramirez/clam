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

#include "SpectralSpread.hxx"
#include "Factory.hxx"

namespace CLAM
{
	
bool SpectralSpread::Do(const Spectrum& 
			inSpec,Spectrum& outSpec)
{

	if (!mConfig.GetPreserveOuts())
	{
		outSpec = inSpec; //TODO big cludge for streaming
	}
	
	int spectrumSize = inSpec.GetSize();
	TData spectralResolution = spectrumSize/inSpec.GetSpectralRange();
	
	int centerPoint = Round (mCenterFreqCtl.GetLastValue()*spectralResolution);
	int amount= Round(mAmount.GetLastValue()*spectralResolution);
	
	mBPFSpectrum.SetSize(spectrumSize);
	
	
	//Shift spectral shape
	Array<Point>& magBPF=mBPFSpectrum.GetMagBPF().GetPointArray();
	//magBPF.SetIntpType(EInterpolation::eLinear);
	Array<Point>& phaseBPF=mBPFSpectrum.GetPhaseBPF().GetPointArray();
	//phaseBPF.SetIntpType(EInterpolation::eLinear);
	
	magBPF.SetSize(0);
	phaseBPF.SetSize(0);
	
	DataArray& inMag = inSpec.GetMagBuffer();
	DataArray& inPhase = inSpec.GetPhaseBuffer();
	
	TData binWidth = 1./spectralResolution;
	int i;
	TData freq = 0;
	TData mag;
	TData phase;
	int nPoints = 0;
	for (i=0; i<centerPoint-amount; i++)
	{
		mag = inMag[i+amount]; 
		phase = inPhase[i+amount];
		magBPF.AddElem(Point(freq,mag));
		phaseBPF.AddElem(Point(freq, phase));
		freq += binWidth;
		nPoints++;
	}
	
	freq = centerPoint * binWidth;
	magBPF.AddElem(Point(freq, inMag[centerPoint]));
	phaseBPF.AddElem(Point(freq, inPhase[centerPoint]));
	nPoints++;
	
	freq = (centerPoint+amount)*binWidth;
	
	for (i=centerPoint+amount ; i<spectrumSize; i++)
	{
		mag = inMag[i-amount]; 
		phase = inPhase[i-amount];
		magBPF.AddElem(Point(freq,mag));
		phaseBPF.AddElem(Point(freq, phase));
		freq += binWidth;
		nPoints++;
	}
	
	mBPFSpectrum.SetBPFSize(nPoints);
	mBPFSpectrum.SynchronizeTo(mFlag);
	outSpec.SetMagBuffer(mBPFSpectrum.GetMagBuffer());
	outSpec.SetPhaseBuffer(mBPFSpectrum.GetPhaseBuffer());
	
	return true;
}

typedef Factory<Processing> ProcessingFactory;
static ProcessingFactory::Registrator<SpectralSpread> regtSpectralSpread( "SpectralSpread" );

}

