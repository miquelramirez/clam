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
#include "CLAMGL.hxx"
#include "CLAM_Math.hxx"
#include "AudioPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	AudioPlotController::AudioPlotController() 
	{
	    _mustProcessData = false;
	    _hugeArrayCondition = false;
	    SetVMin(TData(0.2));
	}

	AudioPlotController::~AudioPlotController()
	{
	}

	void AudioPlotController::SetData(const Audio& audio)
	{
	    _audio = audio;
	    _sampleRate = _audio.GetSampleRate();
	    FullView();
	    SetnSamples(_audio.GetBuffer().Size());
	    SetvRange(TData(2.0));
	    InitialRegionTime();
	    _duration = TData(GetnSamples())/_sampleRate;
	    _mustProcessData = true;
	    SetSelPos(TData(0.0));
	    emit requestRefresh();
	}

	void AudioPlotController::SetDataColor(Color c)
	{
	    _dRenderer.SetColor(c);
	}

	void AudioPlotController::Draw()
	{
	    if(_mustProcessData) ProcessData();
	    _dRenderer.Render();
	    DrawAxis();
	    SelTimeRegionPlotController::Draw();
	}

	void AudioPlotController::FullView()
	{
	    _view.left = TData(0.0);
	    _view.right = TData(_audio.GetBuffer().Size());
	    _view.top = TData(2.0);
	    _view.bottom = TData(0.0);
	    SetHBounds(_view.left,_view.right);
	    SetVBounds(_view.bottom,_view.top);
	    emit sendView(_view);
	    emit initialYRulerRange(-1.0,1.0);
	}

	void AudioPlotController::SetHBounds(const TData& left,const TData& right)
	{
	    SelTimeRegionPlotController::SetHBounds(left,right);
	    _mustProcessData = true;
			
	    double lBound = double(GetLeftBound()/_sampleRate);
	    double hBound = double(GetRightBound()/_sampleRate);
	    
	    emit xRulerRange(lBound,hBound);	
	}

	void AudioPlotController::SetVBounds(const TData& bottom,const TData& top)
	{
	    SelTimeRegionPlotController::SetVBounds(bottom,top);
			
	    double bBound = double(GetBottomBound()-1.0);
	    double tBound = double(GetTopBound()-1.0);
			
	    emit yRulerRange(bBound,tBound);
	}

	void AudioPlotController::SurfaceDimensions(int w,int h)
	{
	    PlotController::SurfaceDimensions(w,h);
	    _mustProcessData = true;
			
	    double lBound = double(GetLeftBound()/_sampleRate);
	    double hBound = double(GetRightBound()/_sampleRate);
			
	    emit xRulerRange(lBound,hBound);

	    double bBound = double(GetBottomBound()-1.0);
	    double tBound = double(GetTopBound()-1.0);
		      
	    emit yRulerRange(bBound,tBound);
	}

	void AudioPlotController::DrawAxis()
	{
	    Color c = _dRenderer.GetColor();
	    glColor3ub(c.r,c.g,c.b);
	    glBegin(GL_LINES);
	    glVertex3f(0.0f,1.0,-1.0f);
	    glVertex3f(GetCurrent(),1.0f,-1.0f);
	    glEnd();
	}

	void AudioPlotController::ProcessData()
	{
	    DetermineVisibleSamples();
	    if(_hugeArrayCondition)
	    {
		_dRenderer.SetArrays(_maxs.GetPtr(), _mins.GetPtr(), _maxs.Size());
		_dRenderer.SetVBounds(_view.top,_view.bottom);
		return;
	    }

	    for(int i=0;i < _processedData.Size();i++) _processedData[i] += TData(1.0);

	    TData range = GetRightBound()-GetLeftBound();
	    TData threshold = GetHMin()*TData(2.0);
	    int mode = (range < threshold) ? DetailMode : NormalMode;
	    _dRenderer.SetDataPtr(_processedData.GetPtr(),_processedData.Size(),mode);	
	    _mustProcessData = false;
	}

	void AudioPlotController::DetermineVisibleSamples()
	{
	    TSize offset = TSize(GetLeftBound());
	    TSize len = TSize(GetRightBound() - GetLeftBound())+1;

	    if(len/5 >= _viewport.w)
	    {
		_hugeArrayCondition = true;
		BuildMaxMinArrays(offset,len);
		return;
	    }
					
	    _hugeArrayCondition = false;
							
	    if(_processedData.Size() <= len)
		_processedData.Resize(len+1);

	    _processedData.SetSize(len+1);

	    std::copy(_audio.GetBuffer().GetPtr()+offset,_audio.GetBuffer().GetPtr()+offset+len+1,_processedData.GetPtr());
	}

	void AudioPlotController::BuildMaxMinArrays(TSize offset,TSize len)
	{
	    TSize startSearch, endSearch, searchIntervalLen, searchRemIntervalLen;
	    TSize maxs = _viewport.w;

	    if(_maxs.Size() < maxs) _maxs.Resize(maxs);
	    if(_mins.Size() < maxs)	_mins.Resize(maxs);

	    _maxs.SetSize(maxs);
	    _mins.SetSize(maxs);

	    searchIntervalLen = len / maxs;
	    searchRemIntervalLen = len % maxs;
			
	    int firstPassIterations = (searchRemIntervalLen) ? maxs-1 : maxs;

	    startSearch = offset;
	    endSearch = startSearch + searchIntervalLen;				

	    for(int i = 0; i < firstPassIterations; i++)
	    {
		_maxs[i] = *std::max_element(_audio.GetBuffer().GetPtr()+startSearch, _audio.GetBuffer().GetPtr()+endSearch)+TData(1.0);
		_mins[i] = *std::min_element(_audio.GetBuffer().GetPtr()+startSearch, _audio.GetBuffer().GetPtr()+endSearch)+TData(1.0);
							
		startSearch = endSearch;
		endSearch += searchIntervalLen;	
	    }
	    if(searchRemIntervalLen)
	    {
		_maxs[maxs-1] = *std::max_element(_audio.GetBuffer().GetPtr()+startSearch, _audio.GetBuffer().GetPtr()+startSearch+searchRemIntervalLen)+TData(1.0);
		_mins[maxs-1] = *std::min_element(_audio.GetBuffer().GetPtr()+startSearch, _audio.GetBuffer().GetPtr()+startSearch+searchRemIntervalLen)+TData(1.0);
	    }
	}

	TData AudioPlotController::GetAmp(TData t) const
	{
	    TIndex index = TIndex(t*_sampleRate);
	    TData value = _audio.GetBuffer()[index];
	    return (fabs(value) >= 0.002) ? value : TData(0.0);
	}

	void AudioPlotController::InitialRegionTime()
	{
	    MediaTime time;
	    time.SetBegin(TData(0.0));
	    time.SetEnd(TData(GetnSamples())/_sampleRate);
	    emit selectedRegion(time);
	}

	bool AudioPlotController::MustProcessData() const
	{
	    return _mustProcessData;
	}

	void AudioPlotController::SetMousePos(TData x,TData y)
	{
	    TData tbound = GetTopBound()-GetBottomBound();
	    TData bBound = GetBottomBound()-TData(1.0);
	    TData ycoord=y;
	    ycoord *= tbound;
	    ycoord /= TData(_viewport.h);
	    ycoord += bBound;
	    SegmentationMarksPlotController::SetMousePos(x,ycoord);
	    if(!HasSentTag())
	    {
		TData t=GetMouseXPos()/_sampleRate;
		TData amp=GetMouseYPos();
		QString s = "t="+QString::number(t,'f',3)+"s amp="+QString::number(amp,'f',3);
		emit toolTip(s);
	    }
	}

	void AudioPlotController::SetSelPos(const TData& value)
	{
	    if(CanDrawSelectedPos())
	    {
		if(GetDialPos() != value)
		{
		    SelTimeRegionPlotController::SetSelPos(value);
		    emit requestRefresh();
		    emit selPos(value);
		    emit selectedXPos(value/_sampleRate);
		}
	    }
	}

	 void AudioPlotController::SetSelectedXPos(double xpos)
	 {
	     SetSelPos(TData(xpos)*_sampleRate);
	 }
    }
}

// END

