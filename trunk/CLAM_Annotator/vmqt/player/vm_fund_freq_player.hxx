#ifndef __VMQT_FUND_FREQ_PLAYER_H__
#define __VMQT_FUND_FREQ_PLAYER_H__

#include "vm_player.hxx"

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

			void set_data(const Segment& segment);

		private:
			const Segment* pl_segment;

			void run();
		};
	}
}

#endif

