#include <algorithm>
#include "NetAudioBuffPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetAudioBuffPlotController::NetAudioBuffPlotController()
	    : mMonitor(0),_index(0), _first(true), _frameSize(0)
	{
	    SetvRange(TData(-1.0),TData(1.0));
	}

	NetAudioBuffPlotController::~NetAudioBuffPlotController()
	{
	}

	void NetAudioBuffPlotController::SetData(const Audio& audio)
	{
	    if(_first) Init(audio.GetBuffer().Size());
	    AddData(audio.GetBuffer());
	    FullView();
	}

	void NetAudioBuffPlotController::SetMonitor(MonitorType & monitor)
	{
	    mMonitor = &monitor;
	}

	void NetAudioBuffPlotController::SetDataColor(Color c)
	{
	    _renderer.SetColor(c);
	}

	void NetAudioBuffPlotController::Draw()
	{
	    if(!mMonitor)
	    {
		_renderer.Render();
		return;
	    }

	    const Audio& audio = mMonitor->FreezeAndGetData();

	    // TODO: Because we have exclusive right for
	    // to the data we could remove some of this copies

	    if(_first && audio.GetBuffer().Size())
	    {
		Init(audio.GetBuffer().Size());
	    }

	    if(audio.GetBuffer().Size())
	    {
		AddData(audio.GetBuffer());
		FullView();

		_renderer.Render();
	    }

	    mMonitor->UnfreezeData();
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
	    _renderer.SetDataPtr(_cachedData.GetPtr(),_cachedData.Size(), (unsigned)_index);
	}

	void NetAudioBuffPlotController::Init(const TSize& frameSize)
	{
	    _frameSize = frameSize;
	    SetnSamples(_frameSize*100);
	    _cachedData.Init();
	    _first = false;
	}

	void NetAudioBuffPlotController::FullView()
	{
	    _view.left=TData(0.0);
	    _view.right=TData(GetnSamples());
	    _view.bottom=GetvMin();
	    _view.top=GetvMax();
	    emit sendView(_view);
	}

    }
}

// END


