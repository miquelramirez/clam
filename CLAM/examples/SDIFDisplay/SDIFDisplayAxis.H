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

#ifndef __SDIFDisplayAxis__
#define __SDIFDisplayAxis__

#include <Fl/Fl_Widget.H>

class SDIFDisplay;

#define FL_HOR_AXIS 1
#define FL_VER_AXIS 2

class SDIFDisplayAxis:public Fl_Widget
{
public:
	SDIFDisplay* mDisplay;
	
	SDIFDisplayAxis(int X,int Y,int W,int H,int T,SDIFDisplay* d);

	void draw(void);
};

#endif
