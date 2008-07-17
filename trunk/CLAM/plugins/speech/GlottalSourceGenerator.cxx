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

#include "GlottalSourceGenerator.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <cmath>

namespace CLAM
{
namespace detail
{
	static const char * metadata[] = {
		"key", "GlottalSourceGenerator",
		"category", "[plugin] Speech (experimental, GSoC)",
		"description", "Glottal Source Generator for input to the vowel resonator",
		0
	};
	static FactoryRegistrator<ProcessingFactory, GlottalSourceGenerator> reg = metadata;
}

bool GlottalSourceGenerator::Do(SpectralPeakArray& out)
{
	out.AddAll();
	out.UpdateData();
	out.InitIndices();
	float sampleRate = 44100;
	float spectralRange = sampleRate/2;
	unsigned nBins =1024;
	unsigned hopSize =512;
	TData fundamental=mFundamental.GetLastValue();
	TData maxJitter = mJitter.GetLastValue();
	TData maxShimmer = mShimmer.GetLastValue();
	unsigned nHarmonics = std::min(30u,unsigned(spectralRange/fundamental));
	out.SetnMaxPeaks(nHarmonics);
	out.SetnPeaks(0);
	out.SetScale(EScale::eLog);
	
	float jitterFraction =  maxJitter * ( (float)(rand()%100) / 100 - 0.5 ); //scale it randomly between -1,1
	fundamental *= 1 + jitterFraction;
	
	float shimmerFraction =  maxShimmer * ( (float) (rand()%100) / 100 - 0.5 ); //scale it randomly between -1,1
	
	IndexArray& outIndexBuffer=out.GetIndexArray();
	DataArray& outMagBuffer=out.GetMagBuffer();
	DataArray& outFreqBuffer=out.GetFreqBuffer();
	DataArray& outPhaseBuffer=out.GetPhaseBuffer();
	DataArray& outBinPosBuffer=out.GetBinPosBuffer();
	DataArray& outBinWidthBuffer=out.GetBinWidthBuffer();
	unsigned firstBin = fundamental*nBins/spectralRange;
	static unsigned t = 0;
	for (unsigned i=1; i<=nHarmonics; i++)
	{
		outIndexBuffer.AddElem(i-1);
		outFreqBuffer.AddElem(i*fundamental);
		outMagBuffer.AddElem(-20.-20*std::log( float(i) ) *  (1+shimmerFraction)  );
		outPhaseBuffer.AddElem(i*fundamental*t*hopSize*2*M_PI/sampleRate);
		outBinPosBuffer.AddElem(i*firstBin);
		outBinWidthBuffer.AddElem(firstBin/2);
	}
	t++;
	out.SetIsIndexUpToDate(true);
	return true;
}


}

