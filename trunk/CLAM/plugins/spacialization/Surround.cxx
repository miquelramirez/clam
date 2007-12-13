#include "Surround.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "Surround",
		"category", "3D Audio",
		"description", "Surround 5.1 Angles -30, 0, 30, -110, 110",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Surround> reg = metadata;
}
