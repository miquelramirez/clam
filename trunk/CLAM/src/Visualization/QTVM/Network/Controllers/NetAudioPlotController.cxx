#include "NetAudioPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetAudioPlotController::NetAudioPlotController() 
		{
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
		}

		void NetAudioPlotController::SetDataColor(Color c)
		{
			_dRenderer.SetColor(c);
		}

		void NetAudioPlotController::Draw()
		{
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
	}
}

// END

