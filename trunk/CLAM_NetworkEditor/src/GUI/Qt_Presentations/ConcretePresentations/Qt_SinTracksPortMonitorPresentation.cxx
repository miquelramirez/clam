#include "Qt_SinTracksPortMonitorPresentation.hxx"
#include "PortMonitor.hxx"
#include "NetSinTracksPlot.hxx"
#include "Factory.hxx"

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


