
#include "Qt_PortMonitorPresentation.hxx"
#include "Qt_OutControlPresentation.hxx"
#include "CLAM_Math.hxx"

#include <qslider.h>
#include <qdial.h>
#include <qspinbox.h>
#include <qpainter.h>
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
	((CLAM::VM::NetPeaksPlot *)mControlRepresentation)->SetMonitor(dynamic_cast<CLAM::PeaksPortMonitor &>( controller.GetObserved()));
}

} // namespace NetworkGUI

