/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _FormantTracker_
#define _FormantTracker_

#include "FormantExtractor.hxx"
#include "MDPModeler.hxx"
#include "TrackFinder.hxx"


namespace CLAM
{
	class FormantTrackerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (FormantTrackerConfig,12,ProcessingConfig);
		DYN_ATTRIBUTE(0,public,std::string,Name);
		DYN_ATTRIBUTE(1,public,TData,Fmax);
		DYN_ATTRIBUTE(2,public,TData,Fmin);
		DYN_ATTRIBUTE(3,public,TData,HopSize);
		DYN_ATTRIBUTE(4,public,TSize,NCandidates);
		DYN_ATTRIBUTE(5, public, TData, SamplingRate);
		DYN_ATTRIBUTE(6, public, TData, SemitoneRange);
		DYN_ATTRIBUTE(7, public, TData, AmplitudeRange);
		DYN_ATTRIBUTE(8, public, TSize, NToTrack);
		DYN_ATTRIBUTE(9,public,EWindowType,WindowType);
		DYN_ATTRIBUTE(10,public,TData,MsWinSize);
		DYN_ATTRIBUTE(11,public,TSize,AudioSize);
	private:
		void DefaultInit();	

	};

	class FormantTracker: public ProcessingComposite
	{
	public:

		FormantTracker();

		FormantTracker( FormantTrackerConfig &cfg );

		virtual ~FormantTracker() {}

		const char *GetClassName() const { return "FormantTracker"; }

		const ProcessingConfig &GetConfig() const { return mConfig; }

		/** Supervised mode execution **/
		bool Do(void) { return true; }

		/** Unsupervised mode execution **/
		bool Do( const Audio& in, Array< SpectralPeakArray > &out );

		inline const TSize GetNFrames(void) const { return mNFrames; }

	protected:

	/** Configuration data **/
		FormantTrackerConfig		mConfig;

		FormantExtractor			mFormantExtractor;

		MDPModeler					mMDPModeler;

		TrackFinder					mTrackFinder;

		TSize						mNCandidates;

		TSize						mNFrames;

		TSize						mHopSize;

		TSize						mWinSize;

		TData						mSamplingRate;

		TData						mSemitoneRange;

		TData						mAmplitudeRange;

		TSize						mNToTrack;


		bool ConcreteConfigure( const ProcessingConfig& );

		bool ConcreteStart(void);

		bool ConcreteStop(void);

		void AttachChildren(void);

		bool ConfigureChildren(void);	
	};

} //namespace

#endif // FormantTracker.hxx
