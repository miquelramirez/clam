#ifndef __VMQT_PLAYABLE_FUNDAMENTAL_PLOT_H__
#define __VMQT_PLAYABLE_FUNDAMENTAL_PLOT_H__

#include "vm_fundamental_plot.hxx"

class QComboBox;

namespace CLAM
{
	namespace VM
	{
		class CLAM::VM::WPlayer;
		class CLAM::VM::FundFreqPlayer;

		class PlayableFundamentalPlot : public CLAM::VM::FundamentalPlot
		{
			Q_OBJECT
		public:
			PlayableFundamentalPlot(QWidget* parent=0);
			~PlayableFundamentalPlot();

			void set_data(const CLAM::Segment& segment, bool update=false);

		public slots:
			void backgroundWhite();
			void backgroundBlack();

		private slots:
			void setColorSchema(int);
			void setPlayingFlags(int);

		private:
			QComboBox* wp_schema_combo_box;
			QComboBox* wp_play_mode_combo_box;

			CLAM::VM::WPlayer*        wp_wplayer;
			CLAM::VM::FundFreqPlayer* wp_fund_player;

			void init_playable_fundamental_plot();
		};
	}
}

#endif
