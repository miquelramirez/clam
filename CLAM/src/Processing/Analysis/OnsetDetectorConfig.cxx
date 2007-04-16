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

#include "OnsetDetectorConfig.hxx"

namespace CLAM
{
	/////////////////////////////////////////////////////////////////////////////////
	void OnsetDetectorConfig::DefaultInit()
	{
		AddName();
		AddnBands();
		AddLowFreq();
		AddHighFreq();
		AddSmoothFiltSize();
		AddBandThreshold();
		AddMinPeakDist();
		AddGlobalThreshold();
		AddNoiseThreshold();
		AddComputeOffsets();
		AddOffsetThreshold();
		UpdateData();

		/* Set default values */
		SetnBands(20);
		SetLowFreq(100);
		SetHighFreq(10000);
		SetSmoothFiltSize(10);		//in samples (40ms at 245 Hz, the sampling rate used in the algorithm)
		SetBandThreshold(0.25);
		SetMinPeakDist(18);//12);			//in samples (50ms at 245 Hz, the sampling rate used in the algorithm)
		SetGlobalThreshold(25);
		SetNoiseThreshold(0.01);
		SetComputeOffsets(true);
		SetOffsetThreshold(0.08);

	}

}

