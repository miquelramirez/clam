#include "NetPeaksPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
	namespace VM
	{
		NetPeaksPlotController::NetPeaksPlotController()
		    : mMonitor(0)
			, mLinear(false)
		    , mHasData(false)
		    , mRenderingIsDone(false)
			, mTooltip("")
		{
		    SetPeaksColor(VMColor::Cyan(),VMColor::Red());
		    SetvRange(TData(-150.0),TData(0.0));
		    mRenderer.SetVBounds(TData(-150.0),TData(0.0));
		    SetnSamples(22050);
		}

		NetPeaksPlotController::~NetPeaksPlotController()
		{
		}

		void NetPeaksPlotController::SetData(const SpectralPeakArray& peaks)
		{
		    if(!peaks.GetMagBuffer().Size()) return;
		    if(CanGetData())
		    {
				SetCanSendData(false);
				mMagBuffer = peaks.GetMagBuffer();
				mFreqBuffer = peaks.GetFreqBuffer();
				mLinear = (peaks.GetScale() == EScale::eLinear);
				if(First()) Init();
				ProcessPeakData();
				SetCanSendData(true);
		    }
		}

		void NetPeaksPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = & monitor;
			mMonitor->AttachStartSlot(mStartSlot);
			mMonitor->AttachStopSlot(mStopSlot);
		}

		void NetPeaksPlotController::SetPeaksColor(Color cline, Color cpoint)
		{
			mRenderer.SetPeakColor(cline, cpoint);
		}

		void NetPeaksPlotController::Draw()
		{
			if(!mMonitor)
			{
			    if(CanSendData())
			    {
					SetCanGetData(false);
					mRenderer.SetDataPtr(mMagBuffer.GetPtr(), mFreqBuffer.GetPtr(), mMagBuffer.Size());
					SetCanGetData(true);
			    }
			    mRenderer.Render();
			    NetPlotController::Draw();
			    mRenderingIsDone=true;
			    return;
			}
			if(MonitorIsRunning())
			{
			    const CLAM::SpectralPeakArray & peaks = mMonitor->FreezeAndGetData();

			    // TODO: Because we have exclusive right for
			    // to the data we could remove some of this copies
			    mMagBuffer = peaks.GetMagBuffer();
			    mFreqBuffer = peaks.GetFreqBuffer();
			    mLinear = (peaks.GetScale() == EScale::eLinear);
			    if(First() && mMagBuffer.Size()) Init();
			    ProcessPeakData();
				mRenderer.SetDataPtr(mMagBuffer.GetPtr(), mFreqBuffer.GetPtr(), mMagBuffer.Size());
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

		void NetPeaksPlotController::FullView()
		{
			mView.left = TData(0.0);
			mView.right = TData(GetnSamples());
			mView.top = GetvMax();
			mView.bottom = GetvMin();
			emit sendView(mView);
		}

		void NetPeaksPlotController::ProcessPeakData()
		{
		    if(mLinear)
		    {
				TSize nPeaks = mMagBuffer.Size();
				for(int i = 0; i < nPeaks; i++)
				{
					mMagBuffer[i] = 20.0*log10(mMagBuffer[i]);
				}
		    }
		}

		void NetPeaksPlotController::Init()
		{
		    mHasData=true;
		    SetFirst(false);
		    FullView();
		}

		void NetPeaksPlotController::UpdatePoint(const TData& x, const TData& y)
		{
		    NetPlotController::UpdatePoint(x,y);
		    mTooltip="";
		    if(mHasData)
		    {
				mTooltip = "frequency="+(mTooltip.setNum(x,'f',0))+"Hz "+"magnitude="+(mTooltip.setNum(y,'f',0))+"dB";  
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


