#ifndef __VMQT_SINTRACKS_PLOT_H__
#define __VMQT_SINTRACKS_PLOT_H__

#include "Segment.hxx"
#include "vm_segmentation_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		class SinTracksPlot : public CLAM::VM::SegmentationPlot
		{
		public:
			SinTracksPlot(QWidget* parent=0);
			~SinTracksPlot();

			void set_data(const CLAM::Segment& segment, bool update=false);

		private:
			CLAM::Array<CLAM::SpectralPeakArray> wp_peak_matrix;

			void init_sintracks_plot();
		   
			void set_xrange(double xmin, double xmax);
			void set_yrange(double ymin, double ymax);
			void set_zoom_steps(int hsteps, int vsteps);

			void build_peak_matrix(const CLAM::Segment& in);
			std::pair<int,int> get_zoom_steps(CLAM::TSize size, CLAM::TData yrange);
			
		};
	}
}

#endif
