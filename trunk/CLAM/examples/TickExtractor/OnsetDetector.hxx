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

#ifndef __OnsetDetector__
#define __OnsetDetector__

#include "ProcessingComposite.hxx"
#include "Segment.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "TimeIndex.hxx"
#include "GT_FilterBank.hxx"
#include "OnsetDetectorConfig.hxx"
#include <list>
#include <map>
namespace CLAM
{

	/** OnsetDetector class **/
	class OnsetDetector : public ProcessingComposite 
	{
		typedef std::list<TimeIndex>::iterator LI;
		typedef std::map< TIndex, double>::iterator MI;
	public:
	
		OnsetDetector();
		OnsetDetector(OnsetDetectorConfig& c);
		//~OnsetDetector();

		const ProcessingConfig &GetConfig() const {return mConfig;}

		/** Supervised mode execution */
		bool Do(void){return false;}

		/** Unsupervised mode execution */
		bool Do(Segment& originalSegment); //inplace processing only?
		bool Do(Segment& originalSegment, Array<TimeIndex>& out);  //in order to also keep onset weights

		Array<TimeIndex>& GetOnsets() { return mOnsets;}

		const char *GetClassName() const {return "OnsetDetector";}

	private:

		// Configuration data
		OnsetDetectorConfig mConfig;       
		GT_FilterBank	mFilterBank;

		// Internal convenience methods.
		void AttachChildren();
		bool ConfigureChildren();

		// Processing Object compliance methods.

		/* Configuration method */
		bool ConcreteConfigure(const ProcessingConfig&);

	
		bool CalcFilterBankOutput(  );
		void Algorithm(Segment& s, Array<TimeIndex>& out );
		void Smoothing(Array<float>& energy, Array<float> &smoothedEnergy );

		void DetectPosition(Array<float>& in, Array<float> &ret);

		void DetectCandidates( Array<float>& in, Array<float>& weight,
				       TData threshold, std::list<TimeIndex>& candidates );

		void RemoveTooNearOnsetsFromCandidatesList( std::list<TimeIndex>& in );
		void PeakDeletion( LI first, LI last, TimeIndex& newPeak );

		void RemoveTooWeakOnsetsFromCandidatesList( std::list<TimeIndex>& in );
		void PeakSummation( LI first, LI last, TimeIndex& newPeak );

		
		void CheckOffset( Segment &s , Array<TimeIndex>& finalOnsets);
		void CheckOffset( Segment& s, std::list<TimeIndex>& candidates );

		void DecimationForEnvelopeComputation( Array<float>& envelope );
		void ComputeSmoothingFilterCoeffs( Array<float>& coeffs );
		void ExtractAudioEnvelope( Array<float>& envelope );

		//Member Data
		Audio mAudio;
		TSize mnSamples;						//Number of samples in the segment (after down sampling)	
		TSize mnBands;							//Number of bands in the filter bank
		TSize mLowFreq;							//Lowest central frequency in the filter bank
		TSize mHighFreq;						//Highest central frequency in the filter bank
		TData mNoiseThreshold;
		TSize mWinSize;							//Smoothing Filter Size
		TData mBandThreshold;					//Threshold for Band peak picking 
		TData mMinPeakDist;						//Minimum distance between picked peaks
		TData mGlobalThreshold;					
		TData mSampleRate;						//Sample Rate after down sampling
		TData mOffsetThreshold;
		bool mComputeOffsets;
		Array<float> mWinCoef;						//Smoothing Filter Coefficients
		//Rescaling factor
		double        mWinCoefSummation;
		Array< Array<float> > mFilterBankOutputs;
		Array<TimeIndex> mOnsets;
		Array<float>    mRevSmoothedEnergy;
	};


	
}


#endif // OnsetDetector.hxx

