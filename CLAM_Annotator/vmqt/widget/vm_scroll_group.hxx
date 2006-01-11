#ifndef __VMQT_SCROLL_GROUP_H__
#define __VMQT_SCROLL_GROUP_H__

#include <QWidget>
#include "vm_data_types.hxx"

class QLabel;
class QPushButton;
class QScrollBar;

namespace CLAM
{
	namespace VM
	{

		class CLAM::VM::VerticalLabel;

		class ScrollGroup :  public QWidget
		{
			Q_OBJECT
		public:
			ScrollGroup(CLAM::VM::EOrientation ori, QWidget* parent=0);
			~ScrollGroup();

			void set_pixmaps(const QPixmap& zin, const QPixmap& zout);
			int get_scroll_value() const;
			int get_max_scroll_value() const;

		signals:
			void zoomIn();
			void zoomOut();
			void scrollValueChanged(int);

		public slots:
			void updateScrollValue(int);
			void setMaxScrollValue(int);
			void updateZoomRatio(QString);

		private:
			QScrollBar*   sg_scroll_bar;
			QLabel*       sg_hlabel;
			QPushButton*  sg_zin_button;
			QPushButton*  sg_zout_button;

			CLAM::VM::VerticalLabel* sg_vlabel;

			void build_buttons();
			void build_horizontal();
			void build_vertical();
			void make_connections();
		};
	}
}

#endif

