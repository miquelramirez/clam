#include "Qt_AudioPortMonitorPresentation.hxx"
#include "PortMonitor.hxx"
#include "NetAudioPlot.hxx"
#include "Factory.hxx"

namespace NetworkGUI
{

	QWidget * Qt_AudioPortMonitorPresentation::SetInnerPlot()
	{
		return SetConcreteInnerPlot<CLAM::VM::NetAudioPlot>();
	}
}

typedef CLAM::Factory<NetworkGUI::Qt_ProcessingPresentation> Qt_ProcessingPresentationFactory;

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_AudioPortMonitorPresentation>
	regtAudioPortMonitor( "AudioPortMonitor" );


