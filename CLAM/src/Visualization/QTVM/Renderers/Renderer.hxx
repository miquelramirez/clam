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

#ifndef __RENDERER__
#define __RENDERER__

#include "DataTypes.hxx"
#include "VMColor.hxx"

namespace CLAM
{
	namespace VM
	{
		class Renderer
		{
		public:
			Renderer();
			virtual ~Renderer();

			void SetHBounds(const TData& left, const TData& right);
			void SetVBounds(const TData& top,const TData& bottom);
			void SetColor(const Color& c);
			const Color& GetColor() const;

			void SetEnabled(bool e);

			virtual void Render() = 0;

		protected:
			const bool& IsEnabled() const;

			const TData& LeftBound() const;
			const TData& RightBound() const;
			const TData& BottomBound() const;
			const TData& TopBound() const;

		private:
			bool mEnabled;
			TData mLeftBound;
			TData mRightBound;
			TData mTopBound;
			TData mBottomBound;
			Color mColor;

		};
	}
}

#endif

