#ifndef __VMQT_SCROLL_GROUP_H__
#define __VMQT_SCROLL_GROUP_H__

#include <QWidget>
#include "vmDataTypes.hxx"

class QLabel;
class QPushButton;
class QScrollBar;

namespace CLAM
{
	namespace VM
	{

		class VerticalLabel;

		class ScrollGroup :  public QWidget
		{
			Q_OBJECT
		public:
			ScrollGroup(EOrientation ori, QWidget* parent=0);
			~ScrollGroup();

			void SetPixmaps(const QPixmap& zin, const QPixmap& zout);
			int GetScrollValue() const;
			int GetMaxScrollValue() const;

		signals:
			void zoomIn();
			void zoomOut();
			void scrollValueChanged(int);

		public slots:
			void updateScrollValue(int);
			void setMaxScrollValue(int);
			void updateZoomRatio(QString);

		private:
			QScrollBar*    mScrollBar;
			QLabel*        mHlabel;
			QPushButton*   mZinButton;
			QPushButton*   mZoutButton;

			VerticalLabel* mVlabel;

			void BuildButtons();
			void BuildHorizontal();
			void BuildVertical();
			void MakeConnections();
		};
	}
}

#endif

