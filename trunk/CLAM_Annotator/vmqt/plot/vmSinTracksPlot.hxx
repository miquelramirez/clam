#ifndef __VMQT_SINTRACKS_PLOT_H__
#define __VMQT_SINTRACKS_PLOT_H__

#include "Segment.hxx"
#include "vmSegmentationPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class SinTracksPlot : public SegmentationPlot
		{
		public:
			SinTracksPlot(QWidget* parent=0);
			~SinTracksPlot();

			void SetData(const Segment& segment, bool update=false);

		private: 
			void InitSinTracksPlot();
		   
			void SetXRange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			void SetYRange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);
			void SetZoomSteps(int hsteps, int vsteps);

			std::pair<int,int> GetZoomSteps(TSize size, TData yrange);
			Array<SpectralPeakArray> GetPeakMatrix(const Segment& in);
		};
	}
}

#endif
