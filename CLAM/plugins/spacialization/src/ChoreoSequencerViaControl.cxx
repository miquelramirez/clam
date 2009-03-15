#include "ChoreoSequencerViaControl.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/TypeInfo.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "ChoreoSequencerViaControl",
		"category", "[plugin] 3D Audio",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::ChoreoSequencerViaControl> reg = metadata;
}


