#include "Vbap3DBuffer.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "Vbap3DBuffer",
		"category", "[plugin] 3D Audio",
		"description", "Vbap3DBuffer",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Vbap3DBuffer> reg = metadata;
}
