#include "NetFundPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetFundPlotController::NetFundPlotController() 
		{
		    SetDataColor(VMColor::Green());
		    SetvRange(TData(0.0),TData(1.0));
		    SetnSamples(22050);
		    _renderer.SetVBounds(TData(0.7),TData(0.0));
		    _renderer.SetHBounds(TData(0.0),TData(GetnSamples()));
		    mMonitor = 0;
		}

		NetFundPlotController::~NetFundPlotController()
		{
		}

		void NetFundPlotController::SetData(const Fundamental& data)
		{
		    if(CanGetData())
		    {
			SetCanSendData(false);
			_fund=data;
			if(First()) Init();
			SetCanSendData(true);
		    }
		}

		void NetFundPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = & monitor;
			mMonitor->AttachStartSlot(mStartSlot);
			mMonitor->AttachStopSlot(mStopSlot);
		}

		void NetFundPlotController::SetDataColor(Color c)
		{
			_renderer.SetColor(c);
		}

		void NetFundPlotController::Draw()
		{
		    if (!mMonitor)
		    {
			if(CanSendData())
			{
			   SetCanGetData(false);
			   _renderer.Update(_fund.GetFreq(0));
			   SetCanGetData(true);
			}
			_renderer.Render();
			NetPlotController::Draw();
			return;
		    }
		    
		    if(MonitorIsRunning())
		    {
			const Fundamental & fund = mMonitor->FreezeAndGetData();

			// TODO: Because we have exclusive right for
			// to the data we could remove some of this copies
			if(First()) Init();
			_renderer.Update(fund.GetFreq(0));
			_renderer.Render();
			NetPlotController::Draw();

			mMonitor->UnfreezeData();
		    }
		    else
		    {
			_renderer.Render();
			NetPlotController::Draw();
		    }
		}

		void NetFundPlotController::FullView()
		{
			_view.left = TData(0.0);
			_view.right = TData(GetnSamples());
			_view.top = GetvMax();
			_view.bottom = GetvMin();
			emit sendView(_view);
		}

	        void NetFundPlotController::Init()
		{
		    SetFirst(false);
		    FullView();
		}
	}
}

// END

