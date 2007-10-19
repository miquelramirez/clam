#ifndef _SimpleLoopMetadata_
#define _SimpleLoopMetadata_

#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "SDIFInConfig.hxx"
#include "Filename.hxx"

namespace CLAM
{

class SimpleLoopMetadata : public DynamicType
{
public:
	DYNAMIC_TYPE (SimpleLoopMetadata, 2);
	DYN_ATTRIBUTE(0, public, int, StartFrame);
	DYN_ATTRIBUTE(1, public, int, EndFrame);

	// TODO define a new type for EnvelopeStage
//	DYN_ATTRIBUTE(3,public, EnvelopeStage, EnvelopeStage);
	void DefaultInit();
};

} // end namespace CLAM

#endif

