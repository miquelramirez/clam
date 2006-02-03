#ifndef __VMQT_PLAYABLE_FUNDAMENTAL_PLOT_H__
#define __VMQT_PLAYABLE_FUNDAMENTAL_PLOT_H__

#include "vm_fundamental_plot.hxx"

class QComboBox;

namespace CLAM
{
	namespace VM
	{
		class WPlayer;
		class FundFreqPlayer;

		class PlayableFundamentalPlot : public FundamentalPlot
		{
			Q_OBJECT
		public:
			PlayableFundamentalPlot(QWidget* parent=0);
			~PlayableFundamentalPlot();

			void set_data(const Segment& segment, bool update=false);

		public slots:
			void backgroundWhite();
			void backgroundBlack();

		private slots:
			void setColorSchema(int);
			void setPlayingFlags(int);

		private:
			QComboBox* wp_schema_combo_box;
			QComboBox* wp_play_mode_combo_box;

			WPlayer*        wp_wplayer;
			FundFreqPlayer* wp_fund_player;

			void init_playable_fundamental_plot();
		};
	}
}

#endif
