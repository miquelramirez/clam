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

#ifndef __MULTIPLOTCONTROLLER__
#define __MULTIPLOTCONTROLLER__

#include <map>
#include <string>
#include <qwt/qwt_scldiv.h>
#include "BPF.hxx"
#include "MPDataRenderer.hxx"
#include "SelPosPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class MultiPlotController : public SelPosPlotController
		{
			Q_OBJECT

			public:
				MultiPlotController();
				virtual ~MultiPlotController();

				void SetXRange(const TData& xmin, const TData& xmax);
				void SetYRange(const TData& ymin, const TData& ymax);

				void AddData(std::string key, const DataArray& array);
				void AddData(std::string key, const BPF& data, int samples = 100);
				void RemoveData( std::string key );
				void RemoveAllData();

				void SetColor(std::string key, Color c);
				void SurfaceDimensions(int w,int h);
				void Draw();

				void SetMousePos(TData x,TData y);

			signals:
				void xRulerScaleDiv(QwtScaleDiv);
				void yRulerScaleDiv(QwtScaleDiv);

				void xvalue(TData);
				void yvalue(TData);

			protected:
				virtual void SetHBounds(const TData& left,const TData& right);
				virtual void SetVBounds(const TData& bottom,const TData& top);

			private:
				std::map<std::string, MPDataRenderer*> _renderers;
				std::map<std::string, DataArray> _cacheData;
				DataArray _processedData;
				DataArray _aux;
				bool _mustProcessData;
				bool _haveData;
				TData _xmin,_xmax,_ymin,_ymax;

				void CreateNewRenderer(std::string key);
				bool ExistRenderer(std::string key);

				void FullView();
				void ProcessData();
				
				void SetInitialSettings();
				
		};
	}
}

#endif

