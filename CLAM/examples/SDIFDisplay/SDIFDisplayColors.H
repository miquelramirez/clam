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

#ifndef __SDIFDISPLAYCOLORS__
#define __SDIFDISPLAYCOLORS__

#include <FL/fl_draw.H>

#define NCOLORMAPPING 8192
#define FIRST_INDEX 64
#define NCOLORS 64

class SDIFDisplayColors
/*D* 
  Class to convert values in the range 0-1 to a color scale.
  The color scale can be adjusted, so weaker values can be
	emphasized, are be darkened, so the stronger values stick
	out more clearly.

	Note that this changes the FLTK color map.
*/
{
protected:
	static int inited_;
	static void InitColors(void);
	float value_;
	int colormapping[NCOLORMAPPING];
	void CalcColorMap(void);
public:
	void value(float v);
	
	float value(void) { return value_; }

	Fl_Color Get(float v);

	SDIFDisplayColors(float v);
};

inline void SDIFDisplayColors::value(float v)
{
	//D/ Adjust the color scale.
	value_=v; CalcColorMap();
}

inline Fl_Color SDIFDisplayColors::Get(float v) {
	// Return the corresponding scaled color for value v (0-1).
	return (Fl_Color) (FIRST_INDEX+colormapping[int(v*float(NCOLORMAPPING-1))]);
}

#endif
