#include <algorithm>
#include "NetAudioBuffPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetAudioBuffPlotController::NetAudioBuffPlotController()
	    : _index(0), _first(true), _frameSize(0)
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
	    ProcessData();
	    FullView();
	}

	void NetAudioBuffPlotController::SetDataColor(Color c)
	{
	    _renderer.SetColor(c);
	}

	void NetAudioBuffPlotController::Draw()
	{
	    _renderer.Render();
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

	void NetAudioBuffPlotController::ProcessData()
	{
	    if(_cachedData.Size() < GetnSamples())
	    {
		_renderer.SetDataPtr(_cachedData.GetPtr(),_cachedData.Size(),NormalMode);
	    }
	    else
	    {
		TSize len = _cachedData.Size()-_index;
		std::copy(_cachedData.GetPtr()+_index,_cachedData.GetPtr()+_cachedData.Size(),_processedData.GetPtr());
		std::copy(_cachedData.GetPtr(),_cachedData.GetPtr()+_index,_processedData.GetPtr()+len);
		_renderer.SetDataPtr(_processedData.GetPtr(),_processedData.Size(),NormalMode);
	    }
	}

	void NetAudioBuffPlotController::Init(const TSize& frameSize)
	{
	    _frameSize = frameSize;
	    SetnSamples(_frameSize*100);
	    _cachedData.Init();
	    _processedData.Resize(GetnSamples());
	    _processedData.SetSize(GetnSamples());
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


