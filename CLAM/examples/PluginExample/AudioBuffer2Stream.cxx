#include "AudioBuffer2Stream.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{
namespace Hidden
{
static FactoryRegistrator<ProcessingFactory, AudioBuffer2Stream>
	regAudioBuffer2Stream("AudioBuffer2Stream");
}
}

