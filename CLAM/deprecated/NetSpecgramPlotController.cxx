#include "SpecTypeFlags.hxx"
#include "NetSpecgramPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		NetSpecgramPlotController::NetSpecgramPlotController()
			: mMonitor(0)
			, mIndex(0)
			, mSpecSize(0)
			, mPalette(0.0f)
			, mLeftIndex1(0)
			, mRightIndex1(0)
			, mLeftIndex2(0) 
			, mRightIndex2(0)
			, mBottomBound(0) 
			, mTopBound(0)
			, mSpectralRange(TData(0.0))
			, mHasData(false)
			, mRenderingIsDone(false)
			, mTooltip("")
		{
			SetnSamples(99);
			mSlotNewData.Wrap(this,&NetSpecgramPlotController::OnNewData);
		}

		NetSpecgramPlotController::~NetSpecgramPlotController()
		{
		}

		void NetSpecgramPlotController::SetData(const Spectrum& spec)
		{
			if(!spec.GetMagBuffer().Size()) return;
			if(CanGetData())
			{
				SetCanSendData(false);
				if(First()) Init(spec.GetMagBuffer().Size(),spec.GetSpectralRange());
				AddData(spec);
				SetCanSendData(true);
			}
		}

		void NetSpecgramPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = & monitor;
			mMonitor->AttachStartSlot(mStartSlot);
			mMonitor->AttachStopSlot(mStopSlot);
			mMonitor->AttachSlotNewData(mSlotNewData);
		}

		void NetSpecgramPlotController::Draw()
		{
			if(CanSendData())
			{
				SetCanGetData(false);
				ComputeIndexes();
				mRenderer.SetIndexes(mLeftIndex1, mRightIndex1, mLeftIndex2, mRightIndex2, TIndex(mView.left));
				mRenderer.SetData(mCachedData,mBottomBound, mTopBound);
				SetCanGetData(true);
			}
			mRenderer.Render();
			NetPlotController::Draw();
			mRenderingIsDone=true;
		}

		void NetSpecgramPlotController::AddData(const Spectrum& spec)
		{
			mSpec = spec;
			AdaptSpectralData();
			std::vector<Color> v;
			for(int i = 0; i < mSpecSize; i++)
			{
				Color c;
				TIndex colorIndex = mPalette.Get( ClampToRange( mSpec.GetMagBuffer()[i]) );
				mPalette.GetRGBFromIndex( colorIndex, c.r, c.g, c.b);
				v.push_back(c);
			}
			if((int)mCachedData.size() < GetnSamples())
			{
				mCachedData.push_back(v);
			}
			else
			{
				mCachedData[mIndex++]=v;
				if(mIndex == (int)mCachedData.size()) mIndex = 0;
			}
		}

		void NetSpecgramPlotController::FullView()
		{
			mView.left=TData(0.0);
			mView.right=TData(GetnSamples());
			mView.bottom=GetvMin();
			mView.top=GetvMax();
			emit sendView(mView);
		}

		void NetSpecgramPlotController::AdaptSpectralData()
		{
			SpecTypeFlags spFlags;
			mSpec.GetType(spFlags);

			if(!spFlags.bMagPhase)
			{
				SpecTypeFlags newFlags;
				mSpec.SetTypeSynchronize(newFlags);
			}
			// convert to dB
			mSpec.ToDB();
		}

		void NetSpecgramPlotController::Init(const TSize& specSize, const TData& spectralRange)
		{
			mIndex=0;
			mHasData=true;
			mCachedData.clear();
			mSpectralRange=spectralRange;
			mSpecSize = specSize;
			SetvRange(TData(0.0),TData(mSpecSize-1));
			SetFirst(false);
			FullView();
		}

		float NetSpecgramPlotController::ClampToRange(TData value) const
		{
			if ( value > 0.0 ) // 0 dB is the maximum
				return 1.0f;
			if ( value < -100.0 ) // -100 dB is the minimum
				return 0.0f;

			value += 100.0f;
			value*= 0.01f;
	   
			return value;
		}	

		void NetSpecgramPlotController::OnNewData()
		{
			if(CanGetData())
			{
				SetCanSendData(false);
				if(MonitorIsRunning())
				{
					const Spectrum & spec = mMonitor->FreezeAndGetData();
					TSize bufferSize = spec.GetMagBuffer().Size();
					if(First() && bufferSize) Init(bufferSize,spec.GetSpectralRange());
					if(bufferSize) AddData(spec);
					mMonitor->UnfreezeData();
				}
				SetCanSendData(true);
			}
		}

		void NetSpecgramPlotController::ComputeIndexes()
		{
			mBottomBound=TIndex(mView.bottom);
			mTopBound=TIndex(mView.top);
			if(mTopBound < (TIndex)mSpecSize) mTopBound+=1;
	    
			unsigned width=unsigned(mView.right-mView.left);
			if(width < 4) width=4;
			if(width==unsigned(GetnSamples()))
			{
				mLeftIndex1=TIndex(mIndex);
				mRightIndex1=TIndex(mCachedData.size());
				mLeftIndex2=0;
				mRightIndex2=TIndex(mIndex);
				return;
			}

			unsigned left1=unsigned(mIndex)+unsigned(mView.left);
			unsigned left2=0;
			if(left1 > (unsigned)mCachedData.size())
			{
				left2=left1-unsigned(mCachedData.size());
				left1=unsigned(mCachedData.size());
			}
			unsigned rest=0;
			unsigned right1=left1+width+4;
			if(right1 > (unsigned)mCachedData.size())
			{
				rest=right1-unsigned(mCachedData.size());
				right1=unsigned(mCachedData.size());
			}
	   
			mLeftIndex1=TIndex(left1);
			mRightIndex1=TIndex(right1);
			mLeftIndex2=TIndex(left2);
			mRightIndex2=mLeftIndex2+TIndex(rest);
			if(mRightIndex2 > mLeftIndex1) mRightIndex2=mLeftIndex1;
	    
		}

		void NetSpecgramPlotController::UpdatePoint(const TData& x, const TData& y)
		{
			TData yvalue=y;
			NetPlotController::UpdatePoint(x,y);
			mTooltip="";
			if(mHasData)
			{
				yvalue*=mSpectralRange;
				yvalue/=GetvMax();
				mTooltip = "frequency="+(mTooltip.setNum(yvalue,'f',0))+"Hz";  
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


