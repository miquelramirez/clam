#ifndef _SampleMetadata_
#define _SampleMetadata_

#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "SDIFInConfig.hxx"
#include "Filename.hxx"
#include "SimpleLoopMetadata.hxx"
#include <vector>

namespace CLAM
{

#define SAMPLE_METADATA_PITCH_INDEX 1
#define SAMPLE_METADATA_AMPLITUDE_INDEX 2

/**
* This class should be used to hold meta information about a sound sample.
* This meta information includes both content-based audio descriptors such
* as the pitch and amplitude of a sample as well as information about
* the properties of the sound generation process. Finally, this class
* also holds information about the possible loop points to be used by
* a sample.
*
* \author greg kellum [gkellum@iua.upf.edu] 6/15/2007
* \since CLAM v1.1.
*/
class SampleMetadata : public DynamicType
{
public:
	DYNAMIC_TYPE (SampleMetadata, 6);
	DYN_ATTRIBUTE(0,public, CLAM::Filename, Filename);
	DYN_ATTRIBUTE(SAMPLE_METADATA_PITCH_INDEX, public, double, Pitch);
	DYN_ATTRIBUTE(SAMPLE_METADATA_AMPLITUDE_INDEX,public, double, Amplitude);
	DYN_ATTRIBUTE(3,public, int, SortIndex);
	DYN_ATTRIBUTE(4,public, int, StartSteadyState);
	DYN_CONTAINER_ATTRIBUTE(5, public, std::vector<SimpleLoopMetadata>, ListOfLoops, Loop);
	
//	DYN_ATTRIBUTE(3,public, EnvelopeStage, EnvelopeStage);
	void DefaultInit();
	// when the object is restored from XML, after it is read it only has those
	// attributes which are defined in the XML file. so, we need to add some
	// more attributes to it which will be used later by other algorithms
	// such as the SortIndex
	void ConfigureAfterRestore();
	bool operator<(const SampleMetadata& argSampleMetadata) const;
	bool operator>(const SampleMetadata& argSampleMetadata) const;
	bool operator==(const SampleMetadata& argSampleMetadata) const;

};

} // end namespace CLAM

#endif

