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

#ifndef __VMQT_SONOGRAM_BUILDER_H__
#define __VMQT_SONOGRAM_BUILDER_H__

#include <vector>
#include "Spectrum.hxx"
#include "vmPalette.hxx"
#include "vmDataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		class SonogramBuilder
		{
		public:
			SonogramBuilder();
			~SonogramBuilder();

			void MakeSonogram(const Array<Spectrum>& data_in,
							  std::vector<std::vector<float> >& data_out,
							  std::vector<std::vector<QColor> >& color_sonogram_out,
							  std::vector<std::vector<QColor> >& blackwhite_sonogram_out);

			std::vector<QColor> GetColorScale(int w);
			std::vector<QColor> GetGrayScale(int w);

		private:
			Palette mPalette;

			enum { MaxSpectrums=256, MaxSpectrumSize=256};
			
			float MatrixBlockMean(const Array<Spectrum>& specMtx, 
								  unsigned firstRow, unsigned lastRow, 
								  unsigned  firstCol, unsigned  lastCol);

			float ClampToRange(float value);
		};
	}
}

#endif

