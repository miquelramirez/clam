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

#ifndef __VMQT_PEAKS_RENDERER_H__
#define __VMQT_PEAKS_RENDERER_H__
#include "Array.hxx"
#include "vmRenderer2D.hxx"

namespace CLAM
{
	namespace VM
	{
		class PeaksRenderer : public Renderer2D
		{
		public:
			PeaksRenderer();
			~PeaksRenderer();

			void SetPeaks(const Array<Peak>& peaks);
			void SetPeaksColor(const Color& cpoint, const Color& cline);

		protected:
			void Render();
			void MouseMoveEvent(double x, double y);

		private:
			Array<Peak> mCachedPeaks;

			Color mCpoint;
			Color mCline;

			enum { LINE_WIDTH=1, POINT_SIZE=3, TOLERANCE=3 };

			int PickPeak(double x, double y);
		};
	};
}

#endif

