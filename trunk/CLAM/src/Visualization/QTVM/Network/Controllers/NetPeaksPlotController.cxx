#include "NetPeaksPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
	namespace VM
	{
		NetPeaksPlotController::NetPeaksPlotController()
		{
			_linear = false;
			SetvRange(TData(-150.0),TData(0.0));
			SetnSamples(22050);
			_renderer.SetVBounds(TData(0.0),TData(-150.0));
			mMonitor = 0;
		}

		NetPeaksPlotController::~NetPeaksPlotController()
		{
		}

		void NetPeaksPlotController::SetData(const SpectralPeakArray& peaks)
		{
			_magBuffer = peaks.GetMagBuffer();
			_freqBuffer = peaks.GetFreqBuffer();
			_linear = (peaks.GetScale() == EScale::eLinear);
			ProcessPeakData();
			FullView();
		}

		void NetPeaksPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = & monitor;
		}

		void NetPeaksPlotController::SetPeaksColor(Color cline, Color cpoint)
		{
			_renderer.SetPeakColor(cline, cpoint);
		}

		void NetPeaksPlotController::Draw()
		{
			if (!mMonitor)
			{
				_renderer.Render();
				return;
			}
			const CLAM::SpectralPeakArray & peaks = mMonitor->FreezeAndGetData();

			// TODO: Because we have exclusive right for
			// to the data we could remove some of this copies
			_magBuffer = peaks.GetMagBuffer();
			_freqBuffer = peaks.GetFreqBuffer();
			_linear = (peaks.GetScale() == EScale::eLinear);
			ProcessPeakData();
			FullView();

			_renderer.Render();
			mMonitor->UnfreezeData();
		}

		void NetPeaksPlotController::FullView()
		{
			_view.left = TData(0.0);
			_view.right = TData(GetnSamples());
			_view.top = GetvMax();
			_view.bottom = GetvMin();
			emit sendView(_view);
		}

		void NetPeaksPlotController::ProcessPeakData()
		{
			TSize nPeaks = _magBuffer.Size();
			for(int i = 0; i < nPeaks; i++)
			{
				if(_linear) _magBuffer[i] = 20.0*log10(_magBuffer[i]);
			}
			_renderer.SetDataPtr(_magBuffer.GetPtr(), _freqBuffer.GetPtr(), _magBuffer.Size());
		}
	}
}

// END


