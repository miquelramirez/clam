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
			: mLeftBound(TData(0.0))
			, mRightBound(TData(1.0))
			, mBottomBound(TData(0.0))
			, mTopBound(TData(1.0))
			, mSelPos(TData(0.0))
			, mHMin(TData(50.0))
			, mVMin(TData(50.0))
			, mSamples(0) 
			, mCurrent(0)
			, mVRange(TData(0.0))
			, mVCur(0)
			, mHZRatio(1.0)
			, mVZRatio(1.0)
			, mIsLeftButtonPressed(false)
			, mIsAbleToEdit(false)
		{	
			InitView();
		}
	
		PlotController::~PlotController()
		{
		}

		void PlotController::SetHBounds(const TData& left, const TData& right)
		{
			mLeftBound = left;
			mRightBound = right;
		}

		void PlotController::SetVBounds(const TData& bottom, const TData& top)
		{
			mBottomBound = bottom;
			mTopBound = top;
		}

		TData PlotController::GetLeftBound() const
		{
			return mLeftBound;
		}

		TData PlotController::GetRightBound() const
		{
			return mRightBound;
		}

		TData PlotController::GetBottomBound() const
		{
			return mBottomBound;
		}

		TData PlotController::GetTopBound() const
		{
			return mTopBound;
		}

		void PlotController::SetSelPos(const TData& value)
		{
			mSelPos = value;
		}

		TData PlotController::GetSelPos() const
		{
			return mSelPos;
		}

		void PlotController::SetnSamples(const TSize& n)
		{
			mSamples = n;
			mCurrent = TData(mSamples);
			InitHRatio();
			emit hZoomRatio(mHZRatio);
			int hsv=GetHScrollValue();
			emit hScrollMaxValue(GetnxPixels());
			emit hScrollValue(hsv);
		}

		TSize PlotController::GetnSamples() const
		{
			return mSamples;
		}

		void PlotController::SetvRange(const TData& vr)
		{
			mVRange = vr;
			mVCur = mVRange;
			InitVRatio();
			emit vZoomRatio(mVZRatio);
			int vsv=GetVScrollValue();
			emit vScrollMaxValue(GetnyPixels());
			emit vScrollValue(vsv);
		}

		TData PlotController::GetvRange() const
		{
			return mVRange;
		}

		void PlotController::HZoomIn()
		{
			if(mCurrent/TData(2.0) > GetHMin())
			{
				mCurrent /= TData(2.0);
				mView.right = mCurrent;
				UpdateHBounds(true);
				emit sendView(mView);
				TData left = GetLeftBound();
				TData right = GetRightBound();
				mHZRatio /= 2.0;
				emit hZoomRatio(mHZRatio);
				emit hScrollMaxValue(GetnxPixels());
				emit hScrollValue(GetHScrollValue());
				SetHBounds(left,right);
				emit requestRefresh();
			}
		}

		void PlotController::HZoomOut()
		{
		    if(mCurrent*TData(2.0) <= GetnSamples())
			{
				mCurrent *= TData(2.0);
				mView.right = mCurrent;
				UpdateHBounds();
				emit sendView(mView);
				TData left = GetLeftBound();
				TData right = GetRightBound();
				mHZRatio *= 2.0;
				emit hZoomRatio(mHZRatio);
				emit hScrollMaxValue(GetnxPixels());
				SetHBounds(left,right);
				emit hScrollValue(GetHScrollValue());
				emit requestRefresh();
			}
		}

		void PlotController::VZoomIn()
		{
			if(mVCur/TData(2.0) > GetVMin())
			{
				mVCur /= TData(2.0);
				mView.top = mVCur;
				UpdateVBounds(true);
				emit sendView(mView);
				TData bottom = GetBottomBound();
				TData top = GetTopBound();
				mVZRatio /= 2.0;
				emit vZoomRatio(mVZRatio);
				emit vScrollMaxValue(GetnyPixels());
				emit vScrollValue(GetVScrollValue());
				SetVBounds(bottom,top);
				emit requestRefresh();
			}
		}

		void PlotController::VZoomOut()
		{
			if(mVCur*TData(2.0) <= GetvRange())
			{
				mVCur *= TData(2.0);
				mView.top = mVCur;
				UpdateVBounds();
				emit sendView(mView);
				TData bottom = GetBottomBound();
				TData top = GetTopBound();
				mVZRatio *= 2.0;
				emit vZoomRatio(mVZRatio);
				emit vScrollMaxValue(GetnyPixels());
				SetVBounds(bottom,top);
				emit vScrollValue(GetVScrollValue());
				emit requestRefresh();
			}
		}

		void PlotController::SetHMin(const TData& min)
		{
			mHMin = min;
		}

		TData PlotController::GetHMin() const
		{
			return mHMin;
		}

		void PlotController::SetVMin(const TData& min)
		{
			mVMin = min;
		}

		TData PlotController::GetVMin() const
		{
			return mVMin;
		}

		void PlotController::InitView()
		{
			mView.left = 0.0f;
			mView.right = 1.0f;
			mView.bottom = 0.0f;
			mView.top = 1.0f;

			mViewport.x = 0;
			mViewport.y = 0;
			mViewport.w = 1;
			mViewport.h = 1;
		}

		void PlotController::UpdateHBounds(bool zin)
		{
			TData left,right;
			left = GetLeftBound();
			if(zin)
			{
				right = mCurrent;
				TData pos = GetSelPos()-left;
				if(IsVisibleSelPos())
				{
					if(pos >= (mCurrent/TData(2.0)))
					{
						left += pos-mCurrent/TData(2.0);
					}
				}
				else
				{
					left += mCurrent/TData(2.0);
				}
				if(right+left > GetnSamples())
				{
					right = TData(GetnSamples());
					mView.right = right-left;
				}
				else
				{
					right += left;
				}
			}
			else
			{
				right = GetRightBound();
				TData aux = mCurrent/TData(4.0);
				left -= aux;	
				right += aux;
				if(left < 0)
				{
					left = TData(0.0);
					right = mCurrent;
				}
				if(right > GetnSamples())
				{
					right = TData(GetnSamples());
					left = right-mCurrent;
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
				top = mVCur;
				bottom += mVCur/TData(2.0);
				if(top+bottom > GetvRange())
				{
					top = GetvRange();
					mView.top = top-bottom;
				}
				else
				{
					top += bottom;
				}
			}
			else
			{
				top = GetTopBound();
				TData aux = mVCur/TData(4.0);
				bottom -= aux;	
				top += aux;
				if(bottom < 0)
				{
					bottom = TData(0.0);
					top = mVCur;
				}
				if(top > GetvRange())
				{
					top = GetvRange();
					bottom = top-mVCur;
				}
			}
			SetVBounds(bottom,top);
		}

		void PlotController::InitHRatio()
		{
			double n = double(GetHMin());
			double r = 1.0;
			while(n < GetnSamples())
			{
				n *= 2.0;
				r *= 2.0;
			}
			mHZRatio = r/2.0;
		}

		void PlotController::InitVRatio()
		{
			TData n = GetVMin();
			double r = 1.0;
			while(n < GetvRange())
			{
				n *= 2.0;
				r *= 2.0;
			}
			mVZRatio = r/2.0;
		}

		void PlotController::SurfaceDimensions(int w,int h)
		{
			mViewport.w = w;
			mViewport.h = h;

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
			mView.bottom = GetBottomBound();
			mView.top = GetTopBound();
			emit sendView(mView);
			emit requestRefresh();
		}

		int PlotController::GetnxPixels() const
		{
			TData value = TData(GetnSamples())*TData(mViewport.w)/GetCurrent();
			return int(value);
		}

		int PlotController::GetnyPixels() const
		{
			TData value = GetvRange()*TData(mViewport.h)/GetVCur();
			return int(value);
		}

		TData PlotController::GetCurrent() const
		{
			TData value = mCurrent;
			return (value > GetnSamples()) ? TData(GetnSamples()) : value;
		}

		TData PlotController::GetVCur() const
		{
			TData value = mVCur;
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
			mMouseXPos=x;
			mMouseYPos=y;
		}

		TData PlotController::GetMouseXPos() const
		{
			return mMouseXPos;
		}

		TData PlotController::GetMouseYPos() const
		{
			return mMouseYPos;
		}

		void PlotController::SetLeftButtonPressed(bool pressed)
		{
		    mIsLeftButtonPressed=pressed;
		}

		bool PlotController::IsLeftButtonPressed()
		{
		    return mIsLeftButtonPressed;
		}

		void PlotController::LeaveMouse()
		{
		    mIsAbleToEdit=false;
		}

		void PlotController::EnterMouse()
		{
		    mIsAbleToEdit=true;
		}

		bool PlotController::IsAbleToEdit()
		{
		    return mIsAbleToEdit;
		}

		bool PlotController::IsPlayable()
		{
		    return false;
		}

	}
}

// END


