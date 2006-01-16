#ifndef __VMQT_SPECTRUM_PLOT_H__
#define __VMQT_SPECTRUM_PLOT_H__

#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "vm_segmentation_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		class SpectrumPlot : public CLAM::VM::SegmentationPlot
		{
		public:
			SpectrumPlot(QWidget* parent=0);
			~SpectrumPlot();

			void set_data(const CLAM::Spectrum& spec, bool update=false);
			void set_data(const CLAM::Spectrum& spec, const CLAM::SpectralPeakArray& peaks, bool update=false);
			
		public slots:
		    void showPeaksOnly();
			void showSpectrumOnly();
			void showSpectrumAndPeaks();
			void backgroundWhite();
			void backgroundBlack();
			
		private:
			CLAM::DataArray wp_spectrum_data;

			void init_spectrum_plot();
		   
			void set_xrange(double xmin, double xmax);
			void set_yrange(double ymin, double ymax);
			void set_zoom_steps(int hsteps, int vsteps);

			CLAM::DataArray get_spectrum_data(const CLAM::Spectrum& in);
			CLAM::Array<CLAM::VM::Peak> get_peaks_data(const CLAM::SpectralPeakArray& in);
			std::pair<int,int> get_zoom_steps(CLAM::TSize size);
			
		};
	}
}

#endif

