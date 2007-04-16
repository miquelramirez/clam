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

#ifndef __VMCOLOR__
#define __VMCOLOR__

#include "DataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		struct Color
		{
		    TUInt16 r;
		    TUInt16 g;
		    TUInt16 b;

		    Color();
		    Color(TUInt16 red, TUInt16 green, TUInt16 blue);
		    Color(const Color& c);
		    ~Color();

		    Color& operator=(Color c);
		    bool operator==(Color c);
		};
		
		class VMColor
		{
		public:
			static Color Red();
			static Color Green();
			static Color Blue();
			static Color Magenta();
			static Color Cyan();
			static Color Yellow();
			static Color White();
			static Color Black();
			static Color Gray();
			static Color LightGray();
			static Color Brown();
			static Color Orange();
			static Color Custom(TUInt16 r, TUInt16 g, TUInt16 b);
		};

	}
}

#endif

