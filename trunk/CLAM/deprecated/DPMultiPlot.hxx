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

#ifndef __DPMULTIPLOT__
#define __DPMULTIPLOT__

#include <vector>
#include <string>
#include "Array.hxx"
#include "VMColor.hxx"

namespace CLAM
{
	namespace VM
	{
		class QtMultiPlot;

		/**
		 * Viewing several functions at time.
		 *
		 * @ingroup QTVM
		 */

		class DPMultiPlot
		{
		public:
			DPMultiPlot();
			~DPMultiPlot();
				
			void Label(const std::string& label);
			void Geometry(int x, int y, int w, int h);

			void AddData(const std::string& key, const DataArray& array);
		   
			void SetColor(const std::string& key, const Color& c);

			void SetMarks(std::vector<unsigned>& marks);
				
			void SetXRange(const TData& xmin, const TData& xmax);
			void SetYRange(const TData& ymin, const TData& ymax);

			void SetUnits(const std::string& xunits, const std::string& yunits);
			void SetToolTips(const std::string& xtooltip, const std::string& ytooltip);

			void Run();

		private:
			QtMultiPlot* mPlot;
				
		};
	}
}

#endif

