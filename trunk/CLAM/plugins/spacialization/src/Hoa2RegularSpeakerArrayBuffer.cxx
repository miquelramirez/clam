#include "Hoa2RegularSpeakerArrayBuffer.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "Hoa2RegularSpeakerArrayBuffer",
		"category", "[plugin] 3D Audio",
		"description", "Hoa2RegularSpeakerArrayBuffer",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Hoa2RegularSpeakerArrayBuffer> reg = metadata;
}
