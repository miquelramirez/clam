#ifndef __VMQT_FUNDAMENTAL_PLOT_H__
#define __VMQT_FUNDAMENTAL_PLOT_H__

#include "Segment.hxx"
#include "vmSegmentationPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class FundamentalPlot : public SegmentationPlot
		{
		public:
			FundamentalPlot(QWidget* parent=0);
			virtual ~FundamentalPlot();

			virtual void SetData(const Segment& segment, bool update=false);

		public slots:
			virtual void backgroundWhite();
			virtual void backgroundBlack();

		private:
			void InitFundamentalPlot();
			std::pair<int,int> GetZoomSteps(TSize size, TData yrange);

			void SetXRange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			void SetYRange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);
			void SetZoomSteps(int hsteps, int vsteps);

			DataArray GetFundData(const Segment& segment);
		};
	}
}

#endif

