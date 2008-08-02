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

#ifndef __TICKEXTRACTORCONFIG__
#define __TICKEXTRACTORCONFIG__

#include <CLAM/DynamicType.hxx>
#include <CLAM/DataTypes.hxx>
#include <CLAM/Enum.hxx>

namespace CLAM
{
	class OnsetDetectMethod : public Enum
	{
	public:
		static tEnumValue   sEnumValues[];
		static tValue       sDefault;

		OnsetDetectMethod();
		OnsetDetectMethod( tValue val );
		OnsetDetectMethod( std::string s );
		
		typedef enum {
			eMTG,
			eQMUL_Energy,
			eQMUL_SpectralDifference,
			eQMUL_HFC,
			eQMUL_ComplexDomain,
			eQMUL_Phase
		} tEnum;

		virtual Component* Species() const;
	};

	class TickExtractorConfig : public CLAM::DynamicType 
	{
		DYNAMIC_TYPE(TickExtractorConfig, 17);

		/** FromAudio, boolean (default true)
		 *	If set to false, transients will be computed in the main, otherwise
		 * they will be computed in the Do function of the class TickFromOnsets.
		 */
		DYN_ATTRIBUTE( 0, public, bool, FromAudio);
		/** ComputeBeats, boolean (default false)
		 * If set to true beat indexes will be computed in addition to
		 * tick indexes, they won't otherwise.
		 */	
		DYN_ATTRIBUTE( 1, public, bool, ComputeBeats);
		/** SaveIOIHistData, boolean (default false)
		 *	The IOI histogram will be saved into an xml file (after downsampling) 
		 *  if set to true.
		 */
		DYN_ATTRIBUTE( 2, public, bool, SaveIOIHistData);
		/** DownsamplingFactor_IOIHist, TData (default 100.0)
		 *	Determines the downsampling factor used when
		 *  storing the IOI histogram.
		 */
		DYN_ATTRIBUTE( 3, public, CLAM::TData, DownsamplingFactor_IOIHist);
		/** Threshold_IOIHistPeaks, TData (default 0.0)
		 *	Percent of the highest IOI histogram peak under peaks
		 *  won't be sought.
		 */
		DYN_ATTRIBUTE( 4, public, CLAM::TData, TempoLimInf); //in BPM
		/** TempoLimInf, TData (default 50)
		 *	Inferior limit for tempo (at the beat level), in BPM.
		 */
		DYN_ATTRIBUTE( 5, public, CLAM::TData, TempoLimSup); //in BPM
		/** TempoLimSup, TData (default 180)
		 *	Superior limit for tempo (at the beat level), in BPM
		 */
		DYN_ATTRIBUTE( 6, public, CLAM::TData, TickLimInf); //in seconds
		/** TickLimInf, TData (default 0.6)
		 *	Inferior limit for Tick, in seconds
		 */
		DYN_ATTRIBUTE( 7, public, CLAM::TData, TickLimSup); //in seconds
		/** TickLimSup, TData (default 0.1)
		 *	Superior limit for Tick, in seconds
		 */
		DYN_ATTRIBUTE( 8, public, CLAM::TData, Threshold_IOIHistPeaks);
		/** DeviationPenalty, TData (default 1)
		 *	Weight given to the first error function (that measures
		 *  deviations between the elements of the candidate grids
		 *  and the elements of the grid to match).
		 *  A higher value biases results towards a smaller tick.
		 *  Use in conjunction with OverSubdivisionPenalty.
		 */
		DYN_ATTRIBUTE( 9, public, CLAM::TData, DeviationPenalty);
		/** OverSubdivisionPenalty, TData (default 5)
		 *  Weight given to the 2d error function (that measures
		 *  the number of unmatched elements).
		 *  A higher value biases results towards a larger tick.
		 *  Use in conjunction with DeviationPenalty.
		 */
		DYN_ATTRIBUTE( 10, public, CLAM::TData, OverSubdivisionPenalty);
		/** GaussianWindowSize, TData (default 0.2);
		 *  Size of the Gaussian window used for IOI histogram
		 *  smoothing.
		 */
		DYN_ATTRIBUTE( 11, public, CLAM::TData, GaussianWindowSize);
		/** Scope, TData (default 0.10)
		 *  Temporal window around tick first estimation in which the 
		 *  refined value will be sought.
		 *  Value in second.
		 *  Used only if AdjustWithOnsets is set to true.
		 */
		DYN_ATTRIBUTE( 12, public, CLAM::TData, Scope);
		/** AdjustWithOnsets, boolean (default true)
		 *  Determines if the second step in the tick extraction process will be performed.
		 *  If set to true, the tick value is refined by a grid matching between the signal
		 *  onsets and candidate tick grids.
		 *  If set to false, the tick is solely determined by inspection of the
		 *  IOI histogram.
		 */
		DYN_ATTRIBUTE( 13, public, bool, AdjustWithOnsets);
		//parameters that define the "tracking" 
		//(i.e. length and hop of succesive inductions, in number of transients)
		/** NTrans, int (default 500)
		 * Maximum number to account in one analysis.
		 * If the audio has more than NTrans, the analysis will be done
		 * in several step, determined by TransHop, yielding thus
		 * several values for the tick and several xml result files.
		 * Can be used for tick tracking.
		 */
		DYN_ATTRIBUTE( 14, public, int, NTrans);
		/** TransHop, int (default 100)
		 * Hop size, in number of transients, in the successive analyses.
		 * Can be used for tick tracking.
		 */
		DYN_ATTRIBUTE( 15, public, int, TransHop);

		DYN_ATTRIBUTE( 16, public, OnsetDetectMethod, OnsetDetection );

	public:
		void DefaultInit();
	};

}

#endif // TickExtractorConfig.hxx

