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

			void SetTitle(const QString& title);
			void SetGeometry(int x, int y, int w, int h);
		};
	}
}

#endif
