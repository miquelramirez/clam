#include "AudioBuffer2Stream.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{
namespace Hidden
{
static FactoryRegistrator<ProcessingFactory, AudioBuffer2Stream>
	regAudioBuffer2Stream("AudioBuffer2Stream");

	class regAudioBuffer2StreamMetadata
	{
	public:
		regAudioBuffer2StreamMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("AudioBuffer2Stream", "category", "CLAM");
			factory.AddAttribute("AudioBuffer2Stream", "description", "AudioBuffer2Stream");
		}
	};
	static regAudioBuffer2StreamMetadata dummy;
}
}

