#include <algorithm>
#include "NetFundTrackPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		NetFundTrackPlotController::NetFundTrackPlotController()
			: mMonitor(0) 
			, mHasData(false)
			, mRenderingIsDone(false)
			, mTooltip("")
		{
			SetDataColor(VMColor::Green());
			mSlotNewData.Wrap(this,&NetFundTrackPlotController::OnNewData);
		}

		NetFundTrackPlotController::~NetFundTrackPlotController()
		{
		}

		void NetFundTrackPlotController::SetData(const Fundamental& fund)
		{
			if(CanGetData())
			{
				SetCanSendData(false);
				if(First()) Init();
				AddData(fund.GetFreq(0));
				SetCanSendData(true);
			}
		}

		void NetFundTrackPlotController::SetDataColor(Color c)
		{
			mRenderer.SetColor(c);
		}

		void NetFundTrackPlotController::Draw()
		{
			if(CanSendData())
			{
				SetCanGetData(false);
				mRenderer.SetDataPtr(mCachedData.GetPtr(),mCachedData.Size());
				SetCanGetData(true);
			}
			mRenderer.Render();
			NetPlotController::Draw();
			mRenderingIsDone=true;
		}

		void NetFundTrackPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = & monitor;
			mMonitor->AttachStartSlot(mStartSlot);
			mMonitor->AttachStopSlot(mStopSlot);
			mMonitor->AttachSlotNewData(mSlotNewData);
		}

		void NetFundTrackPlotController::AddData(const TData& data)
		{
			if(mCachedData.Size() < GetnSamples())
			{
				mCachedData.AddElem(data);
			}
			else
			{
				std::copy(mCachedData.GetPtr(),mCachedData.GetPtr()+mCachedData.Size()-1,mCachedData.GetPtr()+1);
				mCachedData[0] = data;
			}
		}

		void NetFundTrackPlotController::Init()
		{
			mHasData=true;
			mCachedData.Init();
			SetvRange(TData(0.0),TData(2050.0));
			SetnSamples(100); 
			SetFirst(false);
			FullView();
		}

		void NetFundTrackPlotController::FullView()
		{
			mView.left=TData(0.0);
			mView.right=TData(GetnSamples());
			mView.bottom=GetvMin();
			mView.top=GetvMax();
			emit sendView(mView);
		}

		void NetFundTrackPlotController::OnNewData()
		{
			if(CanGetData())
			{
				SetCanSendData(false);
				if(MonitorIsRunning())
				{
					const Fundamental & fund = mMonitor->FreezeAndGetData();

					if(First()) Init();
					AddData(fund.GetFreq(0));	
					mMonitor->UnfreezeData();
				}
				SetCanSendData(true);
			}
		}

		void NetFundTrackPlotController::UpdatePoint(const TData& x, const TData& y)
		{
			NetPlotController::UpdatePoint(x,y);
			mTooltip="";
			if(mHasData)
			{
				mTooltip = "frequency="+(mTooltip.setNum(y,'f',0))+"Hz";  
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


