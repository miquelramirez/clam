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

#include <CLAM/QtPalette.hxx>
#include <CLAM/SineTracksAdapter.hxx>
#include <CLAM/SinTracksPlotController.hxx>

namespace CLAM
{
    namespace VM
    {
		SinTracksPlotController::SinTracksPlotController()
			: mSampleRate(44100.0)
			, mDuration(0.0)
			, mHasData(false)
			, mMustProcessData(false)
			, mNumberOfFrames(0)
		{
			SetMinSpanX(50.0);
			SetMinSpanY(50.0);
		}

		SinTracksPlotController::~SinTracksPlotController()
		{
		}

		void SinTracksPlotController::SetData(const Segment& segment)
		{
			mCachedTracks.clear();
			mHasData = false;
			mNumberOfFrames = segment.GetnFrames();
			SineTracksAdapter adapter;
			mCachedTracks = adapter.GetTrackList(segment);
			mHorClipper.SetTrackList( mCachedTracks );
			mStarts.clear();
			mEnds.clear();
			mSampleRate = double(segment.GetSamplingRate());
			mDuration = double(segment.GetEndTime()-segment.GetBeginTime());
			FullView();
			SetnSamples(mDuration*mSampleRate);
			SetYRange(0.0,mSampleRate/2.0);
			Colorize();
			mHasData = true;
			mMustProcessData = true;
			mRenderer.SaveScreen();
			SetSelPos(0.0,true);
			if(IsRenderingEnabled()) emit requestRefresh();
		}

		void SinTracksPlotController::SetData(const Array< SpectralPeakArray >& peakMtx, double sr, double dur)
		{
			mCachedTracks.clear();
			mHasData = false;
			mNumberOfFrames = peakMtx.Size();
			SineTracksAdapter adapter;
			mCachedTracks = adapter.GetTrackList(peakMtx);
			mHorClipper.SetTrackList( mCachedTracks );
			mStarts.clear();
			mEnds.clear();
			mSampleRate = sr;
			mDuration = dur;
			FullView();
			SetnSamples(mDuration*mSampleRate);
			SetYRange(0.0,mSampleRate/2.0);
			Colorize();
			mHasData = true;
			mMustProcessData = true;
			mRenderer.SaveScreen();
			SetSelPos(0.0,true);
			if(IsRenderingEnabled()) emit requestRefresh();
		}

		void SinTracksPlotController::DisplayDimensions(int w, int h)
		{
			mMustProcessData = true;
			PlotController::DisplayDimensions(w,h);
		
			double lBound = GetLeftBound()/mSampleRate;
			double hBound = GetRightBound()/mSampleRate;
			
			emit xRulerRange(lBound,hBound);

			double bBound = GetBottomBound();
			double tBound = GetTopBound();
			
			emit yRulerRange(bBound,tBound);
		}

		void SinTracksPlotController::Draw()
		{
			if(!mHasData || !IsRenderingEnabled()) return;
			if(mMustProcessData) ProcessData();
			mRenderer.Render();
			PlotController::Draw();
		}

		void SinTracksPlotController::SetHBounds(double left, double right)
		{
			PlotController::SetHBounds(left,right);
			mMustProcessData = true;
			mRenderer.SaveScreen();
			double lBound = GetLeftBound()/mSampleRate;
			double hBound = GetRightBound()/mSampleRate;
			
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
			
			emit xRulerRange(lBound,hBound);
		}

		void SinTracksPlotController::SetVBounds(double bottom, double top)
		{
			PlotController::SetVBounds(bottom,top);
			mMustProcessData = true;
			mRenderer.SaveScreen();
			double bBound = GetBottomBound();
			double tBound = GetTopBound();
		       
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
			emit yRulerRange(bBound,tBound);
		}

		void SinTracksPlotController::FullView()
		{
			TData stp = mDuration*mSampleRate/double(mNumberOfFrames);
			SetMinSpanX(stp*5.0);

			mView.left = 0.0;
			mView.right = mDuration*mSampleRate;
			mView.top = mSampleRate/2.0;
			mView.bottom = 0.0;
			SetHBounds(mView.left,mView.right);
			SetVBounds(mView.bottom,mView.top);
			emit viewChanged(mView);
		}

		void SinTracksPlotController::ProcessData()
		{
			TIndex left = TIndex(GetLeftBound()*double(mNumberOfFrames)/GetnSamples());
			TIndex right = TIndex(GetRightBound()*double(mNumberOfFrames)/GetnSamples());
			TIndex bottom = TIndex(GetBottomBound());
			TIndex top = TIndex(GetTopBound());

			if(right-left < 3)
			{
				right = left + 3;
				if(right >= mNumberOfFrames)
				{
					right = mNumberOfFrames - 1;
					left = right - 3;
				}
			}

			mVerClipper.Cull(TData(bottom), TData(top), mStarts, mEnds );
			mHorClipper.Cull(left, right, mStarts, mEnds );

			mRenderer.SetVBounds(GetBottomBound(),GetTopBound()-2.0);
			mRenderer.SetSpanLists(mStarts,mEnds,TData(left),TData(right)-TData(2.0));

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
					itrack->mColorIndex = palette.Get( ClampToRange( float(itrack->mMag) ) );
					itrack++;
				}
				i++;
			}
			mRenderer.SetPalette(palette);
		}

		float SinTracksPlotController::ClampToRange(float value) const
		{
			if ( value > 0.0 ) // 0 dB is the maximum
				return 1.0f;
			if ( value < -100.0 ) // -100 dB is the minimum
				return 0.0f;
					
			value += 100.0f;
			value*= 0.01f;

			return value;
		}

		void SinTracksPlotController::SetMousePos(double x, double y)
		{
			if(x < 0 || x > GetnSamples()) return;
			if(y < 0 || y > GetMaxSpanY()) return;
			
			PlotController::SetMousePos(x,y);
			double t=GetMouseXPos()/mSampleRate;
			double f=GetMouseYPos();
			emit sendTimeFreq(t,f);
			if(!HasSentTag())
			{
				QString s ="t="+QString::number(t,'f',3)+"s freq="+QString::number(f,'f',0)+"Hz"; 
				emit globalToolTip(s);
			}
		}

		void SinTracksPlotController::SetSelPos(double value, bool render)
		{
			if(CanDrawSelectedPos())
			{
				if(GetDialPos() != value)
				{
					PlotController::SetSelPos(value, render);
					if(mHasData && IsRenderingEnabled()) emit requestRefresh();
					emit selectedXPos(value/mSampleRate);
				}
			}
		}

		void SinTracksPlotController::setHBounds(double xmin, double xmax)
		{
			double left = xmin*mSampleRate;
			double right = xmax*mSampleRate;
			SetHBounds(left,right);
		}

		void SinTracksPlotController::setSelectedXPos(double xpos)
		{
			SetSelPos(xpos*mSampleRate,true);
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
		}

		void SinTracksPlotController::setVBounds(double ymin, double ymax)
		{
			SetVBounds(ymin,ymax);
		}

		void SinTracksPlotController::SaveScreen()
		{
			mRenderer.SaveScreen();
		}
    }
}

// END



