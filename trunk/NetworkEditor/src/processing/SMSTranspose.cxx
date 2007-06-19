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

#include "SMSTranspose.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <cmath>

namespace CLAM
{
namespace detail
{
	static FactoryRegistrator<ProcessingFactory, SMSTranspose> regSMSTranspose("SMSTranspose");
}

bool SMSTranspose::Do(const SpectralPeakArray& in, SpectralPeakArray& out)
{
	static const TData toneFactor = std::pow(2.,1./12.);

	//TODO big cludge for streaming
	if ( !mConfig.GetPreserveOuts() ) out = in;

	DataArray& ifreqArray=in.GetFreqBuffer();
	DataArray& ofreqArray=out.GetFreqBuffer();
	TSize nPeaks=in.GetnPeaks();
	TData amount=mAmount.GetLastValue();
	for(int i=0;i<nPeaks;i++)
		ofreqArray[i]=ifreqArray[i]*std::pow(toneFactor,amount);
	return true;
}


}

