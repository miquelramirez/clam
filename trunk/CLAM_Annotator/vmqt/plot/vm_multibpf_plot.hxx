#ifndef __VMQT_MULTIBPF_PLOT_H__
#define __VMQT_MULTIBPF_PLOT_H__

#include <map>
#include "BPF.hxx"
#include "vm_segmentation_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		class MultiBPFPlot : public SegmentationPlot
		{
			Q_OBJECT
		public:
			MultiBPFPlot(QWidget* parent=0);
			virtual ~MultiBPFPlot();

			virtual void add_bpf(const QString& key, BPF* bpf);
			virtual void set_colors(const QString& key, const Color& cline, const Color& chandler);
			virtual void set_flags(const QString& key, int flags);
			
			void set_grid_steps(double xstep, double ystep);
		   
		signals:
			void xValueChanged(QString, unsigned, double);
			void yValueChanged(QString, unsigned, double);
			void elementAdded(QString, unsigned, double, double);
			void elementRemoved(QString, unsigned);
			void currentBPF(QString);

		public slots:
		    virtual void backgroundWhite();
			virtual void backgroundBlack();
			virtual void readOnly();
			virtual void setCurrentBPF(QString);

			void showGrid(bool);
			void snapToGrid(bool);
			
		protected:
			bool exist_key(const QString& key);

		private:
			bool    wp_is_read_only;
			QString wp_current_bpf;

			std::map<QString,int> wp_bpf_keys;

			void init_multibpf_plot();
		};
	}
}

#endif
