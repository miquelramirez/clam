#ifndef __VMQT_SPECTRUM_PLOT_H__
#define __VMQT_SPECTRUM_PLOT_H__

#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "vmSegmentationPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class SpectrumPlot : public SegmentationPlot
		{
		public:
			SpectrumPlot(QWidget* parent=0);
			~SpectrumPlot();

			void SetData(const Spectrum& spec, bool update=false);
			void SetData(const Spectrum& spec, const SpectralPeakArray& peaks, bool update=false);
			
		public slots:
		    void showPeaksOnly();
			void showSpectrumOnly();
			void showSpectrumAndPeaks();
			void backgroundWhite();
			void backgroundBlack();
			
		private:
			void InitSpectrumPlot();
		   
			void SetXRange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			void SetYRange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);
			void SetZoomSteps(int hsteps, int vsteps);

			DataArray GetSpectrumData(const CLAM::Spectrum& in);
			Array<Peak> GetPeaksData(const SpectralPeakArray& in);
			std::pair<int,int> GetZoomSteps(TSize size);
			
		};
	}
}

#endif

