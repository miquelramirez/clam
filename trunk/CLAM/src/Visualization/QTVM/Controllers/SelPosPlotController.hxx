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

#ifndef __SELPOSPLOTCONTROLLER__
#define _SELPOSPLOTCONTROLLER__

#include "VMColor.hxx"
#include "Dial.hxx"
#include "SegmentationMarksPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class SelPosPlotController : public SegmentationMarksPlotController
		{
			Q_OBJECT
			
			public:
				SelPosPlotController();
				virtual ~SelPosPlotController();

				virtual void SetSelPos(const TData& value);
				virtual void Draw();

				void SetDialColor(Color c);
				
			protected:
				TData _sampleRate;
				TData _duration;

				virtual void SetHBounds(const TData& left,const TData& right);
				virtual void SetVBounds(const TData& bottom,const TData& top);
				void UpdateDial(const TData& value);
				TData GetDialPos() const;
			
			private:
				Dial _dial;

		};
	}
}

#endif
