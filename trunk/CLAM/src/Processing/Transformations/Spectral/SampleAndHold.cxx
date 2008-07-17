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

#include "SampleAndHold.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "SampleAndHold",
		"category", "Spectral Transformations",
		"description", "SampleAndHold",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SampleAndHold> reg = metadata;
}

bool SampleAndHold::Do(const Spectrum& in, Spectrum& out)
{
	if ( !mConfig.GetPreserveOuts() )
	{
		out = in; //TODO big cludge for streaming
	}
	
	//first convert from ms to token size
	//mFrameSize should  be set somehow in the configuration! It is the difference between one frame center and the next
	mFrameSize = 1024;
	
	if(mTimeElapsed == 0 ||mAmount.GetLastValue()<1. ) mSpectrum = in;
	
	if (mTimeElapsed>mAmount.GetLastValue()) {
			mTimeElapsed = 0;
		}
	else {
		 mTimeElapsed += mFrameSize / in.GetSpectralRange() / 0.002;
	}
	
	out = mSpectrum;

	return true;
}



}

