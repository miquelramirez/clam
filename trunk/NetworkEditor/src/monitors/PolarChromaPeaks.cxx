
#include <CLAM/ProcessingFactory.hxx>
#include "PolarChromaPeaks.hxx"

namespace detail
{
	static const char * metadata[] = {
		"key", "PolarChromaPeaks",
		"category", "Monitors",
		"description", "PolarChromaPeaks",
		"port_monitor_type", typeid(std::vector<std::pair<CLAM::TData, CLAM::TData> >).name(),
		"icon", "polarchromapeaks.svg",
		0
	};
	//static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, PolarChromaPeaksMonitor> regPolarChromaPeaksMonitor("PolarChromaPeaks");
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, PolarChromaPeaksMonitor> reg = metadata;
/*	static class PolarChromaPeaksMetadata
	{
	public:
		PolarChromaPeaksMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("PolarChromaPeaks", "port_monitor_type", 
					typeid(std::vector<std::pair<CLAM::TData,CLAM::TData> >).name());
			factory.AddAttribute("PolarChromaPeaks", "icon", "polarchromapeaks.svg");
		}
	} dummy;*/
}


