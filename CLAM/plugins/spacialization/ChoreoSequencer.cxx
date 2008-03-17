#include "ChoreoSequencer.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/TypeInfo.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "ChoreoSequencer",
		"category", "[plugin] 3D Audio",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::ChoreoSequencer> reg = metadata;
}


