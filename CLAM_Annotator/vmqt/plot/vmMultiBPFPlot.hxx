#ifndef __VMQT_MULTIBPF_PLOT_H__
#define __VMQT_MULTIBPF_PLOT_H__

#include <map>
#include "BPF.hxx"
#include "vmSegmentationPlot.hxx"

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

			virtual void AddBPF(const QString& key, BPF* bpf);
			virtual void SetColors(const QString& key, const Color& cline, const Color& chandler);
			virtual void SetFlags(const QString& key, int flags);
			
			void SetGridSteps(double xstep, double ystep);
		   
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
			bool ExistKey(const QString& key);

		private:
			bool    mIsReadOnly;
			QString mCurrentBPF;

			std::map<QString,int> mBPFKeys;

			void InitMultiBPFPlot();
		};
	}
}

#endif
