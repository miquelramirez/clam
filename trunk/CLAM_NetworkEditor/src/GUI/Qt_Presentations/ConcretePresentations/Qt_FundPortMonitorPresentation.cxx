
#include "Qt_FundPortMonitorPresentation.hxx"

#include <qlayout.h>

#include "NetFundPlot.hxx"
#include "PortMonitor.hxx"

namespace NetworkGUI
{

Qt_FundPortMonitorPresentation::Qt_FundPortMonitorPresentation()
	: mControlRepresentation(0)
{
	QBoxLayout * layout = new QHBoxLayout(this);
	layout->setMargin(16); // Let the resize handles be visible
	mControlRepresentation = new CLAM::VM::NetFundPlot(this);
	mControlRepresentation->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	layout->addWidget(mControlRepresentation);
	SlotBindMonitor.Wrap( this, &Qt_FundPortMonitorPresentation::BindMonitor );
	SignalControllerAttached.Connect(SlotBindMonitor);

}

void Qt_FundPortMonitorPresentation::BindMonitor(CLAMVM::ProcessingController & controller)
{
	typedef CLAM::FundamentalPortMonitor PortMonitor;
	typedef CLAM::VM::NetFundPlot NetPlot;
	PortMonitor & monitor = dynamic_cast<PortMonitor &>( controller.GetObserved());
	NetPlot * plot = (NetPlot*)mControlRepresentation;
	plot->SetMonitor(monitor);
}

} // namespace NetworkGUI

