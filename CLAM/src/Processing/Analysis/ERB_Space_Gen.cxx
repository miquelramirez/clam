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


//Implemented from Slaney's Auditory Toolbox (http://rvl4.ecn.purdue.edu/~malcolm/interval/1998-010/)


#include "ERB_Space_Gen.hxx"
#include "CLAM_Math.hxx"

namespace CLAM
{

ERB_SpaceGen::ERB_SpaceGen()
{
	Configure(ERB_SpaceGenConfig());
}

ERB_SpaceGen::ERB_SpaceGen(ERB_SpaceGenConfig& c)
{
	Configure(c);
}


ERB_SpaceGen::~ERB_SpaceGen() {}

bool ERB_SpaceGen::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig( mConfig, c );

	mNumFilter=mConfig.GetNumFilter();
	mLowFreq=(TData)mConfig.GetLowFreq();
	mHighFreq=(TData)mConfig.GetHighFreq();
	
	return true;
}

bool ERB_SpaceGen::Do(Array<double>& mERBFreqs)
{
	int i;
	
	//Change the following three parameters if you wish to use a different
	//ERB scale.  Must change in MakeERBCoeffs too.
	double earQ = 9.26449;				// Glasberg and Moore Parameters
	double minBW = 24.7;

	// All of the followFreqing expressions are derived in Apple TR #35, "An
	// Efficient Implementation of the Patterson-Holdsworth Cochlear
	// Filter Bank."  See pages 33-34.
	for( i=0; i<mNumFilter; i++)
	{
		mERBFreqs[i]=-(earQ*minBW) 
			+ std::exp((i+1)*(-std::log(mHighFreq + earQ*minBW) + std::log(mLowFreq + earQ*minBW))/mNumFilter) * (mHighFreq + earQ*minBW);
	}

	return true;
}

} // namespace CLAM

