#ifndef __VMQT_PLAYABLE_FUNDAMENTAL_PLOT_H__
#define __VMQT_PLAYABLE_FUNDAMENTAL_PLOT_H__

#include "vmFundamentalPlot.hxx"

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

			void SetData(const Segment& segment, bool update=false);

		public slots:
			void backgroundWhite();
			void backgroundBlack();

		private slots:
			void setColorSchema(int);
			void setPlayingFlags(int);

		private:
			QComboBox* mSchemaComboBox;
			QComboBox* mPlayModeComboBox;

			WPlayer*        mWPlayer;
			FundFreqPlayer* mFundPlayer;

			void InitPlayableFundamentalPlot();
		};
	}
}

#endif
