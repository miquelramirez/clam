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

#include "PlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		PlotController::PlotController()
		{
			_lBound = TData(0.0);
			_rBound = TData(1.0);
			_bBound = TData(0.0);
			_tBound = TData(1.0);

			_selPos = TData(0.0);
			_hMin = TData(50.0);
			_vMin = TData(50.0);
		    _nSamples = 0; 
			_vRange = TData(0.0);
			_current = _nSamples;
			_vcur = _vRange;

			_hzRatio = 1;
			_vzRatio = 1;

			_isLeftButtonPressed=false;
			_isAbleToEdit=false;
			
			InitView();
		}
	
		PlotController::~PlotController()
		{
		}

		void PlotController::SetHBounds(const TData& left, const TData& right)
		{
			_lBound = left;
			_rBound = right;
		}

		void PlotController::SetVBounds(const TData& bottom, const TData& top)
		{
			_bBound = bottom;
			_tBound = top;
		}

		TData PlotController::GetLeftBound() const
		{
			return _lBound;
		}

		TData PlotController::GetRightBound() const
		{
			return _rBound;
		}

		TData PlotController::GetBottomBound() const
		{
			return _bBound;
		}

		TData PlotController::GetTopBound() const
		{
			return _tBound;
		}

		void PlotController::SetSelPos(const TData& value)
		{
			_selPos = value;
		}

		TData PlotController::GetSelPos() const
		{
			return _selPos;
		}

		void PlotController::SetnSamples(const TSize& n)
		{
			_nSamples = n;
			_current = TData(_nSamples);
			InitHRatio();
			emit hZoomRatio(_hzRatio);
			int hsv=GetHScrollValue();
			emit hScrollMaxValue(GetnxPixels());
			emit hScrollValue(hsv);
		}

		TSize PlotController::GetnSamples() const
		{
			return _nSamples;
		}

		void PlotController::SetvRange(const TData& vr)
		{
			_vRange = vr;
			_vcur = _vRange;
			InitVRatio();
			emit vZoomRatio(_vzRatio);
			int vsv=GetVScrollValue();
			emit vScrollMaxValue(GetnyPixels());
			emit vScrollValue(vsv);
		}

		TData PlotController::GetvRange() const
		{
			return _vRange;
		}

		void PlotController::HZoomIn()
		{
			if(_current/2.0f > GetHMin())
			{
				_current /= 2.0f;
				_view.right = _current;
				UpdateHBounds(true);
				emit sendView(_view);
				TData left = GetLeftBound();
				TData right = GetRightBound();
				_hzRatio /= 2;
				emit hZoomRatio(_hzRatio);
				emit hScrollMaxValue(GetnxPixels());
				emit hScrollValue(GetHScrollValue());
				SetHBounds(left,right);
				emit requestRefresh();
			}
		}

		void PlotController::HZoomOut()
		{
		    if(_current*2.0f <= GetnSamples())
			{
				_current *= 2.0f;
				_view.right = _current;
				UpdateHBounds();
				emit sendView(_view);
				TData left = GetLeftBound();
				TData right = GetRightBound();
				_hzRatio *= 2;
				emit hZoomRatio(_hzRatio);
				emit hScrollMaxValue(GetnxPixels());
				SetHBounds(left,right);
				emit hScrollValue(GetHScrollValue());
				emit requestRefresh();
			}
		}

		void PlotController::VZoomIn()
		{
			if(_vcur/2.0f > GetVMin())
			{
				_vcur /= 2.0f;
				_view.top = _vcur;
				UpdateVBounds(true);
				emit sendView(_view);
				TData bottom = GetBottomBound();
				TData top = GetTopBound();
				_vzRatio /= 2;
				emit vZoomRatio(_vzRatio);
				emit vScrollMaxValue(GetnyPixels());
				emit vScrollValue(GetVScrollValue());
				SetVBounds(bottom,top);
				emit requestRefresh();
			}
		}

		void PlotController::VZoomOut()
		{
			if(_vcur*2.0f <= GetvRange())
			{
				_vcur *= 2.0f;
				_view.top = _vcur;
				UpdateVBounds();
				emit sendView(_view);
				TData bottom = GetBottomBound();
				TData top = GetTopBound();
				_vzRatio *= 2;
				emit vZoomRatio(_vzRatio);
				emit vScrollMaxValue(GetnyPixels());
				SetVBounds(bottom,top);
				emit vScrollValue(GetVScrollValue());
				emit requestRefresh();
			}
		void EnterMouse();}

		void PlotController::SetHMin(const TData& min)
		{
			_hMin = min;
		}

		TData PlotController::GetHMin() const
		{
			return _hMin;
		}

		void PlotController::SetVMin(const TData& min)
		{
			_vMin = min;
		}

		TData PlotController::GetVMin() const
		{
			return _vMin;
		}

		void PlotController::InitView()
		{
			_view.left = 0.0f;
			_view.right = 1.0f;
			_view.bottom = 0.0f;
			_view.top = 1.0f;

			_viewport.x = 0;
			_viewport.y = 0;
			_viewport.w = 1;
			_viewport.h = 1;
		}

		void PlotController::UpdateHBounds(bool zin)
		{
			TData left,right;
			left = GetLeftBound();
			if(zin)
			{
				right = _current;
				TData pos = GetSelPos()-left;
				if(IsVisibleSelPos())
				{
					if(pos >= (_current/2.0f))
					{
						left += pos-_current/2.0f;
					}
				}
				else
				{
					left += _current/2.0f;
				}
				if(right+left > GetnSamples())
				{
					right = TData(GetnSamples());
					_view.right = right-left;
				}
				else
				{
					right += left;
				}
			}
			else
			{
				right = GetRightBound();
				TData aux = _current/4.0f;
				left -= aux;	
				right += aux;
				if(left < 0)
				{
					left = 0.0f;
					right = _current;
				}
				if(right > GetnSamples())
				{
					right = TData(GetnSamples());
					left = right-_current;
				}
			}
			SetHBounds(left,right);
		}

		void PlotController::UpdateVBounds(bool zin)
		{
			TData bottom,top;
			bottom = GetBottomBound();
			if(zin)
			{
				top = _vcur;
				bottom += _vcur/2.0f;
				if(top+bottom > GetvRange())
				{
					top = GetvRange();
					_view.top = top-bottom;
				}
				else
				{
					top += bottom;
				}
			}
			else
			{
				top = GetTopBound();
				TData aux = _vcur/4.0f;
				bottom -= aux;	
				top += aux;
				if(bottom < 0)
				{
					bottom = 0.0f;
					top = _vcur;
				}
				if(top > GetvRange())
				{
					top = GetvRange();
					bottom = top-_vcur;
				}
			}
			SetVBounds(bottom,top);
		}

		void PlotController::InitHRatio()
		{
			int n = int(GetHMin());
			int r = 1;
			while(n < GetnSamples())
			{
				n *= 2;
				r *= 2;
			}
			_hzRatio = r/2;
		}

		void PlotController::InitVRatio()
		{
			TData n = GetVMin();
			int r = 1;
			while(n < GetvRange())
			{
				n *= 2;
				r *= 2;
			}
			_vzRatio = r/2;
		}

		void PlotController::SurfaceDimensions(int w,int h)
		{
			_viewport.w = w;
			_viewport.h = h;

			int hsv = GetHScrollValue();
			emit hScrollMaxValue(GetnxPixels());
			emit hScrollValue(hsv);

			int vsv = GetVScrollValue();
			emit vScrollMaxValue(GetnyPixels());
			emit vScrollValue(vsv);
		}

		void PlotController::UpdateHViewport(int value)
		{
			TData left = TData(GetnSamples())/TData(GetnxPixels())*TData(value);
			TData right = GetCurrent();
			right += left;
			SetHBounds(left,right);
			emit requestRefresh();
		}

		void PlotController::UpdateVViewport(int value)
		{
			TData bottom = GetvRange()/TData(GetnyPixels())*TData(value);
			TData top = GetVCur();
			top += bottom;
			SetVBounds(bottom,top);
			_view.bottom = GetBottomBound();
			_view.top = GetTopBound();
			emit sendView(_view);
			emit requestRefresh();
		}

		int PlotController::GetnxPixels() const
		{
			TData value = TData(GetnSamples())*TData(_viewport.w)/GetCurrent();
			return int(value);
		}

		int PlotController::GetnyPixels() const
		{
			TData value = GetvRange()*TData(_viewport.h)/GetVCur();
			return int(value);
		}

		TData PlotController::GetCurrent() const
		{
			TData value = _current;
			return (value > GetnSamples()) ? TData(GetnSamples()) : value;
		}

		TData PlotController::GetVCur() const
		{
			TData value = _vcur;
			return (value > GetvRange()) ? GetvRange() : value;
		}

		int PlotController::GetHScrollValue() const
		{
			TData value = GetLeftBound()*TData(GetnxPixels())/TData(GetnSamples());
			return int(value);
		}

		int PlotController::GetVScrollValue() const
		{
			TData value = GetBottomBound()*TData(GetnyPixels())/GetvRange();
			return int(value);
		}

		bool PlotController::IsVisibleSelPos()
		{
			TData pos = GetSelPos();
			return (pos >= GetLeftBound() && pos <= GetRightBound());
		}

		void PlotController::SetMousePos(TData x,TData y)
		{
			_mouseXPos=x;
			_mouseYPos=y;
		}

		TData PlotController::GetMouseXPos() const
		{
			return _mouseXPos;
		}

		TData PlotController::GetMouseYPos() const
		{
			return _mouseYPos;
		}

	        void PlotController::SetLeftButtonPressed(bool pressed)
		{
		    _isLeftButtonPressed=pressed;
		}

	        bool PlotController::IsLeftButtonPressed()
		{
		    return _isLeftButtonPressed;
		}

	        void PlotController::LeaveMouse()
		{
		    _isAbleToEdit=false;
		}

	        void PlotController::EnterMouse()
		{
		    _isAbleToEdit=true;
		}

                bool PlotController::IsAbleToEdit()
		{
		    return _isAbleToEdit;
		}

	}
}

// END


