#ifndef __VMQT_SINTRACKS_PLOT_H__
#define __VMQT_SINTRACKS_PLOT_H__

#include "Segment.hxx"
#include "vm_segmentation_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		class SinTracksPlot : public SegmentationPlot
		{
		public:
			SinTracksPlot(QWidget* parent=0);
			~SinTracksPlot();

			void set_data(const Segment& segment, bool update=false);

		private: 
			void init_sintracks_plot();
		   
			void set_xrange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			void set_yrange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);
			void set_zoom_steps(int hsteps, int vsteps);

			std::pair<int,int> get_zoom_steps(TSize size, TData yrange);
			Array<SpectralPeakArray> get_peak_matrix(const Segment& in);
		};
	}
}

#endif
