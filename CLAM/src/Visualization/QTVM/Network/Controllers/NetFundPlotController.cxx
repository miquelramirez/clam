#include "NetFundPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetFundPlotController::NetFundPlotController() 
		{
		    SetvRange(TData(0.0),TData(1.0));
		    _renderer.SetVBounds(TData(0.7),TData(0.0));
		}

		NetFundPlotController::~NetFundPlotController()
		{
		}

		void NetFundPlotController::SetData(const Fundamental& data)
		{
		    SetnSamples(22050);
		    _renderer.SetHBounds(TData(0.0),TData(GetnSamples()));
		    _renderer.Update(data.GetFreq(0));
		    FullView();
		}

		void NetFundPlotController::SetDataColor(Color c)
		{
			_renderer.SetColor(c);
		}

		void NetFundPlotController::Draw()
		{
			_renderer.Render();
		}

		void NetFundPlotController::FullView()
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

