#include "SpecTypeFlags.hxx"
#include "NetSpecgramPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSpecgramPlotController::NetSpecgramPlotController()
	    : _index(0)
	{
	    SetnSamples(200);
	    InitDataArrays();
	}

	NetSpecgramPlotController::~NetSpecgramPlotController()
	{
	}

	void NetSpecgramPlotController::SetData(const Spectrum& spec)
	{
	    if(!spec.HasMagBuffer()) return;
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
	    TSize specSize = _spec.GetMagBuffer().Size();
	    if(GetvMax() < specSize) SetvRange(TData(0.0),TData(specSize));
	    AdaptSpectralData();
	    if(_cachedData.size() < GetnSamples())
	    {
		_cachedData.push_back(_spec);
	    }
	    else
	    {
		_cachedData[_index++] = _spec;
		if(_index == GetnSamples()) _index = 0;
	    }
	}

	void NetSpecgramPlotController::ProcessData()
	{
	    if(_cachedData.size() < GetnSamples())
	    {
		_renderer.SetData(_cachedData);
	    }
	    else
	    {
		int i,j = 0;
		for(i = _index; i < _cachedData.size(); i++)
		    _processedData[j++]=_cachedData[i];
		for(i = 0; i < _index; i++)
		    _processedData[j++] = _cachedData[i];
		 _renderer.SetData(_processedData);
	    }
	}

	void NetSpecgramPlotController::InitDataArrays()
	{
	    _processedData.resize(GetnSamples());
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

		
    }
}

// END


