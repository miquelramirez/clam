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
 * along with this program; ifreq not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <CLAM/FundPlotController.hxx>

namespace CLAM
{
    namespace VM
    {
		FundPlotController::FundPlotController()
			: mMustProcessData(false)
			, mHasData(false)
		{
			SetMinSpanX(50.0);
			SetMinSpanY(40.0);
		}

		FundPlotController::~FundPlotController()
		{
		}

		void FundPlotController::SetData(const Segment& segment)
		{
			mHasData = false;
			mSegment = &segment;
			SetSampleRate( double(mSegment->GetSamplingRate()) );
			if(segment.HasAudio())
			{
				SetDuration( double(mSegment->GetAudio().GetSize())/double(mSegment->GetAudio().GetSampleRate()) );
			}
			else
			{
				SetDuration(  double(mSegment->GetEndTime())- double(mSegment->GetBeginTime()) );
			}
			CacheData();
			FullView();
			SetnSamples(GetDuration()*GetSampleRate());
			SetYRange(0.0,GetSampleRate()/2.0);
			InitialRegionTime();
			mMustProcessData = true;
			SetSelPos(0.0,true);
			mHasData = true;
			mRenderer.SaveScreen();
			if(IsRenderingEnabled()) emit requestRefresh();
		}

		void FundPlotController::SetDataColor(Color c)
		{
			mRenderer.SetColor(c);
			mRenderer.SaveScreen();
			emit requestRefresh();
		}

		void FundPlotController::SetHBounds(double left, double right)
		{
			PlayablePlotController::SetHBounds(left,right);
			mMustProcessData = true;
			mRenderer.SaveScreen();
			double lBound = GetLeftBound()/GetSampleRate();
			double hBound = GetRightBound()/GetSampleRate();
			
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
			emit xRulerRange(lBound,hBound);	
		}

		void FundPlotController::SetVBounds(double bottom, double top)
		{
			PlayablePlotController::SetVBounds(bottom,top);
			mMustProcessData = true;
			mRenderer.SaveScreen();
			double bBound = GetBottomBound();
			double tBound = GetTopBound();
	    
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
			emit yRulerRange(bBound,tBound);
		}

		void FundPlotController::DisplayDimensions(int w, int h)
		{
			mMustProcessData = true;
			PlotController::DisplayDimensions(w,h);
		
			double lBound = GetLeftBound()/GetSampleRate();
			double hBound = GetRightBound()/GetSampleRate();
			
			emit xRulerRange(lBound,hBound);

			double bBound = GetBottomBound();
			double tBound = GetTopBound();
			
			emit yRulerRange(bBound,tBound);
		}

		void FundPlotController::Draw()
		{
			if(!mHasData || !IsRenderingEnabled()) return;
			if(mMustProcessData) ProcessData();
			mRenderer.Render();
			PlayablePlotController::Draw();
		}

		void FundPlotController::ProcessData()
		{
			int nFrames = mSegment->GetnFrames();
			TSize offset = TSize(GetLeftBound()*nFrames/GetnSamples());
			TSize len = TSize(GetRightBound()*nFrames/GetnSamples())-offset+1;
			
			if(mProcessedData.Size() <=len)
				mProcessedData.Resize(len+1);
			mProcessedData.SetSize(len+1);

			std::copy(mCacheData.GetPtr()+offset,mCacheData.GetPtr()+offset+len+1,mProcessedData.GetPtr());
			mRenderer.SetData(mProcessedData,NormalMode);	
			mMustProcessData = false;
		}

		void FundPlotController::SetRenderingStep()
		{
			int nFrames = mSegment->GetnFrames();
			mRenderer.SetStep(TData(GetDuration()*GetSampleRate())/TData(nFrames));
			SetMinSpanX(double(mRenderer.GetStep())*5.0);
		}

		void FundPlotController::FullView()
		{
			mView.left = 0.0;
			mView.right = GetDuration()*GetSampleRate();
			mView.top = GetSampleRate()/2.0;
			mView.bottom = 0.0;
			SetHBounds(mView.left,mView.right);
			SetVBounds(mView.bottom,mView.top);
			SetRenderingStep();
			emit initialYRulerRange(mView.bottom, mView.top);
		}

		void FundPlotController::CacheData()
		{
			int nFrames = mSegment->GetnFrames();
			mCacheData.Resize(nFrames);
			mCacheData.SetSize(nFrames);
			for(int i = 0; i < nFrames; i++)
			{
				mCacheData[i] = mSegment->GetFrame(i).GetFundamental().GetFreq(0);
			}
		}

		void FundPlotController::InitialRegionTime() 
		{
			MediaTime time;
			time.SetBegin(TData(0.0));
			time.SetEnd(TData(GetDuration()));
			emit selectedRegion(time);
		}

		void FundPlotController::SetMousePos(double x, double y)
		{
			PlotController::SetMousePos(x,y);
			if(!HasSentTag())
			{
				double t=GetMouseXPos()/GetSampleRate();
				TData freq=GetMouseYPos();
				QString s = "t="+QString::number(t,'f',3)+"s freq="+QString::number(freq,'f',0)+"Hz";
			
				emit globalToolTip(s);
			}
		}

		void FundPlotController::setSelectedXPos(double xpos)
		{
			SetSelPos(xpos*GetSampleRate(),true);
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
		}

		void FundPlotController::SetSelPos(double value, bool render)
		{
			if(CanDrawSelectedPos())
			{
				if(GetDialPos() != value)
				{
					PlayablePlotController::SetSelPos(value, render);
					if(mHasData && IsRenderingEnabled()) emit requestRefresh();
					emit selectedXPos(value/GetSampleRate());
				}
			}
		}
		
		void FundPlotController::setHBounds(double xmin, double xmax)
		{
			double left = xmin*GetSampleRate();
			double right = xmax*GetSampleRate();
			SetHBounds(left,right);
		}

		void FundPlotController::setVBounds(double ymin, double ymax)
		{
			SetVBounds(ymin,ymax);
		}
    }
}

// END

