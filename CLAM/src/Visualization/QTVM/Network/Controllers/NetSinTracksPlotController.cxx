#include "NetSinTracking.hxx"
#include "NetSinTracksPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSinTracksPlotController::NetSinTracksPlotController()
	    : mMonitor(0), 
	      _index(0)
	{
	    SetnSamples(100);
	    SetvRange(TData(0.0),TData(11025.0));
	    mSlotNewData.Wrap(this,&NetSinTracksPlotController::OnNewData);
	}

	NetSinTracksPlotController::~NetSinTracksPlotController()
	{
	}
	
	void NetSinTracksPlotController::SetData(const SpectralPeakArray& peaks)
	{
	    if(!peaks.GetMagBuffer().Size()) return;
	    if(CanGetData())
	    {
		SetCanSendData(false);
		if(First()) Init();
		AddData(peaks);
		SetCanSendData(true);
	    }
	}

	void NetSinTracksPlotController::Draw()
	{
	    if(CanSendData())
	    {
		SetCanGetData(false);
		NetSinTracking tracksBuilder;
		_renderer.SetData(tracksBuilder.GetTracks(_peakMtx,_index));
		SetCanGetData(true);
	    }
	    _renderer.Render();
	    NetPlotController::Draw();
	}

	void NetSinTracksPlotController::AddData(const SpectralPeakArray& data)
	{
	    if(!data.GetMagBuffer().Size()) return;
	    
	    if(_peakMtx.Size() < GetnSamples())
	    {
		_peakMtx.AddElem(data);
	    }
	    else
	    {
		_peakMtx[_index++] = data;
		if(_index == GetnSamples()) _index = 0;
	    }
	}

	void NetSinTracksPlotController::Init()
	{
	    _index=0;
	    SetFirst(false);
	    _peakMtx.Init();
	    FullView();
	}

	void NetSinTracksPlotController::FullView()
	{
	    _view.left=TData(0.0);
	    _view.right=TData(GetnSamples());
	    _view.top = GetvMax();
	    _view.bottom = GetvMin();

	    emit sendView(_view);
	}

	void NetSinTracksPlotController::SetMonitor(MonitorType & monitor)
	{
	    mMonitor = & monitor;
	    mMonitor->AttachStartSlot(mStartSlot);
	    mMonitor->AttachStopSlot(mStopSlot);
	    mMonitor->AttachSlotNewData(mSlotNewData);
	}

	void NetSinTracksPlotController::OnNewData()
	{
	    if(CanGetData())
	    {
		SetCanSendData(false);
		if(MonitorIsRunning())
		{
		    const SpectralPeakArray & peaks = mMonitor->FreezeAndGetData();
		    TSize bufferSize = peaks.GetMagBuffer().Size();
		    if(First() && bufferSize) Init();
		    if(bufferSize) AddData(peaks);
		    mMonitor->UnfreezeData();
		}
		SetCanSendData(true);
	    }
	}
    }
}

// END

