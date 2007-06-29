/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __VMQT_DATA_ARRAY_RENDERER_H__
#define __VMQT_DATA_ARRAY_RENDERER_H__

#include "Array.hxx"
#include "vmRenderer2D.hxx"
#include <vector>
#include <QtGui/QColor>

namespace CLAM
{
	namespace VM
	{

		class DataArrayRenderer : public Renderer2D
		{
		public:
			DataArrayRenderer();
			~DataArrayRenderer();
	
			void SetData(const DataArray& data);
			void SetDataColor(const QColor& c);

			void SetHugeModeEnabled(bool en);

		protected:
			void Render();
			void SetHBounds(double left, double right);
			void SetVBounds(double bottom, double top);
			void SetViewport(const GLViewport& v);
			
		private:
			std::vector<CLAM::TData> mCachedData;
			std::vector<CLAM::TData> mProcessedData;
			std::vector<CLAM::TData> mMinArray;
			std::vector<CLAM::TData> mMaxArray;

			bool mHugeMode;
			bool mHugeModeEnabled;
			bool mMustProcessData;

			QColor  mDataColor;
			GLView mLocalView;
			
			void ProcessData();
			void DrawHugeMode();
			void DrawNormalMode();
		};
	}
}

#endif


