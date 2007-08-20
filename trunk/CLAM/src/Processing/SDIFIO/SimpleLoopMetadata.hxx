#ifndef _SimpleLoopMetadata_
#define _SimpleLoopMetadata_

#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "SDIFInConfig.hxx"
#include "Filename.hxx"
#include "ELoopPointUnits.hxx"

namespace CLAM
{

/**
* This object represents the start and end points of a loop. The given values
* should be in milliseconds.
*
* \author greg kellum [gkellum@iua.upf.edu] 15/7/2007
* \since CLAM v1.1.
*/
class SimpleLoopMetadata : public DynamicType
{
public:
	DYNAMIC_TYPE (SimpleLoopMetadata, 3);
	DYN_ATTRIBUTE(0, public, ELoopPointUnits, Units);
	DYN_ATTRIBUTE(1, public, int, Start);
	DYN_ATTRIBUTE(2, public, int, End);

	// TODO define a new type for EnvelopeStage
//	DYN_ATTRIBUTE(3,public, EnvelopeStage, EnvelopeStage);
	void DefaultInit();
};

} // end namespace CLAM

#endif

