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

#include "QtPalette.hxx"
#include "SineTracksAdapter.hxx"
#include "SinTracksPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		SinTracksPlotController::SinTracksPlotController()
			: mMustProcessData(false)
			, mNumberOfFrames(0)
		{
			SetVMin(50.0);
		}

		SinTracksPlotController::~SinTracksPlotController()
		{
		}

		void SinTracksPlotController::SetData(const Segment& segment)
		{
			mNumberOfFrames = segment.GetnFrames();
			SineTracksAdapter adapter;
			mCachedTracks = adapter.GetTrackList(segment);
			mHorClipper.SetTrackList( mCachedTracks );
			mStarts.clear();
			mEnds.clear();
			mSampleRate = segment.GetSamplingRate();
			mDuration = segment.GetEndTime()-segment.GetBeginTime();
			FullView();
			SetnSamples(TSize(mDuration*mSampleRate));
			SetvRange(mSampleRate/TData(2.0));
			Colorize();
			InitialRegionTime();
			mMustProcessData = true;
			SetSelPos(TData(0.0));
			emit requestRefresh();
		}

		void SinTracksPlotController::SetData(const Array< SpectralPeakArray >& peakMtx, 
											  const TData& sr, const TData& dur)
		{
			mNumberOfFrames = peakMtx.Size();
			SineTracksAdapter adapter;
			mCachedTracks = adapter.GetTrackList(peakMtx);
			mHorClipper.SetTrackList( mCachedTracks );
			mStarts.clear();
			mEnds.clear();
			mSampleRate = sr;
			mDuration = dur;
			FullView();
			SetnSamples(TSize(mDuration*mSampleRate));
			SetvRange(mSampleRate/TData(2.0));
			Colorize();
			InitialRegionTime();
			mMustProcessData = true;
			SetSelPos(TData(0.0));
			emit requestRefresh();
		}

		void SinTracksPlotController::SurfaceDimensions(int w,int h)
		{
			PlotController::SurfaceDimensions(w,h);
		
			double lBound = double(GetLeftBound()/mSampleRate);
			double hBound = double(GetRightBound()/mSampleRate);
			
			emit xRulerRange(lBound,hBound);

			double bBound = double(GetBottomBound());
			double tBound = double(GetTopBound());
			
			emit yRulerRange(bBound,tBound);
		}

		void SinTracksPlotController::Draw()
		{
			if(mMustProcessData) ProcessData();
			mRenderer.Render();
			SelTimeRegionPlotController::Draw();
		}

		void SinTracksPlotController::SetHBounds(const TData& left,const TData& right)
		{
			SelTimeRegionPlotController::SetHBounds(left,right);
			mMustProcessData = true;
			
			double lBound = double(GetLeftBound()/mSampleRate);
			double hBound = double(GetRightBound()/mSampleRate);
			
			emit xRulerRange(lBound,hBound);
		}

		void SinTracksPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			SelTimeRegionPlotController::SetVBounds(bottom,top);
			mMustProcessData = true;
			
			double bBound = double(GetBottomBound());
			double tBound = double(GetTopBound());
		       
			emit yRulerRange(bBound,tBound);
		}

		void SinTracksPlotController::FullView()
		{
			TData stp = mDuration*mSampleRate/TData(mNumberOfFrames);
			SetHMin(stp*TData(5.0));

			mView.left = TData(0.0);
			mView.right = TData(mDuration*mSampleRate);
			mView.top = mSampleRate/TData(2.0);
			mView.bottom = TData(0.0);
			SetHBounds(mView.left,mView.right);
			SetVBounds(mView.bottom,mView.top);
			emit sendView(mView);
		}

		void SinTracksPlotController::ProcessData()
		{
			TIndex left = TIndex(GetLeftBound()*TData(mNumberOfFrames)/TData(GetnSamples()));
			TIndex right = TIndex(GetRightBound()*TData(mNumberOfFrames)/TData(GetnSamples()));
			TIndex bottom = TIndex(GetBottomBound());
			TIndex top = TIndex(GetTopBound());

			mVerClipper.Cull(TData(bottom), TData(top), mStarts, mEnds );
			mHorClipper.Cull(left, right, mStarts, mEnds );

			mRenderer.SetVBounds(GetTopBound(),GetBottomBound());
			mRenderer.SetSpanLists(mStarts, mEnds, TData(left), TData(right));

			mMustProcessData = false;
		}

		void SinTracksPlotController::Colorize()
		{
			QtPalette palette(0.0f);

			SineTrackList::iterator i = mCachedTracks.begin();
			SineTrackList::iterator end = mCachedTracks.end();

			while ( i != end )
			{
				SinusoidalTrack::iterator itrack = i->begin();
				SinusoidalTrack::iterator endtrack = i->end();

				while( itrack!=endtrack )
				{
					itrack->mColorIndex = palette.Get( ClampToRange( itrack->mMag ) );
					itrack++;
				}
				i++;
			}
			mRenderer.SetPalette(palette);
		}

		float SinTracksPlotController::ClampToRange(TData value) const
		{
			if ( value > 0.0 ) // 0 dB is the maximum
				return 1.0f;
			if ( value < -100.0 ) // -100 dB is the minimum
				return 0.0f;
					
			value += 100.0f;
			value*= 0.01f;

			return value;
		}

		void SinTracksPlotController::InitialRegionTime()
		{
			MediaTime time;
			time.SetBegin(TData(0.0));
			emit selectedRegion(time);
		}

		void SinTracksPlotController::SetMousePos(TData x,TData y)
		{
			TData tbound = GetTopBound()-GetBottomBound();
			TData bBound = GetBottomBound();
			TData ycoord=y;
			ycoord *= tbound;
			ycoord /= TData(mViewport.h);
			ycoord += bBound;
			SegmentationMarksPlotController::SetMousePos(x,ycoord);
			if(!HasSentTag())
			{
				TData t=GetMouseXPos()/mSampleRate;
				TData freq=GetMouseYPos();
				QString s ="t="+QString::number(t,'f',3)+"s freq="+QString::number(freq,'f',0)+"Hz";
		      
				emit toolTip(s);
			}
		}

		bool SinTracksPlotController::IsPlayable()
		{
			return false;
		}
    }
}

// END



