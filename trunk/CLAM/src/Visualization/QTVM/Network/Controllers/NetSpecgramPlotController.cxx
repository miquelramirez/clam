#include "SpecTypeFlags.hxx"
#include "NetSpecgramPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSpecgramPlotController::NetSpecgramPlotController()
	    : mMonitor(0),_index(0),_specSize(0),_palette(0.0f),
	      _leftIndex1(0), _rightIndex1(0), _leftIndex2(0), _rightIndex2(0),
	      _bottomBound(0), _topBound(0),
	      _spectralRange(TData(0.0)),
	      _hasData(false),
	      _tooltip(""),
	      _renderingIsDone(false)
	{
	    SetnSamples(100);
	    mSlotNewData.Wrap(this,&NetSpecgramPlotController::OnNewData);
	}

	NetSpecgramPlotController::~NetSpecgramPlotController()
	{
	}

	void NetSpecgramPlotController::SetData(const Spectrum& spec)
	{
	    if(!spec.GetMagBuffer().Size()) return;
	    if(CanGetData())
	    {
		SetCanSendData(false);
		if(First()) Init(spec.GetMagBuffer().Size(),spec.GetSpectralRange());
		AddData(spec);
		SetCanSendData(true);
	    }
	}

	void NetSpecgramPlotController::SetMonitor(MonitorType & monitor)
	{
	    mMonitor = & monitor;
	    mMonitor->AttachStartSlot(mStartSlot);
	    mMonitor->AttachStopSlot(mStopSlot);
	    mMonitor->AttachSlotNewData(mSlotNewData);
	}

	void NetSpecgramPlotController::Draw()
	{
	    if(CanSendData())
	    {
		SetCanGetData(false);
		ComputeIndexes();
		_renderer.SetIndexes(_leftIndex1, _rightIndex1, _leftIndex2, _rightIndex2, TIndex(_view.left));
		_renderer.SetData(_cachedData,_bottomBound, _topBound);
		SetCanGetData(true);
	    }
	    _renderer.Render();
	    NetPlotController::Draw();

	    _renderingIsDone=true;
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

	void NetSpecgramPlotController::Init(const TSize& specSize, const TData& spectralRange)
	{
	    _index=0;
	    _hasData=true;
	    _cachedData.clear();
	    _spectralRange=spectralRange;
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

	void NetSpecgramPlotController::OnNewData()
	{
	    if(CanGetData())
	    {
		SetCanSendData(false);
		if(MonitorIsRunning())
		{
		    const Spectrum & spec = mMonitor->FreezeAndGetData();
		    TSize bufferSize = spec.GetMagBuffer().Size();
		    if(First() && bufferSize) Init(bufferSize,spec.GetSpectralRange());
		    if(bufferSize) AddData(spec);
		    mMonitor->UnfreezeData();
		}
		SetCanSendData(true);
	    }
	}

	void NetSpecgramPlotController::ComputeIndexes()
	{
	   _bottomBound=TIndex(_view.bottom);
	    _topBound=TIndex(_view.top);
	    if(_topBound < (TIndex)_specSize) _topBound+=1;
	    
	    unsigned width=unsigned(_view.right-_view.left);
	    if(width < 4) width=4;
	    if(width==unsigned(GetnSamples()))
	    {
		_leftIndex1=TIndex(_index);
		_rightIndex1=TIndex(_cachedData.size());
		_leftIndex2=0;
		_rightIndex2=TIndex(_index);
		return;
	    }

	    unsigned left1=unsigned(_index)+unsigned(_view.left);
	    unsigned left2=0;
	    if(left1 > (unsigned)_cachedData.size())
	    {
		left2=left1-unsigned(_cachedData.size());
		left1=unsigned(_cachedData.size());
	    }
	    unsigned rest=0;
	    unsigned right1=left1+width+4;
	    if(right1 > (unsigned)_cachedData.size())
	    {
		rest=right1-unsigned(_cachedData.size());
		right1=unsigned(_cachedData.size());
	    }
	   
	    _leftIndex1=TIndex(left1);
	    _rightIndex1=TIndex(right1);
	    _leftIndex2=TIndex(left2);
	    _rightIndex2=_leftIndex2+TIndex(rest);
	    if(_rightIndex2 > _leftIndex1) _rightIndex2=_leftIndex1;
	    
	}

	void NetSpecgramPlotController::UpdatePoint(const TData& x, const TData& y)
	{
	     TData yvalue=y;
	     NetPlotController::UpdatePoint(x,y);
	     _tooltip="";
	     if(_hasData)
	     {
		 yvalue*=_spectralRange;
		 yvalue/=GetvMax();
		 _tooltip = "frequency="+(_tooltip.setNum(yvalue,'f',0))+"Hz";  
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


