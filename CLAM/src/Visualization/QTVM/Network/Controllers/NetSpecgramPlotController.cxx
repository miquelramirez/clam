#include "SpecTypeFlags.hxx"
#include "NetSpecgramPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSpecgramPlotController::NetSpecgramPlotController()
	    : mMonitor(0),_index(0),_specSize(0),_palette(0.0f)
	{
	    SetnSamples(100);
	}

	NetSpecgramPlotController::~NetSpecgramPlotController()
	{
	}

	void NetSpecgramPlotController::SetData(const Spectrum& spec)
	{
	    if(!spec.GetMagBuffer().Size()) return;
	    if(First()) Init(spec.GetMagBuffer().Size());
	    AddData(spec);
	}

	void NetSpecgramPlotController::SetMonitor(MonitorType & monitor)
	{
	    mMonitor = & monitor;
	    mMonitor->AttachStartSlot(mStartSlot);
	    mMonitor->AttachStopSlot(mStopSlot);
	}

	void NetSpecgramPlotController::Draw()
	{
	    if(!mMonitor)
	    {
		_renderer.Render();
		return;
	    }

	    if(MonitorIsRunning())
	    {
		const Spectrum & spec = mMonitor->FreezeAndGetData();

		// TODO: Because we have exclusive right for
		// to the data we could remove some of this copies

	    
		if(First() && spec.GetMagBuffer().Size()) 
		{
		    Init(spec.GetMagBuffer().Size());
		}

		if(spec.GetMagBuffer().Size())
		{
		    AddData(spec);
		
		    _renderer.Render();
		}

		mMonitor->UnfreezeData();
	    }
	    else
	    {
		_renderer.Render();
	    }
	}

	void NetSpecgramPlotController::AddData(const Spectrum& spec)
	{
	    _spec = spec;
	    AdaptSpectralData();
	    std::vector<Color> v;
	    for(int i = 0; i < _specSize; i++)
	    {
		Color c;
		TIndex colorIndex = _palette.Get( ClampToRange( _spec.GetMagBuffer()[i]) );
		_palette.GetRGBFromIndex( colorIndex, c.r, c.g, c.b);
		v.push_back(c);
	    }
	    if((int)_cachedData.size() < GetnSamples())
	    {
		_cachedData.push_back(v);
	    }
	    else
	    {
		_cachedData[_index++]=v;
		if(_index == (int)_cachedData.size()) _index = 0;
	    }
	    _renderer.SetData(_cachedData,_index);
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
	    _index=0;
	    _cachedData.clear();
	    _specSize = specSize;
	    SetvRange(TData(0.0),TData(_specSize));
	    SetFirst(false);
	    FullView();
	}

	float NetSpecgramPlotController::ClampToRange(TData value) const
	{
	    if ( value > 0.0 ) // 0 dB is the maximum
		return 1.0f;
	    if ( value < -100.0 ) // -100 dB is the minimum
		return 0.0f;

	    value += 100.0f;
	    value*= 0.01f;
	   
	    return value;
	}	
    }
}

// END


