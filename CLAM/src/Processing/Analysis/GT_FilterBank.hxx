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

#ifndef _GTFilterbank_
#define _GTFilterbank_

#include "Processing.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "GT_FilterBankConfig.hxx"

namespace CLAM
{
	
	class Audio;


	class GT_FilterBank: public Processing
	{
	private:

		GT_FilterBankConfig mConfig;		
		bool ConcreteConfigure(const ProcessingConfig& cfg);

		TData mLowFreq;
		TData mHighFreq;
		int mnChannels;
		int mSampleRate;
		Array<double> mCentreFreq;
		double A0, A2, B0;
		Array<double> B1, B2, A11, A12, A13, A14, gain;

	public:

		GT_FilterBank();
		GT_FilterBank(GT_FilterBankConfig& c);
		~GT_FilterBank();

		const char * GetClassName() const {return "GT_FilterBank";}

		bool Do(void);

		bool Do(Audio &in, Array< Array<float> >& filterBankOut);
	
		void MakeERBFilters();
	
		const ProcessingConfig &GetConfig() const { return mConfig;}

	};
	
}

#endif

