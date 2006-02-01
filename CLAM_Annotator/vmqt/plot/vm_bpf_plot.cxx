#include "vm_plot2d.hxx"
#include "vm_bpf_editor.hxx"
#include "vm_bpf_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		BPFPlot::BPFPlot(QWidget* parent)
			: MultiBPFPlot(parent)
		{
			init_bpf_plot();
		}

		BPFPlot::~BPFPlot()
		{
		}

		void BPFPlot::set_data(CLAM::BPF* bpf)
		{
			add_bpf("bpf_editor",bpf);
		}

		void BPFPlot::set_flags(int f)
		{
			set_flags("bpf_editor",f);
		}

		void BPFPlot::backgroundWhite()
		{
			MultiBPFPlot::backgroundWhite();
			set_colors("bpf_editor",CLAM::VM::Color(0,0,0),CLAM::VM::Color(0,0,255));
		}

		void BPFPlot::backgroundBlack()
		{
			MultiBPFPlot::backgroundBlack();
			set_colors("bpf_editor",CLAM::VM::Color(255,255,255),CLAM::VM::Color(255,0,0));
		}

		void BPFPlot::xvalue_changed(QString key, unsigned index, double value)
		{
			emit xValueChanged(index,value);
		}

		void BPFPlot::yvalue_changed(QString key, unsigned index, double value)
		{
			emit yValueChanged(index,value);
		}

		void BPFPlot::element_added(QString key, unsigned index, double xvalue, double yvalue)
		{
			emit elementAdded(index,xvalue,yvalue);
		}

		void BPFPlot::element_removed(QString key, unsigned index)
		{
			emit elementRemoved(index);
		}

		void BPFPlot::setCurrentBPF(QString key)
		{
			MultiBPFPlot::setCurrentBPF(key);
		}

		void BPFPlot::add_bpf(const QString& key, CLAM::BPF* bpf)
		{
			MultiBPFPlot::add_bpf(key,bpf);
		}

		void BPFPlot::set_colors(const QString& key, const CLAM::VM::Color& cline, const CLAM::VM::Color& chandler)
		{
			MultiBPFPlot::set_colors(key,cline,chandler);
		}

		void BPFPlot::set_flags(const QString& key, int flags)
		{
			MultiBPFPlot::set_flags(key,flags);
		}

		void BPFPlot::init_bpf_plot()
		{
			add_bpf("bpf_editor",0);
			setCurrentBPF("bpf_editor");
			connect(static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer("bpf_editor")),
					SIGNAL(xValueChanged(QString,unsigned,double)),
						this,SLOT(xvalue_changed(QString,unsigned,double)));
			connect(static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer("bpf_editor")),
					SIGNAL(yValueChanged(QString,unsigned,double)),
					this,SLOT(yvalue_changed(QString,unsigned,double)));
			connect(static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer("bpf_editor")),
					SIGNAL(elementAdded(QString,unsigned,double,double)),
					this,SLOT(element_added(QString,unsigned,double,double)));
			connect(static_cast<CLAM::VM::BPFEditor*>(wp_plot->get_renderer("bpf_editor")),
					SIGNAL(elementRemoved(QString,unsigned)),this,SLOT(element_removed(QString,unsigned)));
		}
	}
}

// END
