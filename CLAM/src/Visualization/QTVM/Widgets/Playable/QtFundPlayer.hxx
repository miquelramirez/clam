#ifndef __QTFUNDPLAYER__
#define __QTFUNDPLAYER__

#include "Segment.hxx"
#include "QtPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		class QtFundPlayer : public QtPlayer
		{
			public:
				QtFundPlayer(QWidget* parent=0);
				virtual ~QtFundPlayer();

				void SetData(const Segment& segment);
		};
	}
}

#endif
