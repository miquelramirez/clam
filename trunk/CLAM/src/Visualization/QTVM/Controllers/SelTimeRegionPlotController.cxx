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
	    : _sampleRate(TData(44100.0)),
	      _duration(TData(0.0)),
	      _isPlaying(false)
	{
	    SetKeyShiftPressed(false);
	}
		
	SelTimeRegionPlotController::~SelTimeRegionPlotController()
	{
	}

	void SelTimeRegionPlotController::SetSelPos(const TData& value)
	{
	    if(CanDrawSelectedPos())
	    {
		if(_keyShiftPressed)
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
	    _rMarker.Render();
	}

	void SelTimeRegionPlotController::SetRegionColor(Color c)
	{
	    _rMarker.SetColor(c);
	}
		
	void SelTimeRegionPlotController::SetKeyShiftPressed(bool pr)
	{
	    _keyShiftPressed = pr;
	}

	void SelTimeRegionPlotController::SetHBounds(const TData& left,const TData& right)
	{
	    SelPosPlotController::SetHBounds(left,right);
	    _rMarker.SetHBounds(GetLeftBound(),GetRightBound());
	}

	void SelTimeRegionPlotController::SetVBounds(const TData& bottom,const TData& top)
	{
	    SelPosPlotController::SetVBounds(bottom,top);
	    _rMarker.SetVBounds(GetBottomBound(),GetTopBound());
	}

	void SelTimeRegionPlotController::SetBeginRegion(const TData& value)
	{
	    _rMarker.SetBegin(value);
	}

	void SelTimeRegionPlotController::SetEndRegion(const TData& value)
	{
	    _rMarker.SetEnd(value);
	}

	TData SelTimeRegionPlotController::GetBeginRegion() const
	{
	    return _rMarker.GetBegin();
	}

	TData SelTimeRegionPlotController::GetEndRegion() const
	{
	    return _rMarker.GetEnd();
	}

	MediaTime SelTimeRegionPlotController::GetRegionTime() const
	{
	    MediaTime time;
	    time.SetBegin(GetBeginRegion()/_sampleRate);
	    time.SetEnd(GetEndRegion()/_sampleRate);
	    if(time.GetBegin() < time.GetEnd())
	    {
		time.AddDuration();
		time.UpdateData();
		time.SetDuration(time.GetEnd()-time.GetBegin());
	    }
	    else
	    {
		time.SetEnd(_duration);
	    }
	    return time;
	}

	void SelTimeRegionPlotController::UpdateTimePos(const TData& time)
	{
	    if(_isPlaying && time<=GetDialPos()/_sampleRate) return;
	    if(!_isPlaying) 
	    {
		_isPlaying =true;
		emit startPlaying();
	    }
	    UpdateDial(time*_sampleRate);
	    emit currentPlayingTime(float(time));
	}

	void SelTimeRegionPlotController::StopPlaying(const TData& time)
	{
	    if(!_isPlaying) return;
	    _isPlaying=false;
	    UpdateDial(time*_sampleRate);
	    emit stopPlaying(float(time));
	    emit stopPlaying();
	}

	bool SelTimeRegionPlotController::CanDrawSelectedPos()
	{
	    return (!_isPlaying && SegmentationMarksPlotController::CanDrawSelectedPos());
	}

	bool SelTimeRegionPlotController::IsPlayable()
	{
	    return true;
	}
    }
    
}

// END
