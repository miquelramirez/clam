#ifndef __rhythmir_dumper__
#define __rhythmir_dumper__

#include <string>
#include "Pool.hxx"

namespace CLAM
{
	class Pulse;
}

extern void DumpExtractedData( std::string filename, 
			       std::string analyzedFile,
			       const CLAM::Pulse& ticksData,
			       const CLAM::Pulse& beatsData );

namespace RhythmIR
{
	extern void DumpToSimacXML( CLAM::DescriptionDataPool& pool,
				    std::string outXMLFile );

	extern void DumpToWavesurferLabs( CLAM::DescriptionDataPool& pool, std::string outputFilename );					  
}

#endif // rhythmir_dumper.hxx
