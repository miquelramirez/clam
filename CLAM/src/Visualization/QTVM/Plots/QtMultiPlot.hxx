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

#ifndef __QTMULTIPLOT__
#define __QTMULTIPLOT__

#include "Array.hxx"
#include "SingleDisplayPlot.hxx"

namespace CLAM
{
    namespace VM
    {
		class SingleLabel;
	
		/**
		 * Allows viewing multiple functions.
		 * Uses CLAM::DataArray or CLAM::BPF.
		 *
		 * @ingroup QTVM
		 */

		class QtMultiPlot : public SingleDisplayPlot
		{
			Q_OBJECT

		public:
			QtMultiPlot(QWidget* parent=0, const char * name = 0, WFlags f = 0 );
			~QtMultiPlot();

			void AddData(std::string key, const DataArray& array);
			void RemoveData( std::string key );
			void RemoveAllData();

			void SetColor(std::string key, Color c);

			void SetUnits(const std::string& xunits, const std::string& yunits);
			void SetToolTips(const std::string& xtooltip, const std::string& ytooltip);

			void SetXRange(const double& xmin, const double& xmax);
			void SetYRange(const double& ymin, const double& ymax);

		private slots:
			void updateLabels(double,double);
			
		protected:
			void SetPlotController();				
			void Connect();
			void DisplayBackgroundBlack();
			void DisplayBackgroundWhite();

		private:
			SingleLabel* mXLabel;
			SingleLabel* mYLabel;

			void InitMultiPlot();

		};
    }
}

#endif

