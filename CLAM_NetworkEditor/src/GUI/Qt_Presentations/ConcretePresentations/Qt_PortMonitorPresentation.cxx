
#include "Qt_PortMonitorPresentation.hxx"

#include <qlayout.h>

#include <CLAM/NetPeaksPlot.hxx>
#include <CLAM/PortMonitor.hxx>

namespace NetworkGUI
{

Qt_PortMonitorPresentation::Qt_PortMonitorPresentation()
{
}

void Qt_PortMonitorPresentation::ProcessingControllerAttached()
{
	QBoxLayout * layout = new QHBoxLayout(this);
	layout->setMargin(12); // Let the resize handles be visible
	QWidget * innerPlot = SetInnerPlot();
	innerPlot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	layout->addWidget(innerPlot);
	innerPlot->show();
}


} // namespace NetworkGUI

