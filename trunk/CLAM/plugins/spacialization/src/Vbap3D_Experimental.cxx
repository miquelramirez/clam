#include "Vbap3D_Experimental.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "Vbap3DExperimental",
		"category", "[plugin] 3D Audio",
		"description", "Vbap3DExperimental",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Vbap3DExperimental> reg = metadata;
}
