#include "Vbap3D_OneSpeaker.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "Vbap3D_OneSpeaker",
		"category", "[plugin] 3D Audio",
		"description", "Vbap3D_OneSpeaker",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Vbap3D_OneSpeaker> reg = metadata;
}
