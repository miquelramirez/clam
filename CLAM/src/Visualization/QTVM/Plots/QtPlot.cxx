#include "QtPlotter.hxx"
#include "QtPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		QtPlot::QtPlot(QWidget* parent) : QWidget(parent)
		{
		}
		
		QtPlot::~QtPlot()
		{
		}

		void QtPlot::closeEvent(QCloseEvent *e)
		{
			QtPlotter::Remove(this);
			e->accept();
		}
	}
}

// END
