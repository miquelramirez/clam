/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef FL_SMART_TILE
#define FL_SMART_TILE

#include <FL/Fl_Group.H>

class Fl_Smart_Tile : public Fl_Group {
private:
	int current_;
	char action_;
	char buttondown_;
	int minsize_;
	void recalc(void);
public:
	Fl_Smart_Tile(int X,int Y,int W,int H,const char*l=0);
	void draw(void);
	int handle(int e);
	int minsize(void) const { return minsize_; }
	void minsize(int i) { minsize_ = i; }
	void add_adjust(Fl_Widget* w);
};

#endif //FL_SMART_TILE
