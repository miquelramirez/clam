#ifndef __RHYTHMIR_PROCESS__
#define __RHYTHMIR_PROCESS__

#include "Pool.hxx"
#include "TickExtractorConfig.hxx"
#include <string>

namespace RhythmIR
{
	void LoadInputAudio( CLAM::DescriptionDataPool& pool,
			     std::string filename );

	void NormalizeInputAudio( CLAM::DescriptionDataPool& pool,
				  const CLAM::TickExtractorConfig& config );

	void ExtractOnsets( CLAM::DescriptionDataPool& pool,
			    const CLAM::TickExtractorConfig& config );

	void ExtractTicksAndBeats( CLAM::DescriptionDataPool& pool,
				   const CLAM::TickExtractorConfig& config );

	void ExtractMeter( CLAM::DescriptionDataPool& pool,
			   const CLAM::TickExtractorConfig& config );
				  
}

#endif // rhythmir_process.hxx
