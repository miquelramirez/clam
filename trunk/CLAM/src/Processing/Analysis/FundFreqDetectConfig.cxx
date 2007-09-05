/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "FundFreqDetectConfig.hxx"

namespace CLAM
{
	/* The Configuration object has at least to have a name */

	void FundFreqDetectConfig::DefaultInit()
	{
		AddAll();
		/* All Attributes are added */
		UpdateData();
		/* Set default values */
		DefaultValues();
	}
	
	void FundFreqDetectConfig::DefaultValues()
	{		
		SetReferenceFundFreq(1000);
		SetLowestFundFreq(35);
		SetHighestFundFreq(1500); // was 2000
		SetMaxCandMagDiff(30);
		SetMaxFundFreqError(10);//was 10, just testing 
		SetNInt(5);
		SetPMp(0.5);
		SetPMq(TData(1.4));
		SetPMr(TData(0.5));
		SetMPp(0.5);
		SetMPq(TData(1.4));
		SetMPr(0.5);
		SetPMnPeaks(10);
		SetMPnPeaks(10);
		SetNMaxCandidates(40); 
		SetPMCont(1.0);
		SetMPCont(1.0);
	}


}

