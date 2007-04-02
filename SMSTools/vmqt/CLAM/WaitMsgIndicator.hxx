#ifndef __WAITMSGINDICATOR__
#define __WAITMSGINDICATOR__

#include <qframe.h>

namespace CLAM
{
	namespace VM
	{
		class WaitMsgIndicator : public QFrame
		{
			Q_OBJECT
		public:
			WaitMsgIndicator(QWidget* parent);
			~WaitMsgIndicator();

		public slots:
			void refresh();

		protected:
			void paintEvent(QPaintEvent* pe);

		private:
			int     mLeftXCoord;
			int     mState;

			enum { MOV_TO_RIGHT=0, MOV_TO_LEFT=1, STEP=3, RECTWIDTH=60, REFRESH_INTERVAL=20 };
		};
	}
}

#endif

