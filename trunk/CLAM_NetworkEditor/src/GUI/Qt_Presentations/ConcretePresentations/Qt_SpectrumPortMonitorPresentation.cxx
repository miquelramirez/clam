
#include "Qt_SpectrumPortMonitorPresentation.hxx"

#include <qlayout.h>

#include "NetSpectrumPlot.hxx"
#include "PortMonitor.hxx"

namespace NetworkGUI
{

Qt_SpectrumPortMonitorPresentation::Qt_SpectrumPortMonitorPresentation()
	: mControlRepresentation(0)
{
	QBoxLayout * layout = new QHBoxLayout(this);
	layout->setMargin(16); // Let the resize handles be visible
	mControlRepresentation = new CLAM::VM::NetSpectrumPlot(this);
	mControlRepresentation->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	layout->addWidget(mControlRepresentation);
	SlotBindMonitor.Wrap( this, &Qt_SpectrumPortMonitorPresentation::BindMonitor );
	SignalControllerAttached.Connect(SlotBindMonitor);

}

void Qt_SpectrumPortMonitorPresentation::BindMonitor(CLAMVM::ProcessingController & controller)
{
	typedef CLAM::SpectrumPortMonitor PortMonitor;
	typedef CLAM::VM::NetSpectrumPlot NetPlot;
	PortMonitor & monitor = dynamic_cast<PortMonitor &>( controller.GetObserved());
	NetPlot * plot = (NetPlot*)mControlRepresentation;
	plot->SetMonitor(monitor);
}

} // namespace NetworkGUI

