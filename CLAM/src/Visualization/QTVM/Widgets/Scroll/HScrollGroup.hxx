#ifndef __HSCROLLGROUP__
#define __HSCROLLGROUP__

#include <qwidget.h>

class QLabel;
class QPushButton;

namespace CLAM
{
	namespace VM
	{
		class ScrollBar;

		class HScrollGroup : public QWidget
		{
			Q_OBJECT

			public:
				HScrollGroup(QWidget* parent=0);
				virtual ~HScrollGroup();

				int GetScrollValue() const;
				int GetMaxScrollValue() const;
			
			signals:
				void zoomIn();
				void zoomOut();
				void scrollValueChanged(int);
				void zoomRatio(int);
				void maxScrollValue(int);
				void updatedScrollValue(int);
				void scrollBarClicked();
				void scrollBarReleased();
			
			public slots:
				void updateZoomRatio(int);
				void updateScrollValue(int);
				void setMaxScrollValue(int);
			
			private:
				ScrollBar* _sb;
				QLabel* _label;
				QPushButton *_zIn,*_zOut;

				void Init();
		};
	}
}

#endif

