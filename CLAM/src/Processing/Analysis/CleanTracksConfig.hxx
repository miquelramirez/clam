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

#ifndef _CleanTracksConfig_
#define _CleanTracksConfig_

#include "DataTypes.hxx"
#include "ProcessingConfig.hxx"

namespace CLAM {

	/**
	 * Config class for the CleanTracks Processing
	 */
	class CleanTracksConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE(CleanTracksConfig, 5,ProcessingConfig);
		/** Maximum tolerance (in frames) to peak discontinuation */
		DYN_ATTRIBUTE (0,public,TSize,MaxDropOut);
		/** The minimum lenght a track should have to be keeped */
		DYN_ATTRIBUTE (1,public,TSize,MinLength);
		/** Maximum frequency distance between two tracks to be considered one
		as continuation of the other, expressed as a percentage between 0 and 100 */
		DYN_ATTRIBUTE (2,public,TData,FreqDev);
		/** This attribute is necessary so that BinPosition can be recomputed*/
		DYN_ATTRIBUTE (3, public, TData, SamplingRate);
		/** This attribute is necessary so that BinPosition can be recomputed*/
		DYN_ATTRIBUTE (4, public, int, SpecSize);

		void DefaultInit();
		~CleanTracksConfig(){};
	};

} // namespace CLAM

#endif

