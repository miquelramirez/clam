#include <algorithm>
#include "NetAudioBuffPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetAudioBuffPlotController::NetAudioBuffPlotController()
	    : mMonitor(0),_index(0),_frameSize(0)
	{
	    SetDataColor(VMColor::Green());
	    SetvRange(TData(-1.0),TData(1.0));
	    mSlotNewData.Wrap(this,&NetAudioBuffPlotController::OnNewData);
	}

	NetAudioBuffPlotController::~NetAudioBuffPlotController()
	{
	}

	void NetAudioBuffPlotController::SetData(const Audio& audio)
	{
	    if(!audio.GetBuffer().Size()) return;
	    if(CanGetData())
	    {
		SetCanSendData(false);
		if(First()) Init(audio.GetBuffer().Size());
		AddData(audio.GetBuffer());
		SetCanSendData(true);
	    }
	}

	void NetAudioBuffPlotController::SetMonitor(MonitorType & monitor)
	{
	    mMonitor = &monitor;
	    mMonitor->AttachStartSlot(mStartSlot);
	    mMonitor->AttachStopSlot(mStopSlot);
	    mMonitor->AttachSlotNewData(mSlotNewData);
	}

	void NetAudioBuffPlotController::SetDataColor(Color c)
	{

	    _renderer.SetColor(c);
	}

	void NetAudioBuffPlotController::Draw()
	{ 
	    if(CanSendData())
	    {
		SetCanGetData(false);
		_renderer.SetDataPtr(_cachedData.GetPtr(),_cachedData.Size(), (unsigned)_index);
		SetCanGetData(true);
	    }
	    _renderer.Render();
	    NetPlotController::Draw();
	}

	void NetAudioBuffPlotController::AddData(const DataArray& data)
	{
	    if(_cachedData.Size() < GetnSamples())
	    {
		TSize currentSize = _cachedData.Size();
		_cachedData.Resize(currentSize+_frameSize);
		_cachedData.SetSize(currentSize+_frameSize);
		std::copy(data.GetPtr(),data.GetPtr()+_frameSize,_cachedData.GetPtr()+_index);
		_index += _frameSize;
		if(_index == GetnSamples()) _index = 0;
	    }
	    else
	    {
		std::copy(data.GetPtr(),data.GetPtr()+_frameSize,_cachedData.GetPtr()+_index);
		_index += _frameSize;
		if(_index == GetnSamples()) _index = 0;
	    }
	}

	void NetAudioBuffPlotController::Init(const TSize& frameSize)
	{
	    _index=0;
	    _frameSize = frameSize;
	    SetnSamples(_frameSize*100);
	    _cachedData.Init();
	    SetFirst(false);
	    FullView();
	}

	void NetAudioBuffPlotController::FullView()
	{
	    _view.left=TData(0.0);
	    _view.right=TData(GetnSamples());
	    _view.bottom=GetvMin();
	    _view.top=GetvMax();
	    emit sendView(_view);
	}

	void NetAudioBuffPlotController::OnNewData()
	{
	    if(CanGetData())
	    {
		SetCanSendData(false);
		if(MonitorIsRunning())
		{
		    const Audio& audio = mMonitor->FreezeAndGetData();
		    TSize bufferSize = audio.GetBuffer().Size();
		    if(First() && bufferSize) Init(bufferSize);
		    if(bufferSize) AddData(audio.GetBuffer());
		    mMonitor->UnfreezeData();
		}
		SetCanSendData(true);
	    }  
	}
    }
}

// END


