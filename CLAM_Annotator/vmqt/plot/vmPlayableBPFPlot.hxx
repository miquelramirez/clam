#ifndef __VMQT_PLAYABLE_BPF_PLOT_H__
#define __VMQT_PLAYABLE_BPF_PLOT_H__

#include "vmPlayableMultiBPFPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class PlayableBPFPlot : public PlayableMultiBPFPlot
		{
			Q_OBJECT
		public:
			PlayableBPFPlot(QWidget* parent=0);
			~PlayableBPFPlot();

			void SetData(BPF* bpf);
			void SetFlags(int f);

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
			void AddBPF(const QString& key, BPF* bpf);
			void SetColors(const QString& key, const Color& cline, const Color& chandler);
			void SetFlags(const QString& key, int flags);

			void InitPlayableBPFPlot();
		};
	}
}

#endif
