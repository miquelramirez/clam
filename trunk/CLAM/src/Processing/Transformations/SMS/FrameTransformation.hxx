#ifndef _FrameTransformation_hxx
#define _FrameTransformation_hxx

#include "Processing.hxx"

namespace CLAM
{

class Frame;

class FrameTransformation : public Processing
{
public:
	virtual bool Do(const Frame& in, Frame& out) = 0;
	
};

}

#endif
