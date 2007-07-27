
#include <CLAM/ProcessingFactory.hxx>
#include "Tunner.hxx"

namespace Hidden
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, TunnerMonitor> regTunnerMonitor("Tunner");

	static class TunnerMetadata
	{
	public:
		TunnerMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("Tunner", "port_monitor_type", typeid(std::pair<CLAM::TData,CLAM::TData>).name());
			factory.AddAttribute("Tunner", "icon", "tunner.svg");
		}
	} dummy;
}

