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

#include "CLAMGL.hxx"
#include "MPDataRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
		MPDataRenderer::MPDataRenderer()
		{
			SetColor(VMColor::Blue());
		}

		MPDataRenderer::~MPDataRenderer()
		{
		}

		void MPDataRenderer::SetData(const TData* data,unsigned nElems)
		{
			_data.Resize(nElems);
			_data.SetSize(nElems);
			for(int i=0; i < _data.Size(); i++)
			{
				_data[i]=data[i];
			}
		}

		void MPDataRenderer::Render()
		{
			glColor3ub(GLubyte(_color.r),GLubyte(_color.g),GLubyte(_color.b));
			glBegin(GL_LINE_STRIP);
			for(unsigned i = 0; i < unsigned(_data.Size()); i++)
			{
				glVertex3f(float(i),float(_data[i]),-1.0f);
			}
			glEnd();
		}
	}
}

// END

