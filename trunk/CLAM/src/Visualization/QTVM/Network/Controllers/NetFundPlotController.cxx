#include "NetFundPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetFundPlotController::NetFundPlotController() 
		{
			SetvRange(TData(0.0),TData(1.0));
			_renderer.SetVBounds(TData(0.7),TData(0.0));
			SetnSamples(22050);
			mMonitor = 0;
		}

		NetFundPlotController::~NetFundPlotController()
		{
		}

		void NetFundPlotController::SetData(const Fundamental& data)
		{
			_renderer.SetHBounds(TData(0.0),TData(GetnSamples()));
			_renderer.Update(data.GetFreq(0));
			FullView();
		}

		void NetFundPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = & monitor;
		}

		void NetFundPlotController::SetDataColor(Color c)
		{
			_renderer.SetColor(c);
		}

		void NetFundPlotController::Draw()
		{
			if (!mMonitor)
			{
				_renderer.Render();
				return;
			}
			const CLAM::Fundamental & fund = mMonitor->FreezeAndGetData();

			// TODO: Because we have exclusive right for
			// to the data we could remove some of this copies
			_renderer.SetHBounds(TData(0.0),TData(GetnSamples()));
			_renderer.Update(fund.GetFreq(0));
			FullView();

			_renderer.Render();

			mMonitor->UnfreezeData();
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

