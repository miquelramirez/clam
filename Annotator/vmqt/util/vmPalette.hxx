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

#ifndef __VMQT_PALETTE_H__
#define __VMQT_PALETTE_H__

#include <map>
#include "Assert.hxx"
#include "DataTypes.hxx"
#include "vmDataTypes.hxx"
#include <QtGui/QColor>

namespace CLAM
{
	namespace VM
	{
		/** 
		 * Class to convert values in the range 0-1 to a color scale.
		 * The color scale can be adjusted, so weaker values can be
		 * emphasized, are be darkened, so the stronger values stick
		 * out more clearly.
		 *
		 */
		class Palette
		{
			typedef std::map<int,QColor> Colormap;
			
			static const int NCOLORMAPPING;
			static const int FIRST_INDEX;
			static const int NCOLORS;

			static Colormap mColormap;

		public:
			Palette(float v);
			~Palette();

			void Value(float v);	
			float Value() { return mValue; }
			void GetRGBFromIndex( int idx, QColor & c)
			{
				c = mColormap[idx];
			}
			void GetRGBFromIndex( int idx, unsigned short& r, unsigned short& g, unsigned short& b )
			{
				QColor c = mColormap[idx];
				r = c.red();
				g = c.green();
				b = c.blue();
			}

			int Get(float v);
				
		protected:
			static int mInited;

			static void InitColors();
			// This value allows to introduce a 'boost' for input values,
			// thus 'saturating' the resulting colors
			float mValue;
			int*  mColormapping;

			void CalcColormap();

		};

		inline void Palette::Value(float v)
		{
			//D/ Adjust the color scale.
			mValue=v; CalcColormap();
		}

		inline int Palette::Get(float v) 
		{
			// Return the corresponding scaled color for value v (0-1).
			return (FIRST_INDEX+mColormapping[int(v*float(NCOLORMAPPING-1))]);
		}
	}
}

#endif


