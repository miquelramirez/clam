/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************
 *   Maarten de Boer <maarten.deboer@iua.upf.es>, 1999                     *
 *   Music Technology Group                                                *
 *   Institut de l'Audiovisual, Universitat Pompeu Fabra, Barcelona, Spain *
 *   http://www.iua.upf.es/mtg/                                            *
 ***************************************************************************/

#ifndef __FL_PALETTE__
#define __FL_PALETTE__

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include "Assert.hxx"

namespace CLAMVM
{



class Fl_Palette
/*D* 
  Class to convert values in the range 0-1 to a color scale.
  The color scale can be adjusted, so weaker values can be
	emphasized, are be darkened, so the stronger values stick
	out more clearly.

	Note that this changes the FLTK color map.
*/
{
		static const int NCOLORMAPPING;
		static const int FIRST_INDEX;
		static const int NCOLORS;
protected:
		static int inited_;
		static void InitColors(void);
		// This value allows to introduce a 'boost' for input values,
		// thus 'saturating' the resulting colors
		float value_;
		int* colormapping;
		void CalcColorMap(void);
public:
		void value(float v);
		
		float value(void) { return value_; }
		
		void GetRGBFromIndex( int idx, unsigned char& r, unsigned char& g, unsigned char& b )
		{
			Fl::get_color( (Fl_Color)idx, r, g, b );
		}

		Fl_Color Get(float v);
		
		Fl_Palette(float v);
		~Fl_Palette();
};

inline void Fl_Palette::value(float v)
{
		//D/ Adjust the color scale.
		value_=v; CalcColorMap();
}

inline Fl_Color Fl_Palette::Get(float v) 
{
		// Return the corresponding scaled color for value v (0-1).
		return (Fl_Color) (FIRST_INDEX+colormapping[int(v*float(NCOLORMAPPING-1))]);
}

}
#endif
