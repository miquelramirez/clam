#include "ScrollBar.hxx"

namespace CLAM
{
	namespace VM
	{
		ScrollBar::ScrollBar(Orientation orientation, QWidget * parent)
			: QScrollBar(orientation,parent)
		{
		}

		ScrollBar::~ScrollBar()
		{
		}

		void ScrollBar::mousePressEvent(QMouseEvent * e)
		{
			QScrollBar::mousePressEvent(e);
			emit clicked();
		}

		void ScrollBar::mouseReleaseEvent(QMouseEvent * e)
		{
			QScrollBar::mouseReleaseEvent(e);
			emit released();
		}
	}
}

// END

