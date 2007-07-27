
#include <CLAM/ProcessingFactory.hxx>
#include "PolarChromaPeaks.hxx"

namespace detail
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, PolarChromaPeaksMonitor> regPolarChromaPeaksMonitor("PolarChromaPeaks");
	static class PolarChromaPeaksMetadata
	{
	public:
		PolarChromaPeaksMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("PolarChromaPeaks", "port_monitor_type", 
					typeid(std::vector<std::pair<CLAM::TData,CLAM::TData> >).name());
			factory.AddAttribute("PolarChromaPeaks", "icon", "polarchromapeaks.svg");
		}
	} dummy;
}


