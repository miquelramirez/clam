#ifndef __VMQT_PLAYABLE_BPF_PLOT_H__
#define __VMQT_PLAYABLE_BPF_PLOT_H__

#include "vm_bpf_plot.hxx"

class QComboBox;
class QCheckBox;

namespace CLAM
{
	namespace VM
	{
		class CLAM::VM::WPlayer;
		class CLAM::VM::BPFPlayer;

		class PlayableBPFPlot : public CLAM::VM::BPFPlot
		{
			Q_OBJECT
		public:
			PlayableBPFPlot(QWidget* parent=0);
			~PlayableBPFPlot();

			void set_data(CLAM::BPF* bpf);

			void set_xrange(double xmin, double xmax, CLAM::VM::EScale scale=CLAM::VM::eLinearScale);
			void set_yrange(double ymin, double ymax, CLAM::VM::EScale scale=CLAM::VM::eLinearScale);

		public slots:
			void backgroundWhite();
			void backgroundBlack();
			void readOnly();

		private slots:
			void set_color_schema(int);
			void show_grid(int);
			void snap_to_grid(int);

		private:
			QComboBox* wp_combo_box;
			QCheckBox* wp_show_grid;
			QCheckBox* wp_snap_grid;

			CLAM::VM::WPlayer*     wp_wplayer;
			CLAM::VM::BPFPlayer* wp_bpf_player;

			void init_playable_bpf_plot();
		};
	}
}

#endif
