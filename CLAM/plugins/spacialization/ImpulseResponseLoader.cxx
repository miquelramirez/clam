#include "ImpulseResponseLoader.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, ImpulseResponseLoader>
	regImpulseResponseLoader("ImpulseResponseLoader");
	static class ImpulseResponseLoaderMetadata
	{
	public:
		ImpulseResponseLoaderMetadata()
		{
			ProcessingFactory & factory = ProcessingFactory::GetInstance();
			factory.AddAttribute("ImpulseResponseLoader", "category", "CLAM");
			factory.AddAttribute("ImpulseResponseLoader", "description", "ImpulseResponseLoader");
		}
	} dummy;



}

}
