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

#ifndef __RENDERER2D__
#define __RENDERER2D__

#include "Array.hxx"
#include "CLAMGL.hxx"
#include "VertexFormats.hxx"
#include "Renderer2D.hxx"
using CLAM::DataArray;

namespace CLAMGUI
{

class Renderer2D
{

public:

	Renderer2D(const c3f &color) : mColor(color) {}

	~Renderer2D() {}

	void SetColor(const c3f &color) { mColor = color; }

	void SetWidth(GLfloat size) { mGLsize = size; }

protected:

	c3f      mColor;

	GLfloat  mGLsize;

};


}

#endif // ArrayRenderer2D.hxx
