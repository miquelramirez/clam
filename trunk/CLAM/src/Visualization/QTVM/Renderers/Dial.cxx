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
#include "Dial.hxx"

namespace CLAM
{
	namespace VM
	{
		Dial::Dial()
		{
			_data = 0;
			SetColor(VMColor::Black());
		}

		Dial::~Dial()
		{
		}

		void Dial::Render()
		{
			if(IsEnabled())
			{
				if(_data < _bounds.left || _data > _bounds.right) return;
				Draw();
			}
		}

		void Dial::Draw()
		{
			glColor3ub(GLubyte(_color.r),GLubyte(_color.g),GLubyte(_color.b));
			glBegin(GL_LINES);
				glVertex2f(float(_data-_bounds.left),float(_top));
				glVertex2f(float(_data-_bounds.left),float(_bottom));
			glEnd();
		}

		void Dial::Update(const TData& value)
		{
			_data = value;
		}

		TData Dial::GetPos() const
		{
			return _data;
		}
	}
}

// END
