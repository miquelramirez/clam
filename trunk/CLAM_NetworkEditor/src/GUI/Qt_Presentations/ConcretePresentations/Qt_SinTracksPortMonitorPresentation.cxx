#include "Qt_SinTracksPortMonitorPresentation.hxx"
#include <CLAM/PortMonitor.hxx>
#include <CLAM/NetSinTracksPlot.hxx>
#include <CLAM/Factory.hxx>

namespace NetworkGUI
{

	QWidget * Qt_SinTracksPortMonitorPresentation::SetInnerPlot()
	{
		return SetConcreteInnerPlot<CLAM::VM::NetSinTracksPlot>();
	}
}

typedef CLAM::Factory<NetworkGUI::Qt_ProcessingPresentation> Qt_ProcessingPresentationFactory;

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_SinTracksPortMonitorPresentation>
	regtSpecgramPortMonitor( "SinTracksPortMonitor" );


