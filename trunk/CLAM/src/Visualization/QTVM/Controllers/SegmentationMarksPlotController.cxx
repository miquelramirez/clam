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
#include "SegmentationMarksPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	SegmentationMarksPlotController::SegmentationMarksPlotController()
	    : _hit(false), 
	      _currentElem(0),
	      _currentIndex(0), 
	      _keyInsertPressed(false), 
	      _keyDeletePressed(false)
	{
	    _mustProcessMarks=false;
	}

	SegmentationMarksPlotController::~SegmentationMarksPlotController()
	{
	}

	void SegmentationMarksPlotController::SetMarks(std::vector<unsigned>& marks)
	{
	    _marks = marks;
	    sort(_marks.begin(),_marks.end());
	    _mustProcessMarks=true;
	    emit requestRefresh();
	}

	std::vector<unsigned>& SegmentationMarksPlotController::GetMarks()
	{
	    return _marks;
	}

	void SegmentationMarksPlotController::SetMarksColor(Color c)
	{
	    _marksRenderer.SetColor(c);
	}

	void SegmentationMarksPlotController::Draw()
	{
	    if(_marks.size() > 0)
	    {
		if(_mustProcessMarks) ProcessMarksData();
		_marksRenderer.Render();
	    }
			
	}

	void SegmentationMarksPlotController::SurfaceDimensions(int w,int h)
	{
	    PlotController::SurfaceDimensions(w,h);
	    _mustProcessMarks=true;
	}

	void SegmentationMarksPlotController::SetMousePos(TData x, TData y)
	{
	    PlotController::SetMousePos(x,y);
	    if(IsAbleToEdit())
	    {
		int index=Hit(x);
		if(index != -1) 
		{
		    if(_keyDeletePressed)
		    {
			QCursor cCursor(Qt::CrossCursor);
			emit changeCursor(cCursor);
		    }
		    else
		    {
			QCursor hcursor(Qt::SizeHorCursor);
			emit changeCursor(hcursor);
		    }
		    _currentIndex=index;
		    _hit=true;
		}
		else
		{
		    _currentElem = unsigned(x);
		    if(!IsLeftButtonPressed())
		    {
			QCursor acursor(Qt::ArrowCursor);
			emit changeCursor(acursor);
			_hit=false;
		    }
		}
	    }

	    if(_hit && IsLeftButtonPressed())
	    {
		if(!_keyDeletePressed)
		{
		    Update(_currentIndex,unsigned(x));
		}
	    }
	}

	void SegmentationMarksPlotController::SetHBounds(const TData& left, const TData& right)
        {
	    PlotController::SetHBounds(left,right);
	    _marksRenderer.SetHBounds(GetLeftBound(),GetRightBound());
	    _mustProcessMarks=true;
	}

	void SegmentationMarksPlotController::SetVBounds(const TData& bottom, const TData& top)
	{
	    PlotController::SetVBounds(bottom,top);
	    _marksRenderer.SetVBounds(GetBottomBound(),GetTopBound());
	    _mustProcessMarks=true;
	}

	void SegmentationMarksPlotController::ProcessMarksData()
        {
	    unsigned left = unsigned(GetLeftBound());
	    unsigned right = unsigned(GetRightBound());

	    Array<unsigned> processedMarks;

	    std::vector<unsigned>::iterator it = _marks.begin();
	    for(;it != _marks.end();it++)
	    {
		if((*it) > right) break;
		if((*it) >= left) processedMarks.AddElem((*it));
	    }

	    _marksRenderer.SetData(processedMarks);
	    _mustProcessMarks=false;
	}

	void SegmentationMarksPlotController::InsertElem(unsigned elem)
	{
	    if(HaveElem(elem)) return;
	    std::vector<unsigned>::iterator pos = _marks.begin();
	    for(; pos != _marks.end(); pos++)
	    {
		if((*pos) > elem) break;
	    } 
	    _marks.insert(pos,elem);
	    
	    _mustProcessMarks=true;
	    emit requestRefresh();
	    emit insertedMark(elem);
	}

	void SegmentationMarksPlotController::RemoveElem(int index)
	{
	    if(index < 0 || index > (int)_marks.size()-1) return;
	    unsigned elem = _marks[index];
	    std::vector<unsigned>::iterator pos = find(_marks.begin(),_marks.end(),elem);
	    _marks.erase(pos);

	    _mustProcessMarks=true;
	    emit requestRefresh();
	    emit removedMark(index,elem);
	}
	    
	bool SegmentationMarksPlotController::HaveElem(unsigned elem)
	{
	    return find(_marks.begin(),_marks.end(),elem) != _marks.end();
	}

	void SegmentationMarksPlotController::Update(int index, unsigned elem)
	{
	    _marks[index]=elem;
	    
	    _mustProcessMarks=true;
	    emit requestRefresh();
	    emit updatedMark(index,elem);
	}

	unsigned SegmentationMarksPlotController::GetPixel(const TData& x) const
	{
	    TData w=_viewport.w;
	    TData left=GetLeftBound();
	    TData right=GetRightBound();
	    TData xcoord=x-left;
	    TData pixel=xcoord*w/(right-left);
	    return (unsigned)pixel;
	}
	  
	int SegmentationMarksPlotController::Hit(const TData& x)
	{
	    unsigned i;
	    bool hit=false;
	    unsigned selected_pixel=GetPixel(x);
	    for(i=0; i < _marks.size(); i++)
	    {
		unsigned owned_pixel=GetPixel(TData(_marks[i]));
		if(abs(int(selected_pixel-owned_pixel)) <= 1)
		{
		    hit=true;
		    break;
		}
	    }
	    return (hit) ? int(i) : -1;
	}

	void SegmentationMarksPlotController::LeaveMouse()
	{
	    _hit=false;
	    PlotController::LeaveMouse();
	    QCursor cursor(ArrowCursor);
	    emit changeCursor(cursor);
	}
	
	void SegmentationMarksPlotController::SetKeyInsertPressed(bool pressed)
	{
	    _keyInsertPressed=pressed;
	}

	void SegmentationMarksPlotController::SetKeyDeletePressed(bool pressed)
	{
	    _keyDeletePressed=pressed;
	}

	bool SegmentationMarksPlotController::CanDrawSelectedPos()
	{
	    return (!_hit && !_keyInsertPressed);
	}

	void SegmentationMarksPlotController::SetLeftButtonPressed(bool pressed)
	{
	    PlotController::SetLeftButtonPressed(pressed);
	    if(IsLeftButtonPressed())
	    {
		if(!_hit && _keyInsertPressed)
		{
		    InsertElem(_currentElem);
		}

		if(_hit && _keyDeletePressed)
		{
		    RemoveElem(_currentIndex);
		}
	    }
	}

	void SegmentationMarksPlotController::InsertMark(unsigned elem)
	{
	    InsertElem(elem);
	}

	void SegmentationMarksPlotController::RemoveMark(int index, unsigned elem)
	{
	    if(_marks[index] != elem) return;
	    RemoveElem(index);
	}

	void SegmentationMarksPlotController::UpdateMark(int index, unsigned elem)
	{
	    if(_marks[index] == elem) return;
	    Update(index,elem);
	}
    }
}

// END

