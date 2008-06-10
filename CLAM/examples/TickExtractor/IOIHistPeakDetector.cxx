/*
 * Author: fabien gouyon 
 * http://www.iua.upf.es/~fgouyon
 * Description:
 *
 * Syntax: C++
 *
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

#include "IOIHistPeakDetector.hxx"
#include <CLAM/Audio.hxx>
#include <CLAM/CLAM_Math.hxx>
#include <list>

namespace CLAM
{
	namespace RhythmDescription
	{

		void IOIHistPeakDetectorConfig::DefaultInit()
		{
			AddAll();
			UpdateData();
			// MRJ: This is the value achieved by the normal distribution
			// (with mean=0 and stddev=0.15) at exactly stddev
			const TData minPeakMagThreshold = (1.0 / sqrt(2.0 * M_PI * 0.15 * 0.15 ))*std::exp(-0.5);
			SetThreshold(minPeakMagThreshold);
			SetNormalizeWeights(true);
			SetSampleRate( 0 );

		}

		void IOIHistPeakDetectorConfig::Check()
		{
			CLAM_ASSERT( GetThreshold() >= 0.0 && GetThreshold() <= 1.0,
				     "IOIHistPeakDetectorConfig::Check(): Threshold param should be in the [0,1] range" );

			CLAM_ASSERT( GetSampleRate() > 0,
				     "IOIHistPeakDetectorConfig::Check(): SampleRate param was not set!" );

		}


/* Processing  object Method  implementations */

		IOIHistPeakDetector::IOIHistPeakDetector()
		{
		}

		/* Configure the Processing Object according to the Config object */
		bool IOIHistPeakDetector::ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig( mConfig, c );
			mConfig.Check();
			
			return true;
		}
		
		const char* IOIHistPeakDetector::GetClassName() const
		{
			return "IOIHistPeakDetector";
		}

/* The supervised Do() function */
		bool  IOIHistPeakDetector::Do(void) 
		{
			return false;
		}

/* The  unsupervised Do() function */
		bool  IOIHistPeakDetector::Do( IOIHistogram& input, Array<TimeIndex>& out)
		{
			int  size = input.GetBins().Size();
			TData* arr = input.GetBins().GetPtr();


			//int maxpeaks = out.Size(); //mConfig.GetnPeaks();
			TData max=0;
			
			std::list<TimeIndex> detectedPeaks;
			typedef std::list<TimeIndex>::iterator LI;

			TimeIndex newPeak;
			newPeak.SetPosition(0);
			newPeak.SetWeight(0);

			detectedPeaks.push_back( newPeak );

			bool sameSlope=false;
			TData fs = mConfig.GetSampleRate();

			// MRJ: 3 ms is the minimum allowed space between peaks
			const int peaksMinDist = std::max(1,(int)(0.003*fs)); 
			const int twicePeaksMinDist = 2 * peaksMinDist;
			const int maxPeakPos = size - twicePeaksMinDist;

			// MRJ: Actual peak detection loop. Peaks are stored onto
			// a list for O(k) insertion
			
			int i = twicePeaksMinDist;

			while( i < maxPeakPos )
			{
				if ( (arr[i+peaksMinDist]>arr[i])
				     && (!sameSlope) )
					sameSlope=true; 
					
				if ( (sameSlope) 
				     && (arr[i-twicePeaksMinDist] < arr[i-peaksMinDist]) 
				     && (arr[i-peaksMinDist] < arr[i]) 
				     && (arr[i] >  arr[i+peaksMinDist]) 
				     && (arr[i+peaksMinDist] > arr[i+twicePeaksMinDist]) )
				{
					sameSlope=false;
					
					newPeak.SetPosition( i );
					newPeak.SetWeight( arr[i] );
					
					detectedPeaks.push_back( newPeak );
					
					if (arr[i] > max) 
						max=arr[i];

					// MRJ: If once we find a peak we advance the
					// index by peaksMinDist, later there won't be the
					// need to check wether peaks are inside this
					// "minimum peak distance"
					i+=peaksMinDist;
				}
				else
					i++;
			}



			const TData invMax = 1.0 / (double)max;
			const TData minPeakMagThreshold = mConfig.GetThreshold();
			const bool mustNormalize = mConfig.GetNormalizeWeights();


			// MRJ: Unlikely peaks removal and weight normalization

			LI prev = detectedPeaks.begin();
			LI it = detectedPeaks.begin();
			it++;
			
			for ( ; it != detectedPeaks.end();
			      it++, prev++ )
			{
				const TData tmpWeight = it->GetWeight();
				const TData tmpPosition = it->GetPosition();
				const TData normWeight = tmpWeight * invMax;
				
				if ( ( tmpWeight > minPeakMagThreshold) )
				{
					if ( mustNormalize )
						it->SetWeight( normWeight );
				}
				else
					it = detectedPeaks.erase( it );
			}

			out.Resize( detectedPeaks.size() );
			out.SetSize( detectedPeaks.size() );

			int j = 0;

			for ( LI k = detectedPeaks.begin();
			      k != detectedPeaks.end(); k++, j++ )
				out[j] = *k;
			
			return true;
		}

	} // namespace RhythmDescription
} // namespace CLAM

