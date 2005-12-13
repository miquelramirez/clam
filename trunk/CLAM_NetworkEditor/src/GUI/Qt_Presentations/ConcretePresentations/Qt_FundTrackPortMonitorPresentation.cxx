#include "Qt_FundTrackPortMonitorPresentation.hxx"
#include <CLAM/PortMonitor.hxx>
#include <CLAM/NetFundTrackPlot.hxx>
#include <CLAM/Factory.hxx>

namespace NetworkGUI
{

	QWidget * Qt_FundTrackPortMonitorPresentation::SetInnerPlot()
	{
		return SetConcreteInnerPlot<CLAM::VM::NetFundTrackPlot>();
	}
}

typedef CLAM::Factory<NetworkGUI::Qt_ProcessingPresentation> Qt_ProcessingPresentationFactory;

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_FundTrackPortMonitorPresentation>
	regtFundTrackPortMonitor( "FundTrackPortMonitor" );


