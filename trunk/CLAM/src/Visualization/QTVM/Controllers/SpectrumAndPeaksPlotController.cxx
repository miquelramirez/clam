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
			_peaks = peaks;
			CachePeaksData();
			FullView();
			_peaksRenderer.SetVBounds(_view.top,_view.bottom);
		}

		void SpectrumAndPeaksPlotController::SetPeaksColor(Color cline,Color cpoint)
		{
			_peaksRenderer.SetPeakColor(cline,cpoint);
		}

		void SpectrumAndPeaksPlotController::Draw()
		{
			if(MustProcessData()) ProcessPeaksData();
			_peaksRenderer.Render();
			SpectrumPlotController::Draw();
		}

		void SpectrumAndPeaksPlotController::CachePeaksData()
		{
			int i,index;
			TData value;
			TSize size = GetnSamples();
			TSize nPeaks = _peaks.GetMagBuffer().Size();
			bool linear = (_peaks.GetScale() == EScale::eLinear);

			_cacheData.Init();
			_cacheData.Resize(size);;
			_cacheData.SetSize(size);
				
			for(i = 0;i < nPeaks;i++)
			{
				value = _peaks.GetMagBuffer()[i];
				if(linear) value = 20.0*log10(value);
				index = int(_peaks.GetFreqBuffer()[i]*TData(size)/GetSpectralRange());
				_cacheData[index] = value;
			}
		}

		void SpectrumAndPeaksPlotController::ProcessPeaksData()
		{
			TSize offset = TSize(GetLeftBound());
			TSize len = TSize(GetRightBound() - GetLeftBound())+1;

			if(_processedPeaks.Size() <= len)
				_processedPeaks.Resize(len+1);
			_processedPeaks.SetSize(len+1);

			std::copy(_cacheData.GetPtr()+offset,_cacheData.GetPtr()+offset+len+1,_processedPeaks.GetPtr());

			for(int i=0;i < _processedPeaks.Size();i++) _processedPeaks[i] += GetvRange();

			_peaksRenderer.SetDataPtr(_processedPeaks.GetPtr(),_processedPeaks.Size());
		}

	}
}

// END



