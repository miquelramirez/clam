#include "AudioBuffer2Stream.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "AudioBuffer2Stream",
		"category", "Spectral Processing",
		"description", "AudioBuffer2Stream",
		0
	};
	static FactoryRegistrator<ProcessingFactory, AudioBuffer2Stream> reg = metadata;
}

}
