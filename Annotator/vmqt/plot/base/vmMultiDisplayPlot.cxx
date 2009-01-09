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

#include "Assert.hxx"
#include "vmRuler.hxx"
#include "vmScrollGroup.hxx"
#include "vmPlotCanvas.hxx"
#include "vmDataTypes.hxx"
#include "vmMultiDisplayPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		MultiDisplayPlot::MultiDisplayPlot(QWidget* parent)
			: WPlot(parent)
			, mMasterId(-1)
		{
		}
		
		MultiDisplayPlot::~MultiDisplayPlot()
		{
		}

		void MultiDisplayPlot::InitMultiDisplayPlot(int master_id)
		{
			CreateDisplay();
			SetMasterId(master_id);

			CLAM_ASSERT(mDisplay.size()>0,
						"MultiDisplayPlot::InitMultiDisplayPlot: master display has not been created yet.");
			CLAM_ASSERT(mMasterId >=0 && mMasterId < (int)mDisplay.size(), 
						"MultiDisplayPlot::InitMultiDisplayPlot: invalid masterId.");

			mXRuler = new Ruler(this,CLAM::VM::eTop);
			mXRuler->setFixedHeight(40);
			mHScroll = new ScrollGroup(CLAM::VM::eHorizontal,this);
			mHScroll->setFixedHeight(20);

			// connect xruler and horizontal scroll group
			mDisplay[mMasterId]->setXRuler(mXRuler);
			mDisplay[mMasterId]->setXRangeController(mHScroll);

			connect(mHScroll,SIGNAL(zoomIn()),mDisplay[mMasterId],SLOT(hZoomIn()));
			connect(mHScroll,SIGNAL(zoomOut()),mDisplay[mMasterId],SLOT(hZoomOut()));
			connect(mHScroll,SIGNAL(scrollValueChanged(int)),mDisplay[mMasterId],SLOT(updateHScrollValue(int)));

			// synchronize horizontal scrolling
			for(unsigned i=0; i < mDisplay.size(); i++)
			{
				if(i == (unsigned)mMasterId) continue;
				connect(mDisplay[mMasterId],SIGNAL(xRangeChanged(double,double)),
						mDisplay[i],SLOT(setHBounds(double,double)));
			}
		}

		void MultiDisplayPlot::SetMasterId(int id)
		{
			CLAM_ASSERT(mDisplay.size()>0,"MultiDisplayPlot::SetMasterId: master display has not been created yet.");
			CLAM_ASSERT(id >=0 && id < (int)mDisplay.size(), "MultiDisplayPlot::SetMasterId: invalid masterId.");
			mMasterId = id;
		}
	}
}

// END

