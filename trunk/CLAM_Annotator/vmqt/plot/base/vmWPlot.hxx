#ifndef __VMQT_WPLOT_H__
#define __VMQT_WPLOT_H__

#include <QFrame>

namespace CLAM
{
	namespace VM
	{
		class WPlot : public QFrame
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
