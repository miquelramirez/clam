#ifndef __VMQT_SPECTRUM_PLOT_H__
#define __VMQT_SPECTRUM_PLOT_H__

#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "vm_segmentation_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		class SpectrumPlot : public SegmentationPlot
		{
		public:
			SpectrumPlot(QWidget* parent=0);
			~SpectrumPlot();

			void set_data(const Spectrum& spec, bool update=false);
			void set_data(const Spectrum& spec, const SpectralPeakArray& peaks, bool update=false);
			
		public slots:
		    void showPeaksOnly();
			void showSpectrumOnly();
			void showSpectrumAndPeaks();
			void backgroundWhite();
			void backgroundBlack();
			
		private:
			void init_spectrum_plot();
		   
			void set_xrange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			void set_yrange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);
			void set_zoom_steps(int hsteps, int vsteps);

			DataArray get_spectrum_data(const CLAM::Spectrum& in);
			Array<Peak> get_peaks_data(const SpectralPeakArray& in);
			std::pair<int,int> get_zoom_steps(TSize size);
			
		};
	}
}

#endif

