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
			: mLeftBound(0.0)
			, mRightBound(1.0)
			, mTopBound(1.0)
			, mBottomBound(0.0)
			, mColor(VMColor::Green())
		{
		}

		Renderer::~Renderer()
		{
		}

		void Renderer::SetHBounds(double left, double right)
		{
			mLeftBound = left;
			mRightBound = right;
		}

		void Renderer::SetVBounds(double bottom, double top)
		{
		    mTopBound = top;
			mBottomBound = bottom;
		}

		void Renderer::SetColor(const Color& c)
		{
			mColor = c;
		}

		const Color& Renderer::GetColor() const
		{
			return mColor;;
		}

		double Renderer::LeftBound() const
		{
			return mLeftBound;
		}

		double Renderer::RightBound() const
		{
			return mRightBound;
		}

		double Renderer::BottomBound() const
		{
			return mBottomBound;
		}

		double Renderer::TopBound() const
		{
			return mTopBound;
		}

	}
}

// END
