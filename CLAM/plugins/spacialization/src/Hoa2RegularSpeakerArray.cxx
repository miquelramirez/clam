#include "Hoa2RegularSpeakerArray.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "Hoa2RegularSpeakerArray",
		"category", "[plugin] 3D Audio",
		"description", "BFormat to 16 speakers",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Hoa2RegularSpeakerArray> reg = metadata;
}
