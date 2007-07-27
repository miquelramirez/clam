#include <CLAM/ProcessingFactory.hxx>
#include "SpectrumView.hxx"

namespace detail
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, SpectrumViewMonitor> regSpectrumViewMonitor("SpectrumView");

	static class SpectrumViewMetadata
	{
	public:
		SpectrumViewMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("SpectrumView", "port_monitor_type", typeid(CLAM::Spectrum).name());
			factory.AddAttribute("SpectrumView", "icon", "spectrumview.svg");
		}
	} dummy;
}

