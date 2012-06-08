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

#ifndef __DATARENDERER__
#define __DATARENDERER__

#include <CLAM/Array.hxx>
#include <CLAM/Renderer.hxx>

namespace CLAM
{
	namespace VM
	{
		enum {NormalMode=0,DetailMode,HugeMode};

		class DataRenderer : public Renderer
		{
		public:
			DataRenderer();
			~DataRenderer();

			void SetData(const DataArray& data, int mode);
			void SetArrays(const DataArray& maxsArray,const DataArray& minsArray);
			void SetStep(const TData& step);
			const TData& GetStep() const;
			void Render();

		private:
			const DataArray* mData;
			const DataArray* mMaxs;
			const DataArray* mMins;
			unsigned     mElems;
			int          mMode;
			TData        mStep;

			void RenderingNormalMode();
			void RenderingDetailMode();
			void RenderingHugeMode();
		};
	}
}

#endif


