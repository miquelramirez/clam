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
		mHorClipper.Cull( mDataBBox.mLeft, mDataBBox.mRight , mStarts, mEnds );
		mVerClipper.Cull( mDataBBox.mBottom, mDataBBox.mTop, mStarts, mEnds );
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
