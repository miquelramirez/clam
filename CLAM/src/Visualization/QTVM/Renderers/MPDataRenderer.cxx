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

		void MPDataRenderer::SetData(const TData* data, unsigned nElems)
		{
			mData.Resize(nElems);
			mData.SetSize(nElems);
			for(TIndex i=0; i < mData.Size(); i++)
			{
				mData[i] = data[i];
			}
		}

		void MPDataRenderer::Render()
		{
			glColor3ub(GLubyte(GetColor().r),GLubyte(GetColor().g),GLubyte(GetColor().b));
			glBegin(GL_LINE_STRIP);
			for(TIndex i = 0; i < mData.Size(); i++)
			{
				glVertex3f(float(i),float(mData[i]),-1.0f);
			}
			glEnd();
		}
	}
}

// END

