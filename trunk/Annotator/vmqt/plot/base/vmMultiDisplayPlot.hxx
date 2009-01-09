/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __VMQT_MULTIDISPLAY_PLOT_H__
#define __VMQT_MULTIDISPLATPLOT_H__

#include <vector>
#include "vmWPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class Ruler;
		class PlotCanvas;
		class ScrollGroup;

		class MultiDisplayPlot : public WPlot
		{
			Q_OBJECT
		public:
			MultiDisplayPlot(QWidget* parent=0);
			virtual ~MultiDisplayPlot();

		protected:
			Ruler*       mXRuler;
			ScrollGroup* mHScroll;

			std::vector<PlotCanvas*> mDisplay;

			virtual void CreateDisplay()=0;

			void InitMultiDisplayPlot(int master_id);
			void SetMasterId(int id);

		private:
			int mMasterId;
		};
	}
}

#endif

