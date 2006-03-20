#include "vmWPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		WPlot::WPlot(QWidget* parent)
			: QFrame(parent)
		{
		}

		WPlot::~WPlot()
		{
		}

		void WPlot::SetTitle(const QString& title)
		{
			setWindowTitle(title);
		}
			
		void WPlot::SetGeometry(int x, int y, int w, int h)
		{
			setGeometry(x,y,w,h);
		}
	}
}

// END
