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
#include "CLAM_Math.hxx"
#include "Assert.hxx"
#include "MultiPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		MultiPlotController::MultiPlotController()
			: mXMin(0.0)
			, mXMax(1.0)
			, mMustProcessData(false)
			, mHasData(false)
			, mHasXRange(false)
			, mHasYRange(false)
		{
		}
		
		MultiPlotController::~MultiPlotController()
		{
			RemoveAllData();
		}

		void MultiPlotController::SetXRulerRange(double xmin, double xmax)
		{
			if(xmax <= xmin) return;
			mXMin=xmin;
			mXMax=xmax;
			SetMinSpanX(GetnSamples()*0.05);
			FullView();
			mHasXRange = true;
		}

		void MultiPlotController::SetYRulerRange(double ymin, double ymax)
		{
			if(ymax <= ymin) return;
			SetMinSpanY((ymax-ymin)*0.05);
			SetYRange(ymin,ymax);
			FullView();
			mHasYRange = true;
		}

		void MultiPlotController::AddData(std::string key, const DataArray& array)
		{
			if(!ExistRenderer(key))
			{
				CreateNewRenderer(key);	
			}

			mCacheData[key]=array; // cache data
			
			if(array.Size() > GetnSamples() || !mHasData)
			{
				mAux=array;
				SetInitialSettings();
				FullView();
			}
			mHasData = true;
			mMustProcessData=true;
			if(IsRenderingEnabled()) emit requestRefresh();
		}
		
		void MultiPlotController::RemoveData( std::string key )
		{
			std::map<std::string,MultiPlotRenderer*>::iterator rit= mRenderers.find(key);
			if(rit == mRenderers.end()) return;

			delete mRenderers[key];
			mRenderers.erase(rit);

			std::map<std::string,DataArray>::iterator dit=mCacheData.find(key);
			mCacheData.erase(dit);

			if(IsRenderingEnabled()) emit requestRefresh();
		}
		
		void MultiPlotController::RemoveAllData()
		{
			std::map<std::string,MultiPlotRenderer*>::iterator it = mRenderers.begin();
			for(; it != mRenderers.end(); it++) delete it->second;
				
			mRenderers.clear();
			mCacheData.clear();
			if(IsRenderingEnabled()) emit requestRefresh();
		}

		void MultiPlotController::SetColor(std::string key, Color c)
		{
			if(!ExistRenderer(key)) return;
			mRenderers[key]->SetColor(c);
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
		}

		void MultiPlotController::DisplayDimensions(int w, int h)
		{
			PlotController::DisplayDimensions(w,h);
		
			double global_max = GetnSamples();
			double local_max = mXMax-mXMin; 

			double lBound = (GetLeftBound()*local_max/global_max)+mXMin;
			double hBound = (GetRightBound()*local_max/global_max)+mXMin;
			
			emit xRulerRange(lBound,hBound);
			
			double bBound = GetBottomBound();
			double tBound = GetTopBound();
		  
			emit yRulerRange(bBound,tBound);
		}
		
		void MultiPlotController::Draw()
		{
			if(!mHasData || !IsRenderingEnabled()) return;
			if(mMustProcessData) ProcessData();
			std::map<std::string,MultiPlotRenderer*>::iterator it=mRenderers.begin();
			for(; it != mRenderers.end(); it++)
			{
				(it->second)->Render();
			}
			PlotController::Draw();
		}

		void MultiPlotController::SetMousePos(double x, double y)
		{
			if(x < 0 || x > GetnSamples()) return;
			if(y < GetMinY() || y > GetMaxY()) return;

			PlotController::SetMousePos(x,y);
			double global_max = GetnSamples();
			double local_max = mXMax-mXMin;
			double xval = (GetMouseXPos()*local_max/global_max)+mXMin;
			double yval = GetMouseYPos();
			emit sendXYValues(xval,yval);
			if(!HasSentTag())
			{	
				QString s = "x="+QString::number(xval,'f',2)+" y="+QString::number(yval,'f',2);
				emit globalToolTip(s);
			}
		}

		void MultiPlotController::SetHBounds(double left, double right)
		{
			PlotController::SetHBounds(left,right);
			mMustProcessData=true;
		
			double global_max = GetnSamples();
			double local_max = mXMax-mXMin;

			double lBound = (GetLeftBound()*local_max/global_max)+mXMin;
			double hBound = (GetRightBound()*local_max/global_max)+mXMin;
			
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
			emit xRulerRange(lBound,hBound);
		}

		void MultiPlotController::SetVBounds(double bottom, double top)
		{
			PlotController::SetVBounds(bottom,top);
			
			double bBound = GetBottomBound();
			double tBound = GetTopBound();
			
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
			emit yRulerRange(bBound,tBound);
		}

		void MultiPlotController::CreateNewRenderer(std::string key)
		{
			mRenderers[key] = new MultiPlotRenderer;
		}

		bool MultiPlotController::ExistRenderer(std::string key)
		{
			return mRenderers.find( key ) != mRenderers.end();
		}

		void MultiPlotController::FullView()
		{
			mView.left = 0.0;
			mView.right = GetnSamples();
			mView.top = GetMaxY();
			mView.bottom = GetMinY();
			SetHBounds(mView.left,mView.right);
			SetVBounds(mView.bottom,mView.top);
		}

		void MultiPlotController::ProcessData()
		{
			std::string key;
			TSize offset = TSize(GetLeftBound());
			TSize global_len = TSize(GetRightBound() - GetLeftBound())+1;
			
			std::map<std::string, DataArray>::iterator dit = mCacheData.begin();
			for(; dit != mCacheData.end(); dit++)
			{
				TSize local_len = global_len;
				TSize current_size = (*dit).second.Size();
				if(current_size < offset) continue;
				if(current_size < global_len) local_len = current_size;
				DataArray processed_data;
				processed_data.Resize(local_len+1);
				processed_data.SetSize(local_len+1);
				std::copy((*dit).second.GetPtr()+offset,
						  (*dit).second.GetPtr()+offset+local_len+1,
						  processed_data.GetPtr());
				key = (*dit).first;
				mRenderers[key]->SetData(processed_data);	
			}
			mMustProcessData = false;
		}
				
		void MultiPlotController::SetInitialSettings()
		{
			double samples = double(mAux.Size());
			double hmin = samples*0.05;
			if(hmin > 50) hmin = 50.0;
			SetMinSpanX(hmin);

			unsigned len = unsigned(samples);
			double vmax = double(*std::max_element(mAux.GetPtr(), mAux.GetPtr()+len));
			double vmin = double(*std::min_element(mAux.GetPtr(), mAux.GetPtr()+len));

			if(!mHasYRange)
			{
				if(vmin != GetMinY() || vmax != GetMaxY() || !mHasData)
				{
					SetMinSpanY((vmax-vmin)*0.05);
					SetYRange(vmin,vmax);
				}
			}
			else
			{
				SetMinSpanY((GetMaxY()-GetMinY())*0.05);
				SetYRange(GetMinY(),GetMaxY());
			}

			SetnSamples(samples);

			if(!mHasXRange)
			{
				mXMin = 0.0;
				mXMax = samples;
			}
		}

		void MultiPlotController::SetSelPos(double value, bool render)
		{
			if(CanDrawSelectedPos())
			{
				if(GetDialPos() != value)
				{
					PlotController::SetSelPos(value, render);
					if(mHasData && IsRenderingEnabled()) emit requestRefresh();
					double span = mXMax-mXMin;
					double xpos = (value*span/GetnSamples())+mXMin;
					emit selectedXPos(xpos);
				}
			}
		}

		void MultiPlotController::setHBounds(double xmin, double xmax)
		{
			double span = xmax-xmin;
			double left = (xmin-mXMin)*GetnSamples()/span;
			double right = (xmax-mXMin)*GetnSamples()/span;
			SetHBounds(left,right);
		}

		void MultiPlotController::setSelectedXPos(double xpos)
		{
			double span = mXMax-mXMin;
			double pos = (xpos-mXMin)*GetnSamples()/span;
			SetSelPos(pos,true);
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
		}

		void MultiPlotController::setVBounds(double ymin, double ymax)
		{
			SetVBounds(ymin,ymax);
		}

    }
}

// END

