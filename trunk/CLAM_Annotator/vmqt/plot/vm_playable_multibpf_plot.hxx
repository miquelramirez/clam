#ifndef __VMQT_PLAYABLE_MULTIBPF_PLOT_H__
#define __VMQT_PLAYABLE_MULTIBPF_PLOT_H__

#include "vm_multibpf_plot.hxx"

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

			void add_bpf(const QString& key, BPF* bpf);

			void set_xrange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			void set_yrange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);

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
			QCheckBox* wp_show_grid;
			QCheckBox* wp_snap_grid;
			QComboBox* wp_schema_combo_box;
			QComboBox* wp_current_bpf_combo_box;

			WPlayer*   wp_wplayer;
			BPFPlayer* wp_bpf_player;

			void init_playable_multibpf_plot();
		};
	}
}

#endif
