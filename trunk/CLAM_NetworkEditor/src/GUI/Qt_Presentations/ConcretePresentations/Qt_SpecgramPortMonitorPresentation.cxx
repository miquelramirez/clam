#include "Qt_SpecgramPortMonitorPresentation.hxx"
#include <CLAM/PortMonitor.hxx>
#include <CLAM/NetSpecgramPlot.hxx>
#include <CLAM/Factory.hxx>

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


