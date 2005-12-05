#include <algorithm>
#include "NetAudioBuffPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		NetAudioBuffPlotController::NetAudioBuffPlotController()
			: mMonitor(0)
			, mFrameSize(0)
			, mHasData(false)
			, mTooltip("")
			, mRenderingIsDone(false)
		{
			SetDataColor(VMColor::Green());
			SetvRange(TData(-1.0),TData(1.0));
			mSlotNewData.Wrap(this,&NetAudioBuffPlotController::OnNewData);
		}

		NetAudioBuffPlotController::~NetAudioBuffPlotController()
		{
		}

		void NetAudioBuffPlotController::SetData(const Audio& audio)
		{
			if(!audio.GetBuffer().Size()) return;
			if(CanGetData())
			{
				SetCanSendData(false);
				if(First()) Init(audio.GetBuffer().Size());
				AddData(audio.GetBuffer());
				SetCanSendData(true);
			}
		}

		void NetAudioBuffPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = &monitor;
			mMonitor->AttachStartSlot(mStartSlot);
			mMonitor->AttachStopSlot(mStopSlot);
			mMonitor->AttachSlotNewData(mSlotNewData);
		}

		void NetAudioBuffPlotController::SetDataColor(Color c)
		{
			mRenderer.SetColor(c);
		}

		void NetAudioBuffPlotController::Draw()
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

		void NetAudioBuffPlotController::AddData(const DataArray& data)
		{
			std::copy(mCachedData.GetPtr(),mCachedData.GetPtr()+(mCachedData.Size()-mFrameSize),
					  mCachedData.GetPtr()+mFrameSize);
			std::copy(data.GetPtr(),data.GetPtr()+mFrameSize,mCachedData.GetPtr());
		}

		void NetAudioBuffPlotController::Init(const TSize& frameSize)
		{
			mHasData=true;
			mFrameSize = frameSize;
			SetnSamples(mFrameSize*100);
			mCachedData.Init();
			mCachedData.Resize(GetnSamples());
			mCachedData.SetSize(GetnSamples());
			SetFirst(false);
			FullView();
		}

		void NetAudioBuffPlotController::FullView()
		{
			mView.left=TData(0.0);
			mView.right=TData(GetnSamples());
			mView.bottom=GetvMin();
			mView.top=GetvMax();
			emit sendView(mView);
		}

		void NetAudioBuffPlotController::OnNewData()
		{
			if(CanGetData())
			{
				SetCanSendData(false);
				if(MonitorIsRunning())
				{
					const Audio& audio = mMonitor->FreezeAndGetData();
					TSize bufferSize = audio.GetBuffer().Size();
					if(First() && bufferSize) Init(bufferSize);
					if(bufferSize) AddData(audio.GetBuffer());
					mMonitor->UnfreezeData();
				}
				SetCanSendData(true);
			}  
		}

		void NetAudioBuffPlotController::UpdatePoint(const TData& x, const TData& y)
		{
			NetPlotController::UpdatePoint(x,y);
			mTooltip="";
			if(mHasData)
			{
				mTooltip = "amplitude="+(mTooltip.setNum(y,'f',3));  
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


