#ifndef __TICK_EXTRACTOR__
#define __TICK_EXTRACTOR__

#include <string>
#include "Array.hxx"
#include "Pulse.hxx"

namespace CLAM
{
	class TickExtractorConfig;

	void ExtractTicksSequence( std::string pathToFile,
				   CLAM::Pulse& ticksSequence,
				   CLAM::Pulse& beatsSequence );

	void ExtractTicksSequence( std::string pathToFile,
				   const TickExtractorConfig& cfg,
				   CLAM::Pulse& ticksSequence,
				   CLAM::Pulse& beatsSequence );
}

#endif // TickExtractor.hxx
