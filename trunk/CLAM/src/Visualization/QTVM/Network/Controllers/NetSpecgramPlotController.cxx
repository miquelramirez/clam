#include <algorithm>
#include "SpecTypeFlags.hxx"
#include "NetSpecgramPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSpecgramPlotController::NetSpecgramPlotController()
	    : _index(0),_specSize(0),_first(true)
	{
	    SetnSamples(200);
	}

	NetSpecgramPlotController::~NetSpecgramPlotController()
	{
	}

	void NetSpecgramPlotController::SetData(const Spectrum& spec)
	{
	    if(_first) Init(spec.GetMagBuffer().Size());
	    AddData(spec);
	    ProcessData();
	    FullView();
	}

	void NetSpecgramPlotController::Draw()
	{
	    _renderer.Render();
	}

	void NetSpecgramPlotController::AddData(const Spectrum& spec)
	{
	    _spec = spec;
	    AdaptSpectralData();
	    if(_cachedData.Size() < GetnSamples())
	    {
		TSize currentSize = _cachedData.Size();
		_cachedData.Resize(currentSize+1);
		_cachedData.SetSize(currentSize+1);
		_cachedData[currentSize].Resize(_specSize);
		_cachedData[currentSize].SetSize(_specSize);
		std::copy(_spec.GetMagBuffer().GetPtr(),_spec.GetMagBuffer().GetPtr()+_specSize,_cachedData[currentSize].GetPtr());
	    }
	    else
	    {
		std::copy(_spec.GetMagBuffer().GetPtr(),_spec.GetMagBuffer().GetPtr()+_specSize,_cachedData[_index++].GetPtr());
		if(_index == _cachedData.Size()) _index = 0;
	    }
	}

	void NetSpecgramPlotController::ProcessData()
	{
	    if(_cachedData.Size() < GetnSamples())
	    {
		_renderer.SetData(_cachedData);
	    }
	    else
	    {
		int i,j=0;
		for(i = _index; i < _cachedData.Size(); i++)
		{
		    std::copy(_cachedData[i].GetPtr(),_cachedData[i].GetPtr()+_specSize,_processedData[j++].GetPtr());
		}
		for(i = 0; i < _index; i++)
		{
		    std::copy(_cachedData[i].GetPtr(),_cachedData[i].GetPtr()+_specSize,_processedData[j++].GetPtr());
		}
		 _renderer.SetData(_processedData);
	    }
	}

	void NetSpecgramPlotController::FullView()
	{
	    _view.left=TData(0.0);
	    _view.right=TData(GetnSamples());
	    _view.bottom=GetvMin();
	    _view.top=GetvMax();
	    emit sendView(_view);
	}

	void NetSpecgramPlotController::AdaptSpectralData()
	{
	    SpecTypeFlags spFlags;
	    _spec.GetType(spFlags);

	    if(!spFlags.bMagPhase)
	    {
                 // check for conversions
		if(spFlags.bComplex)
		{
		    _spec.Complex2MagPhase();
		}
		else if(spFlags.bPolar)
		{
		    _spec.Polar2MagPhase();
		}
		else if(spFlags.bMagPhaseBPF)
		{
		    _spec.BPF2MagPhase();
		}
	    }
	    // convert to dB
	    _spec.ToDB();
	}

	void NetSpecgramPlotController::Init(const TSize& specSize)
	{
	    _specSize = specSize;
	    SetvRange(TData(0.0),TData(_specSize));
	    _cachedData.Init();
	    _processedData.Resize(GetnSamples());
	    _processedData.SetSize(GetnSamples());
	    for(int i = 0; i < _processedData.Size(); i++)
	    {
		_processedData[i].Resize(_specSize);
		_processedData[i].SetSize(_specSize);
	    }
	    _first = false;
	}

		
    }
}

// END


