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

#include "VowelResonator.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <cmath>

namespace CLAM
{

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, VowelResonator> regVowelResonator("VowelResonator");
}


bool VowelResonator::Do(const SpectralPeakArray& in, SpectralPeakArray& out)
{
	out = in;
	const TSize nPeaks=in.GetnPeaks();
	DataArray & frequencies = out.GetFreqBuffer();
	DataArray & magnitudes = out.GetMagBuffer();
	CLAM_ASSERT(nPeaks==out.GetnPeaks(),"Input and output sizes do not match");
	CLAM_ASSERT(nPeaks==magnitudes.Size(),"Magnitudes is not as long as input size");
	CLAM_ASSERT(nPeaks==frequencies.Size(),"Frequencies is not as long as input size");
	double F1=mF1.GetLastValue();
	double F2=mF2.GetLastValue();
	double F3=2500;
	for(int i=0;i<nPeaks;i++)
	{
		const double & f = frequencies[i];
		const double distance = std::min(std::abs(f-F1),std::abs(F2-f));
		const double distance2 = std::min(std::abs(f-F2),std::abs(F3-f));
		if (f<=F1)
			magnitudes[i] -= 30*log(distance/F1+1);
		if (f>F1 && f<=F2)
			magnitudes[i] -= 30*log(distance/(F2-F1)*2+1);
		if (f>F2 && f<=F3)
			magnitudes[i] -= 30*log(distance2/(F3-F2)*2+1);
		if (f>F3)
			magnitudes[i] -= 30*log(distance2/F3+1);
	}
	return true;

}

}

