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
#include "RegionMarker.hxx"

namespace CLAM
{
	namespace VM
	{
		RegionMarker::RegionMarker()
		{
			_data[0] = _data[1] = 0;
			SetColor(VMColor::LightGray());
		}

		RegionMarker::~RegionMarker()
		{
		}

		void RegionMarker::Render()
		{
			if(IsEnabled())
			{
				if(_data[0] < _data[1])	
				{
					glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
					glEnable(GL_BLEND);
					glColor4ub(GLubyte(_color.r),GLubyte(_color.g),GLubyte(_color.b),185);
					DrawRegion(GetDrawMode(0));
					glDisable(GL_BLEND);
				}
			}
		}

		int RegionMarker::GetDrawMode(int index)
		{
			// 0 -> neither the points nor the region are visible
			// 1 -> the region is visible but the points are not  
			// 2 -> the first point is visible but the second is not
			// 3 -> the second point is visible but the first is not
			// 4 -> both points are visible
			TData p1 = _data[index];
			TData p2 = _data[index+1];
			if(p1 < _bounds.left && p2 > _bounds.right) return 1;
			if(p1 >= _bounds.left && p2 > _bounds.right) return 2;
			if(p1 < _bounds.left && p2 <= _bounds.right) return 3;
			if(p1 >= _bounds.left && p2 <= _bounds.right) return 4;
			return 0;
		}

		void RegionMarker::DrawRegion(int mode)
		{
			glBegin(GL_QUADS);
			switch(mode)
			{
				case 1:
						// the region is visible but the points are not
						glVertex2f(0.0f,float(_top));
						glVertex2f(float(_bounds.right-_bounds.left),float(_top));
						glVertex2f(float(_bounds.right-_bounds.left),float(_bottom));
						glVertex2f(0.0f,float(_bottom));
						break;
				case 2:
						// the first point is visible but the second is not
						glVertex2f(float(_data[0]-_bounds.left),float(_top));
						glVertex2f(float(_bounds.right-_bounds.left),float(_top));
						glVertex2f(float(_bounds.right-_bounds.left),float(_bottom));
						glVertex2f(float(_data[0]-_bounds.left),float(_bottom));
						break;
				case 3:
						// the second point is visible but the first is not
						glVertex2f(0.0f,float(_top));
						glVertex2f(float(_data[1]-_bounds.left),float(_top));
						glVertex2f(float(_data[1]-_bounds.left),float(_bottom));
						glVertex2f(0.0f,float(_bottom));
						break;
				case 4:
						// both points are visible
						glVertex2f(float(_data[0]-_bounds.left),float(_top));
						glVertex2f(float(_data[1]-_bounds.left),float(_top));
						glVertex2f(float(_data[1]-_bounds.left),float(_bottom));
						glVertex2f(float(_data[0]-_bounds.left),float(_bottom));
						break;
				default:
					    // neither the points nor the region are visible
						break;
			}
			glEnd();
		}

		void RegionMarker::SetBegin(const TData& value)
		{
			_data[0] = value;
		}

		void RegionMarker::SetEnd(const TData& value)
		{
			_data[1] = value;
		}

		TData RegionMarker::GetBegin() const
		{
			return _data[0];

		}

		TData RegionMarker::GetEnd() const
		{
			return _data[1];
		}
	}
}

// END
