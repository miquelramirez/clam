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

#include "SMSOddEvenHarmonicRatio.hxx"

using namespace CLAM;


bool SMSOddEvenHarmonicRatio::Do(const SpectralPeakArray& in, SpectralPeakArray& out)
{
	int i;
	DataArray& iMagArray=in.GetMagBuffer();
	DataArray& oMagArray=out.GetMagBuffer();
	TSize nPeaks=in.GetnPeaks();
	TData oddFactor=MIN(1,mAmountCtrl.GetLastValue());
	TData evenFactor=1-oddFactor;
	for(i=0;i<nPeaks-1;i+=2)
	{
		oMagArray[i]=iMagArray[i]*oddFactor;
		oMagArray[i+1]=iMagArray[i+1]*evenFactor;
	}
	return true;
}

bool SMSOddEvenHarmonicRatio::Do(const Frame& in, Frame& out)
{
	return Do(in.GetSpectralPeakArray(),out.GetSpectralPeakArray());
}

bool SMSOddEvenHarmonicRatio::Do(const Segment& in, Segment& out)
{
	int i;
	TSize nFrames=in.GetnFrames();
	for(i=0;i<nFrames;i++)
	{
		UpdateControlValueFromBPF((TData)i/nFrames);
		Do(in.GetFrame(i),out.GetFrame(i));
	}
	return true;
}