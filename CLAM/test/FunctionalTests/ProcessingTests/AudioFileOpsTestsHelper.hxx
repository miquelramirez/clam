#ifndef __AUDIOFILEOPSTESTSHELPER__
#define __AUDIOFILEOPSTESTSHELPER__

#include "Audio.hxx"
#include "AudioFile.hxx"

namespace CLAMTest
{
	double fileSimilarity( const std::string & filename, 
			       int channel, 
			       const CLAM::Audio& rhs );
	
	void fileSimilarity( const std::string & filename,
			     const CLAM::Audio& left, const CLAM::Audio& right,
			     double& similarityLeft, double& similarityRight );

}


#endif // AudioFileOpsTestsHelper.hxx

