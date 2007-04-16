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

#include "FundamentalAdapter.hxx"
#include "Segment.hxx"
#include "Fundamental.hxx"

namespace CLAMVM
{
	FundamentalAdapter::FundamentalAdapter()
		: mAdaptedObject( NULL )
	{
		mFundFreqTrajectory.SetSize(0);
	}

	FundamentalAdapter::~FundamentalAdapter()
	{
	}

	bool FundamentalAdapter::ExtractTrajectory()
	{
		const CLAM::List<CLAM::Frame>& frames = mAdaptedObject->GetFramesArray();

		// Publish fails because there are no frames in the observed segment
		if ( frames.Size() == 0 )
			return false;

		if ( mFundFreqTrajectory.Size() < frames.Size() )
			mFundFreqTrajectory.Resize( frames.Size() );

		mFundFreqTrajectory.SetSize( frames.Size() );

		for ( int i = 0; i < frames.Size(); i++ )
		{
			// if for some reason some frame is lacking the fundamental the Publish must fail
			// since maybe it has not been yet fully computed, or the Segment object is not
			// consistent
			if ( !frames[i].HasFundamental() )
				return false;

			CLAM::Fundamental& frameFundFreq = frames[i].GetFundamental();

			// No fundamental frequency was detected for this frame
			if ( frameFundFreq.GetnCandidates() == 0 )
				mFundFreqTrajectory[i].mFreq = -1.0;
			else
			{
				// It is supposed that the CandidatesFreq elements are ordered ascendently
				// by likelihood
				mFundFreqTrajectory[i].mFreq = frameFundFreq.GetCandidatesFreq()[0];
				mFundFreqTrajectory[i].mAttribute = frameFundFreq.GetCandidatesErr()[0];
			}
				
		}
		
		return true;
	}

	bool FundamentalAdapter::Publish() 
	{
		if ( Adaptee()==NULL )
			return false;

		if ( !ExtractTrajectory() )
			return false;
		
		if ( !Adaptee()->HasSamplingRate() )
			return false;
		
		if ( !Adaptee()->HasBeginTime() )
			return false;

		if ( !Adaptee()->HasEndTime() )
			return false;

		TrajectoryExtracted.Emit( mFundFreqTrajectory, Adaptee()->GetSamplingRate() );
		TimeSpanChanged.Emit( Adaptee()->GetBeginTime(), Adaptee()->GetEndTime() );
		
		return true;
	}

	bool FundamentalAdapter::BindTo( const CLAM::Segment& adaptee )
	{
		mAdaptedObject = &adaptee;

		return true;
	}
}

