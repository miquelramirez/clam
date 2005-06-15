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
			: mEnabled(true)
			, mLeftBound(TData(0.0))
			, mRightBound(TData(0.0))
			, mTopBound(TData(0.0))
			, mBottomBound(TData(0.0))
			, mColor(VMColor::Green())
		{
		}

		Renderer::~Renderer()
		{
		}

		void Renderer::SetHBounds(const TData& left, const TData& right)
		{
			mLeftBound = left;
			mRightBound = right;
		}

		void Renderer::SetVBounds(const TData& top,const TData& bottom)
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

		void Renderer::SetEnabled(bool e)
		{
		    mEnabled = e;
		}

		const bool& Renderer::IsEnabled() const
		{
			return mEnabled;
		}

		const TData& Renderer::LeftBound() const
		{
			return mLeftBound;
		}

		const TData& Renderer::RightBound() const
		{
			return mRightBound;
		}

		const TData& Renderer::BottomBound() const
		{
			return mBottomBound;
		}

		const TData& Renderer::TopBound() const
		{
			return mTopBound;
		}

	}
}

// END
