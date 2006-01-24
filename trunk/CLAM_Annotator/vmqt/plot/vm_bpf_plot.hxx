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
		   
		public slots:
			virtual void backgroundWhite();
			virtual void backgroundBlack();

		private:
			void init_bpf_plot();
		};
	}
}

#endif

