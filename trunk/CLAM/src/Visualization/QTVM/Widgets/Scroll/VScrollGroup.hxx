#ifndef __VSCROLLGROUP__
#define __VSCROLLGROUP__

#include <qwidget.h>

class QPushButton;

namespace CLAM
{
	namespace VM
	{
		class VZLabel;
		class ScrollBar;

		class VScrollGroup : public QWidget
		{
			Q_OBJECT

			public:
				VScrollGroup(QWidget* parent=0);
				virtual ~VScrollGroup();

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
				VZLabel* _label;
				QPushButton *_zIn,*_zOut;

				void Init();
		};
	}
}

#endif

