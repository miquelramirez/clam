
#include "Qt_AudioPortMonitorPresentation.hxx"

#include <qlayout.h>

#include "NetAudioPlot.hxx"
#include "PortMonitor.hxx"

namespace NetworkGUI
{

Qt_AudioPortMonitorPresentation::Qt_AudioPortMonitorPresentation()
	: mControlRepresentation(0)
{
	QBoxLayout * layout = new QHBoxLayout(this);
	layout->setMargin(16); // Let the resize handles be visible
	mControlRepresentation = new CLAM::VM::NetAudioPlot(this);
	mControlRepresentation->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	layout->addWidget(mControlRepresentation);
	SlotBindMonitor.Wrap( this, &Qt_AudioPortMonitorPresentation::BindMonitor );
	SignalControllerAttached.Connect(SlotBindMonitor);

}

void Qt_AudioPortMonitorPresentation::BindMonitor(CLAMVM::ProcessingController & controller)
{
	typedef CLAM::AudioPortMonitor PortMonitor;
	typedef CLAM::VM::NetAudioPlot NetPlot;
	PortMonitor & monitor = dynamic_cast<PortMonitor &>( controller.GetObserved());
	NetPlot * plot = (NetPlot*)mControlRepresentation;
	plot->SetMonitor(monitor);
}

} // namespace NetworkGUI

