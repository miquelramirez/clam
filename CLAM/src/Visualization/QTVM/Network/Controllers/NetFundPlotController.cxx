#include "NetFundPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetFundPlotController::NetFundPlotController() 
		    : mMonitor(0)
		    , mHasData(false)
			, mRenderingIsDone(false)
		    , mTooltip("")  
		{
		    SetDataColor(VMColor::Green());
		    SetvRange(TData(0.0),TData(1.0));
		    SetnSamples(22050);
		    mRenderer.SetVBounds(TData(0.0),TData(0.7));
		    mRenderer.SetHBounds(TData(0.0),TData(GetnSamples()));
		}

		NetFundPlotController::~NetFundPlotController()
		{
		}

		void NetFundPlotController::SetData(const Fundamental& data)
		{
		    if(CanGetData())
		    {
				SetCanSendData(false);
				mFund=data;
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
			mRenderer.SetColor(c);
		}

		void NetFundPlotController::Draw()
		{
		    if (!mMonitor)
		    {
				if(CanSendData())
				{
					SetCanGetData(false);
					mRenderer.Update(mFund.GetFreq(0));
					SetCanGetData(true);
				}
				mRenderer.Render();
				NetPlotController::Draw();
				mRenderingIsDone=true;
				return;
		    }
		    
		    if(MonitorIsRunning())
		    {
				const Fundamental & fund = mMonitor->FreezeAndGetData();

				// TODO: Because we have exclusive right for
				// to the data we could remove some of this copies
				if(First()) Init();
				mRenderer.Update(fund.GetFreq(0));
				mRenderer.Render();
				NetPlotController::Draw();
				mMonitor->UnfreezeData();
		    }
		    else
		    {
				mRenderer.Render();
				NetPlotController::Draw();
		    }
		    mRenderingIsDone=true;
		}

		void NetFundPlotController::FullView()
		{
			mView.left = TData(0.0);
			mView.right = TData(GetnSamples());
			mView.top = GetvMax();
			mView.bottom = GetvMin();
			emit sendView(mView);
		}

		void NetFundPlotController::Init()
		{
		    mHasData=true;
		    SetFirst(false);
		    FullView();
		}

		void NetFundPlotController::UpdatePoint(const TData& x, const TData& y)
		{
		    NetPlotController::UpdatePoint(x,y);
		    mTooltip="";
		    if(mHasData)
		    {
				mTooltip = "frequency="+(mTooltip.setNum(x,'f',0))+"Hz";  
		    }
		    if(mRenderingIsDone)
		    {
				mRenderingIsDone=false;
				emit toolTip(mTooltip);
		    }
		}
	}
}

// END

