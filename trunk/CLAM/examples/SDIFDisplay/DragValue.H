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

#ifndef DragValue_H
#define DragValue_H

#ifndef Fl_Valuator_H
#include <FL/Fl_Valuator.H>
#endif

class DragValue : public Fl_Valuator {
  uchar soft_,down_;
public:
  int handle(int);
  void draw();
  DragValue(int x,int y,int w,int h,const char *l=0);

  void soft(uchar x) {soft_ = x;}
  uchar soft() const {return soft_;}
};

#endif

