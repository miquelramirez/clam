#include "Vbap3D.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "Vbap3D",
		"category", "[plugin] 3D Audio",
		"description", "Vbap3D",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Vbap3D> reg = metadata;
}
