#include "Qt_AudioBuffPortMonitorPresentation.hxx"
#include "PortMonitor.hxx"
#include "NetAudioBuffPlot.hxx"
#include "Factory.hxx"

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


