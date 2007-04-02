#include "NetAudioPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetAudioPlotController::NetAudioPlotController() 
		    : mMonitor(0)
			, mHasData(false)
			, mRenderingIsDone(false)
			, mTooltip("")
		{
		    SetDataColor(VMColor::Green());
		    SetvRange(TData(-1.0),TData(1.0));
		}

		NetAudioPlotController::~NetAudioPlotController()
		{
		}

		void NetAudioPlotController::SetData(const Audio& audio)
		{
		    if(!audio.GetBuffer().Size()) return;
		    if(CanGetData())
		    {
				SetCanSendData(false);
				mCacheData=audio;
				if(First()) Init(mCacheData.GetBuffer().Size());
				SetCanSendData(true);
		    }
		}

		void NetAudioPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = & monitor;
			mMonitor->AttachStartSlot(mStartSlot);
			mMonitor->AttachStopSlot(mStopSlot);
		}

		void NetAudioPlotController::SetDataColor(Color c)
		{
			mRenderer.SetColor(c);
		}

		void NetAudioPlotController::Draw()
		{
			if (!mMonitor)
			{
			    if(CanSendData())
			    {
					SetCanGetData(false);
					mRenderer.SetDataPtr(mCacheData.GetBuffer().GetPtr(),mCacheData.GetBuffer().Size());
					SetCanGetData(true);
			    }
			    mRenderer.Render();
			    NetPlotController::Draw();
			    mRenderingIsDone=true;
			    
			    return;
			}
		      
			if(MonitorIsRunning())
			{
			    const Audio& audio = mMonitor->FreezeAndGetData();
			    
			    // TODO: Because we have exclusive right for
			    // to the data we could remove some of this copies

			    TSize audioSize = audio.GetBuffer().Size();
			    if(First() && audioSize) Init(audioSize);
			    mRenderer.SetDataPtr(audio.GetBuffer().GetPtr(),audioSize);

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

		void NetAudioPlotController::FullView()
		{
			mView.left = TData(0.0);
			mView.right = TData(GetnSamples());
			mView.top = GetvMax();
			mView.bottom = GetvMin();
			emit sendView(mView);
		}

		void NetAudioPlotController::Init(const TSize& frameSize)
		{
		    mHasData=true;
		    SetnSamples(frameSize);
		    SetFirst(false);
		    FullView();
		}

		void NetAudioPlotController::UpdatePoint(const TData& x, const TData& y)
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

