#include "Qt_SpectrumPortMonitorPresentation.hxx"
#include "PortMonitor.hxx"
#include "NetSpectrumPlot.hxx"
#include "Factory.hxx"

namespace NetworkGUI
{

	QWidget * Qt_SpectrumPortMonitorPresentation::SetInnerPlot()
	{
		return SetConcreteInnerPlot<CLAM::VM::NetSpectrumPlot>();
	}
}

typedef CLAM::Factory<NetworkGUI::Qt_ProcessingPresentation> Qt_ProcessingPresentationFactory;

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_SpectrumPortMonitorPresentation>
	regtSpectrumPortMonitor( "SpectrumPortMonitor" );


