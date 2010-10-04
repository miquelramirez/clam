#include "Hoa2IrregularSpeakerArray.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "Hoa2IrregularSpeakerArray",
		"category", "[plugin] 3D Audio",
		"description", "Hoa2IrregularSpeakerArray",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Hoa2IrregularSpeakerArray> reg = metadata;
}



