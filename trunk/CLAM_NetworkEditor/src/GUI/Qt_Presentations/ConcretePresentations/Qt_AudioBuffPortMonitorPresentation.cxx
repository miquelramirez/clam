#include "Qt_AudioBuffPortMonitorPresentation.hxx"
#include <CLAM/PortMonitor.hxx>
#include <CLAM/NetAudioBuffPlot.hxx>
#include <CLAM/Factory.hxx>

namespace NetworkGUI
{

	QWidget * Qt_AudioBuffPortMonitorPresentation::SetInnerPlot()
	{
		return SetConcreteInnerPlot<CLAM::VM::NetAudioBuffPlot>();
	}
}

typedef CLAM::Factory<NetworkGUI::Qt_ProcessingPresentation> Qt_ProcessingPresentationFactory;

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_AudioBuffPortMonitorPresentation>
	regtAudioBuffPortMonitor( "AudioBuffPortMonitor" );


