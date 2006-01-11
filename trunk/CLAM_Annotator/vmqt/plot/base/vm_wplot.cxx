#include "vm_wplot.hxx"

namespace CLAM
{
	namespace VM
	{
		WPlot::WPlot(QWidget* parent)
			: QWidget(parent)
		{
		}

		WPlot::~WPlot()
		{
		}

		void WPlot::set_title(const std::string& title)
		{
			setWindowTitle(title.c_str());
		}
			
		void WPlot::set_geometry(int x, int y, int w, int h)
		{
			setGeometry(x,y,w,h);
		}
	}
}

// END
