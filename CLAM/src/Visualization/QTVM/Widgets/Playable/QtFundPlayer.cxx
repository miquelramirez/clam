#include "FundPlayer.hxx"
#include "QtFundPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		QtFundPlayer::QtFundPlayer(QWidget* parent) : QtPlayer(parent)
		{
			SetPlayer(new FundPlayer());
		}
		
		QtFundPlayer::~QtFundPlayer()
		{
		}

		void QtFundPlayer::SetData(const Segment& segment)
		{
			((FundPlayer*)_player)->SetData(segment);
		}
	}
}

// END
