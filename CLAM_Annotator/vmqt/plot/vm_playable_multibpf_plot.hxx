#ifndef __VMQT_PLAYABLE_MULTIBPF_PLOT_H__
#define __VMQT_PLAYABLE_MULTIBPF_PLOT_H__

#include "vm_multibpf_plot.hxx"

class QCheckBox;
class QComboBox;

namespace CLAM
{
	namespace VM
	{
		class CLAM::VM::WPlayer;
		class CLAM::VM::BPFPlayer;

		class PlayableMultiBPFPlot : public CLAM::VM::MultiBPFPlot
		{
			Q_OBJECT
		public:
			PlayableMultiBPFPlot(QWidget* parent=0);
			virtual ~PlayableMultiBPFPlot();

			void add_bpf(const QString& key, CLAM::BPF* bpf);

			void set_xrange(double xmin, double xmax, CLAM::VM::EScale scale=CLAM::VM::eLinearScale);
			void set_yrange(double ymin, double ymax, CLAM::VM::EScale scale=CLAM::VM::eLinearScale);

		public slots:
			void backgroundWhite();
			void backgroundBlack();
			void readOnly();

		private slots:
		    void setCurrentBPF(QString);

			void set_color_schema(int);
			void show_grid(int);
			void snap_to_grid(int);

		private:
			QCheckBox* wp_show_grid;
			QCheckBox* wp_snap_grid;
			QComboBox* wp_schema_combo_box;
			QComboBox* wp_current_bpf_combo_box;

			CLAM::VM::WPlayer*     wp_wplayer;
			CLAM::VM::BPFPlayer* wp_bpf_player;

			void init_playable_multibpf_plot();
		};
	}
}

#endif
