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
		{
			SetnSamples(1);
			_xmin=TData(0.0); _xmax=TData(1.0);
			_ymin=TData(0.0); _ymax=TData(1.0);
			_mustProcessData=false;
			_haveData=false;
		}
		
		MultiPlotController::~MultiPlotController()
		{
			RemoveAllData();
		}

		void MultiPlotController::SetXRange(const TData& xmin, const TData& xmax)
		{
			if(xmax <= xmin) return;
			_xmin=fabs(xmin);
			_xmax=fabs(xmax);
			HZoomIn();
			HZoomOut();
		}

		void MultiPlotController::SetYRange(const TData& ymin, const TData& ymax)
		{
			if(ymax <= ymin) return;
			_ymin=fabs(ymin);
			_ymax=fabs(ymax);
			SetvRange(_ymax+_ymin);
			VZoomIn();
			VZoomOut();
		}

		void MultiPlotController::AddData(std::string key, const DataArray& array)
		{
			if(_haveData)
			{
				CLAM_ASSERT(_aux.Size()==array.Size(),"Data size not match!");
			}

			if(!ExistRenderer(key))
			{
				CreateNewRenderer(key);	
			}

			_cacheData[key]=array; // cache data
			
			if(!_haveData)
			{
				_aux=array;
				SetInitialSettings();
				FullView();
				_haveData=true;
			}
			_mustProcessData=true;
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

			if(_haveData)
			{
				CLAM_ASSERT(_aux.Size()==array.Size(),"Data size not match!");
			}

			if(!ExistRenderer(key))
			{
				CreateNewRenderer(key);
			}

			_cacheData[key]=array;

			if(!_haveData)
			{
				_aux=array;
				SetInitialSettings();
				FullView();
				_haveData=true;
			}	
			_mustProcessData=true;
			emit requestRefresh();
		}
		
		void MultiPlotController::RemoveData( std::string key )
		{
			std::map<std::string,MPDataRenderer*>::iterator rit= _renderers.find(key);
			if(rit == _renderers.end()) return;

			delete _renderers[key];
			_renderers.erase(rit);

			std::map<std::string,DataArray>::iterator dit=_cacheData.find(key);
			_cacheData.erase(dit);

			emit requestRefresh();
		}
		
		void MultiPlotController::RemoveAllData()
		{
			std::map<std::string,MPDataRenderer*>::iterator it = _renderers.begin();
			for(; it != _renderers.end(); it++) delete it->second;
				
			_renderers.clear();
			_cacheData.clear();
			emit requestRefresh();
		}

		void MultiPlotController::SetColor(std::string key, Color c)
		{
			if(!ExistRenderer(key)) return;
			_renderers[key]->SetColor(c);
			emit requestRefresh();
		}
		
		void MultiPlotController::SurfaceDimensions(int w,int h)
		{
			PlotController::SurfaceDimensions(w,h);
		
			TData global_max=TData(GetnSamples());
			TData local_max=_xmax+_xmin;

			double lBound = double(GetLeftBound()*local_max/global_max);
		    double hBound = double(GetRightBound()*local_max/global_max);
			QwtScaleDiv div;
			lBound-=double(_xmin); hBound-=double(_xmin);
			div.rebuild(lBound,hBound,7,3,false);
			emit xRulerScaleDiv(div);
			
			global_max=GetvRange();
			local_max=_ymax+fabs(_ymin);

			double bBound = double(GetBottomBound()*local_max/global_max);
			double tBound = double(GetTopBound()*local_max/global_max);
			bBound-=double(_ymin); tBound-=double(_ymin);
			div.rebuild(bBound,tBound,7,3,false);
			emit yRulerScaleDiv(div);
		}
		
		void MultiPlotController::Draw()
		{
			if(_mustProcessData) ProcessData();
			std::map<std::string,MPDataRenderer*>::iterator it=_renderers.begin();
			for(; it != _renderers.end(); it++)
			{
				(it->second)->Render();
			}
			SelPosPlotController::Draw();
		}

		void MultiPlotController::SetMousePos(TData x,TData y)
		{
			TData tbound = GetTopBound()-GetBottomBound();
			TData bBound = GetBottomBound()-_ymin;
			TData ycoord=y;
			ycoord *= tbound;
			ycoord /= TData(_viewport.h);
			ycoord += bBound;
			SegmentationMarksPlotController::SetMousePos(x,ycoord);
			TData global_max=TData(GetnSamples());
			TData local_max=_xmax+_xmin;
			TData xval=(GetMouseXPos()*local_max/global_max)-_xmin;
			TData yval=GetMouseYPos();
	
			emit xvalue(xval);
			emit yvalue(yval);
		}

		void MultiPlotController::SetHBounds(const TData& left,const TData& right)
		{
			SelPosPlotController::SetHBounds(left,right);
			_mustProcessData=true;
		
			TData global_max=TData(GetnSamples());
			TData local_max=_xmax+_xmin;

			double lBound = double(GetLeftBound()*local_max/global_max);
		    double hBound = double(GetRightBound()*local_max/global_max);
			QwtScaleDiv div;
			lBound-=double(_xmin); hBound-=double(_xmin);
			div.rebuild(lBound,hBound,7,3,false);
			emit xRulerScaleDiv(div);
		}

		void MultiPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			SelPosPlotController::SetVBounds(bottom,top);
			_mustProcessData=true;

			TData global_max=GetvRange();
			TData local_max=_ymax+fabs(_ymin);

			double bBound = double(GetBottomBound()*local_max/global_max);
			double tBound = double(GetTopBound()*local_max/global_max);
			QwtScaleDiv div;
			bBound-=double(_ymin); tBound-=double(_ymin);
			div.rebuild(bBound,tBound,7,3,false);
			emit yRulerScaleDiv(div);
		}

		void MultiPlotController::CreateNewRenderer(std::string key)
		{
			_renderers[key] = new MPDataRenderer;
		}

		bool MultiPlotController::ExistRenderer(std::string key)
		{
			return _renderers.find( key ) != _renderers.end();
		}

		void MultiPlotController::FullView()
		{
			_view.left = TData(0.0);
			_view.right = GetnSamples();
			_view.top = GetvRange();
			_view.bottom = TData(0.0);
			SetHBounds(_view.left,_view.right);
			SetVBounds(_view.bottom,_view.top);
			emit sendView(_view);
		}

		void MultiPlotController::ProcessData()
		{
			std::string key;
			TSize offset = TSize(GetLeftBound());
			TSize len = TSize(GetRightBound() - GetLeftBound())+1;
			
			if(_processedData.Size() < len+1)
			{
				_processedData.Resize(len+1);
				_processedData.SetSize(len+1);
			}

			std::map<std::string, DataArray>::iterator dit = _cacheData.begin();
			for(; dit != _cacheData.end(); dit++)
			{
				std::copy(	(*dit).second.GetPtr()+offset,
							(*dit).second.GetPtr()+offset+len+1,
							_processedData.GetPtr()	);
			
				for(int i=0;i < _processedData.Size();i++) _processedData[i] += _ymin;
				key=(*dit).first;
				_renderers[key]->SetData(_processedData.GetPtr(),_processedData.Size());	
			}
			_mustProcessData = false;
		}
				
		void MultiPlotController::SetInitialSettings()
		{
			TData samples = TData(_aux.Size());
			TData hmin=samples*TData(5.0)/TData(100.0);
			if(hmin > 50) hmin=TData(50.0);
			SetHMin(hmin);

			TData vmax=TData(-1000000.0);
			TData vmin=TData(1000000.0);
			for(int i=0; i < _aux.Size(); i++)
			{
				if(vmax < _aux[i]) vmax=_aux[i];
				if(vmin > _aux[i]) vmin=_aux[i];
			}
			SetVMin((vmax+fabs(vmin))*TData(5.0)/TData(100.0));
			SetvRange(vmax+fabs(vmin));

			_xmin=TData(0.0); _xmax=_aux.Size();
			_ymin=fabs(vmin); _ymax=fabs(vmax);
			SetnSamples(_aux.Size());
		}
	}
}

// END

