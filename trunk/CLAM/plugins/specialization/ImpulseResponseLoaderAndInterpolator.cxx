#include "ImpulseResponseLoaderAndInterpolator.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, ImpulseResponseLoaderAndInterpolator>
	regImpulseResponseLoaderAndInterpolator("ImpulseResponseLoaderAndInterpolator");
	static class ImpulseResponseLoaderAndInterpolatorMetadata
	{
	public:
		ImpulseResponseLoaderAndInterpolatorMetadata()
		{
			ProcessingFactory & factory = ProcessingFactory::GetInstance();
			factory.AddAttribute("ImpulseResponseLoaderAndInterpolator", "category", "CLAM");
			factory.AddAttribute("ImpulseResponseLoaderAndInterpolator", "description", "ImpulseResponseLoaderAndInterpolator");
		}
	} dummy;



}

}
