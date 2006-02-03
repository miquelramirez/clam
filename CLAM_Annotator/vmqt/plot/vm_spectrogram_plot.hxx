#ifndef __VMQT_SPECTROGRAM_PLOT_H__
#define __VMQT_SPECTROGRAM_PLOT_H__

#include "Segment.hxx"
#include "vm_segmentation_plot.hxx"

class QLabel;

namespace CLAM
{
	namespace VM
	{
		class SpecgramPanel;

		class SpectrogramPlot : public SegmentationPlot
		{
			Q_OBJECT
		public:
			SpectrogramPlot(QWidget* parent=0);
			~SpectrogramPlot();

			void set_data(const Segment& segment, bool update=false);

		public slots:
			void colorSpecgram();
			void blackWhiteSpecgram();

		private slots:
			void updateColorScale(int);

		private:
			int            wp_sonogram_colormap;
			int            wp_color_scale_width;
			SpecgramPanel* wp_panel;
		   
			enum { COLOR_SONOGRAM=0, BW_SONOGRAM=1 };

			void init_spectrogram_plot();
		   
			void set_xrange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			void set_yrange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);
			void set_zoom_steps(int hsteps, int vsteps);

			std::pair<int,int> get_zoom_steps(TSize size, TData yrange);
			Array<Spectrum> get_spec_matrix(const Segment& in);
		};
	}
}

#endif

