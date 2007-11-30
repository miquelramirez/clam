#include "ControlSequencer.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "ControlSequencer",
		"category", "3D Audio",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::ControlSequencer> reg = metadata;
}

