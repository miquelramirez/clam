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

#ifndef _SinTrackingConfig_
#define _SinTrackingConfig_
#include <typeinfo>

#include "ProcessingConfig.hxx"

namespace CLAM {

	/**
	 * Config class for the SinTracking Processing
	 */
	class SinTrackingConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SinTrackingConfig, 3,ProcessingConfig);
		/** Frequency deviation in percentage allowed for a peak to be continued (default: 20). */
		DYN_ATTRIBUTE (0, public, TData, Threshold);
		/** Maximum number of sines which can be tracked at once (default: 50). */
		DYN_ATTRIBUTE (1, public, TSize, nMaxSines);
		/**
		* Determines if an algorithm specialized for inharmonic input 
		* is used or one specialized harmonic input is used. Default: false.
		*/
		DYN_ATTRIBUTE (2, public, bool, IsHarmonic);

		void DefaultInit();
		void DefaultValues();
	};
	
} // namespace CLAM

#endif

