#include "SpecTypeFlags.hxx"
#include "NetSpectrumPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetSpectrumPlotController::NetSpectrumPlotController()
		    : mMonitor(0),
		      _spectralRange(TData(0.0)),
		      _hasData(false),
		      _tooltip(""),
		      _renderingIsDone(false)
		{
		    SetDataColor(VMColor::Green());
		    SetvRange(TData(-150.0),TData(0.0));
		}

		NetSpectrumPlotController::~NetSpectrumPlotController()
		{
		}

		void NetSpectrumPlotController::SetData(const Spectrum& spec)
		{
		    if(!spec.GetMagBuffer().Size()) return;
		    if(CanGetData())
		    {
			SetCanSendData(false);
			_spec = spec;
			if(First()) Init(_spec.GetMagBuffer().Size());
			AdaptSpectralData();
			SetCanSendData(true);
		    }
		}

		void NetSpectrumPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = & monitor;
			mMonitor->AttachStartSlot(mStartSlot);
			mMonitor->AttachStopSlot(mStopSlot);
		}

		void NetSpectrumPlotController::SetDataColor(Color c)
		{
			_renderer.SetColor(c);
		}

		void NetSpectrumPlotController::Draw()
		{
		    if (!mMonitor)
		    {
			if(CanSendData())
			{
			    SetCanGetData(false);
			    _renderer.SetDataPtr(_spec.GetMagBuffer().GetPtr(), _spec.GetMagBuffer().Size(), NormalMode);
			    SetCanGetData(true);
			}
			_renderer.Render();
			NetPlotController::Draw();

			_renderingIsDone=true;

			return;
		    }

		    if(MonitorIsRunning())
		    {
			const Spectrum & spectrum = mMonitor->FreezeAndGetData();

			// TODO: Because we have exclusive right for
			// to the data we could remove some of this copies

			_spec = spectrum;
			TSize specSize = _spec.GetMagBuffer().Size();
			if(First() && specSize) Init(specSize);
			AdaptSpectralData();
			_renderer.SetDataPtr(_spec.GetMagBuffer().GetPtr(), _spec.GetMagBuffer().Size(), NormalMode);
			_renderer.Render();
			NetPlotController::Draw();

			mMonitor->UnfreezeData();
		    }
		    else
		    {
			_renderer.Render();
			NetPlotController::Draw();
		    }

		    _renderingIsDone=true;
		}

		void NetSpectrumPlotController::FullView()	
		{
			_view.left = TData(0.0);
			_view.right = TData(GetnSamples());
			_view.top = GetvMax();
			_view.bottom = GetvMin();
			emit sendView(_view);
		}

		void NetSpectrumPlotController::AdaptSpectralData()
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

	        void NetSpectrumPlotController::Init(const TSize& size)
		{
		    _hasData=true;
		    _spectralRange=_spec.GetSpectralRange();
		    SetnSamples(size);
		    SetFirst(false);
		    FullView();
		}

	        void NetSpectrumPlotController::UpdatePoint(const TData& x, const TData& y)
		{
		    TData xvalue=x;
		    NetPlotController::UpdatePoint(x,y);
		    _tooltip="";
		    if(_hasData)
		    {
			xvalue*=_spectralRange;
			xvalue/=TData(GetnSamples());
			_tooltip = "frequency="+(_tooltip.setNum(xvalue,'f',0))+"Hz "+"magnitude="+(_tooltip.setNum(y,'f',0))+"dB";  
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

