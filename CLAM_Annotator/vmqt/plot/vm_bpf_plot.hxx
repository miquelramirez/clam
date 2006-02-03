#ifndef __VMQT_BPF_PLOT_H__
#define __VMQT_BPF_PLOT_H__

#include "vm_multibpf_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		class BPFPlot : public MultiBPFPlot
		{
			Q_OBJECT
		public:
			BPFPlot(QWidget* parent=0);
			~BPFPlot();

			void set_data(BPF* bpf);
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
			void add_bpf(const QString& key, BPF* bpf);
			void set_colors(const QString& key, const Color& cline, const Color& chandler);
			void set_flags(const QString& key, int flags);

			void init_bpf_plot();
		};
	}
}

#endif

