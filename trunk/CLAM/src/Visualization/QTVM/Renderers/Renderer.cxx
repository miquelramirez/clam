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

#include "Renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		Renderer::Renderer()
		{
			SetHBounds(TData(0.0),TData(1.0));
			SetVBounds(TData(1.0),TData(-1.0));
			SetEnabled(true);
			SetColor(VMColor::Green());
		}

		Renderer::~Renderer()
		{
		}

		void Renderer::SetHBounds(const TData& left, const TData& right)
		{
			_bounds.left = left;
			_bounds.right = right;
		}

		void Renderer::SetVBounds(const TData& top,const TData& bottom)
		{
			_top = top;
			_bottom = bottom;
		}

		void Renderer::SetColor(Color c)
		{
			_color.r = c.r;
			_color.g = c.g;
			_color.b = c.b;
		}

		Color Renderer::GetColor() const
		{
			return _color;
		}

		void Renderer::SetEnabled(bool e)
		{
			_enabled = e;
		}

		bool Renderer::IsEnabled()
		{
			return _enabled;
		}
	}
}

// END
