#include "NetAudioPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetAudioPlotController::NetAudioPlotController() 
		{
			SetvRange(TData(-1.0),TData(1.0));
			mMonitor = 0;
		}

		NetAudioPlotController::~NetAudioPlotController()
		{
		}

		void NetAudioPlotController::SetData(const Audio& audio)
		{
			TSize audioSize = audio.GetBuffer().Size();
			SetnSamples(audioSize);
			_dRenderer.SetDataPtr(audio.GetBuffer().GetPtr(),audioSize,NormalMode);
			FullView();
		}

		void NetAudioPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = & monitor;
		}

		void NetAudioPlotController::SetDataColor(Color c)
		{
			_dRenderer.SetColor(c);
		}

		void NetAudioPlotController::Draw()
		{
			if (!mMonitor)
			{
				_dRenderer.Render();
				return;
			}
		      
			const Audio& audio = mMonitor->FreezeAndGetData();

			// TODO: Because we have exclusive right for
			// to the data we could remove some of this copies

			TSize audioSize = audio.GetBuffer().Size();
			SetnSamples(audioSize);
			_dRenderer.SetDataPtr(audio.GetBuffer().GetPtr(),audioSize,NormalMode);
			FullView();

			_dRenderer.Render();

			mMonitor->UnfreezeData();
		}

		void NetAudioPlotController::FullView()
		{
			_view.left = TData(0.0);
			_view.right = TData(GetnSamples());
			_view.top = GetvMax();
			_view.bottom = GetvMin();
			emit sendView(_view);
		}
	}
}

// END

