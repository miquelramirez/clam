/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "GridLayout.hxx"
using namespace CLAMGUI;

Recti GridLayout::operator<< (const Recti& r)
{
	double cl = static_cast<double> (mCols);
	double rw = static_cast<double> (mRows);
	double x1 = static_cast<double> (r.GetX());
	double y1 = static_cast<double> (r.GetY());
	double w1 = static_cast<double> (r.GetW());
	double h1 = static_cast<double> (r.GetH());
	double Sw = static_cast<double> (mArea.GetW());
	double Sh = static_cast<double> (mArea.GetH());

	Recti newRect( static_cast<int>((x1/cl)*Sw),
	               static_cast<int>((y1/rw)*Sh),
	               static_cast<int>((w1/cl)*Sw),
	               static_cast<int>((h1/rw)*Sh) );

	return newRect;
}

