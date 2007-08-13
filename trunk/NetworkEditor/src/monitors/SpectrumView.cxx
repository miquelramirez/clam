#include <CLAM/ProcessingFactory.hxx>
#include "SpectrumView.hxx"

namespace detail
{
	static const char * metadata[] = {
		"key", "SpectrumView",
		"category", "Monitors",
		"description", "SpectrumView",
		"port_monitor_type", typeid(CLAM::Spectrum).name(),
		"icon", "spectrumview.svg",
		0
	};
	//static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, SpectrumViewMonitor> regSpectrumViewMonitor("SpectrumView");
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, SpectrumViewMonitor> reg = metadata;

/*	static class SpectrumViewMetadata
	{
	public:
		SpectrumViewMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("SpectrumView", "port_monitor_type", typeid(CLAM::Spectrum).name());
			factory.AddAttribute("SpectrumView", "icon", "spectrumview.svg");
		}
	} dummy;*/
}

