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
#include "BPF.hxx"
#include "QtPresentation.hxx"

namespace CLAM
{
	namespace VM
	{
		class SingleLabel;

		class QtMultiPlot : public QtPresentation
		{
			Q_OBJECT

			public:
				QtMultiPlot(QWidget* parent=0);
				virtual ~QtMultiPlot();

				void AddData(std::string key, const DataArray& array);
				void AddData(std::string key, const BPF& data, int samples = 100);
				void RemoveData( std::string key );
				void RemoveAllData();

				void SetColor(std::string key, Color c);

				void SetUnits(const std::string& xunits, const std::string& yunits);
				void SetToolTips(const std::string& xtooltip, const std::string& ytooltip);

				void SetXRange(const TData& xmin, const TData& xmax);
				void SetYRange(const TData& ymin, const TData& ymax);

				void SetMarks(std::list<unsigned>& marks);
				void SetMarksColor(Color c);

			private slots:
				void updateXLabel(TData);
				void updateYLabel(TData);

				void clearLabels();

			protected:
				virtual void SetPlotController();
				virtual void Connect();

				virtual void DisplayBackgroundBlack();
				virtual void DisplayBackgroundWhite();

			private:
				SingleLabel *_xLabel,*_yLabel;

				void InitMultiPlot();

		};
	}
}

#endif

