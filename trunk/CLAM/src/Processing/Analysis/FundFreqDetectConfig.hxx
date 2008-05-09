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

#include "ProcessingConfig.hxx"
#include <typeinfo>

#ifndef _FundFreqDetectConfig__
#define _FundFreqDetectConfig__

#include "DataTypes.hxx"

namespace CLAM {

	/**
	 * Config class for FundFreqDetect Processing.
	 */
	class FundFreqDetectConfig: public ProcessingConfig
	{
	public:
	  DYNAMIC_TYPE_USING_INTERFACE (FundFreqDetectConfig, 17,ProcessingConfig);
	  /** */
	  DYN_ATTRIBUTE (0,public,TData, ReferenceFundFreq);
	  /** Lowest frequency allowed for the fundamental. */
	  DYN_ATTRIBUTE (1,public,TData, LowestFundFreq); 
	  /** Highest frequency allowed for the fundamental. */
	  DYN_ATTRIBUTE (2,public,TData, HighestFundFreq); 
	  /** Maximum magnitude difference allowed between candidates (default: 30dB). */
	  DYN_ATTRIBUTE (3,public,TData, MaxCandMagDiff); 
	  /** Maximun allowed Two-Way Mismatch (TWM) error (default: 10). */
	  DYN_ATTRIBUTE (4,public,TData, MaxFundFreqError);
	  /** Parameter to weight candidates (Harmonic Error Constants). Default: 0.5. 
		* Note, if the CLAM_OPTIMIZE is active this parameter is not configurable and
		* is always taken to be the default 0.5 */
	  DYN_ATTRIBUTE (5,public,TData, PMp); 
	  /** Parameter to weight candidates (Harmonic Error Constants). Default: 1.4. */
	  DYN_ATTRIBUTE (6,public,TData, PMq );
	  /** Parameter to weight candidates (Harmonic Error Constants). Default: 0.5. */
	  DYN_ATTRIBUTE (7,public,TData, PMr );
	  /** Parameter to weight candidates (Harmonic Error Constants). Default: 0.5. 
		* Note, if the CLAM_OPTIMIZE is active this parameter is not configurable and
		* is always taken to be the default 0.5 */
	  DYN_ATTRIBUTE (8,public,TData, MPp );
	  /** Parameter to weight candidates (Harmonic Error Constants). Default: 1.4. */
	  DYN_ATTRIBUTE (9,public,TData, MPq );
	  /** Parameter to weight candidates (Harmonic Error Constants). Default: 0.5. */
	  DYN_ATTRIBUTE (10,public,TData, MPr);
	  /** Harmonic peaks to weight error (default: 10). */
	  DYN_ATTRIBUTE (11,public,TSize, PMnPeaks); 
	  /** Harmonic peaks to weight error (default: 10). */
	  DYN_ATTRIBUTE (12,public,TSize, MPnPeaks);
	  /** Harmonic error contributions (default: 1). */
	  DYN_ATTRIBUTE (13,public,TData, PMCont );
	  /** Harmonic error contributions (default: 1). */
	  DYN_ATTRIBUTE (14,public,TData, MPCont );
	  /** Number of integer frequency ratios considered as candidates (default: 5). */
	  DYN_ATTRIBUTE (15,public,TSize, NInt);
	  /** Maximum number of candidates. */
	  DYN_ATTRIBUTE (16,public,TSize, NMaxCandidates); 
	  void DefaultInit();
	  void DefaultValues();
	};
}

#endif

