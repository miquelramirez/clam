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

#ifndef __VERTEXFORMATS__
#define __VERTEXFORMATS__

#include "CLAMGL.hxx"
#include "Assert.hxx"

namespace CLAMGUI
{


	// One simple vertex format

	struct c3f_v3f
	{
		GLfloat r;
		GLfloat g;
		GLfloat b;

		GLfloat x;
		GLfloat y;
		GLfloat z;

		GLfloat& operator[]( unsigned int elem_idx )
		{
			return *( &r + elem_idx );
		}

		GLfloat operator[]( unsigned int elem_idx ) const
		{
			return *( &r + elem_idx );
		}

		static GLuint Size()
		{
			return sizeof( GLfloat ) * 6;
		}

	};

	// And one yet simpler vertex format. When depth and changing
	// color are not needed, a more compact memory storage allows more
	// efficient memory access, due to less cache misses.

	struct v2f
	{
		GLfloat x;
		GLfloat y;

		GLfloat& operator[]( unsigned int elem_idx )
		{
			CLAM_DEBUG_ASSERT(elem_idx < Size(),"CLAMGUI::v2f:: index too big");
			return *( &x + elem_idx );
		}

		GLfloat operator[]( unsigned int elem_idx ) const
		{
			CLAM_DEBUG_ASSERT(elem_idx < Size(),"CLAMGUI::v2f:: index too big");
			return *( &x + elem_idx );
		}

		static GLuint Size()
		{
			return sizeof( GLfloat ) * 2;
		}

	};

	// And only the color...

	struct c3f
	{
		GLfloat r;
		GLfloat g;
		GLfloat b;

		GLfloat& operator[]( unsigned int elem_idx )
		{
			CLAM_DEBUG_ASSERT(elem_idx < Size(),"CLAMGUI::c3f:: index too big");
			return *( &r + elem_idx );
		}

		GLfloat operator[]( unsigned int elem_idx ) const
		{
			CLAM_DEBUG_ASSERT(elem_idx < Size(),"CLAMGUI::c3f:: index too big");
			return *( &r + elem_idx );
		}

		static GLuint Size()
		{
			return sizeof( GLfloat ) * 3;
		}

		c3f() : r(1.0), g(1.0), b(1.0) {}
		c3f(GLfloat re, GLfloat gr, GLfloat bl) : r(re), g(gr), b(bl) {}

	};



}

#endif // VertexFormats.hxx
