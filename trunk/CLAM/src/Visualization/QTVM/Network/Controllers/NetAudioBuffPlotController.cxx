#include <algorithm>
#include "NetAudioBuffPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetAudioBuffPlotController::NetAudioBuffPlotController()
	    : mMonitor(0),
	      _index(0),
	      _frameSize(0),
	      _leftIndex1(0),
	      _rightIndex1(0),
	      _leftIndex2(0),
	      _rightIndex2(0),
	      _hasData(false),
	      _tooltip(""),
	      _renderingIsDone(false)
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
		ComputeIndexes();
		_renderer.SetIndexes(_leftIndex1,_rightIndex1,_leftIndex2,_rightIndex2,TIndex(_view.left));
		_renderer.SetDataPtr(_cachedData.GetPtr());
		SetCanGetData(true);
	    }
	    _renderer.Render();
	    NetPlotController::Draw();

	    _renderingIsDone=true;
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
	    _hasData=true;
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

	void NetAudioBuffPlotController::ComputeIndexes()
	{
	    unsigned width=unsigned(_view.right-_view.left);
	    if(width < 512) width=512;
	    if(width==unsigned(GetnSamples()))
	    {
		_leftIndex1=TIndex(_index);
		_rightIndex1=TIndex(_cachedData.Size());
		_leftIndex2=0;
		_rightIndex2=TIndex(_index);
		return;
	    }

	    unsigned left1=unsigned(_index)+unsigned(_view.left);
	    unsigned left2=0;
	    if(left1 > (unsigned)_cachedData.Size())
	    {
		left2=left1-unsigned(_cachedData.Size());
		left1=unsigned(_cachedData.Size());
	    }
	    unsigned rest=0;
	    unsigned right1=left1+width+4;
	    if(right1 > (unsigned)_cachedData.Size())
	    {
		rest=right1-unsigned(_cachedData.Size());
		right1=unsigned(_cachedData.Size());
	    }
	   
	    _leftIndex1=TIndex(left1);
	    _rightIndex1=TIndex(right1);
	    _leftIndex2=TIndex(left2);
	    _rightIndex2=_leftIndex2+TIndex(rest);
	    if(_rightIndex2 > _leftIndex1) _rightIndex2=_leftIndex1;
	}

	 void NetAudioBuffPlotController::UpdatePoint(const TData& x, const TData& y)
	 {
	     NetPlotController::UpdatePoint(x,y);
	     _tooltip="";
	     if(_hasData)
	     {
		 _tooltip = "amplitude="+(_tooltip.setNum(y,'f',3));  
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


