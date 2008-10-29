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

#ifndef __GLOBALPULSERATEESTIMATOR__
#define __GLOBALPULSERATEESTIMATOR__

#include <CLAM/ProcessingComposite.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include "RD_GlobalPulse.hxx"
#include "IOIHistPeakDetector.hxx"
#include "IOIHistogram.hxx"
#include <CLAM/DataTypes.hxx>
#include "Pulse.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{
		class GlobalPulseRateEstimatorConfig : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE( GlobalPulseRateEstimatorConfig, 3, ProcessingConfig );
			DYN_ATTRIBUTE( 0, public, TData, SampleRate );
			DYN_ATTRIBUTE( 1, public, TData, RateLowerBound );
			DYN_ATTRIBUTE( 2, public, TData, GaussianSize );

		protected:

			void DefaultInit();
		};

		class GlobalPulseRateEstimator : public ProcessingComposite
		{
		public:
			GlobalPulseRateEstimator();

			const char* GetClassName() const;
			
			const ProcessingConfig& GetConfig() const;

			bool Do();
			bool Do( const Array<TData>& pulses,
				 Pulse& pulseRate );
		protected:

			bool ConcreteConfigure( const ProcessingConfig& cfg );
			
		private:

			GlobalPulseRateEstimatorConfig mConfig;

			GlobalPulseGenerator           mPulseGenerator;
			IOIHistPeakDetector            mPeakDetector;

			IOIHistogram                   mPulseHist;
			Array<TimeIndex>               mPulseHistPeaks;

			FloatInControl                      mRateLowerBound;
			
		};
	}
}

#endif // GlobalPulseRateEstimator.hxx

