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

#include <algorithm>
#include "SpecTypeFlags.hxx"
#include "SpectrumPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		SpectrumPlotController::SpectrumPlotController()
		{
			_spectralRange = TData(0.0);
			_mustProcessData = false;
			SetVMin(TData(5.0));
		}
		
		SpectrumPlotController::~SpectrumPlotController()
		{
		}

		void SpectrumPlotController::SetData(const Spectrum& spec)
		{
			_spec = spec;
			CacheData();
			_spectralRange = _spec.GetSpectralRange();
			SetnSamples(_magBuffer.Size());
			SetvRange(TData(150.0));
			FullView();
			_mustProcessData = true;
			emit requestRefresh();
			emit freq(TData(0.0));
			emit mag(_magBuffer[0]);
		}

		void SpectrumPlotController::SetDataColor(Color c)
		{
			_renderer.SetColor(c);
		}

		void SpectrumPlotController::SetHBounds(const TData& left,const TData& right)
		{
			SelPosPlotController::SetHBounds(left,right);
			_mustProcessData = true;
			
			double lBound = double(GetLeftBound()*_spectralRange/TData(GetnSamples()));
		    double hBound = double(GetRightBound()*_spectralRange/TData(GetnSamples()));
			QwtScaleDiv div;
			div.rebuild(lBound,hBound,GetXMaxMajStep(_viewport.w),3,false);
			emit xRulerScaleDiv(div);
		}

		void SpectrumPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			SelPosPlotController::SetVBounds(bottom,top);
			
			double bBound = double(GetBottomBound()-GetvRange());
		    double tBound = double(GetTopBound()-GetvRange());
			QwtScaleDiv div;
			div.rebuild(bBound,tBound,GetXMaxMajStep(_viewport.h),3,false);
			emit yRulerScaleDiv(div);
		}

		void SpectrumPlotController::SurfaceDimensions(int w,int h)
		{
			PlotController::SurfaceDimensions(w,h);
		
			double lBound = double(GetLeftBound()*_spectralRange/TData(GetnSamples()));
		    double hBound = double(GetRightBound()*_spectralRange/TData(GetnSamples()));
			QwtScaleDiv div;
			div.rebuild(lBound,hBound,GetXMaxMajStep(w),3,false);
			emit xRulerScaleDiv(div);
				
			double bBound = double(GetBottomBound()-GetvRange());
			double tBound = double(GetTopBound()-GetvRange());
			div.rebuild(bBound,tBound,GetYMaxMajStep(h),3,false);
			emit yRulerScaleDiv(div);
		}

		void SpectrumPlotController::Draw()
		{
			if(_mustProcessData) ProcessData();
			_renderer.Render();
			SelPosPlotController::Draw();
		}

		void SpectrumPlotController::AdaptSpectralData()
		{
			SpecTypeFlags spFlags;
			_spec.GetType(spFlags);

			if(!spFlags.bMagPhase)
			{
				// check for conversions
				if(spFlags.bComplex)
				{
					_spec.Complex2MagPhase();
				}
				else if(spFlags.bPolar)
				{
					_spec.Polar2MagPhase();
				}
				else if(spFlags.bMagPhaseBPF)
				{
					_spec.BPF2MagPhase();
				}
			}
			// convert to dB
			_spec.ToDB();
		}

		void SpectrumPlotController::CacheData()
		{
			AdaptSpectralData();

			_magBuffer=_spec.GetMagBuffer();
			_phaseBuffer=_spec.GetPhaseBuffer();
		}

		void SpectrumPlotController::FullView()
		{
			_view.left = TData(0.0);
			_view.right = TData(_magBuffer.Size());
			_view.top = TData(150.0);
			_view.bottom = TData(0.0);
			SetHBounds(_view.left,_view.right);
			SetVBounds(_view.bottom,_view.top);
			emit sendView(_view);
		}

		void SpectrumPlotController::ProcessData()
		{
			TSize offset = TSize(GetLeftBound());
			TSize len = TSize(GetRightBound() - GetLeftBound())+1;

			if(_processedData.Size() <= len)
				_processedData.Resize(len);
			_processedData.SetSize(len);

			std::copy(_magBuffer.GetPtr()+offset,_magBuffer.GetPtr()+offset+len,_processedData.GetPtr());

			for(int i = 0;i < _processedData.Size();i++) _processedData[i] += TData(150.0);

			TData range = GetRightBound()-GetLeftBound();
			TData threshold = GetHMin()*TData(2.0);
			int mode = (range < threshold) ? DetailMode : NormalMode;
			_renderer.SetDataPtr(_processedData.GetPtr(),_processedData.Size(),mode);	
			_mustProcessData = false;
		}

		int SpectrumPlotController::GetXMaxMajStep(int ref) const
		{
			if(ref > 1024) ref = 1024;
			int step = ref*20/1024;
			if(step < 4) step = 4;
			return step;
		}

		int SpectrumPlotController::GetYMaxMajStep(int ref) const
		{
			if(ref > 750) ref = 750;
			int step = ref*20/750;
			if(step < 4) step = 4;
			return step;
		}

		void SpectrumPlotController::SetSelPos(const TData& value)
		{
			SelPosPlotController::SetSelPos(value);

			int index = int(value);
			emit mag(_magBuffer[index]);
			emit freq(value*_spectralRange/TData(GetnSamples()));
		}

		TData SpectrumPlotController::GetSpectralRange() const
		{
			return _spectralRange;
		}

		bool SpectrumPlotController::MustProcessData() const
		{
			return _mustProcessData;
		}
        
		void SpectrumPlotController::SetMousePos(TData x,TData y)
		{
			TData xcoord=x;
			xcoord *= _spectralRange;
			xcoord /= TData(GetnSamples());
			TData tbound = GetTopBound()-GetBottomBound();
			TData bBound = GetBottomBound()-TData(150.0);
			TData ycoord=y;
			ycoord *= tbound;
			ycoord /= TData(_viewport.h);
			ycoord += bBound;
			PlotController::SetMousePos(xcoord,ycoord);
			TData freq=GetMouseXPos();
			TData dB=GetMouseYPos();
			QString s;
			s = "freq="+(s.setNum(freq,'f',3))+"Hz amp="+(s.setNum(dB,'f',3))+"dB";
			emit toolTip(s);
		}
		
	}
}

// END

