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

#include "SinTracksRenderingManager.hxx"
#include "Assert.hxx"

namespace CLAMVM
{
	SinTracksRM::SinTracksRM()
		: mHorClipper(  ), mMustProcessData( false ), mPalette( 0.0f ), mColorsAssigned( false )
	{
		mCachedTracks = NULL;
	}

	SinTracksRM::~SinTracksRM()
	{
	}

	void SinTracksRM::SetWorldBounds( const DataBoundBox& worldBox )
	{
		mDataBBox = worldBox;
		mMustProcessData = true;

	}

	void SinTracksRM::SetPixelBounds( const ScreenBoundBox& wndBox )
	{
		mScreenBBox = wndBox;

		mMustProcessData = true;

	}

	void SinTracksRM::CacheData( SineTrackList& list )
	{
		mCachedTracks = &list;
		mHorClipper.SetTrackList( *mCachedTracks );
		mStarts.clear();
		mEnds.clear();
		mMustProcessData = true;
		mColorsAssigned = false;
	}

	void SinTracksRM::ProcessData()
	{
		DetermineVisibleSamples();

		mRenderer.SetSpanLists( mStarts, mEnds );

		mMustProcessData = false;
	}

	void SinTracksRM::DetermineVisibleSamples()
	{
		mVerClipper.Cull( (int)mDataBBox.mBottom, (int)mDataBBox.mTop, mStarts, mEnds );
		mHorClipper.Cull( (int)mDataBBox.mLeft, (int)mDataBBox.mRight , mStarts, mEnds );

	}

	void SinTracksRM::RenderData()
	{
		if ( !mColorsAssigned )
		{
			Colorize();
			mRenderer.SetPalette( mPalette );
			mColorsAssigned = true;
		}

		if ( mMustProcessData )
			ProcessData();
//				if ( !mStarts.empty() )
		mRenderer.ExecuteGLCommands();
	}

	float SinTracksRM::ClampToRange( TData value )
	{
		if ( value > 0.0 ) // 0 dB is the maximum
			return 1.0f;
		if ( value < -100.0 ) // -100 dB is the minimum
			return 0.0f;
				
		value += 100.0f;
		value*= 0.01f;

		return value;
	}

	void SinTracksRM::Colorize()
	{
		SineTrackList::iterator i = mCachedTracks->begin();
		SineTrackList::iterator end = mCachedTracks->end();

		while ( i != end )
		{
			SinusoidalTrack::iterator itrack = i->begin();
			SinusoidalTrack::iterator endtrack = i->end();

			while( itrack!=endtrack )
			{
				itrack->mColorIndex = mPalette.Get( ClampToRange( itrack->mMag ) );
				itrack++;
			}

			i++;
		}
	}
}
