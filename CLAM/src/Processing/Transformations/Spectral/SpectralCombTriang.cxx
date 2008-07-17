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

#include "SpectralCombTriang.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "SpectralCombTriang",
	//	"category", "Spectral Transformations",
	//	"description", "SpectralCombTriang",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SpectralCombTriang> reg = metadata;
}


bool SpectralCombTriang::Do(const Spectrum& in, Spectrum& out)
{
	if (!mConfig.GetPreserveOuts())
	{
		out = in; //TODO big cludge for streaming
	}
	DataArray& inMag = in.GetMagBuffer();
	DataArray& outMag = out.GetMagBuffer();
	
	int spectrumSize = in.GetSize();
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	int fundamental = Round(mFundamentalCtl.GetLastValue()* spectralResolution);
	
	int i;
	int n;
	n = 0;
	int bandlimit = fundamental;
	//control expected in dB
	TData overallGain = log2lin(mAmount.GetLastValue());
	for(i = 0; i<spectrumSize; i++)
	{
		if(i > bandlimit)
		{
			n++;
			bandlimit = fundamental * n;
		}
		outMag[i] = inMag[i]*GetGain(fundamental, i, n)*overallGain;
	}
	return true;
}

TData SpectralCombTriang::GetGain(int fundamental, int binPos, int bandNum)
{
	//first we find whether the binPos is on a positive or negative slope part
	//auxiliary data
	TData period = fundamental;
	
	//compute mid point
	int midPoint = int (period * 0.5);
	
	bool ascending = false;
	if ((binPos - fundamental*bandNum)> midPoint || bandNum == 0) ascending = true;
	
		
	//compute linear equation y = ax + b
	//first the 'a'
	TData a = 2/fundamental;
	if (!ascending) a = -a;
	//now the 'b'
	TData b = 1;
	if(ascending) b = 0;
	//now the x
	int x;
	if (bandNum==0) x = binPos;
	else if(ascending) x = binPos - int(fundamental*bandNum + midPoint);
	else x = binPos - int(fundamental * bandNum);
	
	//ready to compute the gain
	return a*x + b;
}


}

