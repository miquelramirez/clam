
#include "Qt_PortMonitorPresentation.hxx"

#include <qlayout.h>

#include "NetPeaksPlot.hxx"
#include "PortMonitor.hxx"

namespace NetworkGUI
{

Qt_PortMonitorPresentation::Qt_PortMonitorPresentation()
	: mControlRepresentation(0)
{
	QBoxLayout * layout = new QHBoxLayout(this);
	layout->setMargin(16); // Let the resize handles be visible
	mControlRepresentation = new CLAM::VM::NetPeaksPlot(this);
	mControlRepresentation->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	layout->addWidget(mControlRepresentation);
	SlotBindMonitor.Wrap( this, &Qt_PortMonitorPresentation::BindMonitor );
	SignalControllerAttached.Connect(SlotBindMonitor);

}

void Qt_PortMonitorPresentation::BindMonitor(CLAMVM::ProcessingController & controller)
{
	CLAM::PeaksPortMonitor & monitor = 
		dynamic_cast<CLAM::PeaksPortMonitor &>(controller.GetObserved());
	((CLAM::VM::NetPeaksPlot *)mControlRepresentation)->SetMonitor(monitor);
}

} // namespace NetworkGUI

