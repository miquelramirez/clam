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

#include "FundPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	FundPlotController::FundPlotController()
	{
	    _maxFreq = TData(0.0);
	    _mustProcessData = false;
	    SetVMin(10.0);
	}

	FundPlotController::~FundPlotController()
	{
	}

	void FundPlotController::SetData(const Segment& segment)
	{
	    _segment = segment;
	    _sampleRate = _segment.GetSamplingRate();
	    _duration = _segment.GetEndTime()-_segment.GetBeginTime();
	    CacheData();
	    FullView();
	    SetnSamples(TSize(_duration*_sampleRate));
	    SetvRange(TData(_maxFreq));
	    InitialRegionTime();
	    _mustProcessData = true;
	    SetSelPos(TData(0.0));
	    emit requestRefresh();
	}

	void FundPlotController::SetDataColor(Color c)
	{
	    _renderer.SetColor(c);
	}

	void FundPlotController::SetHBounds(const TData& left,const TData& right)
	{
	    SelTimeRegionPlotController::SetHBounds(left,right);
	    _mustProcessData = true;
			
	    double lBound = double(GetLeftBound()/_sampleRate);
	    double hBound = double(GetRightBound()/_sampleRate);
			
	    emit xRulerRange(lBound,hBound);	
	}

	void FundPlotController::SetVBounds(const TData& bottom,const TData& top)
	{
	    SelTimeRegionPlotController::SetVBounds(bottom,top);
			
	    double bBound = double(GetBottomBound());
	    double tBound = double(GetTopBound());
	    
	    emit yRulerRange(bBound,tBound);
	}

	void FundPlotController::SurfaceDimensions(int w,int h)
	{
	    PlotController::SurfaceDimensions(w,h);
		
	    double lBound = double(GetLeftBound()/_sampleRate);
	    double hBound = double(GetRightBound()/_sampleRate);
			
	    emit xRulerRange(lBound,hBound);

	    double bBound = double(GetBottomBound());
	    double tBound = double(GetTopBound());
			
	    emit yRulerRange(bBound,tBound);
	}

	void FundPlotController::Draw()
	{
	    if(_mustProcessData) ProcessData();
	    _renderer.Render();
	    SelTimeRegionPlotController::Draw();
	}

	void FundPlotController::ProcessData()
	{
	    int nFrames = _segment.GetnFrames();
	    TSize offset = TSize(GetLeftBound()*nFrames/GetnSamples());
	    TSize len = TSize(GetRightBound()*nFrames/GetnSamples())-offset+1;
			
	    if(_processedData.Size() <=len)
		_processedData.Resize(len+1);
	    _processedData.SetSize(len+1);

	    std::copy(_cacheData.GetPtr()+offset,_cacheData.GetPtr()+offset+len+1,_processedData.GetPtr());
	    _renderer.SetDataPtr(_processedData.GetPtr(),_processedData.Size(),NormalMode);	
	    _mustProcessData = false;
	}

	void FundPlotController::SetRenderingStep()
	{
	    int nFrames = _segment.GetnFrames();
	    _renderer.SetStep(_duration*_sampleRate/TData(nFrames));
	    SetHMin(_renderer.GetStep()*TData(5.0));
	}

	void FundPlotController::FullView()
	{
	    _maxFreq += TData(50.0);
	    _view.left = TData(0.0);
	    _view.right = TData(_duration*_sampleRate);
	    _view.top = _maxFreq;
	    _view.bottom = TData(0.0);
	    SetHBounds(_view.left,_view.right);
	    SetVBounds(_view.bottom,_view.top);
	    SetRenderingStep();
	    emit sendView(_view);
	    emit initialYRulerRange(double(_view.bottom),double(_view.top));
	}

	void FundPlotController::CacheData()
	{
	    _maxFreq = TData(0.0);
	    int nFrames = _segment.GetnFrames();
	    _cacheData.Resize(nFrames);
	    _cacheData.SetSize(nFrames);
	    TData curFreq;
	    for(int i = 0; i < nFrames; i++)
	    {
		curFreq = _segment.GetFrame(i).GetFundamental().GetFreq(0);
		if(curFreq > _maxFreq) _maxFreq = curFreq;
		_cacheData[i] = curFreq;	
	    }
	}

	TData FundPlotController::GetFreq(TData t) const
	{
	    TData value = t*_sampleRate;
	    int nFrames = _segment.GetnFrames();
	    int index = int(value*nFrames/GetnSamples());
	    return _cacheData[index];
	}

	void FundPlotController::InitialRegionTime() 
	{
	    MediaTime time;
	    time.SetBegin(TData(0.0));
	    emit selectedRegion(time);
	}

	void FundPlotController::SetMousePos(TData x,TData y)
	{
	    TData tbound = GetTopBound()-GetBottomBound();
	    TData bBound = GetBottomBound();
	    TData ycoord=y;
	    ycoord *= tbound;
	    ycoord /= TData(_viewport.h);
	    ycoord += bBound;
	    SegmentationMarksPlotController::SetMousePos(x,ycoord);
	    TData t=GetMouseXPos()/_sampleRate;
	    TData freq=GetMouseYPos();
	    QString s = "t="+QString::number(t,'f',3)+"s freq="+QString::number(freq,'f',0)+"Hz";
			
	    emit toolTip(s);
	}
    }
}

// END
