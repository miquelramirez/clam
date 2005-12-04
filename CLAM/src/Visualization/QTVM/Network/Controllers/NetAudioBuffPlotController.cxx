#include <algorithm>
#include "NetAudioBuffPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		NetAudioBuffPlotController::NetAudioBuffPlotController()
			: mMonitor(0)
			, mIndex(0)
			, mFrameSize(0)
			, mLeftIndex1(0)
			, mRightIndex1(0)
			, mLeftIndex2(0)
			, mRightIndex2(0)
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
				ComputeIndexes();
				mRenderer.SetIndexes(mLeftIndex1,mRightIndex1,mLeftIndex2,mRightIndex2,TIndex(mView.left));
				mRenderer.SetData(mCachedData);
				SetCanGetData(true);
			}
			mRenderer.Render();
			NetPlotController::Draw();
			mRenderingIsDone=true;
		}

		void NetAudioBuffPlotController::AddData(const DataArray& data)
		{
			if(mCachedData.Size() < GetnSamples())
			{
				TSize currentSize = mCachedData.Size();
				mCachedData.Resize(currentSize+mFrameSize);
				mCachedData.SetSize(currentSize+mFrameSize);
				std::copy(data.GetPtr(),data.GetPtr()+mFrameSize,mCachedData.GetPtr()+mIndex);
				mIndex += mFrameSize;
				if(mIndex == GetnSamples()) mIndex = 0;
			}
			else
			{
				std::copy(data.GetPtr(),data.GetPtr()+mFrameSize,mCachedData.GetPtr()+mIndex);
				mIndex += mFrameSize;
				if(mIndex == GetnSamples()) mIndex = 0;
			}
		}

		void NetAudioBuffPlotController::Init(const TSize& frameSize)
		{
			mHasData=true;
			mIndex=0;
			mFrameSize = frameSize;
			SetnSamples(mFrameSize*100);
			mCachedData.Init();
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

		void NetAudioBuffPlotController::ComputeIndexes()
		{
			unsigned width=unsigned(mView.right-mView.left);
			if(width < 512) width=512;
			if(width==unsigned(GetnSamples()))
			{
				mLeftIndex1=TIndex(mIndex);
				mRightIndex1=TIndex(mCachedData.Size());
				mLeftIndex2=0;
				mRightIndex2=TIndex(mIndex);
				return;
			}

			unsigned left1=unsigned(mIndex)+unsigned(mView.left);
			unsigned left2=0;
			if(left1 > (unsigned)mCachedData.Size())
			{
				left2=left1-unsigned(mCachedData.Size());
				left1=unsigned(mCachedData.Size());
			}
			unsigned rest=0;
			unsigned right1=left1+width+4;
			if(right1 > (unsigned)mCachedData.Size())
			{
				rest=right1-unsigned(mCachedData.Size());
				right1=unsigned(mCachedData.Size());
			}
	   
			mLeftIndex1=TIndex(left1);
			mRightIndex1=TIndex(right1);
			mLeftIndex2=TIndex(left2);
			mRightIndex2=mLeftIndex2+TIndex(rest);
			if(mRightIndex2 > mLeftIndex1) mRightIndex2=mLeftIndex1;
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


