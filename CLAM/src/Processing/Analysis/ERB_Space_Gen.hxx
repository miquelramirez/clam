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

#ifndef _ERBSpaceGen_
#define _ERBSpaceGen_

#include "DataTypes.hxx"
#include "Processing.hxx"
#include "Array.hxx"
#include "ERB_Space_GenConfig.hxx"

namespace CLAM
{

//CLASS ERB_SpaceGen
//This function computes an array of NumFilter frequencies uniformly spaced between
//highFreq and lowFreq on an ERB scale.  N is set to 100 if not specified.
//
//For a definition of ERB, see Moore, B. C. J., and Glasberg, B. R. (1983).
//"Suggested formulae for calculating auditory-filter bandwidths and
// excitation patterns," J. Acoust. Soc. Am. 74, 750-753.


class ERB_SpaceGen : public Processing
{
public:
	ERB_SpaceGen();
	ERB_SpaceGen(ERB_SpaceGenConfig& c);
	~ERB_SpaceGen();

	const ProcessingConfig &GetConfig() const { return mConfig;}
	
	/** Supervised mode execution **/
	bool Do(void){return false;}

	/**Unsupervised mode execution **/
	bool Do(Array<double>& mERBFreqs);

	const char * GetClassName() const {return "ERB_SpaceGen";}
		
private:
	bool ConcreteConfigure(const ProcessingConfig&  c);

	ERB_SpaceGenConfig    mConfig;

	TSize mNumFilter;
	TSize mLowFreq;
	TSize mHighFreq;

};

} // namespace CLAM

#endif // _ERBSpaceGen_

