#ifndef __VMQT_PLAYABLE_MULTIBPF_PLOT_H__
#define __VMQT_PLAYABLE_MULTIBPF_PLOT_H__

#include "vmMultiBPFPlot.hxx"

class QCheckBox;
class QComboBox;

namespace CLAM
{
	namespace VM
	{
		class WPlayer;
		class BPFPlayer;

		class PlayableMultiBPFPlot : public MultiBPFPlot
		{
			Q_OBJECT
		public:
			PlayableMultiBPFPlot(QWidget* parent=0);
			virtual ~PlayableMultiBPFPlot();

			void AddBPF(const QString& key, BPF* bpf);

			void SetXRange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			void SetYRange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);

		public slots:
			void backgroundWhite();
			void backgroundBlack();
			void readOnly();

		protected slots:
			virtual void setCurrentBPF(QString);

		private slots:		    
			void set_color_schema(int);
			void show_grid(int);
			void snap_to_grid(int);

		private:
			QCheckBox* mShowGrid;
			QCheckBox* mSnapGrid;
			QComboBox* mSchemaComboBox;
			QComboBox* mCurrentBPFComboBox;

			WPlayer*   mWPlayer;
			BPFPlayer* mBPFPlayer;

			void InitPlayableMultiBPFPlot();
		};
	}
}

#endif
