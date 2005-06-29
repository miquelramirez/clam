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
#include "MultiPlotRenderer.hxx"
#include "PlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		class MultiPlotController : public PlotController
		{
			Q_OBJECT

		public:
			MultiPlotController();
			~MultiPlotController();

			void SetXRulerRange(const double& xmin, const double& xmax);
			void SetYRulerRange(const double& ymin, const double& ymax);

			void AddData(std::string key, const DataArray& array);
			void RemoveData( std::string key );
			void RemoveAllData();

			void SetColor(std::string key, Color c);
			void DisplayDimensions(const int& w, const int& h);
			void Draw();

			void SetMousePos(const double& x, const double& y);
			void SetSelPos(const double& value, bool render);

		signals:
			void sendXYValues(double,double);

		public slots:
			void setHBounds(double, double);
			void setSelectedXPos(double);

		protected:
			void SetHBounds(const double& left, const double& right);
			void SetVBounds(const double& bottom,const double& top);

			void FullView();

		private:
			std::map<std::string, MultiPlotRenderer*> mRenderers;
			std::map<std::string, DataArray>          mCacheData;
			DataArray                                 mAux;
			double                                    mXMin;
			double                                    mXMax;
			bool                                      mMustProcessData;
			bool                                      mHasData;
			bool                                      mHasXRange;
			bool                                      mHasYRange;
		
			void CreateNewRenderer(std::string key);
			bool ExistRenderer(std::string key);

			void ProcessData();
				
			void SetInitialSettings();
				
		};
    }
}

#endif

