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

#ifndef __FL_AXIS_ENUMERATIONS__
#define __FL_AXIS_ENUMERATIONS__

namespace CLAMVM
{

#define DEFAULT_POINT_SIZE 4
#define FL_AXIS_DEFAULT_LABEL_SIZE 12

enum Fl_Axis_Damage 
{
		FL_AXIS_DAMAGE_ALL=1, 
		FL_AXIS_DAMAGE_ADD=2
};

enum Fl_Axis_When
{
		FL_AXIS_WHEN_MIN=1,
		FL_AXIS_WHEN_MAX=2
};

enum Fl_Axis_AxisAlign
{
		FL_AXIS_LEFT=0, 
		FL_AXIS_RIGHT=1, 
		FL_AXIS_BOTTOM=FL_AXIS_LEFT, 
		FL_AXIS_TOP=FL_AXIS_RIGHT,
		FL_AXIS_CENTER=3,
		FL_AXIS_ALIGNMENT=3,
		FL_AXIS_NO_LABELS=32, 
		FL_AXIS_NO_TICS=64, 
		FL_AXIS_LINE=128
};

enum Fl_Axis_PointStyle
{
		FL_AXIS_SIMPLE=0,
		FL_AXIS_ROUND=1,
		FL_AXIS_SQUARE=2,
		FL_AXIS_UP_TRIANGLE=3, 
		FL_AXIS_DOWN_TRIANGLE=4,
		FL_AXIS_DIAMOND=5,
		FL_AXIS_NO_POINT=6, 
		FL_AXIS_POINT_STYLE=127, 
		FL_AXIS_BORDER=128
};

enum Fl_Axis_GridVisible
{
		FL_AXIS_MINOR_GRID=0xF,
		FL_AXIS_LEFT_MINOR_TICK=0x1, 
		FL_AXIS_BOTTOM_MINOR_TICK=FL_AXIS_LEFT_MINOR_TICK,
		FL_AXIS_RIGHT_MINOR_TICK=0x2, 
		FL_AXIS_TOP_MINOR_TICK=FL_AXIS_RIGHT_MINOR_TICK, 
		FL_AXIS_MINOR_TICK=0x3,
		FL_AXIS_MAJOR_GRID=0xF0, 
		FL_AXIS_LEFT_MAJOR_TICK=0x10, 
		FL_AXIS_BOTTOM_MAJOR_TICK=FL_AXIS_LEFT_MAJOR_TICK,
		FL_AXIS_RIGHT_MAJOR_TICK=0x20, 
		FL_AXIS_TOP_MAJOR_TICK=FL_AXIS_RIGHT_MAJOR_TICK,
		FL_AXIS_MAJOR_TICK=0x30,
		FL_AXIS_LABEL_GRID=0xF00,
		FL_AXIS_LEFT_LABEL_TICK=0x100,
		FL_AXIS_BOTTOM_LABEL_TICK=FL_AXIS_LEFT_LABEL_TICK,
		FL_AXIS_RIGHT_LABEL_TICK=0x200, 
		FL_AXIS_TOP_LABEL_TICK=FL_AXIS_RIGHT_LABEL_TICK, 
		FL_AXIS_LABEL_TICK=0x300,
		FL_AXIS_ALWAYS_VISIBLE=0x1000,
		FL_AXIS_FRONT=0x2000
};

enum Fl_Axis_AxisType
{
		FL_AXIS_LIN=0, 
		FL_AXIS_LOG=1, 
		FL_AXIS_REV=2
};

}

#endif // Fl_Axis_Enumerations.hxx
