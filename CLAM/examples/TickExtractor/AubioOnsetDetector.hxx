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

#ifndef __AUBIOONSETDETECTOR__
#define __AUBIOONSETDETECTOR__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include <string>
#include <aubio/aubio.h>
#include "Audio.hxx"
#include "Array.hxx"
#include "TimeIndex.hxx"
#include "Enum.hxx"

namespace CLAM
{

	namespace RhythmDescription
	{
		class EODAlgorithms : public Enum
		{
		public:
			static tEnumValue  sEnumValues[];
			static tValue      sDefault;
			
			EODAlgorithms();
			EODAlgorithms( tValue val );
			EODAlgorithms( std::string s );

			typedef enum {
				eEnergy,
				eSpectralDifference,
				eHFC,
				eComplexDomain,
				ePhase
			};
			
			virtual Component* Species() const;
		};

		class AubioOnsetDetectorConfig : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE( AubioOnsetDetectorConfig, 5, ProcessingConfig );
			DYN_ATTRIBUTE( 0, public, TData,         OnsetThreshold );
			DYN_ATTRIBUTE( 1, public, TData,         SilenceThreshold );
			DYN_ATTRIBUTE( 2, public, TSize,         WindowSize );
			DYN_ATTRIBUTE( 3, public, TSize,         HopSize );
			DYN_ATTRIBUTE( 4, public, EODAlgorithms, Method );

		protected:

			void DefaultInit();
		};

		class AubioOnsetDetector : public Processing
		{
		public:

			AubioOnsetDetector();
			~AubioOnsetDetector();

			const char* GetClassName() const;
			const ProcessingConfig& GetConfig() const;
			
			bool Do();
			bool Do( Audio& input, Array<TimeIndex>& onsets );

		protected:

			bool ConcreteConfigure( const ProcessingConfig& cfg );
		private:

			AubioOnsetDetectorConfig mConfig;
			
			// aubio stuff
			// "Processings"
			aubio_pvoc_t*           mPhaseVocoder;
			aubio_onsetdetection_t*  mOnsetDetector;
			pickparams_t*           mPeakPickingParms;

			fvec_t*                     mCurrentFrame;
			fvec_t*                     mInputAudio;
			fvec_t*                     mOnsetBuffer;
			cvec_t*                     mSpectrum;			
			aubio_onsetdetection_type  mOnsetType;
			
		};

	}
       

}

#endif // AubioOnsetDetector.hxx

