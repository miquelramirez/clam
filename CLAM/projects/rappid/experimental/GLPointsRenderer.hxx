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

#ifndef __GLPOINTSRENDERER__
#define __GLPOINTSRENDERER__

#include "Array.hxx"
#include "CLAMGL.hxx"
#include "VertexFormats.hxx"
#include "PointsRenderer2D.hxx"
using CLAM::DataArray;

namespace CLAMGUI
{

class GLPointsRenderer : public PointsRenderer2D
{

public:

	GLPointsRenderer(const c3f &color = c3f(1.0,1.0,1.0),
						unsigned size = 1024,
						TData x_first = 0.0,
						TData x_last  = 1.0);

	~GLPointsRenderer() {}

	void Draw();

	void Draw(TData x_start, TData x_width);

protected:

	void ResetXValues();

	void ResizeVertexArray(unsigned n_data_elems);

	void DataTransform(const DataArray& positions,
					   const DataArray& values);

};


}

#endif // GLPointsRenderer.hxx
