#ifndef __SCROLLBAR__
#define __SCROLLBAR__

#include <qscrollbar.h>

namespace CLAM
{
	namespace VM
	{
		class ScrollBar : public QScrollBar
		{
			Q_OBJECT
			
			signals:
				void clicked();
				void released();
			
			public: 
				ScrollBar(Orientation orientation, QWidget * parent);
				virtual ~ScrollBar();
			
			protected:
				virtual void mousePressEvent(QMouseEvent * e);
				virtual void mouseReleaseEvent(QMouseEvent * e);
		};
	}
}

#endif

