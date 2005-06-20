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

#ifndef __SPECTRUMANDPEAKSPLOTCONTROLLER__
#define __SPECTRUMANDPEAKSPLOTCONTROLLER__

#include "SpectralPeakArray.hxx"
#include "SpectralPeaksRenderer.hxx"
#include "SpectrumPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class SpectrumAndPeaksPlotController : public SpectrumPlotController
		{
		public:
			SpectrumAndPeaksPlotController();
			~SpectrumAndPeaksPlotController();

			void SetData(const Spectrum& spec,const SpectralPeakArray& peaks);
			void SetPeaksColor(Color cline,Color cpoint);
			void Draw();

		private:
			SpectralPeakArray     mPeaks;
			DataArray             mProcessedPeaks;
			DataArray             mCacheData;        
			SpectralPeaksRenderer mPeaksRenderer;

			void CachePeaksData();
			void ProcessPeaksData();
		};
	}
}

#endif

