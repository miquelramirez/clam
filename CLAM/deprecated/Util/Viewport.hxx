/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __VIEWPORT__
#define __VIEWPORT__

#include "DataTypes.hxx"
using CLAM::TData;

namespace CLAMGUI
{
	struct Viewport
	{
		TData top;
		bool  top_es;
		TData bottom;
		bool  bottom_es;
		TData left;
		TData right;
		bool isIntX;
		bool isIntY;
		
		Viewport()
			: top( 0 ), top_es(false), bottom( 0 ), bottom_es( false ), left( 0 ), right( 0 ),
			  isIntX( false ), isIntY( false )
			{
			}
		
		void SetTopEstimation( TData estimation );
		
		void SetBottomEstimation( TData estimation );

		void Dump() const;

	};
}

#endif // Viewport.hxx

