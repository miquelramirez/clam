#include "AudioBufferCrossFader.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "AudioBufferCrossFader",
		"category", "Spectral Processing",
		"description", "AudioBufferCrossFader",
		0
	};
	static FactoryRegistrator<ProcessingFactory, AudioBufferCrossFader> reg = metadata;
}

}
