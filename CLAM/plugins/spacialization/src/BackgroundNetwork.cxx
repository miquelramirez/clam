#include "BackgroundNetwork.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/TypeInfo.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "BackgroundNetwork",
		"category", "[plugin] 3D Audio",
		"description", "It plays a network player in a background thread in non real-time",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::BackgroundNetwork> reg = metadata;
}


