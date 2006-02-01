#ifndef __VMQT_WPLOT_H__
#define __VMQT_WPLOT_H__

#include <QWidget>

namespace CLAM
{
	namespace VM
	{
		class WPlot : public QWidget
		{
		public:
			WPlot(QWidget* parent=0);
			virtual ~WPlot();

			void set_title(const QString& title);
			void set_geometry(int x, int y, int w, int h);
		};
	}
}

#endif
