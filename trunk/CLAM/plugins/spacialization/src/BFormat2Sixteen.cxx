#include "BFormat2Sixteen.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "BFormat2Sixteen",
		"category", "[plugin] 3D Audio",
		"description", "BFormat to 16 speakers",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, BFormat2Sixteen> reg = metadata;
}
