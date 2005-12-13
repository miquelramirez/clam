#include "Qt_AudioPortMonitorPresentation.hxx"
#include <CLAM/PortMonitor.hxx>
#include <CLAM/NetAudioPlot.hxx>
#include <CLAM/Factory.hxx>

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


