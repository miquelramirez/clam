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
			: mMustProcessData(false)
			, mHaveData(false)
			, mXMin(TData(0.0))
			, mXMax(TData(1.0))
			, mYMin(TData(0.0))
			, mYMax(TData(1.0))
		{
			SetnSamples(1);
		}
		
		MultiPlotController::~MultiPlotController()
		{
			RemoveAllData();
		}

		void MultiPlotController::SetXRange(const TData& xmin, const TData& xmax)
		{
			if(xmax <= xmin) return;
			mXMin=fabs(xmin);
			mXMax=fabs(xmax);
			HZoomIn();
			HZoomOut();
		}

		void MultiPlotController::SetYRange(const TData& ymin, const TData& ymax)
		{
			if(ymax <= ymin) return;
			mYMin=fabs(ymin);
			mYMax=fabs(ymax);
			SetvRange(mYMax+mYMin);
			VZoomIn();
			VZoomOut();
		}

		void MultiPlotController::AddData(std::string key, const DataArray& array)
		{
			if(mHaveData)
			{
				CLAM_ASSERT(mAux.Size()==array.Size(),"Data size not match!");
			}

			if(!ExistRenderer(key))
			{
				CreateNewRenderer(key);	
			}

			mCacheData[key]=array; // cache data
			
			if(!mHaveData)
			{
				mAux=array;
				SetInitialSettings();
				FullView();
				mHaveData=true;
			}
			mMustProcessData=true;
			emit requestRefresh();
		}
		
		void MultiPlotController::AddData(std::string key, const BPF& data, int samples)
		{
			DataArray array;
			TData start = data.GetXValue(0);
			TData end = data.GetXValue(data.Size() - 1);

			TData dx = fabs(end - start) / TData(samples);

			array.Resize(samples+1);
			array.SetSize(samples+1);

			TData x = start;

			for(int i = 0; i < array.Size(); i++)
			{
				array[i] = data.GetValue(x);
				x += dx;
			}

			if(mHaveData)
			{
				CLAM_ASSERT(mAux.Size()==array.Size(),"Data size not match!");
			}

			if(!ExistRenderer(key))
			{
				CreateNewRenderer(key);
			}

			mCacheData[key]=array;

			if(!mHaveData)
			{
				mAux=array;
				SetInitialSettings();
				FullView();
				mHaveData=true;
			}	
			mMustProcessData=true;
			emit requestRefresh();
		}
		
		void MultiPlotController::RemoveData( std::string key )
		{
			std::map<std::string,MPDataRenderer*>::iterator rit= mRenderers.find(key);
			if(rit == mRenderers.end()) return;

			delete mRenderers[key];
			mRenderers.erase(rit);

			std::map<std::string,DataArray>::iterator dit=mCacheData.find(key);
			mCacheData.erase(dit);

			emit requestRefresh();
		}
		
		void MultiPlotController::RemoveAllData()
		{
			std::map<std::string,MPDataRenderer*>::iterator it = mRenderers.begin();
			for(; it != mRenderers.end(); it++) delete it->second;
				
			mRenderers.clear();
			mCacheData.clear();
			emit requestRefresh();
		}

		void MultiPlotController::SetColor(std::string key, Color c)
		{
			if(!ExistRenderer(key)) return;
			mRenderers[key]->SetColor(c);
			emit requestRefresh();
		}
		
		void MultiPlotController::SurfaceDimensions(int w,int h)
		{
			PlotController::SurfaceDimensions(w,h);
		
			TData global_max=TData(GetnSamples());
			TData local_max=mXMax+mXMin;

			double lBound = double(GetLeftBound()*local_max/global_max);
			double hBound = double(GetRightBound()*local_max/global_max);
			lBound-=double(mXMin); 
			hBound-=double(mXMin);
			
			emit xRulerRange(lBound,hBound);
			
			global_max=GetvRange();
			local_max=mYMax+fabs(mYMin);

			double bBound = double(GetBottomBound()*local_max/global_max);
			double tBound = double(GetTopBound()*local_max/global_max);
			bBound-=double(mYMin); 
			tBound-=double(mYMin);
			
			emit yRulerRange(bBound,tBound);
		}
		
		void MultiPlotController::Draw()
		{
			if(mMustProcessData) ProcessData();
			std::map<std::string,MPDataRenderer*>::iterator it=mRenderers.begin();
			for(; it != mRenderers.end(); it++)
			{
				(it->second)->Render();
			}
			SelPosPlotController::Draw();
		}

		void MultiPlotController::SetMousePos(TData x,TData y)
		{
			TData tbound = GetTopBound()-GetBottomBound();
			TData bBound = GetBottomBound()-mYMin;
			TData ycoord=y;
			ycoord *= tbound;
			ycoord /= TData(mViewport.h);
			ycoord += bBound;
			SegmentationMarksPlotController::SetMousePos(x,ycoord);
	    
			TData global_max=TData(GetnSamples());
			TData local_max=mXMax+mXMin;
			TData xval=(GetMouseXPos()*local_max/global_max)-mXMin;
			TData yval=GetMouseYPos();
	
			emit xvalue(xval);
			emit yvalue(yval);
		}

		void MultiPlotController::SetHBounds(const TData& left,const TData& right)
		{
			SelPosPlotController::SetHBounds(left,right);
			mMustProcessData=true;
		
			TData global_max=TData(GetnSamples());
			TData local_max=mXMax+mXMin;

			double lBound = double(GetLeftBound()*local_max/global_max);
			double hBound = double(GetRightBound()*local_max/global_max);
			lBound-=double(mXMin); 
			hBound-=double(mXMin);
			
			emit xRulerRange(lBound,hBound);
		}

		void MultiPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			SelPosPlotController::SetVBounds(bottom,top);
			mMustProcessData=true;

			TData global_max=GetvRange();
			TData local_max=mYMax+fabs(mYMin);

			double bBound = double(GetBottomBound()*local_max/global_max);
			double tBound = double(GetTopBound()*local_max/global_max);
			bBound-=double(mYMin); 
			tBound-=double(mYMin);
			
			emit yRulerRange(bBound,tBound);
		}

		void MultiPlotController::CreateNewRenderer(std::string key)
		{
			mRenderers[key] = new MPDataRenderer;
		}

		bool MultiPlotController::ExistRenderer(std::string key)
		{
			return mRenderers.find( key ) != mRenderers.end();
		}

		void MultiPlotController::FullView()
		{
			mView.left = TData(0.0);
			mView.right = GetnSamples();
			mView.top = GetvRange();
			mView.bottom = TData(0.0);
			SetHBounds(mView.left,mView.right);
			SetVBounds(mView.bottom,mView.top);
			emit sendView(mView);
		}

		void MultiPlotController::ProcessData()
		{
			std::string key;
			TSize offset = TSize(GetLeftBound());
			TSize len = TSize(GetRightBound() - GetLeftBound())+1;
			
			if(mProcessedData.Size() < len+1)
			{
				mProcessedData.Resize(len+1);
				mProcessedData.SetSize(len+1);
			}

			std::map<std::string, DataArray>::iterator dit = mCacheData.begin();
			for(; dit != mCacheData.end(); dit++)
			{
				std::copy((*dit).second.GetPtr()+offset,(*dit).second.GetPtr()+offset+len+1,mProcessedData.GetPtr());
			
				for(int i=0;i < mProcessedData.Size();i++) mProcessedData[i] += mYMin;
				key=(*dit).first;
				mRenderers[key]->SetData(mProcessedData.GetPtr(),mProcessedData.Size());	
			}
			mMustProcessData = false;
		}
				
		void MultiPlotController::SetInitialSettings()
		{
			TData samples = TData(mAux.Size());
			TData hmin=samples*TData(5.0)/TData(100.0);
			if(hmin > 50) hmin=TData(50.0);
			SetHMin(hmin);

			TData vmax=TData(-1000000.0);
			TData vmin=TData(1000000.0);
			for(int i=0; i < mAux.Size(); i++)
			{
				if(vmax < mAux[i]) vmax=mAux[i];
				if(vmin > mAux[i]) vmin=mAux[i];
			}
			SetVMin((vmax+fabs(vmin))*TData(5.0)/TData(100.0));
			SetvRange(vmax+fabs(vmin));

			mXMin=TData(0.0); mXMax=mAux.Size();
			mYMin=fabs(vmin); mYMax=fabs(vmax);
			SetnSamples(mAux.Size());
		}
    }
}

// END

