#ifndef __VMQT_FUNDAMENTAL_PLOT_H__
#define __VMQT_FUNDAMENTAL_PLOT_H__

#include "Segment.hxx"
#include "vm_segmentation_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		class FundamentalPlot : public CLAM::VM::SegmentationPlot
		{
		public:
			FundamentalPlot(QWidget* parent=0);
			virtual ~FundamentalPlot();

			virtual void set_data(const CLAM::Segment& segment, bool update=false);

		public slots:
			virtual void backgroundWhite();
			virtual void backgroundBlack();

		private:
			void init_fundamental_plot();
			std::pair<int,int> get_zoom_steps(CLAM::TSize size, CLAM::TData yrange);

			void set_xrange(double xmin, double xmax);
			void set_yrange(double ymin, double ymax);
			void set_zoom_steps(int hsteps, int vsteps);

			CLAM::DataArray get_fund_data(const CLAM::Segment& segment);
		};
	}
}

#endif

