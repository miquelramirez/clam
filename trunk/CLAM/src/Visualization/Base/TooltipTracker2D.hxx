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

#ifndef __TOOLTIPTRACKER2D__
#define __TOOLTIPTRACKER2D__

#include "Slotv4.hxx"

namespace CLAMVM
{

	class DrawingSurface2D;

	class TooltipTracker2D
	{
	public:
		TooltipTracker2D();
		virtual ~TooltipTracker2D();

		void Track( DrawingSurface2D* surf );
		bool HandleMotion( int x, int y );
		void ForceText( const char* text );
		
		SigSlot::Slotv4< int, int, char*, int >  RenderTooltipText;

	protected:

		void DefaultTooltipTextRender( int x, int y, char* text, int maxLength );

		char               mTextBuffer[256];
		DrawingSurface2D*  mTrackedSurface;
	};

}

#endif // TooltipTracker2D.hxx
