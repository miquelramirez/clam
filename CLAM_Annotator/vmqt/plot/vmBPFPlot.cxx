#include "vmPlot2D.hxx"
#include "vmBPFEditor.hxx"
#include "vmBPFPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		BPFPlot::BPFPlot(QWidget* parent)
			: MultiBPFPlot(parent)
		{
			InitBPFPlot();
		}

		BPFPlot::~BPFPlot()
		{
		}

		void BPFPlot::SetData(BPF* bpf)
		{
			AddBPF("bpf_editor",bpf);
		}

		void BPFPlot::SetFlags(int f)
		{
			SetFlags("bpf_editor",f);
		}

		void BPFPlot::backgroundWhite()
		{
			MultiBPFPlot::backgroundWhite();
			SetColors("bpf_editor",Color(0,0,0),Color(0,0,255));
		}

		void BPFPlot::backgroundBlack()
		{
			MultiBPFPlot::backgroundBlack();
			SetColors("bpf_editor",Color(255,255,255),Color(255,0,0));
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

		void BPFPlot::AddBPF(const QString& key, BPF* bpf)
		{
			MultiBPFPlot::AddBPF(key,bpf);
		}

		void BPFPlot::SetColors(const QString& key, const Color& cline, const Color& chandler)
		{
			MultiBPFPlot::SetColors(key,cline,chandler);
		}

		void BPFPlot::SetFlags(const QString& key, int flags)
		{
			MultiBPFPlot::SetFlags(key,flags);
		}

		void BPFPlot::InitBPFPlot()
		{
			AddBPF("bpf_editor",0);
			setCurrentBPF("bpf_editor");
			connect(static_cast<BPFEditor*>(mPlot->GetRenderer("bpf_editor")),
					SIGNAL(xValueChanged(QString,unsigned,double)),
					this,SLOT(xvalue_changed(QString,unsigned,double)));
			connect(static_cast<BPFEditor*>(mPlot->GetRenderer("bpf_editor")),
					SIGNAL(yValueChanged(QString,unsigned,double)),
					this,SLOT(yvalue_changed(QString,unsigned,double)));
			connect(static_cast<BPFEditor*>(mPlot->GetRenderer("bpf_editor")),
					SIGNAL(elementAdded(QString,unsigned,double,double)),
					this,SLOT(element_added(QString,unsigned,double,double)));
			connect(static_cast<BPFEditor*>(mPlot->GetRenderer("bpf_editor")),
					SIGNAL(elementRemoved(QString,unsigned)),this,SLOT(element_removed(QString,unsigned)));
		}
	}
}

// END
