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
				virtual ~SpectrumAndPeaksPlotController();

				void SetData(const Spectrum& spec,const SpectralPeakArray& peaks);
				void SetPeaksColor(Color cline,Color cpoint);
				void Draw();

			private:
				SpectralPeakArray _peaks;
				DataArray _processedPeaks;
				DataArray _cacheData;

				SpectralPeaksRenderer _peaksRenderer;

				void CachePeaksData();
				void ProcessPeaksData();
		};
	}
}

#endif

