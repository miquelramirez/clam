#ifndef __VMQT_BPF_PLOT_H__
#define __VMQT_BPF_PLOT_H__

#include "BPF.hxx"
#include "vm_segmentation_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		class BPFPlot : public CLAM::VM::SegmentationPlot
		{
			Q_OBJECT
		public:
			BPFPlot(QWidget* parent=0);
			virtual ~BPFPlot();

			virtual void set_data(CLAM::BPF* bpf);

			void set_flags(int f);
			void set_grid_steps(double xstep, double ystep);
		   
		public slots:
			virtual void backgroundWhite();
			virtual void backgroundBlack();
			
			void show_grid(bool);
			void snap_to_grid(bool);

		private:
			void init_bpf_plot();
		};
	}
}

#endif

