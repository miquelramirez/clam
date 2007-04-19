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

#ifndef __QTPALETTE__
#define __QTPALETTE__

#include <qglcolormap.h>
#include <CLAM/Assert.hxx>
#include <CLAM/DataTypes.hxx>

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
		class QtPalette
		{
			static const int NCOLORMAPPING;
			static const int FIRST_INDEX;
			static const int NCOLORS;

			static QGLColormap mColorMap;

		protected:
			static int mInited;

			static void InitColors(void);
			// This value allows to introduce a 'boost' for input values,
			// thus 'saturating' the resulting colors
			float mValue;
			int*  mColorMapping;
			void CalcColorMap(void);
		public:
			void value(float v);	
			float value(void) { return mValue; }
			void GetRGBFromIndex( int idx, TUInt16& r, TUInt16& g, TUInt16& b )
				{
					QColor c = mColorMap.entryColor(idx);
					r = (TUInt16)c.red();
					g = (TUInt16)c.green();
					b = (TUInt16)c.blue();
				}

			int Get(float v);
				
			QtPalette(float v);
			~QtPalette();
		};

		inline void QtPalette::value(float v)
		{
			//D/ Adjust the color scale.
			mValue=v; CalcColorMap();
		}

		inline int QtPalette::Get(float v) 
		{
			// Return the corresponding scaled color for value v (0-1).
			return (FIRST_INDEX+mColorMapping[int(v*float(NCOLORMAPPING-1))]);
		}
	}

}

#endif


