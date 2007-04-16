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

#include "VMColor.hxx"

namespace CLAM
{
    namespace VM
    {
		Color::Color()
			: r(0)
			, g(0)
			, b(0)
		{}

		Color::Color(TUInt16 red, TUInt16 green, TUInt16 blue)
			: r(red)
			, g(green)
			, b(blue)
		{}

		Color::Color(const Color& c)
			: r(c.r)
			, g(c.g)
			, b(c.b)
		{}

		Color::~Color()
		{
		}

		Color& Color::operator=(Color c)
		{
			r=c.r;
			g=c.g;
			b=c.b;
			return *this;
		}
	
		bool Color::operator==(Color c)
		{
			return (r==c.r && g==c.g && b==c.b);
		}

		Color VMColor::Red()
		{
			return Custom(255,0,0);
		}

		Color VMColor::Green()
		{
			return Custom(0,255,0);
		}

		Color VMColor::Blue()
		{
			return Custom(0,0,255);
		}

		Color VMColor::Magenta()
		{
			return Custom(255,0,255);
		}

		Color VMColor::Cyan()
		{
			return Custom(0,255,255);
		}

		Color VMColor::White()
		{
			return Custom(255,255,255);
		}

		Color VMColor::Black()
		{
			return Custom(0,0,0);
		}

		Color VMColor::Yellow()
		{
			return Custom(255,255,0);
		}

		Color VMColor::Gray()
		{
			return Custom(127,127,127);
		}
		
		Color VMColor::LightGray()
		{
			return Custom(185,185,185);
		}

		Color VMColor::Brown()
		{
			return Custom(153,102,31);
		}

		Color VMColor::Orange()
		{
			return Custom(250,159,31);
		}

		Color VMColor::Custom(TUInt16 r, TUInt16 g, TUInt16 b)
		{
			Color c;
			c.r = r;
			c.g = g;
			c.b = b;
			return c;
		}

    }
}

// END

