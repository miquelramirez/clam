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

#ifndef __TICKSEQUENCETRACKERCONFIG__
#define __TICKSEQUENCETRACKERCONFIG__

#include "ProcessingConfig.hxx"
#include "DataTypes.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{

		class TickSequenceTrackerConfig : public ProcessingConfig
		{
		public:
			DYNAMIC_TYPE_USING_INTERFACE (TickSequenceTrackerConfig, 21,ProcessingConfig);
			DYN_ATTRIBUTE ( 0, public, TData, SampleRate);

			//Tick and beat computation attributes
			DYN_ATTRIBUTE ( 1, public, bool, ComputeBeats);
			DYN_ATTRIBUTE ( 2, public, TData, Threshold_IOIHistPeaks);
			DYN_ATTRIBUTE ( 3, public, TData, TempoLimInf); //in BPM
			DYN_ATTRIBUTE ( 4, public, TData, TempoLimSup); //in BPM
			DYN_ATTRIBUTE ( 5, public, TData, TickLimInf); //in seconds
			DYN_ATTRIBUTE ( 6, public, TData, TickLimSup); //in seconds
			DYN_ATTRIBUTE ( 7, public, TData, DeviationPenalty);
			DYN_ATTRIBUTE ( 8, public, TData, OverSubdivisionPenalty);
			DYN_ATTRIBUTE ( 9, public, TData, GaussianWindowSize);
			DYN_ATTRIBUTE ( 10, public, TData, Scope);		
			DYN_ATTRIBUTE ( 11, public, int, NTrans);
			DYN_ATTRIBUTE ( 12, public, int, TransHop);

			//Adjust tick to quarter-note for swing estimation matters
			DYN_ATTRIBUTE (13, public, bool, TickAdjustForSwing);

			//Adjust the tick and the beat to the onset list
			//Set to false just when interested by the beat rate and not the beats
			DYN_ATTRIBUTE (14, public, bool, AdjustWithOnsets);

//XA_C2S Added these attributes to make it compile!!!
			DYN_ATTRIBUTE(15,public, int, AdaptNTrans);
			DYN_ATTRIBUTE(16,public, TData, GapMin_Transients);
			DYN_ATTRIBUTE(17,public, int, NPreviousFrames_Transients);
			DYN_ATTRIBUTE(18,public, TData, Percentil_Transients);
			DYN_ATTRIBUTE(19,public, TData, Threshold_Transients);
			DYN_ATTRIBUTE(20,public, TData, AnalysisFrameSize_Transients);
		protected:
			void DefaultInit();
		};

	}

}


#endif // TickSequenceTrackerConfig.hxx

