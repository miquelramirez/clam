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

#ifndef __SELTIMEREGIONPLOTCONTROLLER__
#define __SELTIMEREGIONPLOTCONTROLLER__

#include "MediaTime.hxx"
#include "RegionMarker.hxx"
#include "SelPosPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class SelTimeRegionPlotController : public SelPosPlotController
		{
			Q_OBJECT

			public:
				SelTimeRegionPlotController();
				virtual ~SelTimeRegionPlotController();

				virtual void SetSelPos(const TData& value);
				virtual void Draw();

				void SetRegionColor(Color c);
				void SetKeyShiftPressed(bool pr);

			signals:
				void selectedRegion(MediaTime);
			
			protected:
				virtual void SetHBounds(const TData& left,const TData& right);
				virtual void SetVBounds(const TData& bottom,const TData& top);

			private:
				RegionMarker _rMarker;
				bool _keyShiftPressed;
				
				TData GetBeginRegion() const;
				TData GetEndRegion() const;
				void SetBeginRegion(const TData& value);
				void SetEndRegion(const TData& value);

				MediaTime GetRegionTime() const;
		};
	}
}

#endif

