/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "FundPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		FundPlotController::FundPlotController()
			: mMustProcessData(false)
			, mMaxFreq(TData(0.0))
		{
			SetVMin(10.0);
		}

		FundPlotController::~FundPlotController()
		{
		}

		void FundPlotController::SetData(const Segment& segment)
		{
			mSegment = segment;
			mSampleRate = mSegment.GetSamplingRate();
			mDuration = mSegment.GetEndTime()-mSegment.GetBeginTime();
			CacheData();
			FullView();
			SetnSamples(TSize(mDuration*mSampleRate));
			SetvRange(TData(mMaxFreq));
			InitialRegionTime();
			mMustProcessData = true;
			SetSelPos(TData(0.0));
			emit requestRefresh();
		}

		void FundPlotController::SetDataColor(Color c)
		{
			mRenderer.SetColor(c);
		}

		void FundPlotController::SetHBounds(const TData& left,const TData& right)
		{
			SelTimeRegionPlotController::SetHBounds(left,right);
			mMustProcessData = true;
			
			double lBound = double(GetLeftBound()/mSampleRate);
			double hBound = double(GetRightBound()/mSampleRate);
			
			emit xRulerRange(lBound,hBound);	
		}

		void FundPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			SelTimeRegionPlotController::SetVBounds(bottom,top);
			
			double bBound = double(GetBottomBound());
			double tBound = double(GetTopBound());
	    
			emit yRulerRange(bBound,tBound);
		}

		void FundPlotController::SurfaceDimensions(int w,int h)
		{
			PlotController::SurfaceDimensions(w,h);
		
			double lBound = double(GetLeftBound()/mSampleRate);
			double hBound = double(GetRightBound()/mSampleRate);
			
			emit xRulerRange(lBound,hBound);

			double bBound = double(GetBottomBound());
			double tBound = double(GetTopBound());
			
			emit yRulerRange(bBound,tBound);
		}

		void FundPlotController::Draw()
		{
			if(mMustProcessData) ProcessData();
			mRenderer.Render();
			SelTimeRegionPlotController::Draw();
		}

		void FundPlotController::ProcessData()
		{
			int nFrames = mSegment.GetnFrames();
			TSize offset = TSize(GetLeftBound()*nFrames/GetnSamples());
			TSize len = TSize(GetRightBound()*nFrames/GetnSamples())-offset+1;
			
			if(mProcessedData.Size() <=len)
				mProcessedData.Resize(len+1);
			mProcessedData.SetSize(len+1);

			std::copy(mCacheData.GetPtr()+offset,mCacheData.GetPtr()+offset+len+1,mProcessedData.GetPtr());
			mRenderer.SetDataPtr(mProcessedData.GetPtr(),mProcessedData.Size(),NormalMode);	
			mMustProcessData = false;
		}

		void FundPlotController::SetRenderingStep()
		{
			int nFrames = mSegment.GetnFrames();
			mRenderer.SetStep(mDuration*mSampleRate/TData(nFrames));
			SetHMin(mRenderer.GetStep()*TData(5.0));
		}

		void FundPlotController::FullView()
		{
			mMaxFreq += TData(50.0);
			mView.left = TData(0.0);
			mView.right = TData(mDuration*mSampleRate);
			mView.top = mMaxFreq;
			mView.bottom = TData(0.0);
			SetHBounds(mView.left,mView.right);
			SetVBounds(mView.bottom,mView.top);
			SetRenderingStep();
			emit sendView(mView);
			emit initialYRulerRange(double(mView.bottom),double(mView.top));
		}

		void FundPlotController::CacheData()
		{
			mMaxFreq = TData(0.0);
			int nFrames = mSegment.GetnFrames();
			mCacheData.Resize(nFrames);
			mCacheData.SetSize(nFrames);
			TData curFreq;
			for(int i = 0; i < nFrames; i++)
			{
				curFreq = mSegment.GetFrame(i).GetFundamental().GetFreq(0);
				if(curFreq > mMaxFreq) mMaxFreq = curFreq;
				mCacheData[i] = curFreq;	
			}
		}

		TData FundPlotController::GetFreq(TData t) const
		{
			TData value = t*mSampleRate;
			int nFrames = mSegment.GetnFrames();
			int index = int(value*nFrames/GetnSamples());
			return mCacheData[index];
		}

		void FundPlotController::InitialRegionTime() 
		{
			MediaTime time;
			time.SetBegin(TData(0.0));
			emit selectedRegion(time);
		}

		void FundPlotController::SetMousePos(TData x,TData y)
		{
			TData tbound = GetTopBound()-GetBottomBound();
			TData bBound = GetBottomBound();
			TData ycoord=y;
			ycoord *= tbound;
			ycoord /= TData(mViewport.h);
			ycoord += bBound;
			SegmentationMarksPlotController::SetMousePos(x,ycoord);
			if(!HasSentTag())
			{
				TData t=GetMouseXPos()/mSampleRate;
				TData freq=GetMouseYPos();
				QString s = "t="+QString::number(t,'f',3)+"s freq="+QString::number(freq,'f',0)+"Hz";
			
				emit toolTip(s);
			}
		}
    }
}

// END
