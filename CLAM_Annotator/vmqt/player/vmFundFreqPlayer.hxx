#ifndef __VMQT_FUND_FREQ_PLAYER_H__
#define __VMQT_FUND_FREQ_PLAYER_H__

#include "vmPlayer.hxx"

namespace CLAM
{
	class Segment;

	namespace VM
	{
		class FundFreqPlayer : public Player
		{
			Q_OBJECT
		public:
			FundFreqPlayer(QObject* parent=0);
			~FundFreqPlayer();

			void SetData(const Segment& segment);

		private:
			const Segment* mSegment;

			void run();
		};
	}
}

#endif

