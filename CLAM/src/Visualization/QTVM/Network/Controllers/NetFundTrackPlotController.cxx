#include "NetFundTrackPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetFundTrackPlotController::NetFundTrackPlotController()
	    : _index(0),mMonitor(0),
	      _hasData(false),_tooltip(""),
	      _renderingIsDone(false)
	{
	    SetDataColor(VMColor::Green());
	    mSlotNewData.Wrap(this,&NetFundTrackPlotController::OnNewData);
	}

	NetFundTrackPlotController::~NetFundTrackPlotController()
	{
	}

	void NetFundTrackPlotController::SetData(const Fundamental& fund)
	{
	    if(CanGetData())
	    {
		SetCanSendData(false);
		if(First()) Init();
		AddData(fund.GetFreq(0));
		SetCanSendData(true);
	    }
	}

	void NetFundTrackPlotController::SetDataColor(Color c)
	{
	    _renderer.SetColor(c);
	}

	void NetFundTrackPlotController::Draw()
	{
	    if(CanSendData())
	    {
		SetCanGetData(false);
		_renderer.SetIndexes(TIndex(_index),TIndex(_cachedData.Size()),0,TIndex(_index),0);
		_renderer.SetDataPtr(_cachedData.GetPtr());
		SetCanGetData(true);
	    }
	    _renderer.Render();
	    NetPlotController::Draw();

	    _renderingIsDone=true;
	}

	void NetFundTrackPlotController::SetMonitor(MonitorType & monitor)
	{
	    mMonitor = & monitor;
	    mMonitor->AttachStartSlot(mStartSlot);
	    mMonitor->AttachStopSlot(mStopSlot);
	    mMonitor->AttachSlotNewData(mSlotNewData);
	}

	void NetFundTrackPlotController::AddData(const TData& data)
	{
	    if(_cachedData.Size() < GetnSamples())
	    {
		_cachedData.AddElem(data);
	    }
	    else
	    {
		_cachedData[_index++] = data;
		if(_index == GetnSamples()) _index = 0;
	    }
	}

	void NetFundTrackPlotController::Init()
	{
	    _hasData=true;
	    _index=0;
	    _cachedData.Init();
	    SetvRange(TData(0.0),TData(2050.0));
	    SetnSamples(100); 
	    SetFirst(false);
	    FullView();
	}

	void NetFundTrackPlotController::FullView()
	{
	     _view.left=TData(0.0);
	    _view.right=TData(GetnSamples());
	    _view.bottom=GetvMin();
	    _view.top=GetvMax();

	    emit sendView(_view);
	}

	void NetFundTrackPlotController::OnNewData()
	{
	    if(CanGetData())
	    {
		SetCanSendData(false);
		if(MonitorIsRunning())
		{
		    const Fundamental & fund = mMonitor->FreezeAndGetData();

		    if(First()) Init();
		    AddData(fund.GetFreq(0));	

		    mMonitor->UnfreezeData();
		}
		SetCanSendData(true);
	    }
	}

	void NetFundTrackPlotController::UpdatePoint(const TData& x, const TData& y)
	{
	     NetPlotController::UpdatePoint(x,y);
	     _tooltip="";
	     if(_hasData)
	     {
		 _tooltip = "frequency="+(_tooltip.setNum(y,'f',0))+"Hz";  
	     }
	     if(_renderingIsDone)
	     {
		 _renderingIsDone=false;
		 emit toolTip(_tooltip);
	     }
	}

    }
}

// END


