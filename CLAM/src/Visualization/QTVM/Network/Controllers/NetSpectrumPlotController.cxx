#include "SpecTypeFlags.hxx"
#include "NetSpectrumPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetSpectrumPlotController::NetSpectrumPlotController()
		{
			_mustProcessData = false;
			SetvRange(TData(-150.0),TData(0.0));
		}

		NetSpectrumPlotController::~NetSpectrumPlotController()
		{
		}
		void NetSpectrumPlotController::SetData(const Spectrum& spec)
		{
			_spec = spec;
			CacheData();
			FullView();
			_mustProcessData = true;

			emit requestRefresh();
		}

		void NetSpectrumPlotController::SetDataColor(Color c)
		{
			_renderer.SetColor(c);
		}

		void NetSpectrumPlotController::Draw()
		{
			if(_mustProcessData) ProcessData();
			_renderer.Render();
		}

		void NetSpectrumPlotController::FullView()
		{
			_view.left = TData(0.0);
			_view.right = TData(_magBuffer.Size());
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

		void NetSpectrumPlotController::CacheData()
		{
			AdaptSpectralData();
			_magBuffer=_spec.GetMagBuffer();
		}

		void NetSpectrumPlotController::ProcessData()
		{
			_renderer.SetDataPtr(_magBuffer.GetPtr(),_magBuffer.Size(),NormalMode);	
			_mustProcessData = false;
		}
	}
}

// END

