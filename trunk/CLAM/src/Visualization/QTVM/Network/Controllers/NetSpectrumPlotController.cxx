#include "SpecTypeFlags.hxx"
#include "NetSpectrumPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetSpectrumPlotController::NetSpectrumPlotController()
		{
		    SetDataColor(VMColor::Green());
		    SetvRange(TData(-150.0),TData(0.0));
		    mMonitor = 0;
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

			mMonitor->UnfreezeData();
		    }
		    else
		    {
			_renderer.Render();
		    }
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
		    SetnSamples(size);
		    SetFirst(false);
		    FullView();
		}
	}
}

// END

