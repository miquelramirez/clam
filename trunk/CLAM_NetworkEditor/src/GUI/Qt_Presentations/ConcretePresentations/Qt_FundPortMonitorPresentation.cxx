#include "Qt_FundPortMonitorPresentation.hxx"
#include "PortMonitor.hxx"
#include "NetFundPlot.hxx"
#include "Factory.hxx"

namespace NetworkGUI
{

	QWidget * Qt_FundPortMonitorPresentation::SetInnerPlot()
	{
		return SetConcreteInnerPlot<CLAM::VM::NetFundPlot>();
	}
}

typedef CLAM::Factory<NetworkGUI::Qt_ProcessingPresentation> Qt_ProcessingPresentationFactory;

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_FundPortMonitorPresentation>
	regtFundPortMonitor( "FundamentalPortMonitor" );


