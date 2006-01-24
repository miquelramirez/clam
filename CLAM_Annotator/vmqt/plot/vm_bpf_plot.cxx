#include "vm_plot2d.hxx"
#include "vm_bpf_editor.hxx"
#include "vm_bpf_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		BPFPlot::BPFPlot(QWidget* parent)
			: SegmentationPlot(parent)
		{
			init_bpf_plot();
		}

		BPFPlot::~BPFPlot()
		{
		}

		void BPFPlot::set_data(CLAM::BPF* bpf)
		{
			static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer("bpf_editor"))->set_data(bpf);
		}

		void BPFPlot::set_flags(int f)
		{
			static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer("bpf_editor"))->set_flags(f);
		}

		void BPFPlot::backgroundWhite()
		{
			CLAM::VM::Color black(0,0,0);
			CLAM::VM::Color blue(0,0,255);
			SegmentationPlot::backgroundWhite();
			static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer("bpf_editor"))->set_colors(black,blue);
		}

		void BPFPlot::backgroundBlack()
		{
			CLAM::VM::Color white(255,255,255);
			CLAM::VM::Color red(255,0,0);
			SegmentationPlot::backgroundBlack();
			static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer("bpf_editor"))->set_colors(white,red);
		}

		void BPFPlot::init_bpf_plot()
		{
			wp_plot->add_renderer("bpf_editor", new CLAM::VM::BPFEditor());
			wp_plot->send_to_back("bpf_editor");
			wp_plot->bring_to_front("locator");
			backgroundWhite();
		}
	}
}

// END
