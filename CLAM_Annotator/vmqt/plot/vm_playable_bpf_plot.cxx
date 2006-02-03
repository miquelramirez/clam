#include "vm_plot2d.hxx"
#include "vm_bpf_editor.hxx"
#include "vm_playable_bpf_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayableBPFPlot::PlayableBPFPlot(QWidget* parent)
			: PlayableMultiBPFPlot(parent)
		{
			init_playable_bpf_plot();
		}

		PlayableBPFPlot::~PlayableBPFPlot()
		{
		}

		void PlayableBPFPlot::set_data(BPF* bpf)
		{
			add_bpf("bpf_editor",bpf);
		}

		void PlayableBPFPlot::set_flags(int f)
		{
			set_flags("bpf_editor",f);
		}

		void PlayableBPFPlot::backgroundWhite()
		{
			PlayableMultiBPFPlot::backgroundWhite();
			set_colors("bpf_editor",Color(0,0,0),Color(0,0,255));
		}

		void PlayableBPFPlot::backgroundBlack()
		{
			PlayableMultiBPFPlot::backgroundBlack();
			set_colors("bpf_editor",Color(255,255,255),Color(255,0,0));
		}

		void PlayableBPFPlot::xvalue_changed(QString key, unsigned index, double value)
		{
			emit xValueChanged(index,value);
		}

		void PlayableBPFPlot::yvalue_changed(QString key, unsigned index, double value)
		{
			emit yValueChanged(index,value);
		}

		void PlayableBPFPlot::element_added(QString key, unsigned index, double xvalue, double yvalue)
		{
			emit elementAdded(index,xvalue,yvalue);
		}

		void PlayableBPFPlot::element_removed(QString key, unsigned index)
		{
			emit elementRemoved(index);
		}

		void PlayableBPFPlot::setCurrentBPF(QString key)
		{
			PlayableMultiBPFPlot::setCurrentBPF(key);
		}

		void PlayableBPFPlot::add_bpf(const QString& key, BPF* bpf)
		{
			PlayableMultiBPFPlot::add_bpf(key,bpf);
		}

		void PlayableBPFPlot::set_colors(const QString& key, const Color& cline, const Color& chandler)
		{
			PlayableMultiBPFPlot::set_colors(key,cline,chandler);
		}

		void PlayableBPFPlot::set_flags(const QString& key, int flags)
		{
			PlayableMultiBPFPlot::set_flags(key,flags);
		}

		void PlayableBPFPlot::init_playable_bpf_plot()
		{
			add_bpf("bpf_editor",0);
			setCurrentBPF("bpf_editor");
			connect(static_cast<BPFEditor*>(wp_plot->get_renderer("bpf_editor")),
					SIGNAL(xValueChanged(QString,unsigned,double)),
					this,SLOT(xvalue_changed(QString,unsigned,double)));
			connect(static_cast<BPFEditor*>(wp_plot->get_renderer("bpf_editor")),
					SIGNAL(yValueChanged(QString,unsigned,double)),
					this,SLOT(yvalue_changed(QString,unsigned,double)));
			connect(static_cast<BPFEditor*>(wp_plot->get_renderer("bpf_editor")),
					SIGNAL(elementAdded(QString,unsigned,double,double)),
					this,SLOT(element_added(QString,unsigned,double,double)));
			connect(static_cast<BPFEditor*>(wp_plot->get_renderer("bpf_editor")),
					SIGNAL(elementRemoved(QString,unsigned)),this,SLOT(element_removed(QString,unsigned)));
		}
	}
}

// END
