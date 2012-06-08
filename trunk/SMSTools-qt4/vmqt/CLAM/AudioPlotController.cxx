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
#include <CLAM/CLAMGL.hxx>
#include <CLAM/CLAM_Math.hxx>
#include <CLAM/AudioPlotController.hxx>

namespace CLAM
{
    namespace VM
    {
		AudioPlotController::AudioPlotController() 
			: mMustProcessData(false)
			, mHugeArrayCondition(false)
			, mHasData(false)
		{
			SetMinSpanX(100.0);
			SetMinSpanY(0.2);
		}

		AudioPlotController::~AudioPlotController()
		{
		}

		void AudioPlotController::SetData(const Audio& audio)
		{
			mHasData = false;
			mAudio = &audio;
			SetSampleRate( double(mAudio->GetSampleRate()) );
			FullView();
			SetnSamples(double(mAudio->GetSize()));
			InitialRegionTime();
			SetDuration( double(GetnSamples())/GetSampleRate() );
			mMustProcessData = true;
			SetSelPos(0.0,true);
			mHasData = true;
			mRenderer.SaveScreen();
			if(IsRenderingEnabled()) emit requestRefresh();
		}

		void AudioPlotController::UpdateData(const Audio& audio)
		{
			mHasData = false;
			mAudio = &audio;
			mMustProcessData = true;
			mHasData = true;
			mRenderer.SaveScreen();
			if(IsRenderingEnabled()) emit requestRefresh();
		}

		void AudioPlotController::SetDataColor(Color c)
		{
			mRenderer.SetColor(c);
			mRenderer.SaveScreen();
			emit requestRefresh();
		}

		void AudioPlotController::Draw()
		{
			if(!mHasData || !IsRenderingEnabled()) return;
			if(mMustProcessData) 
			{
				ProcessData();
				mMustProcessData = false;
			}
			mRenderer.Render();
			DrawAxis();
			PlayablePlotController::Draw();
		}

		void AudioPlotController::FullView()
		{
			mView.left = 0.0;
			mView.right = double(mAudio->GetSize());
			mView.bottom = -1.0;
			mView.top = 1.0;
			SetYRange(mView.bottom, mView.top);
			SetHBounds(mView.left,mView.right);
			SetVBounds(mView.bottom,mView.top);
			emit viewChanged(mView);
			emit initialYRulerRange(-1.0,1.0);
		}

		void AudioPlotController::SetHBounds(double left, double right)
		{
			PlayablePlotController::SetHBounds(left,right);
			mMustProcessData = true;
			mRenderer.SaveScreen();
			double lBound = GetLeftBound()/GetSampleRate();
			double hBound = GetRightBound()/GetSampleRate();
	    
			emit xRulerRange(lBound,hBound);
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();	
		}

		void AudioPlotController::SetVBounds(double bottom, double top)
		{
			PlayablePlotController::SetVBounds(bottom,top);
			mMustProcessData = true;
			mRenderer.SaveScreen();
			double bBound = GetBottomBound();
			double tBound = GetTopBound();
			
			emit yRulerRange(bBound,tBound);
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
		}

		void AudioPlotController::DisplayDimensions(int w, int h)
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

		void AudioPlotController::DrawAxis()
		{
			Color c = mRenderer.GetColor();
			glColor3ub(c.r,c.g,c.b);
			glBegin(GL_LINES);
			glVertex3f(0.0f,0.0f,-1.0f);
			glVertex3f(float(GetSpanX()),0.0f,-1.0f);
			glEnd();
		}

		void AudioPlotController::ProcessData()
		{
			DetermineVisibleSamples();
			if(mHugeArrayCondition)
			{
				mRenderer.SetArrays(mMaxs, mMins);
				mRenderer.SetVBounds(mView.bottom,mView.top);
				return;
			}

			double range = GetRightBound()-GetLeftBound();
			double threshold = GetMinSpanX()*2.0;
			int mode = (range < threshold) ? DetailMode : NormalMode;
			mRenderer.SetData(mProcessedData,mode);	
		}

		void AudioPlotController::DetermineVisibleSamples()
		{
			TSize offset = TSize(GetLeftBound());
			TSize len = TSize(GetRightBound() - GetLeftBound())+1;

			if(len/5 >= GetDisplayWidth())
			{
				mHugeArrayCondition = true;
				BuildMaxMinArrays(offset,len);
				return;
			}
					
			mHugeArrayCondition = false;
							
			if(mProcessedData.Size() <= len)
				mProcessedData.Resize(len+1);
			mProcessedData.SetSize(len+1);

			std::copy(mAudio->GetBuffer().GetPtr()+offset,
					  mAudio->GetBuffer().GetPtr()+offset+len+1,
					  mProcessedData.GetPtr());
		}

		void AudioPlotController::BuildMaxMinArrays(TSize offset,TSize len)
		{
			TSize startSearch, endSearch, searchIntervalLen, searchRemIntervalLen;
			TSize maxs = GetDisplayWidth();

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
				mMaxs[i] = *std::max_element(mAudio->GetBuffer().GetPtr()+startSearch, 
											 mAudio->GetBuffer().GetPtr()+endSearch);
				mMins[i] = *std::min_element(mAudio->GetBuffer().GetPtr()+startSearch, 
											 mAudio->GetBuffer().GetPtr()+endSearch);
							
				startSearch = endSearch;
				endSearch += searchIntervalLen;	
			}
			if(searchRemIntervalLen)
			{
				mMaxs[maxs-1] = *std::max_element(mAudio->GetBuffer().GetPtr()+startSearch, 
												  mAudio->GetBuffer().GetPtr()+startSearch+searchRemIntervalLen);
				mMins[maxs-1] = *std::min_element(mAudio->GetBuffer().GetPtr()+startSearch, 
												  mAudio->GetBuffer().GetPtr()+startSearch+searchRemIntervalLen);
			}
		}

		void AudioPlotController::InitialRegionTime()
		{
			MediaTime time;
			time.SetBegin(TData(0.0));
			time.SetEnd(TData(GetDuration()));
			emit selectedRegion(time);
		}

		void AudioPlotController::SetMousePos(double x, double y)
		{
			PlotController::SetMousePos(x,y);
			if(!HasSentTag())
			{
				double t=GetMouseXPos()/GetSampleRate();
				double amp=GetMouseYPos();
				QString s = "t="+QString::number(t,'f',3)+"s amp="+QString::number(amp,'f',3);
				emit globalToolTip(s);
			}
		}

		void AudioPlotController::SetSelPos(double value, bool render)
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

		void AudioPlotController::setSelectedXPos(double xpos)
		{
			SetSelPos(xpos*GetSampleRate(),true);
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
		}

			
		void AudioPlotController::setHBounds(double xmin, double xmax)
		{
			double left = xmin*GetSampleRate();
			double right = xmax*GetSampleRate();
			SetHBounds(left,right);
		}

		void AudioPlotController::setVBounds(double ymin, double ymax)
		{
			SetVBounds(ymin,ymax);
		}
    }
}

// END

