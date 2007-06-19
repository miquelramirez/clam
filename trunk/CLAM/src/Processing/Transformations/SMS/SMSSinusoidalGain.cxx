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

#include "SMSSinusoidalGain.hxx"
#include "ProcessingFactory.hxx"


namespace CLAM
{

namespace Hidden
{	
	static FactoryRegistrator<ProcessingFactory, SMSSinusoidalGain> regSMSSinusoidalGain("SMSSinusoidalGain");
}


bool SMSSinusoidalGain::Do(const SpectralPeakArray& in, SpectralPeakArray& out)
{
	out = in; // TODO  big cludge for streaming
	
	TSize nPeaks=in.GetnPeaks();
	out.SetnMaxPeaks(in.GetnMaxPeaks());
	out.SetnPeaks(nPeaks);
	
	TData amount=mAmount.GetLastValue();
	SpectralPeak tmpPeak;
	IndexArray indexArray=in.GetIndexArray();
	for(int i=0;i<nPeaks;i++)
	{
		tmpPeak=in.GetSpectralPeak(i);
		tmpPeak.SetMag(tmpPeak.GetMag()+amount);
		out.SetSpectralPeak(i,tmpPeak,indexArray[i]);
	}
	return true;
}

}

