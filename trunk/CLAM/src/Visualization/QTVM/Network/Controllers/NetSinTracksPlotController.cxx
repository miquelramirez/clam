#include "NetSinTracking.hxx"
#include "NetSinTracksPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSinTracksPlotController::NetSinTracksPlotController()
	    : mMonitor(0), 
	      _index(0),
	      _leftIndex1(0),
	      _rightIndex1(0),
	      _leftIndex2(0),
	      _rightIndex2(0),
	      _hasData(false),
	      _tooltip(""),
	      _renderingIsDone(false)
	{
	    SetnSamples(100);
	    SetvRange(TData(0.0),TData(11025.0));
	    mSlotNewData.Wrap(this,&NetSinTracksPlotController::OnNewData);
	    mSinTracking.Start();
	}

	NetSinTracksPlotController::~NetSinTracksPlotController()
	{
	     mSinTracking.Stop();
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
		ComputeIndexes();
		NetSinTracking tracksBuilder;
		tracksBuilder.SetIndexes(_leftIndex1,_rightIndex1,_leftIndex2,_rightIndex2,TIndex(_view.left));
		_renderer.SetData(tracksBuilder.GetTracks(_peakMtx));
		SetCanGetData(true);
	    }
	    _renderer.Render();
	    NetPlotController::Draw();
	    
	    _renderingIsDone=true;
	}

	void NetSinTracksPlotController::AddData(const SpectralPeakArray& data)
	{
	    SpectralPeakArray outPeaks;
	    mSinTracking.Do(data,outPeaks);
	    if(_peakMtx.Size() < GetnSamples())
	    {
		_peakMtx.AddElem(outPeaks);
	    }
	    else
	    {
		_peakMtx[_index++] = outPeaks;
		if(_index == GetnSamples()) _index = 0;
	    }
	}

	void NetSinTracksPlotController::Init()
	{
	    _hasData=true;
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

	void NetSinTracksPlotController::ComputeIndexes()
	{
	    unsigned width=unsigned(_view.right-_view.left);
	    if(width < 4) width=4;
	    if(width==unsigned(GetnSamples()))
	    {
		_leftIndex1=TIndex(_index);
		_rightIndex1=TIndex(_peakMtx.Size());
		_leftIndex2=0;
		_rightIndex2=TIndex(_index);
		return;
	    }

	    unsigned left1=unsigned(_index)+unsigned(_view.left);
	    unsigned left2=0;
	    if(left1 > (unsigned)_peakMtx.Size())
	    {
		left2=left1-unsigned(_peakMtx.Size());
		left1=unsigned(_peakMtx.Size());
	    }
	    unsigned rest=0;
	    unsigned right1=left1+width+4;
	    if(right1 > (unsigned)_peakMtx.Size())
	    {
		rest=right1-unsigned(_peakMtx.Size());
		right1=unsigned(_peakMtx.Size());
	    }
	   
	    _leftIndex1=TIndex(left1);
	    _rightIndex1=TIndex(right1);
	    _leftIndex2=TIndex(left2);
	    _rightIndex2=_leftIndex2+TIndex(rest);
	    if(_rightIndex2 > _leftIndex1) _rightIndex2=_leftIndex1;
	}

	void NetSinTracksPlotController::UpdatePoint(const TData& x, const TData& y)
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

