#include "vm_plot2d.hxx"
#include "vm_grid.hxx"
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

		void BPFPlot::set_grid_steps(double xstep, double ystep)
		{
			static_cast<CLAM::VM::Grid*>(wp_plot->get_renderer("grid"))->set_grid_steps(xstep,ystep);
		}

		void BPFPlot::backgroundWhite()
		{
			CLAM::VM::Color brown(152,102,32);
			CLAM::VM::Color black(0,0,0);
			CLAM::VM::Color blue(0,0,255);
			SegmentationPlot::backgroundWhite();
			static_cast<CLAM::VM::Grid*>(wp_plot->get_renderer("grid"))->set_grid_color(brown);
			static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer("bpf_editor"))->set_colors(black,blue);
		}

		void BPFPlot::backgroundBlack()
		{
			CLAM::VM::Color yellow(255,255,0);
			CLAM::VM::Color white(255,255,255);
			CLAM::VM::Color red(255,0,0);
			SegmentationPlot::backgroundBlack();
			static_cast<CLAM::VM::Grid*>(wp_plot->get_renderer("grid"))->set_grid_color(yellow);
			static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer("bpf_editor"))->set_colors(white,red);
		}

		void BPFPlot::show_grid(bool show)
		{
			static_cast<CLAM::VM::Grid*>(wp_plot->get_renderer("grid"))->show_grid(show);
		}

		void BPFPlot::snap_to_grid(bool snap)
		{
				static_cast<CLAM::VM::Grid*>(wp_plot->get_renderer("grid"))->snap_to_grid(snap);
		}

		void BPFPlot::init_bpf_plot()
		{
			wp_plot->add_renderer("grid", new CLAM::VM::Grid());
			wp_plot->add_renderer("bpf_editor", new CLAM::VM::BPFEditor());
			wp_plot->send_to_back("bpf_editor");
			wp_plot->send_to_back("grid");
			wp_plot->bring_to_front("locator");
			wp_plot->get_renderer("bpf_editor")->set_grid(static_cast<CLAM::VM::Grid*>(wp_plot->get_renderer("grid")));
			backgroundWhite();
		}
	}
}

// END
