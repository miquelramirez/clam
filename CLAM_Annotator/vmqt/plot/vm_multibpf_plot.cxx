#include "vm_plot2d.hxx"
#include "vm_grid.hxx"
#include "vm_bpf_editor.hxx"
#include "vm_multibpf_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		MultiBPFPlot::MultiBPFPlot(QWidget* parent)
			: SegmentationPlot(parent)
			, wp_is_read_only(false)
			, wp_current_bpf("")
		{
			init_multibpf_plot();
		}

		MultiBPFPlot::~MultiBPFPlot()
		{
		}

		void MultiBPFPlot::add_bpf(const QString& key, CLAM::BPF* bpf)
		{
			if(exist_key(key))
			{
				static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer(key))->set_data(bpf);
			}
			else
			{
				wp_plot->add_renderer(key, new CLAM::VM::BPFEditor());
				wp_plot->send_to_back("grid");
				wp_plot->bring_to_front("segmentation");
				wp_plot->bring_to_front("locator");
				wp_plot->get_renderer(key)->set_grid(static_cast<CLAM::VM::Grid*>(wp_plot->get_renderer("grid")));
				static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer(key))->set_data(bpf);
				wp_bpf_keys[key]=static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer(key))->get_flags();
				setCurrentBPF(key);
				connect(static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer(key)),
						SIGNAL(xValueChanged(QString,unsigned,double)),
						this,SIGNAL(xValueChanged(QString,unsigned,double)));
				connect(static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer(key)),
						SIGNAL(yValueChanged(QString,unsigned,double)),
						this,SIGNAL(yValueChanged(QString,unsigned,double)));
				connect(static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer(key)),
						SIGNAL(elementAdded(QString,unsigned,double,double)),
						this,SIGNAL(elementAdded(QString,unsigned,double,double)));
				connect(static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer(key)),
						SIGNAL(elementRemoved(QString,unsigned)),this,SIGNAL(elementRemoved(QString,unsigned)));
			}
		}

		void MultiBPFPlot::set_colors(const QString& key, const CLAM::VM::Color& cline, const CLAM::VM::Color& chandler)
		{
			if(!exist_key(key)) return;
			static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer(key))->set_colors(cline,chandler);
		}

		void MultiBPFPlot::set_flags(const QString& key, int flags)
		{
			if(wp_is_read_only) return;
			if(!exist_key(key)) return;
			static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer(key))->set_flags(flags);
			wp_bpf_keys[key]=static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer(key))->get_flags();
		}
			
		void MultiBPFPlot::set_grid_steps(double xstep, double ystep)
		{
			static_cast<CLAM::VM::Grid*>(wp_plot->get_renderer("grid"))->set_grid_steps(xstep,ystep);
		}

		void MultiBPFPlot::backgroundWhite()
		{
			SegmentationPlot::backgroundWhite();
			static_cast<CLAM::VM::Grid*>(wp_plot->get_renderer("grid"))->set_grid_color(CLAM::VM::Color(152,102,32));
		}

		void MultiBPFPlot::backgroundBlack()
		{
			SegmentationPlot::backgroundBlack();
			static_cast<CLAM::VM::Grid*>(wp_plot->get_renderer("grid"))->set_grid_color(CLAM::VM::Color(255,255,0));
		}

		void MultiBPFPlot::showGrid(bool show)
		{
			static_cast<CLAM::VM::Grid*>(wp_plot->get_renderer("grid"))->show_grid(show);
		}

		void MultiBPFPlot::snapToGrid(bool snap)
		{
			static_cast<CLAM::VM::Grid*>(wp_plot->get_renderer("grid"))->snap_to_grid(snap);
		}

		void MultiBPFPlot::readOnly()
		{
			std::map<QString,int>::iterator it = wp_bpf_keys.begin();
			for(; it != wp_bpf_keys.end(); it++)
			{
				static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer(it->first))->set_flags(0);
			}
			wp_is_read_only = true;
		}

		void MultiBPFPlot::setCurrentBPF(QString key)
		{
			if(key == wp_current_bpf) return;
			if(!exist_key(key)) return;
			std::map<QString,int>::iterator it = wp_bpf_keys.begin();
			for(; it != wp_bpf_keys.end(); it++)
			{
				static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer(it->first))->set_flags(0);
			}
			static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer(key))->set_flags(wp_bpf_keys[key]);
			wp_current_bpf = key;
			emit currentBPF(wp_current_bpf);
		}

		void MultiBPFPlot::init_multibpf_plot()
		{
			wp_plot->add_renderer("grid", new CLAM::VM::Grid());
			wp_plot->send_to_back("grid");
			backgroundWhite();
		}

		bool MultiBPFPlot::exist_key(const QString& key)
		{
			return (wp_bpf_keys.find(key) != wp_bpf_keys.end());
		}

	}
}

// END
