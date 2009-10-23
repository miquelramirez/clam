#include "HoaEncoderBuffer.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "HoaEncoderBuffer",
		"category", "[plugin] 3D Audio",
		"description", "HoaEncoderBuffer",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, HoaEncoderBuffer> reg = metadata;
}
