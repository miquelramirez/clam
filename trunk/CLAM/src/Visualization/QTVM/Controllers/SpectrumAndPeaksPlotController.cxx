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

#include <algorithm>
#include "SpectrumAndPeaksPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		SpectrumAndPeaksPlotController::SpectrumAndPeaksPlotController()
		{
		}

		SpectrumAndPeaksPlotController::~SpectrumAndPeaksPlotController()
		{
		}

		void SpectrumAndPeaksPlotController::SetData(const Spectrum& spec,
													 const SpectralPeakArray& peaks)
		{
		    SpectrumPlotController::SetData(spec);
		    mPeaks = peaks;
		    CachePeaksData();
		    mPeaksRenderer.SetVBounds(mView.top,mView.bottom);
		    emit requestRefresh();
		}

		void SpectrumAndPeaksPlotController::SetPeaksColor(Color cline,Color cpoint)
		{
		    mPeaksRenderer.SetPeakColor(cline,cpoint);
		}

		void SpectrumAndPeaksPlotController::Draw()
		{
		    if(MustProcessData()) ProcessPeaksData();
		    mPeaksRenderer.Render();
		    SpectrumPlotController::Draw();
		}

		void SpectrumAndPeaksPlotController::CachePeaksData()
		{
			int i,index;
			TData value;
			TSize size = GetnSamples();
			TSize nPeaks = mPeaks.GetMagBuffer().Size();
			bool linear = (mPeaks.GetScale() == EScale::eLinear);

			mCacheData.Init();
			mCacheData.Resize(size);;
			mCacheData.SetSize(size);
				
			for(i = 0;i < nPeaks;i++)
			{
				value = mPeaks.GetMagBuffer()[i];
				if(linear) value = 20.0*log10(value);
				if(value >= 0) value = -0.1;
				index = int(mPeaks.GetFreqBuffer()[i]*TData(size)/GetSpectralRange());
				mCacheData[index] = value;
			}
			ProcessPeaksData();
		}

		void SpectrumAndPeaksPlotController::ProcessPeaksData()
		{
			if(!mCacheData.Size()) return;
			TSize offset = TSize(GetLeftBound());
			TSize len = TSize(GetRightBound() - GetLeftBound())+1;

			if(mProcessedPeaks.Size() <= len)
				mProcessedPeaks.Resize(len+1);
			mProcessedPeaks.SetSize(len+1);

			std::copy(mCacheData.GetPtr()+offset,mCacheData.GetPtr()+offset+len+1,mProcessedPeaks.GetPtr());

			for(int i=0;i < mProcessedPeaks.Size();i++) mProcessedPeaks[i] += GetvRange();

			mPeaksRenderer.SetDataPtr(mProcessedPeaks.GetPtr(),mProcessedPeaks.Size());
		}

	}
}

// END



