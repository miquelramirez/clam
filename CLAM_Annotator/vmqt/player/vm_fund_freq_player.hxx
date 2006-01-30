#ifndef __VMQT_FUND_FREQ_PLAYER_H__
#define __VMQT_FUND_FREQ_PLAYER_H__

#include "vm_player.hxx"

namespace CLAM
{
	class CLAM::Segment;

	namespace VM
	{
		class FundFreqPlayer : public CLAM::VM::Player
		{
			Q_OBJECT
		public:
			FundFreqPlayer(QObject* parent=0);
			~FundFreqPlayer();

			void set_data(const CLAM::Segment& segment);

		private:
			const CLAM::Segment* pl_segment;

			void run();
		};
	}
}

#endif

