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
			: _nFrames(0)
		{
			_mustProcessData = false;
			SetVMin(50.0);
		}

		SinTracksPlotController::~SinTracksPlotController()
		{
		}

		void SinTracksPlotController::SetData(const Segment& segment)
		{
			_nFrames = segment.GetnFrames();
			SineTracksAdapter _adapter;
			_cachedTracks = _adapter.GetTrackList(segment);
			_horClipper.SetTrackList( _cachedTracks );
			_starts.clear();
			_ends.clear();
			_sampleRate = segment.GetSamplingRate();
			_duration = segment.GetEndTime()-segment.GetBeginTime();
			FullView();
			SetnSamples(TSize(_duration*_sampleRate));
			SetvRange(_sampleRate/TData(2.0));
			Colorize();
			InitialRegionTime();
			_mustProcessData = true;
			emit requestRefresh();
		}

		void SinTracksPlotController::SetData(const Array< SpectralPeakArray >& peakMtx, 
												const TData& sr, const TData& dur)
		{
			_nFrames = peakMtx.Size();
			SineTracksAdapter _adapter;
			_cachedTracks = _adapter.GetTrackList(peakMtx);
			_horClipper.SetTrackList( _cachedTracks );
			_starts.clear();
			_ends.clear();
			_sampleRate = sr;
			_duration = dur;
			FullView();
			SetnSamples(TSize(_duration*_sampleRate));
			SetvRange(_sampleRate/TData(2.0));
			Colorize();
			InitialRegionTime();
			_mustProcessData = true;
			emit requestRefresh();
		}

		void SinTracksPlotController::SurfaceDimensions(int w,int h)
		{
			PlotController::SurfaceDimensions(w,h);
		
			double lBound = double(GetLeftBound()/_sampleRate);
		    double hBound = double(GetRightBound()/_sampleRate);
			QwtScaleDiv div;
			div.rebuild(lBound,hBound,GetXMaxMajStep(w),3,false);
			emit xRulerScaleDiv(div);

			double bBound = double(GetBottomBound());
			double tBound = double(GetTopBound());
			div.rebuild(bBound,tBound,GetYMaxMajStep(h),3,false);
			emit yRulerScaleDiv(div);
		}

		void SinTracksPlotController::Draw()
		{
			if(_mustProcessData) ProcessData();
			_renderer.Render();
			SelTimeRegionPlotController::Draw();
		}

		void SinTracksPlotController::SetHBounds(const TData& left,const TData& right)
		{
			SelTimeRegionPlotController::SetHBounds(left,right);
			_mustProcessData = true;
			
			double lBound = double(GetLeftBound()/_sampleRate);
		    double hBound = double(GetRightBound()/_sampleRate);
			QwtScaleDiv div;
			div.rebuild(lBound,hBound,GetXMaxMajStep(_viewport.w),3,false);
			emit xRulerScaleDiv(div);
		}

		void SinTracksPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			SelTimeRegionPlotController::SetVBounds(bottom,top);
			_mustProcessData = true;
			
			double bBound = double(GetBottomBound());
			double tBound = double(GetTopBound());
			QwtScaleDiv div;
			div.rebuild(bBound,tBound,GetYMaxMajStep(_viewport.h),3,false);
			emit yRulerScaleDiv(div);
		}

		void SinTracksPlotController::FullView()
		{
			TData stp = _duration*_sampleRate/TData(_nFrames);
			SetHMin(stp*TData(5.0));

			_view.left = TData(0.0);
			_view.right = TData(_duration*_sampleRate);
			_view.top = _sampleRate/TData(2.0);
			_view.bottom = TData(0.0);
			SetHBounds(_view.left,_view.right);
			SetVBounds(_view.bottom,_view.top);
			emit sendView(_view);
		}

		int SinTracksPlotController::GetXMaxMajStep(int ref) const
		{
			if(ref > 1024) ref = 1024;
			int step = ref*40/1024;
			if(step < 4 || _view.right <= _sampleRate/TData(100.0)*TData(2.0)) step = 4;
			return step;
		}

		int SinTracksPlotController::GetYMaxMajStep(int ref) const
		{
			if(ref > 750) ref = 750;
			int step = ref*35/750;
			if(step < 4) step = 4;
			return step;
		}

		void SinTracksPlotController::ProcessData()
		{
			TIndex left = TIndex(GetLeftBound()*TData(_nFrames)/TData(GetnSamples()));
			TIndex right = TIndex(GetRightBound()*TData(_nFrames)/TData(GetnSamples()));
			TIndex bottom = TIndex(GetBottomBound());
			TIndex top = TIndex(GetTopBound());

			_verClipper.Cull(TData(bottom), TData(top), _starts, _ends );
			_horClipper.Cull(left, right, _starts, _ends );

			_renderer.SetVBounds(GetTopBound(),GetBottomBound());
			_renderer.SetSpanLists(_starts, _ends, TData(left), TData(right));

			_mustProcessData = false;
		}

		void SinTracksPlotController::Colorize()
		{
			QtPalette palette(0.0f);

			SineTrackList::iterator i = _cachedTracks.begin();
			SineTrackList::iterator end = _cachedTracks.end();

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
			_renderer.SetPalette(palette);
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
			ycoord /= TData(_viewport.h);
			ycoord += bBound;
			PlotController::SetMousePos(x,ycoord);
			TData t=GetMouseXPos()/_sampleRate;
			TData freq=GetMouseYPos();
			QString s;
			s = "t="+(s.setNum(t,'f',3))+"s freq="+(s.setNum(freq,'f',3))+"Hz";
			emit toolTip(s);
		}
	}
}

// END



