#include "NetSinTracking.hxx"
#include "NetSinTracksPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		NetSinTracksPlotController::NetSinTracksPlotController()
			: mMonitor(0) 
			, mIndex(0)
			, mLeftIndex1(0)
			, mRightIndex1(0)
			, mLeftIndex2(0)
			, mRightIndex2(0)
			, mHasData(false)
			, mRenderingIsDone(false)
			, mTooltip("")
		{
			SetnSamples(100);
			SetvRange(TData(0.0),TData(11025.0));
			mSlotNewData.Wrap(this,&NetSinTracksPlotController::OnNewData);
			mSinTracking.Start();
		}

		NetSinTracksPlotController::~NetSinTracksPlotController()
		{
			mSinTracking.Stop();
		}
	
		void NetSinTracksPlotController::SetData(const SpectralPeakArray& peaks)
		{
			if(!peaks.GetMagBuffer().Size()) return;
			if(CanGetData())
			{
				SetCanSendData(false);
				if(First()) Init();
				AddData(peaks);
				SetCanSendData(true);
			}
		}

		void NetSinTracksPlotController::Draw()
		{
			if(CanSendData())
			{
				SetCanGetData(false);
				ComputeIndexes();
				NetSinTracking tracksBuilder;
				tracksBuilder.SetIndexes(mLeftIndex1,mRightIndex1,mLeftIndex2,mRightIndex2,TIndex(mView.left));
				mRenderer.SetData(tracksBuilder.GetTracks(mPeakMtx));
				SetCanGetData(true);
			}
			mRenderer.Render();
			NetPlotController::Draw();
			mRenderingIsDone=true;
		}

		void NetSinTracksPlotController::AddData(const SpectralPeakArray& data)
		{
			SpectralPeakArray outPeaks;
			mSinTracking.Do(data,outPeaks);
			if(mPeakMtx.Size() < GetnSamples())
			{
				mPeakMtx.AddElem(outPeaks);
			}
			else
			{
				mPeakMtx[mIndex++] = outPeaks;
				if(mIndex == GetnSamples()) mIndex = 0;
			}
		}

		void NetSinTracksPlotController::Init()
		{
			mHasData=true;
			mIndex=0;
			SetFirst(false);
			mPeakMtx.Init();
			FullView();
		}

		void NetSinTracksPlotController::FullView()
		{
			mView.left=TData(0.0);
			mView.right=TData(GetnSamples());
			mView.top = GetvMax();
			mView.bottom = GetvMin();
			emit sendView(mView);
		}

		void NetSinTracksPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = & monitor;
			mMonitor->AttachStartSlot(mStartSlot);
			mMonitor->AttachStopSlot(mStopSlot);
			mMonitor->AttachSlotNewData(mSlotNewData);
		}

		void NetSinTracksPlotController::OnNewData()
		{
			if(CanGetData())
			{
				SetCanSendData(false);
				if(MonitorIsRunning())
				{
					const SpectralPeakArray & peaks = mMonitor->FreezeAndGetData();
					TSize bufferSize = peaks.GetMagBuffer().Size();
					if(First() && bufferSize) Init();
					if(bufferSize) AddData(peaks);
					mMonitor->UnfreezeData();
				}
				SetCanSendData(true);
			}
		}

		void NetSinTracksPlotController::ComputeIndexes()
		{
			unsigned width=unsigned(mView.right-mView.left);
			if(width < 4) width=4;
			if(width==unsigned(GetnSamples()))
			{
				mLeftIndex1=TIndex(mIndex);
				mRightIndex1=TIndex(mPeakMtx.Size());
				mLeftIndex2=0;
				mRightIndex2=TIndex(mIndex);
				return;
			}

			unsigned left1=unsigned(mIndex)+unsigned(mView.left);
			unsigned left2=0;
			if(left1 > (unsigned)mPeakMtx.Size())
			{
				left2=left1-unsigned(mPeakMtx.Size());
				left1=unsigned(mPeakMtx.Size());
			}
			unsigned rest=0;
			unsigned right1=left1+width+4;
			if(right1 > (unsigned)mPeakMtx.Size())
			{
				rest=right1-unsigned(mPeakMtx.Size());
				right1=unsigned(mPeakMtx.Size());
			}
	   
			mLeftIndex1=TIndex(left1);
			mRightIndex1=TIndex(right1);
			mLeftIndex2=TIndex(left2);
			mRightIndex2=mLeftIndex2+TIndex(rest);
			if(mRightIndex2 > mLeftIndex1) mRightIndex2=mLeftIndex1;
		}

		void NetSinTracksPlotController::UpdatePoint(const TData& x, const TData& y)
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

