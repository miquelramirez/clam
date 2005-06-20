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

#include <qstring.h>
#include "SelTimeRegionPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		SelTimeRegionPlotController::SelTimeRegionPlotController()
			: mSampleRate(TData(44100.0))
			, mDuration(TData(0.0))
			, mKeyShiftPressed(false)
			, mIsPlaying(false)
		{
		}
		
		SelTimeRegionPlotController::~SelTimeRegionPlotController()
		{
		}

		void SelTimeRegionPlotController::SetSelPos(const TData& value)
		{
			if(CanDrawSelectedPos())
			{
				if(mKeyShiftPressed)
				{
					if(value > GetBeginRegion() && value < GetEndRegion())
					{
						if(value > GetBeginRegion()+((GetEndRegion()-GetBeginRegion())/TData(2.0)))
						{
							SetEndRegion(value);
						}
						else
						{
							SetBeginRegion(value);
						}
					}
					else
					{
						if(value > GetEndRegion())
						{
							SetEndRegion(value);
						}
						if(value < GetBeginRegion())
						{
							SetBeginRegion(value);
						}
					}
					UpdateDial(value);
					PlotController::SetSelPos(GetBeginRegion()+((GetEndRegion()-GetBeginRegion())/TData(2.0)));
				}
				else
				{
					SelPosPlotController::SetSelPos(value);
					SetBeginRegion(GetSelPos());
					SetEndRegion(GetSelPos());
				}
				emit selectedRegion(GetRegionTime());
			}
		}
		
		void SelTimeRegionPlotController::Draw()
		{
			SelPosPlotController::Draw();
			mRegionMarker.Render();
		}

		void SelTimeRegionPlotController::SetRegionColor(Color c)
		{
			mRegionMarker.SetColor(c);
		}
		
		void SelTimeRegionPlotController::SetKeyShiftPressed(bool pr)
		{
			mKeyShiftPressed = pr;
		}

		void SelTimeRegionPlotController::SetHBounds(const TData& left,const TData& right)
		{
			SelPosPlotController::SetHBounds(left,right);
			mRegionMarker.SetHBounds(GetLeftBound(),GetRightBound());
		}

		void SelTimeRegionPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			SelPosPlotController::SetVBounds(bottom,top);
			mRegionMarker.SetVBounds(GetBottomBound(),GetTopBound());
		}

		void SelTimeRegionPlotController::SetBeginRegion(const TData& value)
		{
			mRegionMarker.SetBegin(value);
		}

		void SelTimeRegionPlotController::SetEndRegion(const TData& value)
		{
			mRegionMarker.SetEnd(value);
		}

		TData SelTimeRegionPlotController::GetBeginRegion() const
		{
			return mRegionMarker.GetBegin();
		}

		TData SelTimeRegionPlotController::GetEndRegion() const
		{
			return mRegionMarker.GetEnd();
		}

		MediaTime SelTimeRegionPlotController::GetRegionTime() const
		{
			MediaTime time;
			time.SetBegin(GetBeginRegion()/mSampleRate);
			time.SetEnd(GetEndRegion()/mSampleRate);
			if(time.GetBegin() < time.GetEnd())
			{
				time.AddDuration();
				time.UpdateData();
				time.SetDuration(time.GetEnd()-time.GetBegin());
			}
			else
			{
				time.SetEnd(mDuration);
			}
			return time;
		}

		void SelTimeRegionPlotController::UpdateTimePos(const TData& time)
		{
			if(mIsPlaying && time<=GetDialPos()/mSampleRate) return;
			if(!mIsPlaying) 
			{
				mIsPlaying =true;
				emit startPlaying();
			}
			UpdateDial(time*mSampleRate);
			emit currentPlayingTime(float(time));
		}

		void SelTimeRegionPlotController::StopPlaying(const TData& time)
		{
			if(!mIsPlaying) return;
			mIsPlaying=false;
			UpdateDial(time*mSampleRate);
			emit stopPlaying(float(time));
			emit stopPlaying();
		}

		bool SelTimeRegionPlotController::CanDrawSelectedPos()
		{
			return (!mIsPlaying && SegmentationMarksPlotController::CanDrawSelectedPos());
		}

		bool SelTimeRegionPlotController::IsPlayable()
		{
			return true;
		}

    }    
}

// END
