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

#include <algorithm>
#include "CLAMGL.hxx"
#include "CLAM_Math.hxx"
#include "AudioPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		AudioPlotController::AudioPlotController() 
			: mMustProcessData(false)
			, mHugeArrayCondition(false)
		{
			SetVMin(TData(0.2));
			SetvRange(TData(2.0));
		}

		AudioPlotController::~AudioPlotController()
		{
		}

		void AudioPlotController::SetData(const Audio& audio)
		{
			mAudio = audio;
			mSampleRate = mAudio.GetSampleRate();
			FullView();
			SetnSamples(mAudio.GetBuffer().Size());
			InitialRegionTime();
			mDuration = TData(GetnSamples())/mSampleRate;
			mMustProcessData = true;
			SetSelPos(TData(0.0));
			emit requestRefresh();
		}

		void AudioPlotController::SetDataColor(Color c)
		{
			mRenderer.SetColor(c);
		}

		void AudioPlotController::Draw()
		{
			if(mMustProcessData) ProcessData();
			mRenderer.Render();
			DrawAxis();
			SelTimeRegionPlotController::Draw();
		}

		void AudioPlotController::FullView()
		{
			mView.left = TData(0.0);
			mView.right = TData(mAudio.GetBuffer().Size());
			mView.top = TData(2.0);
			mView.bottom = TData(0.0);
			SetHBounds(mView.left,mView.right);
			SetVBounds(mView.bottom,mView.top);
			emit sendView(mView);
			emit initialYRulerRange(-1.0,1.0);
		}

		void AudioPlotController::SetHBounds(const TData& left,const TData& right)
		{
			SelTimeRegionPlotController::SetHBounds(left,right);
			mMustProcessData = true;
			
			double lBound = double(GetLeftBound()/mSampleRate);
			double hBound = double(GetRightBound()/mSampleRate);
	    
			emit xRulerRange(lBound,hBound);	
		}

		void AudioPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			SelTimeRegionPlotController::SetVBounds(bottom,top);
			
			double bBound = double(GetBottomBound()-1.0);
			double tBound = double(GetTopBound()-1.0);
			
			emit yRulerRange(bBound,tBound);
		}

		void AudioPlotController::SurfaceDimensions(int w,int h)
		{
			PlotController::SurfaceDimensions(w,h);
			mMustProcessData = true;
			
			double lBound = double(GetLeftBound()/mSampleRate);
			double hBound = double(GetRightBound()/mSampleRate);
			
			emit xRulerRange(lBound,hBound);

			double bBound = double(GetBottomBound()-1.0);
			double tBound = double(GetTopBound()-1.0);
		      
			emit yRulerRange(bBound,tBound);
		}

		void AudioPlotController::DrawAxis()
		{
			Color c = mRenderer.GetColor();
			glColor3ub(c.r,c.g,c.b);
			glBegin(GL_LINES);
			glVertex3f(0.0f,1.0,-1.0f);
			glVertex3f(GetCurrent(),1.0f,-1.0f);
			glEnd();
		}

		void AudioPlotController::ProcessData()
		{
			DetermineVisibleSamples();
			if(mHugeArrayCondition)
			{
				mRenderer.SetArrays(mMaxs.GetPtr(), mMins.GetPtr(), mMaxs.Size());
				mRenderer.SetVBounds(mView.top,mView.bottom);
				return;
			}

			for(int i=0;i < mProcessedData.Size();i++) mProcessedData[i] += TData(1.0);

			TData range = GetRightBound()-GetLeftBound();
			TData threshold = GetHMin()*TData(2.0);
			int mode = (range < threshold) ? DetailMode : NormalMode;
			mRenderer.SetDataPtr(mProcessedData.GetPtr(),mProcessedData.Size(),mode);	
			mMustProcessData = false;
		}

		void AudioPlotController::DetermineVisibleSamples()
		{
			TSize offset = TSize(GetLeftBound());
			TSize len = TSize(GetRightBound() - GetLeftBound())+1;

			if(len/5 >= mViewport.w)
			{
				mHugeArrayCondition = true;
				BuildMaxMinArrays(offset,len);
				return;
			}
					
			mHugeArrayCondition = false;
							
			if(mProcessedData.Size() <= len)
				mProcessedData.Resize(len+1);
			mProcessedData.SetSize(len+1);

			std::copy(mAudio.GetBuffer().GetPtr()+offset,mAudio.GetBuffer().GetPtr()+offset+len+1,mProcessedData.GetPtr());
		}

		void AudioPlotController::BuildMaxMinArrays(TSize offset,TSize len)
		{
			TSize startSearch, endSearch, searchIntervalLen, searchRemIntervalLen;
			TSize maxs = mViewport.w;

			if(mMaxs.Size() < maxs) mMaxs.Resize(maxs);
			if(mMins.Size() < maxs)	mMins.Resize(maxs);

			mMaxs.SetSize(maxs);
			mMins.SetSize(maxs);

			searchIntervalLen = len / maxs;
			searchRemIntervalLen = len % maxs;
			
			int firstPassIterations = (searchRemIntervalLen) ? maxs-1 : maxs;

			startSearch = offset;
			endSearch = startSearch + searchIntervalLen;				

			for(int i = 0; i < firstPassIterations; i++)
			{
				mMaxs[i] = *std::max_element(mAudio.GetBuffer().GetPtr()+startSearch, mAudio.GetBuffer().GetPtr()+endSearch)+TData(1.0);
				mMins[i] = *std::min_element(mAudio.GetBuffer().GetPtr()+startSearch, mAudio.GetBuffer().GetPtr()+endSearch)+TData(1.0);
							
				startSearch = endSearch;
				endSearch += searchIntervalLen;	
			}
			if(searchRemIntervalLen)
			{
				mMaxs[maxs-1] = *std::max_element(mAudio.GetBuffer().GetPtr()+startSearch, mAudio.GetBuffer().GetPtr()+startSearch+searchRemIntervalLen)+TData(1.0);
				mMins[maxs-1] = *std::min_element(mAudio.GetBuffer().GetPtr()+startSearch, mAudio.GetBuffer().GetPtr()+startSearch+searchRemIntervalLen)+TData(1.0);
			}
		}

		TData AudioPlotController::GetAmp(TData t) const
		{
			TIndex index = TIndex(t*mSampleRate);
			TData value = mAudio.GetBuffer()[index];
			return (fabs(value) >= 0.002) ? value : TData(0.0);
		}

		void AudioPlotController::InitialRegionTime()
		{
			MediaTime time;
			time.SetBegin(TData(0.0));
			time.SetEnd(TData(GetnSamples())/mSampleRate);
			emit selectedRegion(time);
		}

		bool AudioPlotController::MustProcessData() const
		{
			return mMustProcessData;
		}

		void AudioPlotController::SetMousePos(TData x,TData y)
		{
			TData tbound = GetTopBound()-GetBottomBound();
			TData bBound = GetBottomBound()-TData(1.0);
			TData ycoord=y;
			ycoord *= tbound;
			ycoord /= TData(mViewport.h);
			ycoord += bBound;
			SegmentationMarksPlotController::SetMousePos(x,ycoord);
			if(!HasSentTag())
			{
				TData t=GetMouseXPos()/mSampleRate;
				TData amp=GetMouseYPos();
				QString s = "t="+QString::number(t,'f',3)+"s amp="+QString::number(amp,'f',3);
				emit toolTip(s);
			}
		}

		void AudioPlotController::SetSelPos(const TData& value)
		{
			if(CanDrawSelectedPos())
			{
				if(GetDialPos() != value)
				{
					SelTimeRegionPlotController::SetSelPos(value);
					emit requestRefresh();
					emit selPos(value);
					emit selectedXPos(value/mSampleRate);
				}
			}
		}

		void AudioPlotController::SetSelectedXPos(double xpos)
		{
			SetSelPos(TData(xpos)*mSampleRate);
		}
    }
}

// END

