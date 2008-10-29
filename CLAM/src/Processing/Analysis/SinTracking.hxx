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

#ifndef _SinTracking__
#define _SinTracking__
#include <typeinfo>

#include "DataTypes.hxx"
#include "Processing.hxx"
#include "SpectralPeakArray.hxx"
#include "Array.hxx"
#include "SinTrackingConfig.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"

namespace CLAM {

	class Fundamental;	
	class SpectralPeak;
	class ProcessingConfig;
	
	/**
	 * Config class for the SinTracking Processing.
	 */
	/* SinTrackingConfig moved to SinTrackingConfig.hxx */

	typedef struct SGuide
	{
		TData freq;
		TData mag;
		int trackId;
		bool isDead;//true if track died in previous step, false else
	}TGuide;


	/**
	 * Processing which does sinusoidal peak tracking (or continuation).
	 * In order for SinTracking to produce meaningful results, it must be called 
	 * repeatively using SpectralPeakArrays generated from the different 'frames' 
	 * of the same input material (ie, if you want to track two different sources, 
	 * you must use two seperate SinTrackings).
	 */
	class SinTracking: public Processing
	{
		mutable SinTrackingConfig mConfig;

		const char *GetClassName() const {return "SinTracking";} 

		/** 
		 * Config change method
		 */
		virtual bool ConcreteConfigure(const ProcessingConfig&);

		InPort<SpectralPeakArray> mInput;
		OutPort<SpectralPeakArray> mOutput;
		FloatInControl mFundFreqValue;

	public:

		SinTracking();
		SinTracking(const SinTrackingConfig &c);
		~SinTracking();

		//Configuration accessor
		const ProcessingConfig &GetConfig() const { return mConfig;};		

		//Peak Continuation for one frame
		bool Do(const SpectralPeakArray& iPeakArray, SpectralPeakArray& oPeakArray);
		bool Do(void);

		int GetnTracks() const {return mNextTrackId;};

	private:

		bool DoHarmonic(const SpectralPeakArray& in, SpectralPeakArray& out,TData funFreq);
		bool DoInharmonic(const SpectralPeakArray& in, SpectralPeakArray& out);
	

		/** Initialization of the first output peak array=addtrack for each peak */
		void Initialization(const SpectralPeakArray& iPeakArray, SpectralPeakArray& oPeakArray);

		void InitHarmonicTracks(SpectralPeakArray& peaks, TData funFreq);

		
		/** Adds a non existing track and assigns pPeakArray as its first peak array 
		* (spectral frame)*/
		void AddNewTrack(int trackPosition, const SpectralPeak& currentPeak,SpectralPeakArray& oPeakArray) const;
		
		/** Continue, kill or create track*/
		void Tracking(const SpectralPeakArray& iPeakArray, SpectralPeakArray& oPeakArray, TIndex processedPeakPos) const;

		void HarmonicTracking(const SpectralPeakArray& in,SpectralPeakArray& out,TData funFreq);

		/** True if peakArray[peakIndex] is already assigned to a track*/
		inline bool IsPeakAssigned(const SpectralPeakArray &peakArray, TIndex peakIndex) const;

		/** True if candidate's distance to fixedPeak is less than threshold*/
		inline bool IsCandidate(const SpectralPeak& fixedPeak,const SpectralPeak& candidate) const;

		/** True if currentFramePeak has a candidate peak in nextFramePeakArray*/
		inline bool ThereIsCandidate(TData currentFramePeakFreq, 
                        const SpectralPeakArray& iPeakArray,SpectralPeakArray& oPeakArray) const;
		
		/** Returns index of candidate and distance to currentFramePeak*/
		inline TIndex GetCandidate(TData currentFramePeakFreq, 
						const SpectralPeakArray& nextFramePeakArray,TData& distance) const;
  
		/** Makes sure that candidate does not match better another peak in currentFramePeakArray*/
		inline bool IsBestCandidate(TData candidateFreq, TData currentFreq) const;

		  
		/** Returns index of first peak not assigned to a track in framePeakArray, beginning
		* at beginAt index*/
		inline TIndex GetFirstNonAssignedPeakPos(const SpectralPeakArray& framePeakArray, TIndex beginAt) const;
  
		/** Assigns trackId to nextPeakArray[peakIndex]*/
		inline void Match(TIndex trackId, TIndex peakIndex,const SpectralPeak& currentPeak, SpectralPeakArray& oPeakArray) const;
  
		/** Kills track identified by trackId and assigns pPeakArray as its last peak array
		* (spectral frame)*/
		inline void KillTrack(int trackPosition) const;

		void KillAll();
  
		
  
		/** Finds newborn tracks in a peak array by finding those peaks not assigned to a
		* track and assigning a new track to them.*/
		void CheckForNewBornTracks(const SpectralPeakArray& iPeakArray,
						SpectralPeakArray& oPeakArray) const;

		/** Returns index of frame2PeakArray peak that is assigned to same track as 
		* frame1PeakArray[peakIndex]*/
		TIndex GetMatchedPeak(const SpectralPeakArray &iPeakArray, TIndex peakIndex) const;
  
		/** Returns index of first peak in frame2PeakArray that is not assigned to a track
		* existing in frame1PeakArray (belongs to a newborn track or belongs to no track).
		* Beginning at beginAt index.*/
		TIndex GetFirstNonMatchedPeakPos(const SpectralPeakArray& iPeakArray, TIndex beginAt) const;
	


	//Member variables
		TData mThreshold;
		mutable bool mInitialized;
		SpectralPeakArray mPreviousPeakArray;
		mutable Array<TGuide> mGuideArray;
		int mnMaxSines;
		mutable int mnActiveGuides;		
		mutable int mNextTrackId;
		bool mHarmonic;//TODO: this should be a runtime modificable control

		bool mLastHarmonic;

	};

};//namespace

#endif

