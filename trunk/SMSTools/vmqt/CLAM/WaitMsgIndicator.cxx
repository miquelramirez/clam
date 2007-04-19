#include <qtimer.h>
#include <qpainter.h>
#include <CLAM/Message.hxx>
#include <CLAM/WaitMsgIndicator.hxx>

namespace CLAM
{
	namespace VM
	{
		WaitMsgIndicator::WaitMsgIndicator(QWidget* parent)
			: QFrame(parent)
			, mLeftXCoord(0)
			, mState(MOV_TO_RIGHT)
		{
			setFrameStyle(QFrame::Panel | QFrame::Sunken);

			QTimer* timer = new QTimer(this);
			timer->start(REFRESH_INTERVAL);
			connect(timer,SIGNAL(timeout()),SLOT(refresh()));
		}

		WaitMsgIndicator::~WaitMsgIndicator()
		{
		}

		void WaitMsgIndicator::paintEvent(QPaintEvent* pe)
		{
			QFrame::paintEvent(pe);
			QColor brush(0,0,0);
			QPainter painter(this);
			painter.setBrush(brush);
			painter.drawRect(mLeftXCoord,0,RECTWIDTH,height());
		}

		void WaitMsgIndicator::refresh()
		{
			if(mLeftXCoord < 0)
			{
				mState = MOV_TO_RIGHT;
			}
			else if(mLeftXCoord+RECTWIDTH > width())
			{
				mState = MOV_TO_LEFT;
			}

			switch(mState)
			{
				case MOV_TO_RIGHT:
					mLeftXCoord += STEP;
					break;
				case MOV_TO_LEFT:
					mLeftXCoord -= STEP;
					break;
				default:
					break;
			}
			update();
		}
	}
}

// END

