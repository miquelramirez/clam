#include "NSurround.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "NSurround",
//		"category", "CLAM",
		"description", "Surround N sources equidistant",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, NSurround> reg = metadata;
}
