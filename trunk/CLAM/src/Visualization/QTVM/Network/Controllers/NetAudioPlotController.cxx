#include "CLAMGL.hxx"
#include "NetAudioPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetAudioPlotController::NetAudioPlotController() 
		{
			_mustProcessData = false;
			SetvRange(TData(-1.0),TData(1.0));
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
			_mustProcessData = true;
			emit requestRefresh();
		}

		void NetAudioPlotController::SetDataColor(Color c)
		{
			_dRenderer.SetColor(c);
		}

		void NetAudioPlotController::Draw()
		{
			if(_mustProcessData) ProcessData();
			_dRenderer.Render();
		}

		void NetAudioPlotController::FullView()
		{
			_view.left = TData(0.0);
			_view.right = TData(GetnSamples());
			_view.top = GetvMax();
			_view.bottom = GetvMin();
	
			emit sendView(_view);
		}

		void NetAudioPlotController::ProcessData()
		{
			_mustProcessData = false;
		}
	}
}

// END

