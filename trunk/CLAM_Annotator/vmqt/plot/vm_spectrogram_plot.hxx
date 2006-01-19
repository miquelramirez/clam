#ifndef __VMQT_SPECTROGRAM_PLOT_H__
#define __VMQT_SPECTROGRAM_PLOT_H__

#include "Segment.hxx"
#include "vm_segmentation_plot.hxx"

class QLabel;

namespace CLAM
{
	namespace VM
	{
		class CLAM::VM::SpecgramPanel;

		class SpectrogramPlot : public CLAM::VM::SegmentationPlot
		{
			Q_OBJECT
		public:
			SpectrogramPlot(QWidget* parent=0);
			~SpectrogramPlot();

			void set_data(const CLAM::Segment& segment, bool update=false);

		public slots:
			void colorSpecgram();
			void blackWhiteSpecgram();

		private slots:
			void updateColorScale(int);

		private:
			int                         wp_sonogram_colormap;
			int                         wp_color_scale_width;
			CLAM::Array<CLAM::Spectrum> wp_spec_matrix;
			CLAM::VM::SpecgramPanel*    wp_panel;
		   
			enum { COLOR_SONOGRAM=0, BW_SONOGRAM=1 };

			void init_spectrogram_plot();
		   
			void set_xrange(double xmin, double xmax);
			void set_yrange(double ymin, double ymax);
			void set_zoom_steps(int hsteps, int vsteps);

			void build_spec_matrix(const CLAM::Segment& in);
			std::pair<int,int> get_zoom_steps(CLAM::TSize size, CLAM::TData yrange);

		};
	}
}

#endif

