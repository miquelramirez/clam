#include "HoaEncoder.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "HoaEncoder",
		"category", "[plugin] 3D Audio",
		"description", "HoaEncoder",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, HoaEncoder> reg = metadata;
}
