#include "Qt_PeaksPortMonitorPresentation.hxx"
#include "PortMonitor.hxx"
#include "NetPeaksPlot.hxx"
#include "Factory.hxx"

namespace NetworkGUI
{

	QWidget * Qt_PeaksPortMonitorPresentation::SetInnerPlot()
	{
		return SetConcreteInnerPlot<CLAM::VM::NetPeaksPlot>();
	}
}

typedef CLAM::Factory<NetworkGUI::Qt_ProcessingPresentation> Qt_ProcessingPresentationFactory;

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_PeaksPortMonitorPresentation>
	regtPeakPortMonitor( "PeaksPortMonitor" );


