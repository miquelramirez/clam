#include "Qt_SpecgramPortMonitorPresentation.hxx"
#include "PortMonitor.hxx"
#include "NetSpecgramPlot.hxx"
#include "Factory.hxx"

namespace NetworkGUI
{

	QWidget * Qt_SpecgramPortMonitorPresentation::SetInnerPlot()
	{
		return SetConcreteInnerPlot<CLAM::VM::NetSpecgramPlot>();
	}
}

typedef CLAM::Factory<NetworkGUI::Qt_ProcessingPresentation> Qt_ProcessingPresentationFactory;

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_SpecgramPortMonitorPresentation>
	regtSpecgramPortMonitor( "SpecgramPortMonitor" );


