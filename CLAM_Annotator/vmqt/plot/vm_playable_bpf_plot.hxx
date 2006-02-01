#ifndef __VMQT_PLAYABLE_BPF_PLOT_H__
#define __VMQT_PLAYABLE_BPF_PLOT_H__

#include "vm_playable_multibpf_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		class PlayableBPFPlot : public CLAM::VM::PlayableMultiBPFPlot
		{
			Q_OBJECT
		public:
			PlayableBPFPlot(QWidget* parent=0);
			~PlayableBPFPlot();

			void set_data(CLAM::BPF* bpf);
			void set_flags(int f);

		signals:
			void xValueChanged(unsigned, double);
			void yValueChanged(unsigned, double);
			void elementAdded(unsigned, double, double);
			void elementRemoved(unsigned);

		public slots:
			void backgroundWhite();
			void backgroundBlack();

		private slots:
			void xvalue_changed(QString, unsigned, double);
			void yvalue_changed(QString, unsigned, double);
			void element_added(QString, unsigned, double, double);
			void element_removed(QString, unsigned);
			void setCurrentBPF(QString);

		private:
			void add_bpf(const QString& key, CLAM::BPF* bpf);
			void set_colors(const QString& key, const CLAM::VM::Color& cline, const CLAM::VM::Color& chandler);
			void set_flags(const QString& key, int flags);

			void init_playable_bpf_plot();
		};
	}
}

#endif
