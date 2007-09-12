#include "ImpulseResponseDatabaseFetcher.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "ImpulseResponseDatabaseFetcher",
		"category", "Spectral Processing",
		"description", "ImpulseResponseDatabaseFetcher",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, ImpulseResponseDatabaseFetcher>
	reg = metadata;
}

}
